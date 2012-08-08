#include <stdio.h>

#include "util.h"

void reverse_endian(int *val, int *buffer)
{
	*buffer = ((*val & 0x000000FF) << 24) | ((*val & 0x0000FF00) << 8) | ((*val & 0x00FF0000) >> 8) | ((*val & 0xFF000000) >> 24);
}

void print_buffer(unsigned char *buffer, int length)
{
	int i;

	for(i = 0; i < length; i++)
	{
		if(!(i % 16))
			printf("\n");
		printf("%02x ", buffer[i] & 0x0FF);
	}
	printf("\n\n");
}

void print_buffer_ascii(unsigned char *buffer, int length)
{
	int i;

	for(i = 0; i < length; i++)
	{
		if(buffer[i] < 0x20 || buffer[i] > 0x7F)
			printf(".");
		else
			printf("%c", buffer[i]);
	}
	printf("\n\n");
}
