#include <stdio.h>
#include <stdint.h>
#include <gmp.h>

#include "constants.h"

#include "crypto.h"

/* 128-byte integers */
void verify(uint8_t *data, uint8_t *out) {
  mpz_t C, M, e, N;

	mpz_init(C);
  mpz_init(M);

  mpz_import(M, 128, 1, 1, 1, 0, (void*)data);
  mpz_init_set_str(e, STR_e, 16);
  mpz_init_set_str(N, STR_N, 16);
	mpz_powm(C, M, e, N);

  mpz_export(out, (size_t*)NULL, 1, 1, 1, 0, C);

	mpz_clear(C);
	mpz_clear(M);
	mpz_clear(e);
	mpz_clear(N);
}

/* 128-byte integers */
#if 0
void sign(uint8_t *data, uint8_t *out) {
  mpz_t C, M, d, N;

  mpz_init(M);
	mpz_init(C); /* TODO: populate */

  mpz_import(C, 128, 1, 1, 1, 0, (void*)data);
  mpz_init_set_str(d, STR_d, 16);
  mpz_init_set_str(N, STR_N, 16);
	mpz_powm(M, C, d, N);

  mpz_export(out, (size_t*)NULL, 1, 1, 1, 0, M);

	mpz_clear(M);
	mpz_clear(C);
	mpz_clear(d);
	mpz_clear(N);
}
#endif

void print_hex(uint8_t *data) {
  int i;

  for(i = 0; i < 128; i++) {
    printf("%02x", data[i]);
  }
  printf("\n");
}
