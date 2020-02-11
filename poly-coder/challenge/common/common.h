#include <stdio.h>
#include <stdlib.h>
#include <seccomp.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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

#define PIPE_READ 0
#define PIPE_WRITE 1

typedef char secret[32];
typedef struct {
  int to_child[2];
  int from_child[2];
} pipes;

// This is provided by assembly
void execute_alternate(void *execp, void *stack, void *ptr);

size_t round_next_page(size_t size);
void *allocate_stack();
void *allocate_with_guard(size_t size, int prot);
int setup_sandbox(int outfd, int infd);
int make_pipes(pipes *p);
int write_exactly(int fd, void *buf, size_t n);
int read_exactly(int fd, void *buf, size_t n);
int random_fd();
