#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Assembly.h"
#include "Warden_SHA1.h" /* For SHA1_CTX */

void load_maive(char *filename)
{
	FILE *file;
	char buffer[0x10000];
	size_t length;
	void *memory;

	fopen_s(&file, filename, "rb");
	length = fread(buffer, 1, 0x10000, file);
	printf("Length of Maive.bin = %d bytes\n", length);
	memory = VirtualAlloc((void*)0x2e30000, 0x10000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	printf("Got memory @ %p (has to be 0x02e30000)\n\n", memory);
	memcpy((void*)0x2e31000, buffer, length);

	memset((void*)0x02E33010, 0x00, 0x40);
	memset((void*)0x02E33010, 0x80, 0x01);
}

void generate_shared_keys_real(int *buffer, char *seed, int seed_length)
{
	char *ret = "\x33\xc0\xc2\x08\x00"; /* ret 08h */
	int ret_array[] = { 0, 0, 0, 0, 0, 0, (int)ret, 0, 0, 0, 0 };
	char *ptr_ret_array = (char*)&ret_array;
	void *func = (void*)0x02e32893;

	buffer[0] = 0;
	buffer[1] = (int) &ptr_ret_array;
	buffer[2] = 0;

	__asm
	{
		mov ecx, buffer
		push seed_length
		push seed
		call func
	}
}

void SHA1_hash_real(int *buffer, char *data, int data_length)
{
	int func = 0x02E323BC;

	__asm
	{
		push data_length
		push data
		push buffer
		call func
	}
}

void SHA1_Init_real(SHA1_CTX *ctx)
{
	int func = 0x02E31F26;

	__asm
	{
		mov eax, ctx
		call func
	}
}

void SHA1_Update_real(SHA1_CTX *ctx, char *data, int lngth)
{
	int func = 0x02E3226A;

	__asm
	{
		mov eax, data
		mov esi, ctx
		push lngth
		call func
	}
}

void SHA1_Final_real(SHA1_CTX *ctx, int *buffer)
{
	int func = 0x02E32300;

	__asm
	{
		mov eax, ctx
		push buffer
		call func
	}
}

void SHA1_Transform_real(int data, int state)
{
	int func = 0x02E31F51;

	__asm
	{
		mov ebx, state
		push data
		call func
	}
}

void generate_key_real(unsigned char *key_buffer, unsigned char *base, unsigned int base_length)
{
	int func = 0x02E31473;

	__asm
	{
		mov esi, key_buffer
		push base_length
		push base
		call func
	}
}

void do_crypt_real(unsigned char *key, unsigned char *data, int lngth)
{
	int func = 0x02E31410;

	__asm
	{
		mov eax, key
		push lngth
		push data

		call func
	}
}