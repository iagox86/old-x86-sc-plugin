#ifndef __WARDEN_BINARY_H__
#define __WARDEN_BINARY_H__

/** Actual implementations of Warden functions, to test against. These call the .dll or functions in memory. */

#include "Warden_SHA1.h" /* For SHA1_CTX */

void load_maive(char *filename);
void generate_key_real(unsigned char *key_buffer, unsigned char *base, unsigned int base_length);
void do_crypt_real(unsigned char *key, unsigned char *data, int lngth);
void generate_shared_keys_real(int *buffer, char *seed, int seed_length);


void SHA1_hash_real(int *buffer, char *data, int data_length);
void SHA1_Init_real(SHA1_CTX *ctx);
void SHA1_Update_real(SHA1_CTX *ctx, char *data, int lngth);
void SHA1_Final_real(SHA1_CTX *ctx, int *buffer);
void SHA1_Transform_real(int data, int state);

#endif