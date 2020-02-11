#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <sys/mman.h>

int sha1_compare(uint8_t *file1, uint8_t *file2, size_t length)
{
  SHA_CTX sha_ctx;
  uint8_t digest1[SHA_DIGEST_LENGTH];
  uint8_t digest2[SHA_DIGEST_LENGTH];

  SHA1_Init(&sha_ctx);
  SHA1_Update(&sha_ctx, file1, length);
  SHA1_Final(digest1, &sha_ctx);

  SHA1_Init(&sha_ctx);
  SHA1_Update(&sha_ctx, file2, length);
  SHA1_Final(digest2, &sha_ctx);

  return !memcmp(digest1, digest2, SHA_DIGEST_LENGTH);
}

int md5_compare(uint8_t *file1, uint8_t *file2, size_t length)
{
  MD5_CTX md5_ctx;
  uint8_t digest1[MD5_DIGEST_LENGTH];
  uint8_t digest2[MD5_DIGEST_LENGTH];

  MD5_Init(&md5_ctx);
  MD5_Update(&md5_ctx, file1, length);
  MD5_Final(digest1, &md5_ctx);

  MD5_Init(&md5_ctx);
  MD5_Update(&md5_ctx, file2, length);
  MD5_Final(digest2, &md5_ctx);

  return !memcmp(digest1, digest2, MD5_DIGEST_LENGTH);
}

int main(int argc, char *argv[])
{
  uint16_t length;
  uint8_t *file1;
  uint8_t *file2;

  printf("Please send length as an 16-bit little endian value\n");
  read(0, &length, 2);
  printf("Length = %d\n", length);

  file1 = mmap((void*)0x41410000, length, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
  read(0, file1, length);

  file2 = mmap((void*)0x41410000, length, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
  read(0, file2, length);

  if(!memcmp(file1, file2, length)) {
    printf("ERROR! The files can't be the same!\n");
    abort();
  }

  if(sha1_compare(file1, file2, length)) {
    printf("sha1 checks out!\n");
    if(md5_compare(file1, file2, length)) {
      printf("Congratulations!\n");
      asm("call *%0\n" : :"r"(file1));
    }
  }

  printf("Sorry :(\n");

  return 0;
}
