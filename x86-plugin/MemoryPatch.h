#ifndef __MEMORY_PATCH_H__
#define __MEMORY_PATCH_H__

#include <windows.h>

#include "ParameterList.h"
#include "Buffer.h"

/* This module stores a single memory patch, and can apply/remove it. A memory patch affects
 * the current process, and patches a jump over some known code. The patch jumps to a wrapper
 * which may or may not run the code that was overwritten, and may or may not preserve 
 * the registers. 
 * 
 * It is important:
 * - Not to patch over parts of instructions, you have to patch over the entire instruction
 * - Not to patch over one part of a jmp or call, since jmps and calls are relative. 
 */

typedef struct 
{
	/* The address to patch over. */
	void *address;

	/* The number of bytes to replace with the patch. Must be at least 5, and must 
	 * not bisect instructions. */
	int length;

	/* This is a pointer to the function that will be called for the patch. 
	 * Must be set. */
	void *call_function;

	/* This is the original data, used for restoring. This is automatically set. */
	t_buffer *original;

	/* This is the actual patch. */
	t_buffer *patch;

	/* This is the wrapper that will hold temporary code. */
	t_buffer *wrapper;

	/* If set, the machine code that is overwritten by the patch is preserved and run. 
	 * Default: off. */
	BOOL preserve_original;

	/* If set, pushad and popad are added to the patch, ensuring the registers don't change.
	 * Default: on. */
	BOOL preserve_registers;

	/* This is the register that will store the return address. If "preserveoriginal" is on, 
	 * this must be set. This register may not contain any important data, since its value 
	 * will be overwritten. 
	 * Default: NO_REGISTER */
	t_register return_register;

	/* This is a list of the parameters that will be pushed before the function is called. 
	 * Default: none */
	t_parameterlist *parameters;

	/* Keeps track of whether or not the patch is applied. */
	BOOL is_applied;
} t_memorypatch;

/* Creates a new patch for the specified address with the following properties:
 * - At the specified address, to the specified address, and of the specified length
 * - Original code will not be automatically run (change with set_preserve_original())
 * - Registers will not be preserved with pushad/popad
 * - Return address register will not be set (must be set to run original code)
 * - No parameters are passed (add parameters with the add_parameter() function) */
t_memorypatch *mp_initialize(void *address, void *call_function, int length);

/* Creates a new patch for the specified address with the following properties:
 * - At the specified address, to the specified address, and of the specified length
 * - Original code will be run
 * - Registers will be preserved
 * - Return address will be stored in ecx (which will be whacked!)
 * - No parameters are passed
 */
t_memorypatch *mp_initialize_useful(void *address, void *call_function, int length);

/* Free all memory involved with this patch. The patch pointer should no longer be used after this. */
void mp_destroy(t_memorypatch *patch);

/* Preserve the original code. In order to do this, set_return_register() must be called. */
void mp_set_preserve_original(t_memorypatch *patch, BOOL preserve_original);

/* Preserve the original registers' values. */
void mp_set_preserve_registers(t_memorypatch *patch, BOOL preserve_registers);

/* Set the register that stores the return address. This register must not contain any useful data, since
 * it will have a different value when it returns. As long as preserve_registers is enabled,
 * this register will be protected, so it doesn't matter if it's overwritten. The only thing
 * to keep in mind is that any value that was stored in it is gone. */
void mp_set_return_register(t_memorypatch *patch, t_register return_register);

/* Add a parameter that will be pushed onto the stack immediately before the function is called. */
void mp_add_register_parameter(t_memorypatch *patch, t_register parameter);

/* Add a parameter that will take the form "push [reg + offset]" */
void mp_add_memoryoffset_parameter(t_memorypatch *patch, t_register parameter, char offset);

/* Add a parameter that will be, "lea tempreg, [reg + offset] / push tempreg". Note that 
 * the value in tempreg will be whacked for the purposes of other parameters (although if
 * preserve_registers is enabled, it'll eventually be fixed). The same register that stores 
 * the return address is probably the best one to use, since it's whacked anyways. */
void mp_add_pointeroffset_parameter(t_memorypatch *patch, t_register baseregister, t_register tempregister, char offset);

/* Add a custom parameter (or, technically, command), stored in patch_string. */
void mp_add_custom_parameter(t_memorypatch *patch, char *patch_string, int patch_length);

/* Only call this once, unless you remove it. Otherwise, stuff will break! */
BOOL mp_apply(t_memorypatch *patch);

/* Only call this after applying the patch. */
void mp_remove(t_memorypatch *patch);

/* Print the patches, for debugging purposes. */
void mp_print(t_memorypatch *patch);

/* Check if the patch is applied. */
BOOL is_applied(t_memorypatch *patch);

#endif