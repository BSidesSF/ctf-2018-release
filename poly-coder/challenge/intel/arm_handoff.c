#include "common.h"
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

int handoff(void *sc_buf, size_t sc_len, secret *arm_secret);
struct addrinfo *resolve(char *host_port);

#ifdef DEBUG
# define sockerr(s) perror(s)
#else
# define sockerr(s)
#endif

// Returns 0 on success, -1 on failure
int handoff(void *sc_buf, size_t sc_len, secret *arm_secret) {
  int sock;
  char *arm_host = getenv("ARM_HOST");
  if (!arm_host) {
    print_err("Some idiot has forgotten to configure the challenge.");
    return -1;
  }

  struct addrinfo *ai = resolve(arm_host);
  if (!ai) {
    print_err("Failed resolving!");
    return -1;
  }
  struct addrinfo *rp;
  for (rp = ai; rp ; rp = rp->ai_next) {
    sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sock == -1) {
      continue;
    }
    if (connect(sock, rp->ai_addr, rp->ai_addrlen) != -1)
      break; /* Success */
    close(sock);
  }
  freeaddrinfo(ai);
  if (!rp) {
    print_err("Failed to connect to ARM.");
    return -1;
  }

  // Send over the length, then the shellcode, then the secret
  if (write_exactly(sock, (void *)&sc_len, sizeof(size_t)) != sizeof(size_t)) {
    print_err("Failed sending.");
    return -1;
  }
  if (write_exactly(sock, sc_buf, sc_len) != sc_len) {
    print_err("Failed sending.");
    return -1;
  }
  if (write_exactly(sock, arm_secret, sizeof(secret)) != sizeof(secret)) {
    print_err("Failed sending.");
    return -1;
  }
  shutdown(sock, SHUT_WR);

  secret buf;
  if (read_exactly(sock, &buf, sizeof(secret)) != sizeof(secret)) {
    print_err("Failed receiving.");
    return -1;
  }

  if (memcmp(arm_secret, &buf, sizeof(secret))) {
    print_err("Invalid secret.");
    return -1;
  }
  return 0;
}

struct addrinfo *resolve(char *host_port) {
  char *port = "9999";  // default port
  char *colon = strchr(host_port, ':');
  if (colon) {
    port = colon + 1;
    *colon = '\0';
  }
  struct addrinfo *ai;
  int err;
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_ADDRCONFIG;
  if((err = getaddrinfo(host_port, port, &hints, &ai)) != 0) {
#ifdef DEBUG
    print_dbg(gai_strerror(err));
#endif
    return NULL;
  }
  return ai;
}
