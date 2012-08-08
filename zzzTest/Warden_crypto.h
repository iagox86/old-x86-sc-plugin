#ifndef __WARDEN_CRYPTO_H__
#define __WARDEN_CRYPTO_H__

#include "Warden_random.h"

typedef struct
{
	char outgoing[0x104];
	char incoming[0x104];
} t_warden_keys;

/** Generates a 0x100-byte key based on the provided base. */
void generate_key(unsigned char *key_buffer, unsigned char *base, unsigned int base_length);
void do_crypt(unsigned char *key, unsigned char *data, int length);

void generate_shared_key(t_warden_keys *key, char *seed, int seed_length);

#endif