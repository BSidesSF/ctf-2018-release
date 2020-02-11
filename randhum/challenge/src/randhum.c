#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <signal.h>
#include <ucontext.h>

#define LENGTH 4096
#define disable_buffering(_fd) setvbuf(_fd, NULL, _IONBF, 0)

// $ dd if=/dev/urandom of=/tmp/rngseed bs=4096 count=1
void do_encrypt(uint8_t *data, size_t length)
{
  FILE *f = fopen("/tmp/rngseed", "rb");
  size_t i;

  for(i = 0; i < length; i++) {
    data[i] = data[i] ^ fgetc(f);
  }

  fclose(f);
}

void signal_handler(int signal)
{
  printf("Whoops! Crashed with signal %d\n", signal);
  exit(0);
}

int main(int argc, char *argv[])
{
  uint8_t *buffer = mmap(NULL, LENGTH, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
  ssize_t len;

  alarm(30);

  signal(SIGSEGV, signal_handler);
  signal(SIGTRAP, signal_handler);
  signal(SIGKILL, signal_handler);

  disable_buffering(stdin);
  disable_buffering(stdout);
  disable_buffering(stderr);

  printf("Send me stuff!!\n");
  len = read(0, buffer, LENGTH);

  if(len < 0) {
    printf("Error reading!\n");
    exit(1);
  }

  do_encrypt(buffer, len);

  printf("%02x %02x\n", buffer[0], buffer[1]);

  printf("Returned 0x%08x!\n", ((uint (*)(void)) buffer)());

  return 0;
}
