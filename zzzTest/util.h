#ifndef __UTIL_H__
#define __UTIL_H__

#define SWAP(a,b) ((a == b) ? (a=a) : ((char)a ^= (char)b ^= (char)a ^= (char)b))

void reverse_endian(int *val, int *buffer);
void print_buffer(unsigned char *buffer, int length);
void print_buffer_ascii(unsigned char *buffer, int length);

#endif