#include "common.h"

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

int setup_sandbox(int outfd, int infd) {
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

  // Must write precisely 32 bytes
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(write), 2,
      SCMP_A0(SCMP_CMP_EQ, outfd),
      SCMP_A2(SCMP_CMP_EQ, 32));

  // Only read from allowed fd
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
      SCMP_A0(SCMP_CMP_EQ, infd));

  // Other reads and writes give an error
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(write), 0);
  rv |= seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(read), 0);

  // Load the filter
  if (!rv)
    rv = seccomp_load(ctx);

  seccomp_release(ctx);
  return rv;
}

int make_pipes(pipes *p) {
  // Make pipes and randomize the fd number on the child end
  if(pipe(p->to_child))
    return -1;
  if(pipe(p->from_child))
    return -1;
  int new_fd = random_fd();
  if(dup2(p->to_child[PIPE_READ], new_fd))
    return -1;
  close(p->to_child[PIPE_READ]);
  p->to_child[PIPE_READ] = new_fd;
  new_fd = random_fd();
  if(dup2(p->from_child[PIPE_WRITE], new_fd))
    return -1;
  close(p->from_child[PIPE_WRITE]);
  p->from_child[PIPE_WRITE] = new_fd;
  return 0;
}

int random_fd() {
  // Get a random unused FD
  int rfd = open("/dev/urandom", O_RDONLY);
  int fd;
  while (1) {
    char fdl;
    read(rfd, &fdl, 1);
    fd = (int)fdl;
    if (fcntl(fd, F_GETFD) == -1)
      break;
  }
  close(rfd);
  return fd;
}

int read_exactly(int fd, void *buf, size_t n) {
  if (n == 0)
    return 0;
  int tmp, done=0;
  while(done < n) {
    tmp = read(fd, buf+done, n-done);
    if(tmp <= 0)
      return tmp;
    done += tmp;
  }
  return done;
}

int write_exactly(int fd, void *buf, size_t n) {
  if (n == 0)
    return 0;
  int tmp, done=0;
  while(done < n) {
    tmp = write(fd, buf+done, n-done);
    if(tmp <= 0)
      return tmp;
    done += tmp;
  }
  return done;
}
