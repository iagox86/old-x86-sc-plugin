/*#include "MemoryPatch.h"

#include "Buffer.h"

#include <stdio.h>

void __stdcall patchfunc(int a, int b, int c, int d)
{
	printf("\n\n --- Patch ran --- \n");
	printf(" -- eax = %08x -- \n", a);
	printf(" -- ebx = %08x -- \n", b);
	printf(" -- ecx = %08x -- \n", c);
	printf(" -- edx = %08x -- \n", d);

}

int main(int argc, char *argv[])
{
	char data[] = "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"
					"\xBB\xFF\xFF\xFF\xFF\x90\x90\x90\x90\x90"
					"\x90\x90\x90\x90\x90\x90\x90\x90\x90\xC3";
	char *ptr_data = (char*) &data;

	t_memorypatch *patch = mp_initialize((void*) (data + 10), (void*) patchfunc, 10);
	t_buffer *buffer = buffer_initialize();

	mp_add_parameter(patch, EAX);
	mp_add_parameter(patch, EBX);
	mp_add_parameter(patch, ECX);
	mp_add_parameter(patch, EDX);
	mp_set_return_register(patch, ECX);
	mp_set_preserve_original(patch, TRUE);
	mp_print(patch);



	mp_apply(patch);
	mp_remove(patch);
	mp_apply(patch);
	mp_remove(patch);
	mp_apply(patch);

	__asm 
	{ 
		mov eax, 1
		mov ebx, 2
		mov ecx, 3
		mov edx, 4

		call ptr_data 
		call ptr_data 
	};


	mp_remove(patch);

	__asm 
	{ 
		call ptr_data 
	};


	system("pause");
	return 0;
}
*/