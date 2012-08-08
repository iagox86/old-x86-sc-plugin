#include <windows.h>

#include "Assembly.h"
#include "Maive_SHA1.h"

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
	char vars[8];
	char *MysteryBuffer = "\x80\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00";
	int len;
	int eax, ebx, edx, esi, edi;
	unsigned int ecx;

	vars[0] = ctx->bitlen[1] >> 24;
	vars[1] = ctx->bitlen[1] >> 16;
	vars[2] = ctx->bitlen[1] >> 8;
	vars[3] = ctx->bitlen[1] >> 0;

	vars[4] = ctx->bitlen[0] >> 24;
	vars[5] = ctx->bitlen[0] >> 16;
	vars[6] = ctx->bitlen[0] >> 8;
	vars[7] = ctx->bitlen[0] >> 0;

	len = ((-9 - (ctx->bitlen[0] >> 3)) & 0x3F) + 1;
//	len = 0x0e;
	sha1_update(ctx, MysteryBuffer, len);
	sha1_update(ctx, vars, 8);

//Maive.mod:02E3233A 014                 mov     edi, [ebp+buffer]
	edi = hash;
//Maive.mod:02E3233D 014                 push    5
//Maive.mod:02E3233F 018                 lea     eax, [esi+8]
	eax = ((char*)ctx) + 8;
//Maive.mod:02E32342 018                 pop     esi
	esi = 5;
//Maive.mod:02E32343
//Maive.mod:02E32343     loc_2E32343:                            ; CODE XREF: maive_SHA1_Final_2+58j
loc_2e32343:
//Maive.mod:02E32343 014                 mov     ecx, [eax]
	ecx = *((int*)eax);
//Maive.mod:02E32345 014                 push    3
//Maive.mod:02E32347 018                 pop     edx
	edx = 3;
//Maive.mod:02E32348
//Maive.mod:02E32348     loc_2E32348:                            ; CODE XREF: maive_SHA1_Final_2+4Fj
loc_2e32348:
//Maive.mod:02E32348 014                 mov     [edi+edx], cl
	*((char*)(edi + edx)) = cl;
//Maive.mod:02E3234B 014                 shr     ecx, 8
	ecx = ecx >> 8;
//Maive.mod:02E3234E 014                 dec     edx
	edx--;
//Maive.mod:02E3234F 014                 jns     short loc_2E32348 ; Loop 4 times
	if(edx >= 0)
		goto loc_2e32348;
//Maive.mod:02E32351 014                 add     eax, 4
	eax += 4;
//Maive.mod:02E32354 014                 add     edi, 4
	edi += 4;
//Maive.mod:02E32357 014                 dec     esi
	esi--;
//Maive.mod:02E32358 014                 jnz     short loc_2E32343 ; Loop 5 times
	if(esi != 0)
		goto loc_2e32343;
//

//	for(i = 0; i < 5; i++)
//		hash[i] = ctx->state[i];
}

void sha1_transform(int *data, unsigned int *state)
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
