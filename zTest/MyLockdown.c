#include <windows.h>

#include "MyLockdown.h"

void sha1_tweedle(int *ptr_rotator, int bitwise, int bitwise2, int bitwise3, int *ptr_adder, int *ptr_ret)
{
    *ptr_ret = *ptr_ret + (((RotateLeft32(bitwise3, 5)) + ( (~(*ptr_rotator) & bitwise2) | (*ptr_rotator & bitwise))) + *ptr_adder + 0x5A827999);
	*ptr_adder = 0;
	*ptr_rotator = RotateLeft32(*ptr_rotator, 0x1e);
}

void sha1_twitter(int *ptr_rotator, int bitwise, int rotator2, int bitwise2, int *ptr_rotator3, int *ptr_ret)
{
	*ptr_ret = *ptr_ret + ((((bitwise2 | bitwise) & *(ptr_rotator)) | (bitwise2 & bitwise)) + ((RotateLeft32(rotator2, 5)) + *ptr_rotator3) - 0x70e44324);
	*ptr_rotator3 = 0;
	*ptr_rotator = RotateLeft32(*ptr_rotator, 0x1e);
}

void sha1_init(SHA1_CTX *ctx)
{
	ctx->bitlen[0] = 0;
	ctx->bitlen[1] = 0;
	ctx->state[0]  = 0x67452301;
	ctx->state[1]  = 0xEFCDAB89;
	ctx->state[2]  = 0x98BADCFE;
	ctx->state[3]  = 0x10325476;
	ctx->state[4]  = 0xC3D2E1F0;
}

void sha1_update(SHA1_CTX *ctx, char *data, int len)
{
	int *bitlen = ctx->bitlen;
	char *state = (char *) ctx->state;
	int a;
	int b;
	int c;
	int i;

	/* The next two lines multiply len by 8. */
	c = len >> 29;
	b = len << 3;

	a = (bitlen[0] / 8) & 0x3F;

	/* Check for overflow. */
	if(bitlen[0] + b < bitlen[0] || bitlen[0] + b < b)
		bitlen[1]++;
	bitlen[0] = bitlen[0] + b;
	bitlen[1] = bitlen[1] + c;

	len = len + a;
	data = data - a;

	if(len >= 0x40)
	{
		if(a)
		{
			while(a < 0x40)
			{
				state[0x14 + a] = data[a];
				a++;
			}

			sha1_transform((int *) (state + 0x14), (int *) state);
			len -= 0x40;
			data += 0x40;
			a = 0;
		}

		if(len >= 0x40)
		{
			b = len;
			for(i = 0; i < b / 0x40; i++)
			{
				sha1_transform((int *) data, (int *) state);
				len -= 0x40;
				data += 0x40;
			}
		}
	}
	
	for(; a < len; a++)
		state[a + 0x1c - 8] = data[a];

	return;
}

void sha1_final(SHA1_CTX *ctx, int *hash)
{
	int i;
	int vars[2];
	char *MysteryBuffer = "\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";

	vars[0] = ctx->bitlen[0];
	vars[1] = ctx->bitlen[1];

	sha1_update(ctx, MysteryBuffer, (-((ctx->bitlen[0] >> 3 | ctx->bitlen[1] << 29) + 9) & 0x3f) + 1);
	sha1_update(ctx, (char *)vars, 8);

	for(i = 0; i < 5; i++)
		hash[i] = ctx->state[i];
}

