#ifndef __ASSEMBLY_H__
#define __ASSEMBLY_H__

/** The Assembly module is used to simplify the generation of machine code. */

/* These registers are defined the same way as they're defined in x86 machine code. 
 * As a result, you can typically OR machine code instructions (like 0x50 for PUSH)
 * by the appropriate register. */
typedef enum 
{
	NO_REGISTER  = -0x01,
	EAX          = 0x00,
	ECX          = 0x01,
	EDX          = 0x02,
	EBX          = 0x03,
	ESP          = 0x04,
	EBP          = 0x05,
	ESI          = 0x06,
	EDI          = 0x07,
	TOP_REGISTER = 0x08
} t_register;
/* Since we have to print them, might as well make it easy. To use this, make sure that the 
 * register is >NO_REGISTER and <TOP_REGISTER. */
static char *t_register_list[] = { "EAX", "ECX", "EDX", "EBX", "ESP", "EBP", "ESI", "EDI" };

/* OR this by the register to make "push [reg]". */
static char PUSH       = 0x50;

/* OR this by the register to make "pop [reg]". */
static char POP        = 0x58;

/* NOP is used alone. */
static char NOP        = 0x90;

/* This is used alone to use a simple "ret". */
static char RET        = 0xC3;

/* This has to be followed by 2-bytes for the number of bytes to pop. */
static char RET_BYTES  = 0xC2;

/* This has to be followed by 4-bytes for the offset. */
static char CALL       = 0xE8;

/* This has to be followed by 4-bytes for the offset. */
static char LONGJMP    = 0xE9;

/* This has to be followed by 1-byte for the offset. */
static char SHORTJMP   = 0xEB;

/* The next four go alone: */
static char PUSHAD     = 0x60;
static char POPAD      = 0x61;
static char PUSHFD     = 0x9C;
static char POPFD      = 0x9D;

/* "push reg" */
char asm_push(t_register reg);

/* "pop reg" */
char asm_pop(t_register reg);

/* "nop" */
char asm_nop();

/* "ret" */
char asm_ret();

/* "ret bytes" */
char *asm_ret_bytes(int bytes, char buffer[3]);

/* "call destination" */
char *asm_call(void *source, void *destination, char buffer[5]);

/* "jmp destination" */
char *asm_jmp(void *source, void *destination, char buffer[5]);

/* "pushad" */
char asm_pushad();

/* "popad" */
char asm_popad();

/* "pushfd" */
char asm_pushfd();

/* "popfd" */
char asm_popfd();

/* "push [reg + offset]" */
char *asm_pushregisteroffset(t_register reg, char offset, char buffer[3]);

/* "lea dest, [src + offset]" */
char *asm_leaoffset(t_register dest, t_register src, char offset, char buffer[3]);

#endif