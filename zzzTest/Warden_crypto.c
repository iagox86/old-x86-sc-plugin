#include <string.h>

#include "util.h"
#include "Warden_crypto.h"



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
		SWAP(key[key[0x101]], key[key[0x100]]);

		data[i] = data[i] ^ key[(key[key[0x101]] + key[key[0x100]]) & 0x0FF];
	}
}

void generate_shared_key(t_warden_keys *keys, char *seed, int seed_length)
{
	t_random_data source;
	char random_outgoing[0x10];
	char random_incoming[0x10];

	memset(keys, 0, sizeof(t_warden_keys));

	random_data_initialize(&source, seed, seed_length);
	random_data_get_bytes(&source, random_outgoing, 0x10);
	random_data_get_bytes(&source, random_incoming, 0x10);

	generate_key(keys->outgoing, random_outgoing, 0x10);
	generate_key(keys->incoming, random_incoming, 0x10);
}

#if 0
int main(int argc, char *argv[])
{
	char key[0x102];
	char str[] = "This is not a test Of the emergency broadcast system Where malibu fires and radio towers Conspire to dance again And I cannot believe the media Mecca They're only trying to peddle reality, Catch it on prime time, story at nine The whole world is going insane";
	int len = strlen(str);
	char base[] = "This is my key. Don't hurt it!!";

	generate_key(key, base, strlen(base));
	do_crypt(key, str, len);
	print_buffer(str, len);
}
#endif

#if 0
int main(int argc, char *argv[])
{
	t_warden_keys test;
	char *seed = "\x11\x22\x33\x44\x55";

	generate_shared_key(&test, seed, 5);
	printf("Outbound key:\n");
	print_buffer(test.outgoing, 0x102);
	printf("\n\n");
	printf("Inbound key:\n");
	print_buffer(test.incoming, 0x102);

}
#endif

#if 0
int main(int argc, char *argv[])
{
	t_warden_keys test;
	char *seed  = "\xd7\x66\xad\x0e";
	char incoming_data[] = "\x41\x5f\xb6\xe8\xf5\xb4\xab\x94\xaa\xc2\x4f\x73\x32\x7d\x1a\x4a\x39\x6e\xe2\x29\x2a\xfc\x73\x7f\x40\x3c\xc4\xf6\x4b\xbb\x11\x9d\x63\x5d\x83\xc1\xb7";
	char outgoing_data[] = "\xc6";
	generate_shared_key(&test, seed, 4);
	do_crypt(test.incoming, incoming_data, 37);
	do_crypt(test.outgoing, outgoing_data, 1);
	print_buffer(incoming_data, 37);
	print_buffer(outgoing_data, 1);



	return 0;
}
#endif