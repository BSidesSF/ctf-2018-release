#include "common.h"

int main(int argc, char **argv) {
  int infd = random_fd();
  int outfd = random_fd();
  size_t sc_len;

  if (read_exactly(STDIN_FILENO, &sc_len, sizeof(size_t)) != sizeof(size_t)) {
    print_dbg("Error reading length.");
    return 1;
  }

  if (sc_len > SHELLCODE_BYTES_MAX) {
    print_dbg("Too long length.");
    return 1;
  }

  void *sc_buf = allocate_with_guard(round_next_page(sc_len), PROT_EXEC);
  if (!sc_buf) {
    print_dbg("Error allocating sc_buf.");
    return 1;
  }

  if (read_exactly(STDIN_FILENO, sc_buf, sc_len) != sc_len) {
    print_dbg("Error reading sc_buf.");
    return 1;
  }

  void *stack = allocate_stack();

  dup2(STDIN_FILENO, infd);
  dup2(STDOUT_FILENO, outfd);
  close(STDIN_FILENO);
#ifndef DEBUG
  close(STDOUT_FILENO);
#endif
  close(STDERR_FILENO);
  if(setup_sandbox(outfd, infd)) {
    print_dbg("Error setting up sandbox.");
    return 1;
  }

  execute_alternate(sc_buf, stack, execute_alternate);
}
