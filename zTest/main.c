
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#include "MyLockdown.h"
#include "Lockdown.h"
#include "util.h"

HMODULE ix86;

BOOL test_tweedle()
{
	int rotator, real_rotator;
	int bitwise;
	int bitwise2;
	int bitwise3;
	int adder, real_adder;
	int ret, real_ret;
	int i;


	for(i = 0; i < 5000; i++)
	{
		rotator = real_rotator = rand();
		bitwise = rand();
		bitwise2 = rand();
		bitwise3 = rand();
		adder = real_adder = rand();
		ret = real_ret = rand();

		real_sha1_tweedle(&real_rotator, bitwise, bitwise2, bitwise3, &real_adder, &real_ret);
		sha1_tweedle(&rotator, bitwise, bitwise2, bitwise3, &adder, &ret);

		if(rotator != real_rotator || adder != real_adder || ret != real_ret)
			return FALSE;
	}
	return TRUE;
}

BOOL test_twitter()
{
	int rotator, real_rotator;
	int bitwise;
	int rotator2;
	int bitwise2;
	int rotator3, real_rotator3;
	int ret, real_ret;
	int i;

	for(i = 0; i < 5000; i++)
	{
		rotator = real_rotator = rand();
		bitwise = rand();
		rotator2 = rand();
		bitwise2 = rand();
		rotator3 = real_rotator3 = rand();
		ret = real_ret = rand();

		real_sha1_tweedle(&real_rotator, bitwise, rotator2, bitwise2, &real_rotator3, &real_ret);
		sha1_tweedle(&rotator, bitwise, rotator2, bitwise2, &rotator3, &ret);

		if(rotator != real_rotator || rotator3 != real_rotator3 || ret != real_ret)
			return FALSE;
	}
	return TRUE;
}

BOOL test_transform()
{
	int data[64];
	int state[5];
	int real_state[5];
	int i, j;

	for(i = 0; i < 5; i++)
		state[i] = real_state[i] = rand();

	for(i = 0; i < 500; i++)
	{
		for(j = 0; j < 64; j++)
			data[j] = rand();

		real_sha1_transform(data, real_state);
		sha1_transform(data, state);



		if(memcmp(state, real_state, 4))
			return FALSE;
	}

	return TRUE;
}

BOOL test_sha1()
{
	SHA1_CTX ctx;
	real_SHA1_CTX real_ctx;

	char *data = "1234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456123456789012345612345678901234561234567890123456";
	int i;
	int len = 0;
	char buf[64];
	char real_buf[64];

	sha1_init(&ctx);
	real_sha1_init(&real_ctx);
	if(memcmp(&ctx, &real_ctx, sizeof(SHA1_CTX)))
		return FALSE;

	for(i = 0; i < 5000; i++)
	{
		int len = (i + 10) % 1023;

		real_sha1_update(&real_ctx, data, len);
		sha1_update(&ctx, data, len);
		if(memcmp(&ctx, &real_ctx, sizeof(SHA1_CTX)))
			return FALSE;

		sha1_final(&ctx, (int *)buf);
		real_sha1_final(&real_ctx, real_buf);
		if(memcmp(buf, real_buf, 64))
			return FALSE;

	}

	return TRUE;
}

