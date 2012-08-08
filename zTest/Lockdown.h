#ifndef __LOCKDOWNSHA1_H__
#define __LOCKDOWNSHA1_H__

typedef struct
{
	int bitlen[2];
	int state[32];
} real_SHA1_CTX;

/* This has to be called exactly once. I don't check for it. */
void real_sha1_initialize();

void real_sha1_tweedle(int *ptr_rotator, int bitwise, int bitwise2, int bitwise3, int *ptr_adder, int *ptr_ret);
void real_sha1_twitter(int *ptr_rotator, int bitwise, int rotator2, int bitwise2, int *ptr_rotator3, int *ptr_ret);
void real_sha1_init(real_SHA1_CTX *ctx);
void real_sha1_update(real_SHA1_CTX *ctx, char *data, int len);
void real_sha1_final(real_SHA1_CTX *ctx, char *buffer);
void real_sha1_transform(int *data, int *state);

int DecodeModule(HMODULE storm, int num);
int CheckRevision(int *buf1, int *buf2, int *buf3);

BOOL real_CalculateValueString(char *str, int len, char *buffer);
void real_WordShifter(short *str1, short *str2);
int real_StringCombine(char *buf1,int *length,char *buf2);
int real_noname(int *arg1, int arg2);

int real_NumberCompare(int *num1, int *num2);

#endif