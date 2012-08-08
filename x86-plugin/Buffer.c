#include "Buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

t_buffer *buffer_initialize()
{
	t_buffer *buffer = (t_buffer *) malloc(sizeof(t_buffer));
	
	if(!buffer)
	{
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	memset(buffer, 0, sizeof(t_buffer));

	buffer->current_length = 0;
	buffer->max_length = BUFFER_INITIAL_LENGTH;
	buffer->data = (char*) malloc(sizeof(char) * buffer->max_length);

	return buffer;
}

void buffer_destroy(t_buffer *buffer)
{
	if(buffer->data)
		free(buffer->data);

	memset(buffer, 0, sizeof(t_buffer));
	free(buffer);
}

void buffer_allocate(t_buffer *buffer, int amount)
{
	char *newdata;

	if(buffer->current_length + amount < buffer->current_length)
	{
		fprintf(stderr, "Buffer got too long: %d\n", buffer->current_length);
		exit(1);
	}

	while(buffer->current_length + amount >= buffer->max_length)
	{
		if(buffer->max_length * 2 < buffer->max_length)
		{
			fprintf(stderr, "Buffer got too long: %d\n", buffer->max_length);
			exit(1);
		}
		buffer->max_length *= 2;
		newdata = (char *) malloc(sizeof(char) * buffer->max_length);
		memcpy(newdata, buffer->data, buffer->current_length);
		memset(buffer->data, 0, buffer->current_length);
		free(buffer->data);
		buffer->data = newdata;
	}
}

t_buffer *buffer_insert_byte(t_buffer *buffer, int byte)
{
	buffer_allocate(buffer, 1);
	buffer->data[buffer->current_length++] = (char) (byte & 0x000000FF);

	return buffer;
}

t_buffer *buffer_insert_word(t_buffer *buffer, int word)
{
	buffer_insert_byte(buffer, (word >> 0) & 0x000000FF);
	buffer_insert_byte(buffer, (word >> 8) & 0x000000FF);
		
	return buffer;
}

t_buffer *buffer_insert_dword(t_buffer *buffer, int dword)
{
	buffer_insert_byte(buffer, (dword >> 0)  & 0x000000FF);
	buffer_insert_byte(buffer, (dword >> 8)  & 0x000000FF);
	buffer_insert_byte(buffer, (dword >> 16) & 0x000000FF);
	buffer_insert_byte(buffer, (dword >> 24) & 0x000000FF);

	return buffer;
}

t_buffer *buffer_insert_ntstring(t_buffer *buffer, char *string)
{
	buffer_insert_bytes(buffer, string, (int) strlen(string) + 1);

	return buffer;
}

t_buffer *buffer_insert_bytes(t_buffer *buffer, char *bytes, int count)
{
	int i;

	for(i = 0; i < count; i++)
		buffer_insert_byte(buffer, bytes[i]);
	return buffer;
}

t_buffer *buffer_insert_buffer(t_buffer *buffer, t_buffer *src)
{
	return buffer_insert_bytes(buffer, src->data, src->current_length);
}

char *buffer_get_cstring(t_buffer *buffer)
{
	return buffer->data;
}

int buffer_get_length(t_buffer *buffer)
{
	return buffer->current_length;
}

char *buffer_to_string(t_buffer *buffer)
{
	char *str;

	t_buffer *string = buffer_initialize();
	char temp[65];
	int i;
	int j;

	for(i = 0; i < (buffer->current_length / 16); i++)
	{
		sprintf_s(temp, 64, "%08x ", i * 16);
		buffer_insert_bytes(string, temp, 9);
		for(j = 0; j < 16; j++)
		{
			sprintf_s(temp, 64, "%02x ", buffer->data[(i * 16) + j]);
			buffer_insert_bytes(string, temp, 3);
		}
		buffer_insert_bytes(string, "    ", 4);
		for(j = 0; j < 16; j++)
		{
			unsigned char byte = buffer->data[(i * 16) + j];
			if(byte < 0x20 || byte > 0x7F)
				buffer_insert_byte(string, '.');
			else
				buffer_insert_byte(string, byte);
		}
		buffer_insert_bytes(string, "\r\n", 2);
	}

	sprintf_s(temp, 64, "%08x ", (buffer->current_length / 16) * 16);
	buffer_insert_bytes(string, temp, 9);
	for(i = (buffer->current_length / 16) * 16; i < buffer->current_length; i++)
	{
		sprintf_s(temp, 64, "%02x ", buffer->data[i]);
		buffer_insert_bytes(string, temp, 3);
	}

	for(i = 0; i < ((((buffer->current_length / 16) * 16) + 16) - buffer->current_length) * 3 + 4; i++)
		buffer_insert_byte(string, ' ');

	for(i = (buffer->current_length / 16) * 16; i < buffer->current_length; i++)
	{
		unsigned char byte = buffer->data[i];
		if(byte < 0x20 || byte > 0x7F)
			buffer_insert_byte(string, '.');
		else
			buffer_insert_byte(string, byte);
	}
	buffer_insert_bytes(string, "\r\n", 2);
	sprintf_s(temp, 64, "Length: %d (allocated: %d)\r\n", buffer->current_length, buffer->max_length);
	buffer_insert_bytes(string, temp, (int) strlen(temp));
	buffer_insert_byte(string, '\0');

	str = malloc(sizeof(char) * string->current_length + 1);
	memcpy(str, string->data, string->current_length);

	return str;
}

char *buffer_gettipaddress(t_buffer *buffer)
{
	return buffer->data + buffer->current_length;
}

void buffer_clear(t_buffer *buffer)
{
	memset(buffer->data, 0, buffer->current_length);
	buffer->current_length = 0;
}

void buffer_print(t_buffer *buffer)
{
	char *str = buffer_to_string(buffer);
	printf("%s", str);
	free(str);
}

/*int main(int argc, char **argv)
{
	t_buffer *test = buffer_initialize();

	buffer_insert_dword(test, 0x00010203);
	buffer_insert_dword(test, 0x04050607);
	buffer_insert_dword(test, 0x08090a0b);
	buffer_insert_dword(test, 0x0c0d0e0f);

	buffer_insert_dword(test, 0x00010203);
	buffer_insert_dword(test, 0x04050607);
	buffer_insert_dword(test, 0x08090a0b);
	buffer_insert_dword(test, 0x0c0d0e0f);

	buffer_insert_dword(test, 0x00010203);
	buffer_insert_dword(test, 0x04050607);

	buffer_insert_ntstring(test, "This is a test string that should be printed in plaintext in the buffer thing!");

	buffer_print(test);

	system("pause");

	return 0;
}*/