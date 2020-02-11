#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/mman.h>

#include "crypto.h"
#include "flag.h"

#define WELCOME        ("Welcome to our special beta server! Please verify yourself with a 0x02 message!")
#define UNAUTHORIZED   ("Sorry, please verify yourself first!")
#define UNKNOWN        ("Unknown opcode!")
#define VERIFIED       ("Verified! Send an opcode 0x03 message for the flag!")
#define NOT_VERIFIED   ("Verification failed!")
#define BAD_SIGNATURE  ("Bad signature!")
#define BAD_HEADER     ("There was something wrong with the packet's header!")
#define BAD_PACKET_IN  ("There was something wrong with the incoming packet!")
#define BAD_PACKET_OUT ("There was something wrong with the outgoing packet!")

#define DEBUG 1

#define disable_buffering(_fd) setvbuf(_fd, NULL, _IONBF, 0)

void do_send(uint8_t code, uint8_t *data, uint16_t length)
{
  uint8_t *buffer;

  /* If the length is too long, just die */
  if(length > 0xFF00)
  {
    do_send(0x01, (uint8_t*)BAD_PACKET_OUT, strlen(BAD_PACKET_OUT) + 1);
    exit(1);
  }

  buffer = (uint8_t *) malloc(length + 4);

  /* Header byte */
  buffer[0] = (char) 0xFF;

  /* Packet code */
  buffer[1] = (char) code;

  /* Length */
  buffer[2] = ((length + 4) << 0) & 0xFF;
  buffer[3] = ((length + 4) << 8) & 0xFF;

  /* Data */
  memcpy(buffer+4, data, length);

  write(fileno(stdout), buffer, length+4);

  free(buffer);
}

uint16_t do_recv(uint8_t *code, uint8_t **data)
{
  uint8_t header[4];
  uint16_t length;

  ssize_t len = read(fileno(stdin), header, 4);

  if(len == 0) {
    exit(0);
  }

  if(len != 4) {
    do_send(0x01, (uint8_t*)BAD_HEADER, strlen(BAD_HEADER) + 1);
    exit(1);
  }

  if(header[0] != 0xFF) {
    do_send(0x01, (uint8_t*)BAD_HEADER, strlen(BAD_HEADER) + 1);
    exit(1);
  }

  *code = header[1];
  length = (header[2] << 0) |
           (header[3] << 8);


  if(length < 4) {
    do_send(0x01, (uint8_t*)BAD_PACKET_IN, strlen(BAD_PACKET_IN) + 1);
    exit(1);
  }

  // We don't want to receive the header again
  length -= 4;

#if DEBUG
  printf("\n");
  printf("Length1 = 0x%02x\n", header[2]);
  printf("Length2 = 0x%02x\n", header[3]);
  printf("Length = 0x%04x\n", length);
#endif

  *data = (uint8_t*) malloc(length);
  len = read(fileno(stdin), *data, length);

  if(len != length) {
    // TODO: Error?
    do_send(0x01, (uint8_t*)BAD_PACKET_IN, strlen(BAD_PACKET_IN) + 1);
    exit(1);
  }

  return len;
}

int check_ip(uint8_t *ip, uint8_t *signature) {
  uint8_t buffer[128];
  verify(signature, buffer);
  return !memcmp(buffer, ip, 4);
}

int main(int argc, char *argv[])
{
  disable_buffering(stdin);
  disable_buffering(stdout);
  disable_buffering(stderr);

  char *str_ip = getenv("REMOTE_HOST");
  uint8_t ip[4];
  uint8_t is_verified = 0;

  inet_pton(AF_INET, str_ip, &ip);

  if(!str_ip) {
    //fprintf(stderr, "Couldn't determine your ip!\n");
    exit(1);
  }

  //fprintf(stderr, "Your IP is: %d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);

  alarm(30);

  srand(time(NULL));

  do_send(0x01, (uint8_t*)WELCOME, strlen(WELCOME) + 1);

  for(;;) {
    uint8_t code;
    uint16_t length;
    uint8_t *data;

    length = do_recv(&code, &data);

    printf("Received packet type 0x%02x", code);

    switch(code) {
      case 0x01: /* MESSAGE */
        printf("Message from client: %s\n", data);
        break;

      case 0x02: /* VALIDATE */
        if(length != 128) {
          do_send(0x01, (uint8_t*)BAD_SIGNATURE, strlen(BAD_SIGNATURE) + 1);
          break;
        }

        if(check_ip(ip, data)) {
          do_send(0x01, (uint8_t*)VERIFIED, strlen(VERIFIED) + 1);
          is_verified = 1;
#if DEBUG
          printf("\n**User is verified!\n");
#endif
        } else {
          do_send(0x01, (uint8_t*)NOT_VERIFIED, strlen(NOT_VERIFIED) + 1);
        }
        break;

      case 0x03: /* GETKEY */
        if(!is_verified) {
          do_send(0x01, (uint8_t*)UNAUTHORIZED, strlen(UNAUTHORIZED) + 1);
        } else {
          do_send(0x03, (uint8_t*)FLAG, strlen(FLAG) + 1);
        }
        break;

      default: /* Unknown */
        do_send(0x01, (uint8_t*)UNKNOWN, strlen(UNKNOWN) + 1);
        break;
    }

    free(data);
  }

  return 0;
}
