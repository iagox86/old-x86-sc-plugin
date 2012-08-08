#ifndef __WARDEN_KEYS__
#define __WARDEN_KEYS__

typedef struct 
{
	int current_position;
	char random_data[0x14];
	char random_source_1[0x14];
	char random_source_2[0x14];
} t_random_data;

void random_data_initialize(t_random_data *source, char *seed, int length);
void random_data_update(t_random_data *source);
char random_data_get_byte(t_random_data *source);
void random_data_get_bytes(t_random_data *source, char *buffer, int bytes);
void random_data_create_keys(char *buffer, char *seed, int seed_length);

#endif