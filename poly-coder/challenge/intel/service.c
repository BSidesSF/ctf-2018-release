#include "common.h"
#include <sys/sendfile.h>
#include <sys/stat.h>

const char *flag_path = "./flag.txt";

typedef struct {
  secret intel;
  secret arm;
} secret_pair;

void make_secrets(secret_pair *p);

// Provided by arm_handoff.c
int handoff(void *sc_buf, size_t sc_len, secret *arm_secret);

int main(int argc, char **argv) {
  char buf[100];
  char *ptr;
  size_t sc_len = 0;
  size_t sc_read = 0;
  ssize_t tmp_len;
  void *stack;
  void *sc_buf;
  secret_pair secrets;
  pid_t child;
  pipes comm;

  print_msg("It's 2018, so we run everything in the cloud.");
  print_msg("And I mean everything -- even our shellcode testing service.");
  print_msg("This version is enhanced with support for both ARM and x86_64.");
  print_msg("It's very simple, in fact.  Just read from one open file and write to the other.");
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

  print_dbg("Making secrets.");
  make_secrets(&secrets);

  print_dbg("Making pipes.");
  if (make_pipes(&comm)) {
    print_msg("Failed making pipes.");
    return 1;
  }

  child = fork();
  if (child < 0) {
    print_msg("Could not fork.  Failed.");
    return 1;
  }
  if (child == 0) {
    // We are the child
    close(comm.from_child[PIPE_READ]);
    close(comm.to_child[PIPE_WRITE]);
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    if ((stack = allocate_stack()) == NULL) {
      print_err("Could not allocate stack.");
      return 1;
    }

    print_msg("Setting up sandbox.");
    if (setup_sandbox(
          comm.from_child[PIPE_WRITE], comm.to_child[PIPE_READ])) {
      print_err("Could not setup sandbox.");
      return 1;
    }

    memset(&comm, 0, sizeof(comm));
    execute_alternate(sc_buf, stack, (void *)execute_alternate);
    return 0;
  } else {
    // We are the parent
    close(comm.from_child[PIPE_WRITE]);
    close(comm.to_child[PIPE_READ]);

    // Write the secret
    if (write_exactly(comm.to_child[PIPE_WRITE], secrets.intel, sizeof(secret))
        != sizeof(secret)) {
      print_err("Error writing.");
      return 1;
    }
    close(comm.to_child[PIPE_WRITE]);

    // Read the secret
    if (read_exactly(comm.from_child[PIPE_READ], buf, sizeof(secret))
        != sizeof(secret)) {
      print_err("Error reading.");
      return 1;
    }
    close(comm.from_child[PIPE_READ]);

    int status;
    waitpid(child, &status, 0);

    if (WEXITSTATUS(status) != 0) {
      print_err("Runner exited with non-zero status!");
      return 1;
    }

    if (memcmp(buf, secrets.intel, sizeof(secret))) {
      print_err("Sorry, wrong secret!");
      return 1;
    }

    // Now do the ARM bits!
    if (handoff(sc_buf, sc_len, &secrets.arm)) {
      return 1;
    }

    // Now give a flag.
    int fd = open(flag_path, O_RDONLY);
    if (fd < 0) {
      print_err("Error reading flag!");
      return 1;
    }
    struct stat sb;
    fstat(fd, &sb);
    sendfile(STDOUT_FILENO, fd, NULL, sb.st_size);
    return 0;
  }
}

void make_secrets(secret_pair *p) {
  size_t left = sizeof(secret_pair);
  size_t tmp = 0;
  int fd = open("/dev/urandom", O_RDONLY);
  while(left) {
    char *ptr = (char *)p + sizeof(secret_pair) - left;
    tmp = read(fd, ptr, left);
    if(tmp <= 0)
      _exit(1);
    left -= tmp;
  }
  close(fd);
}
