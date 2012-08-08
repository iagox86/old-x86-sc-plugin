#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Warden_SHA1.h"

#include "Warden_random.h"


void random_data_update(t_random_data *source)
{
	SHA1_CTX ctx;
	warden_sha1_init(&ctx);
	warden_sha1_update(&ctx, source->random_source_1, 0x14);
	warden_sha1_update(&ctx, source->random_data,     0x14);
	warden_sha1_update(&ctx, source->random_source_2, 0x14);
	warden_sha1_final(&ctx,  (int*) source->random_data);
}

void random_data_initialize(t_random_data *source, char *seed, int length)
{
	int length1 = length >> 1;
	int length2 = length - length1;

	char *seed1 = seed;
	char *seed2 = seed + length1;

	memset(source, 0, sizeof(t_random_data));

	warden_sha1_hash((int*)source->random_source_1, seed1, length1);
	warden_sha1_hash((int*)source->random_source_2, seed2, length2);
	random_data_update(source);

	source->current_position = 0;
}

char random_data_get_byte(t_random_data *source)
{
	int i = source->current_position;
	char value = source->random_data[i];

	i++;
	if(i >= 0x14)
	{
		i = 0;
		random_data_update(source);
	}
	source->current_position = i;

	return value;
}

void random_data_get_bytes(t_random_data *source, char *buffer, int bytes)
{
	int i;

	for(i = 0; i < bytes; i++)
		buffer[i] = random_data_get_byte(source);
}

#if 0

#include "util.h"

int main(int argc, char **argv)
{
	char *seed = "\x11\x22\x33\x44\x55";
	t_random_data source;
	char buffer[0x100];

	random_data_initialize(&source, seed, 5);
	random_data_get_bytes(&source, buffer, 0x100);
	print_buffer(buffer, 0x100);
}


#endif