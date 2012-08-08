#include "Assembly.h"
#include "Buffer.h"

#include <string.h>

char asm_push(t_register reg)
{
	return PUSH | reg;
}

char asm_pop(t_register reg)
{
	return POP | reg;
}

char asm_nop()
{
	return NOP;
}

char asm_ret()
{
	return RET;
}

char *asm_ret_bytes(int bytes, char buffer[3])
{
	t_buffer *buf = buffer_initialize();
	buffer_insert_byte(buf, RET_BYTES);
	buffer_insert_word(buf, bytes);
	strncpy_s(buffer, 3, buffer_get_cstring(buf), 3);
	buffer_destroy(buf);

	return buffer;
}

char *asm_call(void *source, void *destination, char buffer[5])
{
	size_t distance = (size_t) destination - ((size_t) source + 5);
	t_buffer *buf = buffer_initialize();
	buffer_insert_byte(buf, CALL);
	buffer_insert_dword(buf, (int)distance);
	memcpy(buffer, buffer_get_cstring(buf), 5);
	buffer_destroy(buf);

	return buffer;
}

char *asm_jmp(void *source, void *destination, char buffer[5])
{
	size_t distance = (size_t) destination - ((size_t) source + 5);
	t_buffer *buf = buffer_initialize();
	buffer_insert_byte(buf, LONGJMP);
	buffer_insert_dword(buf, (int)distance);
	memcpy(buffer, buffer_get_cstring(buf), 5);
	buffer_destroy(buf);

	return buffer;
}

char asm_pushad()
{
	return PUSHAD;
}

char asm_popad()
{
	return POPAD;
}

char asm_pushfd()
{
	return PUSHFD;
}

char asm_popfd()
{
	return POPFD;
}

char *asm_pushregisteroffset(t_register reg, char offset, char buffer[3])
{
	t_buffer *buf = buffer_initialize();
	buffer_insert_byte(buf, 0xFF);
	buffer_insert_byte(buf, 0x70 | reg);
	buffer_insert_byte(buf, offset);
	memcpy(buffer, buffer_get_cstring(buf), 3);
	buffer_destroy(buf);

	return buffer;
}

char *asm_leaoffset(t_register dest, t_register src, char offset, char buffer[3])
{
	t_buffer *buf = buffer_initialize();
	buffer_insert_byte(buf, 0x8D);
	buffer_insert_byte(buf, 0x40 | (dest << 3) | (src));
	buffer_insert_byte(buf, offset);
	memcpy(buffer, buffer_get_cstring(buf), 3);
	buffer_destroy(buf);

	return buffer;
}