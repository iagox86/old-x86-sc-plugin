#include <assert.h>
#include <stdio.h>
#include <windows.h>

#include "util.h"
#include "Warden_binary.h"
#include "Warden_crypto.h"
#include "Warden_random.h"
#include "Warden_SHA1.h"

#define TESTS 0x1000

void test_md5()
{
	char *teststr = "This is not a test of the emergency broadcast system";
	char buffer[0x40];
	char *ptr_buffer = buffer;

	int func = 0x02E31EC2;

	__asm
	{
		push teststr
		push ptr_buffer
		call func
	}

	print_buffer(buffer, 0x40);
}

int test_simple_crypto()
{
	char buffer1[0x102];
	char buffer2[0x102];
	char base[TESTS];
	int i;

	char crypto1[] = "This is not a test Of the emergency broadcast system Where malibu fires and radio towers Conspire to dance again And I cannot believe the media Mecca They're only trying to peddle reality, Catch it on prime time, story at nine The whole world is going insane";
	char crypto2[] = "This is not a test Of the emergency broadcast system Where malibu fires and radio towers Conspire to dance again And I cannot believe the media Mecca They're only trying to peddle reality, Catch it on prime time, story at nine The whole world is going insane";
	int crypto_len = strlen(crypto1);

	for(i = 0; i < TESTS; i++)
		base[i] = i;

	printf("Testing encryption\n");

	for(i = 2; i < TESTS; i++)
	{
		generate_key(buffer1, base, i);
		generate_key_real(buffer2, base, i);

		if(memcmp(buffer1, buffer2, 0x102))
		{
			printf("Keys didn't match at length = %d\n\n", i);
			return 0;
		}

		do_crypt(buffer1, crypto1, crypto_len);
		do_crypt_real(buffer2, crypto2, crypto_len);

//		print_buffer(crypto1, crypto_len);
//		print_buffer(crypto2, crypto_len);
		if(memcmp(crypto1, crypto2, 0x102))
		{
			printf("Crypto results didn't match at length = %d\n\n", i);
			return 0;
		}
	}
	printf("Encryption passed!\n\n");

	return 1;
}

int test_key_generation()
{
	char buffer_real[0x220];
	t_warden_keys keys;
	char seed[TESTS];
	int i;

	for(i = 0; i < TESTS; i++)
		seed[i] = i;

	printf("Testing key generation...\n");
	for(i = 0; i < TESTS; i++)
	{
		generate_shared_keys_real((int*)buffer_real, seed, i);
		generate_shared_key(&keys, seed, i);

//		print_buffer(buffer_real + 0x0c, 0x102);
//		print_buffer(keys.outgoing, 0x102);
//		printf("\n\n");
//		print_buffer(buffer_real + 0x10e			, 0x102);
//		print_buffer(keys.incoming, 0x102);

		if(memcmp(buffer_real + 0x0c, keys.outgoing, 0x102) || memcmp(buffer_real + 0x10e, keys.incoming, 0x102))
		{
			printf("Key generation failed @ length = %d\n\n", i);
			return 0;
		}
	}
	printf("Key generation passed!\n\n");

	return 1;
}

int test_sha1()
{
	char buffer1[0x14];
	char buffer2[0x14];

	char data[TESTS];
	int i;

	for(i = 0; i < TESTS; i++)
		data[i] = i;

	printf("Testing SHA1\n");
	for(i = 0; i < TESTS; i++)
	{
		warden_sha1_hash((int*)buffer1, data, i);
		SHA1_hash_real((int*)buffer2, data, i);
		
		if(memcmp(buffer1, buffer2, 0x10))
		{
			printf("SHA1 failed at length = %d\n\n", i);
			return 0;
		}
	}
	printf("SHA1 Passed\n\n");

	return 1;
}

#if 0
int main(int argc, char *argv[])
{
	load_maive("C:\\temp\\Maive.bin");

	test_simple_crypto();
	test_key_generation();
	test_sha1();

	return 0;
}
#endif

#if 1

void go(char *strMod, char *name)
{
	char buf[200];
	HMODULE mod;
	FARPROC addr;

//	sprintf(buf, "C:\\windows\\system32\\%s", strMod);
//	printf("%s\n", buf);
	mod = LoadLibraryA(strMod);

	if(!mod)
	{
		printf("Error: %d\n", GetLastError());
		exit(1);
	}

	addr = GetProcAddress(mod, name);

	printf("%s.put(\"%s\", (int)0x%08x);\n", strMod, name, addr);
}

int main(int argc, char *argv[])
{
	go("USER32.dll", "ScrollWindowEx");
}
#endif