BOOL test_valuestring()
{
//	char str[] = "\xC3\xCE\xC1\x35\xCC\x50\xE7\xCC\x02\xAB\x6C\xDA\xAC\x71\xDE\xB6";
	char str[] = "\x07\x34\x4B\x13\x3F\x65\x1C\x87\x26\x93\x31\x0E\x8F\xCF\x34\x0D\x02";
	char real_buffer[16];
	char buffer[16];
	int i, j;
	BOOL real_goodstr;
	BOOL goodstr;

	for(i = 0; i < 10000; i++)
	{
//		printf("%d\n", i);
		for(j = 0; j < 16; j++)
			str[j] = (rand() & 0x000000FF);

		for(j = 0; j < 10; j++)
		{
			str[16] = j;

			real_goodstr = real_CalculateValueString(str, strlen(str), real_buffer);
			goodstr = CalculateValueString(str, strlen(str), buffer);

			if(real_goodstr && !goodstr)
			{
				printf("Rejected a string that the real one accepted\n");
				return FALSE;
			}
			else if(!real_goodstr && goodstr)
			{
				printf("Accepted a string that the real one rejected\n");
				return FALSE;
			}
			else if(real_goodstr && goodstr)
			{
				if(memcmp(buffer, real_buffer, 16))
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL test_stringshifter()
{
	unsigned short word1;
	unsigned short word2;

	unsigned short real_word1;
	unsigned short real_word2;

	int i;

	for(i = 0; i < 1000000; i++)
	{
		word1 = real_word1 = rand();
		word2 = real_word2 = rand();

//		printf("%04x %04x\n", word1, word2);
		real_WordShifter(&real_word1, &real_word2);
		WordShifter(&word1, &word2);

//		printf("%04x %04x\n", real_word1, real_word2);
//		printf("%04x %04x\n\n", word1, word2);

		if(word1 != real_word1)
			return FALSE;
		if(word2 != real_word2)
			return FALSE;
	}

	return TRUE;
}

BOOL test_stringcombine()
{
	char buf1[] = "0123456789abcdef";
	char buf2[] = "fedcba9876543210";
	int length = 16;
	int ret;
	char real_buf1[] = "0123456789abcdef";
	char real_buf2[] = "fedcba9876543210";
	int real_length = 16;
	int real_ret;
	int i, j;

	for(i = 0; i < 10000; i++)
	{
		length = real_length = 16;
		for(j = 0; j < 16; j++)
		{
			buf1[j] = real_buf1[j] = (rand() % 0xFE) + 1;
			buf2[j] = real_buf2[j] = (rand() % 0xFE) + 1;
		}

		real_ret = real_StringCombine(real_buf1, &real_length, real_buf2);
		ret = StringCombine(buf1, &length, buf2);

//		printf("[REAL] Ret: %08x  Length: %d\n", real_ret, real_length);
//		print_hash(real_buf1, 16);
//		print_hash(real_buf2, 16);
//		printf("\n");

//		printf("[MINE] Ret: %08x  Length: %d\n", ret, length);
//		print_hash(buf1, 16);
//		print_hash(buf2, 16);

		if(memcmp(buf1, real_buf1, 16))
			return FALSE;
		if(memcmp(buf2, real_buf2, 16))
			return FALSE;
	}
	
	return TRUE;
}

BOOL test_numbercompare()
{
	int num1;
	int num2;
	int real_num1;
	int real_num2;

	int ret;
	int real_ret;

	int i;

	for(i = 1; i < 500000; i++)
	{
		num1 = real_num1 = ((rand() % i) - (i/2));
		num2 = real_num2 = ((rand() % i) - (i/2));

//		printf("%5d %5d = %d\n", num1, num2, real_NumberCompare(&real_num1, &real_num2));
		real_ret = real_NumberCompare(&real_num1, &real_num2);
		ret = NumberCompare(&num1, &num2);

		if(real_ret != ret)
			return FALSE;
	}

	return TRUE;
}

BOOL test_noname()
{
	char arg2[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
//	char arg1[] = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
	int arg1 = (int) arg2 + 3;
//	int arg2 = 3;
	int ret;
	
	ret = real_noname(arg1, (int) arg2);

	print_hash(arg1, 32);
	print_hash(arg2, 32);
//	printf("arg2: %d\n", arg2);
	printf("ret: %d\n", ret);
	printf("\n\n");

	return FALSE;
}

void test()
{
	printf("Tweedle: %s\n",       test_tweedle()       ? "PASSED" : "FAILED!!");
	printf("Twotter: %s\n",       test_twitter()       ? "PASSED" : "FAILED!!");
	printf("Transform: %s\n",     test_transform()     ? "PASSED" : "FAILED!!");
	printf("SHA1: %s\n",          test_sha1()          ? "PASSED" : "FAILED!!");
	printf("ValueString: %s\n",   test_valuestring()   ? "PASSED" : "FAILED!!");
	printf("StringShifter: %s\n", test_stringshifter() ? "PASSED" : "FAILED!!");
	printf("StringCombine: %s\n", test_stringcombine() ? "PASSED" : "FAILED!!");
	printf("NumberCompare: %s\n", test_numbercompare() ? "PASSED" : "FAILED!!");
	printf("NoName: %s\n", test_noname() ? "PASSED" : "FAILED!!");
}

int main(int argc, char **argv)
{
	int a, b, c;
	int retval;

	real_sha1_initialize();
	srand(31415926);

	retval = CheckRevision(&a, &b, &c);

	printf("%08x %08x %08x | %08x\n", a, b, c, retval);

	test();

	return 0;
}