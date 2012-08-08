#include "Warden.h"



/** Generates the key based on "base" */
void generate_key(unsigned char *key_buffer, unsigned char *base, unsigned int base_length)
{
	unsigned char val = 0;
	unsigned int i;
	unsigned int position = 0;

	for(i = 0; i < 0x100; i++)
		key_buffer[i] = i;

	key_buffer[0x100] = 0;
	key_buffer[0x101] = 0;

	for(i = 1; i <= 0x40; i++)
	{
		val += key_buffer[(i * 4) - 4] + base[position++ % base_length];
		SWAP(key_buffer[(i * 4) - 4], key_buffer[val & 0x0FF]);

		val += key_buffer[(i * 4) - 3] + base[position++ % base_length];
		SWAP(key_buffer[(i * 4) - 3], key_buffer[val & 0x0FF]);

		val += key_buffer[(i * 4) - 2] + base[position++ % base_length];
		SWAP(key_buffer[(i * 4) - 2], key_buffer[val & 0x0FF]);

		val += key_buffer[(i * 4) - 1] + base[position++ % base_length];
		SWAP(key_buffer[(i * 4) - 1], key_buffer[val & 0x0FF]);
	}
}

void do_crypt(unsigned char *key, unsigned char *data, int length)
{
	int i;

	for(i = 0; i < length; i++)
	{
		key[0x100]++;
		key[0x101] += key[key[0x100]];
		SWAP(key[key[0x101]], *((key[0x100]) + key));

		data[i] = data[i] ^ key[(key[key[0x101]] + key[key[0x100]]) & 0x0FF];
	}
}