#include <windows.h>

#include "MyLockdownSHA1.h"

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

BOOL CalculateValueString(char *str, int len, char *buffer)
{
	int eax, ebx, ecx, edx, edi;
	char *esi = buffer;
//                push    ebp
//                mov     ebp, esp
//                xor     edx, edx
	edx = 0;
//                cmp     [ebp+length], edx ; Check if the length is 0
//                push    ebx
//                push    edi
//                jbe     short LengthLessThanZero
	if(len <= 0)
		goto LengthLessThanZero;
//
loc_401179: //                            ; CODE XREF: ValueStringOperation+71j
//                xor     bl, bl          ; Clear ebx
	ebx = 0;
//                xor     edi, edi        ; Clear edi
	edi = 0;
//                test    edx, edx        ; This will be 0 the first time
//                jbe     short loc_4011AB
	if((edx & edx) <= 0)
		goto loc_4011AB;
//
loc_401181: //                            ; CODE XREF: ValueStringOperation+2Fj
//                mov     cl, [edi+esi]   ; value
	ecx = esi[edi] & 0x000000FF;
//                call    MakeNegative    ; Returns ecx, negative
//                movzx   eax, ax
	eax = (-ecx) & 0x000000FF;
//                movzx   ecx, bl
	ecx = ebx & 0x000000FF;
//                add     eax, ecx
	eax = eax + ecx;
//                mov     [edi+esi], al
	printf("%d\n", edi);
	esi[edi] = eax & 0x000000FF;
//                shr     eax, 8
	eax = eax >> 8;
//                inc     edi
	edi++;
//                cmp     edi, edx
//                mov     bl, al
	ebx = eax & 0x000000FF;
//                jb      short loc_401181

	if(edi < edx)
		goto loc_401181;
//                test    bl, bl
//                jbe     short loc_4011AB
	if((ebx & ebx) <= 0)
		goto loc_4011AB;
//                cmp     edx, 10h
//                jnb     short Return0
	if(edx >= 0x10)
		goto Return0;
//                mov     [edx+esi], bl
	printf("%d\n", edx);
	esi[edx] = ebx & 0x000000FF;
//                inc     edx
	edx++;
//
loc_4011AB: //                            ; CODE XREF: ValueStringOperation+12j
//                                        ; ValueStringOperation+33j
//                mov     ecx, [ebp+length]
	ecx = len;
//                mov     eax, [ebp+valuestring_copy]
	eax = (int) str;
//                mov     al, [eax+ecx-1]
	printf("%d\n", ecx - 1);
	eax = *((char*) (eax + ecx - 1));
	eax = eax & 0x000000FF;
//                dec     al
	eax = (eax - 1) & 0x000000FF;
//                push    0
//                pop     ecx
	ecx = 0;
//                jz      short loc_4011CE
	if(eax == 0)
		goto loc_4011CE;
//
loc_4011BC: //                            ; CODE XREF: ValueStringOperation+5Fj
//                cmp     ecx, edx
//                jnb     short loc_4011CE
	if(ecx >= edx)
		goto loc_4011CE;
//                add     [ecx+esi], al
	printf("%d\n", ecx);
	esi[ecx] = esi[ecx] + (eax & 0x000000FF);
//                cmp     [ecx+esi], al
//                inc     ecx
	ecx++;
//                setb    al
	eax = (((esi[ecx - 1] & 0x000000FF) < (eax & 0x000000FF)) ? 1 : 0);
//                test    al, al
//                jnz     short loc_4011BC
	if(eax)
		goto loc_4011BC;
//
loc_4011CE: //                            ; CODE XREF: ValueStringOperation+4Dj
//                                        ; ValueStringOperation+51j
//                test    al, al
//                jbe     short loc_4011DB
	if(eax <= 0)
		goto loc_4011DB;
//                cmp     edx, 10h
//                jnb     short Return0
	if(edx >= 0x10)
		goto Return0;
//                mov     [edx+esi], al
	printf("%d\n", edx);
	esi[edx] = eax;
//                inc     edx
	edx++;
//
loc_4011DB: //                            ; CODE XREF: ValueStringOperation+63j
//                dec     [ebp+length]
	len--;
//                jnz     short loc_401179
	if(len)
		goto loc_401179;
//                cmp     edx, 10h
//                jnb     short Return1
	if(edx >= 0x10)
		goto Return1;
//
LengthLessThanZero: //                    ; CODE XREF: ValueStringOperation+Aj
//                push    10h
//                pop     eax             ; eax = 10
	eax = 0x10;
//                sub     eax, edx
	eax = eax - edx;
//                push    eax             ; 0x10
//                add     edx, esi
	printf("%d\n", edx);
	edx = edx + (int) esi;
//                push    0               ; fill
//                push    edx             ; destination
//                call    RtlFillMemoryWrapper ; Blank out some memory
	RtlFillMemory((void*) edx, 0x10, 0);
//                add     esp, 0Ch
//
Return1: //                               ; CODE XREF: ValueStringOperation+76j
//                xor     eax, eax
	eax = 0;
//                inc     eax
	eax++;

	return eax;
//
//                                ; CODE XREF: ValueStringOperation+94j
//                pop     edi
//                pop     ebx
//                pop     ebp
//                retn
Return0:
	return 0;
//
}


