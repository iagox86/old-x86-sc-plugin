#ifndef __WARDEN_H__
#define __WARDEN_H__

#define SWAP(a,b) ((a == b) ? (a=a) : ((char)a ^= (char)b ^= (char)a ^= (char)b))

/** Generates the key based on "base" */
void generate_key(unsigned char *key_buffer, unsigned char *base, unsigned int base_length);
void do_crypt(unsigned char *key, unsigned char *data, int length);

#endif