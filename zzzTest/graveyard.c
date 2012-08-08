#if 0

/* Good code that I wrote but that just isn't needed.. I don't have the heart to delete it. */

void test_R4(int a, char *b, char *c, char *d, int e, int f)
{
	int func = 0x02E312FC;

	__asm
	{
		mov eax, a
		mov ecx, b
		mov edx, c
		push d
		push e
		push f
		call func
	}

	printf("%08x\n", *((int*)b));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");
}

void R4(int a, char *b, char *c, char *d, int e, int f)
{
	unsigned int eax = a;
	unsigned int ecx = b;
	unsigned int edx = c;
	unsigned int esi;
	unsigned int edi;

	unsigned int ptr_unknown = d;
	unsigned int arg_4 = e;
	unsigned int arg_0 = f;

	esi = ptr_unknown;
	edi = eax;
	eax = eax << 5;
	edi = edi >> 0x1B;
	edi = edi | eax;
	eax = *((int*)ecx);
	eax = eax ^ arg_0;
	eax = eax ^ arg_4;
	edi = edi + eax;
	eax = *((int*)edx);
	eax = edi + eax + 0xCA62C1D6;
	*((int*)esi) += eax;
	eax = *((int*)ecx);
	esi = eax;
	esi = esi << 0x1E;
	eax = eax >> 2;
	esi = esi | eax;
	*((int*)ecx) = esi;
	*((int*)edx) = 0;

	printf("%08x\n", *((int*)b));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");

}

void test_R3(int a, char *b, char *c, char *d, int e, int f)
{
	int func = 0x02E312BD;

	__asm
	{
		mov eax, a
		mov ecx, b
		mov esi, c
		mov edi, d

		push e
		push f
		call func
	}

	printf("%08x\n", *((int*)b));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");

}

void R3(int a, char *b, char *c, char *d, int e, int f)
{
	unsigned int eax = a;
	unsigned int ebx;
	unsigned int ecx = b;
	unsigned int edx;
	unsigned int esi = c;
	unsigned int edi = d;
	unsigned int arg_0 = f;
	unsigned int arg_4 = e;

	edx = arg_0;
	ebx = eax;
	eax = eax & arg_4;
	ebx = ebx | arg_4;
	ebx = ebx & *((int*) ecx);
	ebx = ebx | eax;
	eax = edx;
	eax = eax >> 0x1B;
	edx = edx << 5;
	eax = eax | edx;
	ebx = ebx + eax;
	eax = *((int*)esi);
	eax = ebx + eax + 0x8F1BBCDC;
	*((int*)edi) += eax;
	eax = *((int*)ecx);
	edx = eax;
	edx = edx << 0x1E;
	eax = eax >> 2;
	edx = edx | eax;
	*((int*)ecx) = edx;
	*((int*)esi) = 0;

	printf("%08x\n", *((int*)b));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");
}

void test_R2(int a, char *b, char *c, char *d, int e, int f)
{
	int func = 0x02E31280;

	__asm
	{
		mov eax, a
		mov ecx, b
		mov edx, c
		push d
		push e
		push f
		call func
	}

	printf("%08x\n", *((int*)b));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");
}

void R2(int a, char *b, char *c, char *d, int e, int f)
{
	unsigned int eax = a;
	unsigned int ecx = b;
	unsigned int edx = c;
	unsigned int esi;
	unsigned int edi;

	unsigned int ptr_unknown = d;
	unsigned int arg_4 = e;
	unsigned int arg_0 = f;

	esi = ptr_unknown;
	edi = eax;
	eax = eax << 5;
	edi = edi >> 0x1b;
	edi = edi | eax;
	eax = *((int*) ecx);
	eax = eax ^ arg_0;
	eax = eax ^ arg_4;
	edi = edi + eax;
	eax = *((int*)edx);
	eax = eax + edi + 0x6ED9EBA1;
	*((int*)esi) += eax;
	eax = *((int*) ecx);
	esi = eax;
	esi <<= 0x1E;
	eax >>= 2;
	esi |= eax;
	*((int*)ecx) = esi;
	*((int*) edx) = 0;

	printf("%08x\n", *((int*)b));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");
}

void test_R0(char *a, int b, char *c, char *d, int e, int f)
{
	int func = 0x02E31243;

	__asm
	{
		mov ecx, a
		mov edx, b
		mov esi, c
		mov edi, d

		push e
		push f
		call func
	}

	printf("%08x\n", *((int*)a));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");

}

void R0(char *a, int b, char *c, char *d, int e, int f)
{
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx = a;
	unsigned int edx = b;
	unsigned int esi = c;
	unsigned int edi = d;
	unsigned int arg_0 = f;
	unsigned int arg_4 = e;

	eax = *((int*) ecx);
	ebx = eax;
	eax &= arg_0;
	ebx = ~ebx;
	ebx &= arg_4;
	ebx |= eax;
	eax = edx;
	eax >>= 0x1B;
	edx <<= 5;
	eax |= edx;
	ebx += eax;
	eax = *((int*) esi);
	eax = eax + ebx + 0x5A827999;
	*((int*)edi) += eax;
	eax = *((int*)ecx);
	edx = eax;
	edx <<= 0x1E;
	eax >>= 2;
	edx |= eax;
	*((int*)ecx) = edx;
	*((int*) esi) = 0;

	printf("%08x\n", *((int*)a));
	printf("%08x\n", *((int*)c));
	printf("%08x\n", *((int*)d));
	printf("\n");
}

#endif