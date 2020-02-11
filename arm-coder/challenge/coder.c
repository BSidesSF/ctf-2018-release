#include <stdio.h>
#include <stdlib.h>
#include <seccomp.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define SHELLCODE_BYTES_MAX 1024
#define STACK_SIZE 4*1024

#define _print_msg(ch, ...) do { \
  printf("[%c] ", ch); \
  printf(__VA_ARGS__); \
  printf("\n"); \
  fflush(stdout); \
} while(0)

#define print_msg(...) _print_msg('+', __VA_ARGS__)
#define print_err(...) _print_msg('!', __VA_ARGS__)
#ifdef DEBUG
# define print_dbg(...) _print_msg('@', __VA_ARGS__)
#else
# define print_dbg(...)
#endif

// This is provided by assembly
void execute_alternate(void *execp, void *stack, void *ptr);

// Note: players will have to use *this* pointer to open!
const char *flag_path = "./flag.txt";

size_t round_next_page(size_t size);
void *allocate_stack();
void *allocate_with_guard(size_t size, int prot);
int setup_sandbox();

int main(int argc, char **argv) {
  char buf[100];
  char *ptr;
  size_t sc_len = 0;
  size_t sc_read = 0;
  ssize_t tmp_len;
  void *stack;
  void *sc_buf;

  print_msg("It's 2018, so we run everything in the cloud.");
  print_msg("And I mean everything -- even our shellcode testing service.");
  print_msg("Perhaps you'd like to test your shellcode?");
  print_msg("Please send the length of your shellcode followed by a newline.");
  fgets(buf, sizeof(buf)/sizeof(char), stdin);
  if (*buf == '\0' || *buf == '\n') {
    print_err("Yeah, I'm gonna need a length.");
    return 1;
  }
  sc_len = (size_t)strtoul(buf, &ptr, 0);
  if (!ptr || (*ptr != '\0' && *ptr != '\n')) {
    print_err("Yeah, that length doesn't look valid to me.");
    return 1;
  }
  if (sc_len == 0 || sc_len > SHELLCODE_BYTES_MAX) {
    print_err("Sorry, you must provide up to %d bytes of shellcode.",
        SHELLCODE_BYTES_MAX);
    return 1;
  }
  if ((stack = allocate_stack()) == NULL) {
    print_err("Could not allocate stack.");
    return 1;
  }
  print_dbg("Allocating a place for shellcode.");
  if ((sc_buf = allocate_with_guard(round_next_page(sc_len), PROT_EXEC))
      == NULL) {
    print_err("Could not allocate space for shellcode.");
    return 1;
  }
  print_msg("OK, please send the shellcode.");
  while (sc_read < sc_len) {
    tmp_len = fread((void *)((char *)sc_buf + sc_read),
        1, sc_len - sc_read, stdin);
    if (tmp_len < 1) {
      print_err("Error reading.");
      return 1;
    }
    sc_read += tmp_len;
  }
  __clear_cache((char *)sc_buf, (char *)sc_buf + sc_read);
  print_msg("Setting up sandbox.");
  if (setup_sandbox()) {
    print_err("Could not setup sandbox.");
    return 1;
  }
  execute_alternate(sc_buf, stack, (void *)execute_alternate);
  return 0;
}

size_t round_next_page(size_t size) {
  size_t sz = sysconf(_SC_PAGESIZE);
  if (size < sz)
    return sz;
  if ((size & (sz-1)) == 0)
    return size;
  return (size & (~(sz-1))) + sz;
}

void *allocate_stack() {
  void *stack;
  print_dbg("Allocating your stack.");
  stack = allocate_with_guard(STACK_SIZE, 0);
  if (!stack)
    return NULL;
  // Make a "top" of stack
  stack = (char *)stack + STACK_SIZE - 0x20;
  return stack;
}

void *allocate_with_guard(size_t size, int prot) {
  void *mem_base;
  size_t sz = sysconf(_SC_PAGESIZE);

  mem_base = mmap(NULL, size+2*sz, PROT_READ|PROT_WRITE|prot,
      MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (mem_base == MAP_FAILED) {
    print_err("Unable to map memory.");
    return NULL;
  }
  // Allocate a guard page above and below
  if (mmap(mem_base, sz, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, -1, 0)
      == MAP_FAILED) {
    print_err("Guard page 1 failed.");
    return NULL;
  }
  if (mmap(mem_base+sz+size, sz, PROT_NONE,
        MAP_PRIVATE|MAP_ANONYMOUS|MAP_FIXED, -1, 0) == MAP_FAILED) {
    print_err("Guard page 2 failed.");
    return NULL;
  }
  // Return start of usable memory
  return mem_base + sz;
}

int setup_sandbox() {
  int rv=0;
#ifdef DEBUG
  // This allows GDB to trap the signal
  scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_TRAP);
#else
  scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_KILL);
#endif

  // Add rules
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(close), 0);
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit_group), 0);
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(exit), 0);
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(rt_sigreturn), 0);

#ifdef __amd64__
  // Allow sendfile
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(sendfile), 0);
#else
  // Only write up to 1k to STDOUT
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 2,
      SCMP_A0(SCMP_CMP_EQ, STDOUT_FILENO),
      SCMP_A2(SCMP_CMP_LE, 1024));

  // Prohibit read from STDIN
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
      SCMP_A0(SCMP_CMP_NE, STDIN_FILENO));
#endif

  // Filter open
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(open), 2,
      SCMP_A0(SCMP_CMP_EQ, (scmp_datum_t)flag_path),
      SCMP_A1(SCMP_CMP_EQ, (scmp_datum_t)O_RDONLY));

  // Load the filter
  if (!rv)
    rv = seccomp_load(ctx);

  seccomp_release(ctx);
  return rv;
}