void sha1_transform(int *data, int *state)
{
	int a, b, c, d, e, f, g, h, m, n;
	int i;

	int buf[80];

	memcpy(buf, data, 0x40);

	for(i = 0; i < 0x40; i++)
		buf[i + 16] = RotateLeft32(buf[i + 13] ^ buf[i + 8] ^ buf[i + 0] ^ buf[i + 2], 1);

	m = state[0];
	b = state[1];
	c = state[2];
	n = state[3];
	e = state[4];

	for(i = 0; i < 20; i += 5)
	{
		sha1_tweedle(&b, c, n, m, &buf[0 + i], &e);
		sha1_tweedle(&m, b, c, e, &buf[1 + i], &n);
		sha1_tweedle(&e, m, b, n, &buf[2 + i], &c);
		sha1_tweedle(&n, e, m, c, &buf[3 + i], &b);
		sha1_tweedle(&c, n, e, b, &buf[4 + i], &m);
	}

	f = m;
	d = n;

	for(i = 0x14; i < 0x28; i += 5)
	{
		g =  buf[i] + RotateLeft32(f, 5) + (d ^ c ^ b);
		d = d + RotateLeft32(g + e + 0x6ed9eba1, 5) + (c ^ RotateLeft32(b, 0x1e) ^ f) + buf[i + 1] + 0x6ed9eba1;
		c = c + RotateLeft32(d, 5) + ((g + e + 0x6ed9eba1) ^ RotateLeft32(b, 0x1e) ^ RotateLeft32(f, 0x1e)) + buf[i + 2] + 0x6ed9eba1;
		e = RotateLeft32(g + e + 0x6ed9eba1, 0x1e);
		b = RotateLeft32(b, 0x1e) + RotateLeft32(c, 5) + (e ^ d ^ RotateLeft32(f, 0x1e)) + buf[i + 3] + 0x6ed9eba1;
		d = RotateLeft32(d, 0x1e);
		f = RotateLeft32(f, 0x1e) + RotateLeft32(b, 5) + (e ^ d ^ c) + buf[i + 4] + 0x6ed9eba1;
		c = RotateLeft32(c, 0x1e);

		memset(buf, 0, 20);

	} while(i < 0x28);

	m = f;
	n = d;
	
	for(i = 0x28; i < 0x3c; i += 5)
	{
		sha1_twitter(&b, n, m, c, &buf[i + 0], &e);
		sha1_twitter(&m, c, e, b, &buf[i + 1], &n);
		sha1_twitter(&e, b, n, m, &buf[i + 2], &c);
		sha1_twitter(&n, m, c, e, &buf[i + 3], &b);
		sha1_twitter(&c, e, b, n, &buf[i + 4], &m);
	} 

	f = m;
	a = m;
	d = n;

	for(i = 0x3c; i < 0x50; i += 5)
	{
		g = RotateLeft32(a, 5) + (d ^ c ^ b) + buf[i + 0] + e - 0x359d3e2a;
		b = RotateLeft32(b, 0x1e);
		e = g;
		d = (c ^ b ^ a) + buf[i + 1] + d + RotateLeft32(g, 5) - 0x359d3e2a;
		a = RotateLeft32(a, 0x1e);
		g = RotateLeft32(d, 5);
		g = (e ^ b ^ a) + buf[i + 2] + c + g - 0x359d3e2a;
		e = RotateLeft32(e, 0x1e);
		c = g;
		g = RotateLeft32(g, 5) + (e ^ d ^ a) + buf[i + 3] + b - 0x359d3e2a;
		d = RotateLeft32(d, 0x1e);
		h = (e ^ d ^ c) + buf[i + 4];
		b = g;
		g = RotateLeft32(g, 5);
		c = RotateLeft32(c, 0x1e);
		a = (h + a) + g - 0x359d3e2a;

		buf[i + 0] = 0;
		buf[i + 1] = 0;
		buf[i + 2] = 0;
		buf[i + 3] = 0;
		buf[i + 4] = 0;
	} while(i < 0x50);

	state[0] = state[0] + a;
	state[1] = state[1] + b;
	state[2] = state[2] + c;
	state[3] = state[3] + d;
	state[4] = state[4] + e;
}

BOOL CalculateValueString(unsigned char *str, int len, unsigned char *buffer)
{
	int pos;
	int i;
	unsigned char adder;
	unsigned char shifter;

	pos = 0;

	while(len)
	{
		shifter = 0;
		
		for(i = 0; i < pos; i++)
		{
			unsigned char b = buffer[i];
			buffer[i] = -buffer[i] + shifter;
			shifter = ((((b << 8) - b) + shifter) >> 8);
		}

		if(shifter)
		{
			if(pos >= 0x10)
				return 0;

			buffer[pos++] = shifter;
		}

		adder = str[len - 1] - 1;
		for(i = 0; i < pos && adder; i++)
		{
			buffer[i] = buffer[i] + adder;
			adder = buffer[i] < adder;
		}
		
		if(adder)
		{
			if(pos >= 0x10)
				return 0;

			buffer[pos++] = adder;
		}

		len--;
	}

	/* Fills whatever's left in the buffer with 0. */
	RtlFillMemory(buffer + pos, 0x10 - pos, 0);

	return 1;
}

void WordShifter(unsigned short *str1, unsigned short *str2)
{
	*str2 = (((*str1 >> 8) + (*str1 & 0xFF)) >> 8) + ((((*str1 >> 8) + (*str1 & 0xFF)) & 0xFF));
	*str2 = (*str2 & 0xFFFFFF00) | (((*str2 + 1) & 0xFF) - (((*str2 & 0xFF) != 0xFF) ? 1 : 0));

	*str1 = ((*str1 - *str2) & 0xFFFF00FF) | (((((*str1 - *str2) >> 8) & 0xFF) + 1) ? 0 : 0x100);
	*str1 = (*str1 & 0xFFFFFF00) | (-*str1 & 0x000000FF);
}

int StringCombine(char *str1, int *length, char *str2)
{
	int i, j;
	unsigned short word1, word2;
	char *ptr_str1 = str1;
	int ret = TRUE;
	
	for(i = 0x10; i > 0; )
	{
		/* Skips over null bytes. */
		while(i && !str2[i - 1])
			i--;

		if(i)
		{
			word1 = 0;

			for(j = i - 1; j >= 0; j--)
			{
				word2 = (word1 << 8) + (str2[j] & 0xFF);
				WordShifter(&word2, &word1);
				str2[j] = (char) word2;
			}

			if((0x10 - i) >= *length)
				ret = FALSE;
			else
				ptr_str1[0] = word1 + 1;

			ptr_str1++;
		}
	}

	*length = ptr_str1 - str1;

	return ret;
}

/* I'm positive there's a better way to do this one, but I really don't care. :) */
int NumberCompare(int *num1, int *num2)
{
	if(*num1 == *num2)
		return 0;

	if(*num2 < *num1 && *num1 < 0 && *num2 < 0)
		return 1;
	else if(*num1 < 0 && *num2 == 0)
		return 1;
	else if(*num1 > *num2 && *num2 >= 0)
		return 1;
	else if(*num1 < 0 && *num2 > 0)
		return 1;
	else
		return -1;
}