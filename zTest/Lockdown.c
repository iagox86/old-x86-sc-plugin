#include <windows.h>

#include "Lockdown.h"

static HMODULE ix86;
static HMODULE storm;
static HMODULE battle;

static void *addr_tweedle;
static void *addr_twitter; 
static void *addr_sha1init;
static void *addr_sha1update;
static void *addr_sha1final;
static void *addr_sha1transform;

static void *addr_calcvaluestring;
static void *addr_addrdecode;
static void *addr_checkrevision;
static void *addr_wordshifter;
static void *addr_stringcombine;

static void *addr_numbercompare;
static void *addr_noname;

void real_sha1_initialize()
{
	ix86 = LoadLibrary("c:\\temp\\lockdown-IX86-00.dll");
	storm = LoadLibrary("C:\\program files\\starcraft\\storm.dll");
	battle = LoadLibrary("C:\\program files\\starcraft\\battle.snp");
	addr_tweedle         = (void*) ((int) ix86 + 0x232e);
	addr_twitter         = (void*) ((int) ix86 + 0x2365);
	addr_sha1init        = (void*) ((int) ix86 + 0x2731);
	addr_sha1update      = (void*) ((int) ix86 + 0x275C);
	addr_sha1final       = (void*) ((int) ix86 + 0x27E8);
	addr_sha1transform   = (void*) ((int) ix86 + 0x2396);

	addr_calcvaluestring = (void*) ((int) ix86 + 0x116D);
	addr_addrdecode      = (void*) ((int) ix86 + 0x110E);
	addr_wordshifter     = (void*) ((int) ix86 + 0x14D9);
	addr_stringcombine   = (void*) ((int) ix86 + 0x1000);

	addr_numbercompare   = (void*) ((int) ix86 + 0x1C8C);
	addr_noname          = (void*) ((int) ix86 + 0x1A40);

	addr_checkrevision   = GetProcAddress(ix86, "CheckRevision");
}

void real_sha1_tweedle(int *ptr_rotator, int bitwise, int bitwise2, int bitwise3, int *ptr_adder, int *ptr_ret)
{
	__asm
	{
		mov ecx, bitwise3
		mov edx, ptr_adder
		mov esi, ptr_ret
		push bitwise2
		push bitwise
		push ptr_rotator
		call addr_tweedle
		add esp, 0x0c
	}
}

void real_sha1_twitter(int *ptr_rotator, int bitwise, int rotator2, int bitwise2, int *ptr_rotator3, int *ptr_ret)
{
	__asm
	{
		mov eax, rotator2
		mov edx, bitwise2
		mov esi, ptr_rotator3
		mov edi, ptr_ret
		push bitwise
		push ptr_rotator
		call addr_twitter
		add esp, 0x08
	}
}

void real_sha1_init(real_SHA1_CTX *ctx)
{
	__asm
	{
		mov eax, ctx
		call addr_sha1init
	}
}

void real_sha1_update(real_SHA1_CTX *ctx, char *data, int len)
{
	__asm
	{
		mov eax, data
		mov esi, ctx
		push len
//		int 3
		call addr_sha1update
	}
}

void real_sha1_final(real_SHA1_CTX *ctx, char *buffer)
{
	__asm
	{
		mov ecx, ctx
		mov edi, buffer
//		int 3
		call addr_sha1final
	}
}

void real_sha1_transform(int *data, int *state)
{
	__asm
	{
		mov eax, data
		push state
		call addr_sha1transform
		add esp, 4
	}
}

int CheckRevision(int *buf1, int *buf2, int *buf3)
{
	//int __stdcall CheckRevision(char *scfile, char *stormfile, char *battlefile,char *valuestring,int *buf1,int *buf2,int *buf3);
	char *file1 = "C:\\program files\\starcraft\\starcraft.exe";
	char *file2 = "C:\\program files\\starcraft\\storm.dll";
	char *file3 = "C:\\program files\\starcraft\\battle.snp";
	char valuestring[] = "1234567890123456";
	char *ptr_valuestring = valuestring;

	int retval;

	__asm
	{
		push buf3
		push buf2
		push buf1
		push ptr_valuestring
		push file3
		push file2
		push file1
//		int 3
		call addr_checkrevision
		mov retval, eax
	}

	return retval;
}

BOOL real_CalculateValueString(char *strn, int len, char *buffer)
{
	int retval;
	__asm
	{
		push len
		push strn
		mov esi, buffer
		//int 3
		call addr_calcvaluestring;
		add esp, 8
		mov retval, eax
	}

	return retval;
}

void real_WordShifter(short *str1,short *str2)
{
	__asm
	{
		mov ecx, str1
		mov edx, str2
		call addr_wordshifter
	}
}

int real_StringCombine(char *buf1,int *lngth,char *buf2)
{
	int retval;
	__asm
	{
		push buf2
		push lngth
		push buf1
		call addr_stringcombine
		add esp, 0x0c
		mov retval, eax
	}

	return retval;
}

int real_NumberCompare(int *num1, int *num2)
{
	int retval;
	__asm
	{
		push num2
		push num1
		call addr_numbercompare
		add esp, 8

		mov retval, eax
	}

	return retval;
}

int real_noname(char *arg1, int arg2)
{
	int retval = 0;

	__asm
	{
		push addr_numbercompare
		push arg1
		mov eax, arg2
		call addr_noname
		add esp, 8
		mov retval, eax
	}

	return retval;

}