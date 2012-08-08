#include "Logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "MemoryPatch.h"

/* Initialize a fairly basic patch that basically overwrites the code with a call. */
t_memorypatch *mp_initialize(void *address, void *call_function, int length)
{
	t_memorypatch *newpatch = (t_memorypatch *) malloc(sizeof(t_memorypatch));
	
	if(!newpatch)
	{
		log_add("Out of memory!\n");
		exit(1);
	}
	memset(newpatch, 0, sizeof(t_memorypatch));

	newpatch->address            = address;
	newpatch->call_function      = call_function;
	newpatch->original           = buffer_initialize();
	newpatch->patch              = buffer_initialize();
	newpatch->wrapper            = buffer_initialize();
	newpatch->length             = length;
	newpatch->preserve_original  = FALSE;
	newpatch->preserve_registers = FALSE;
	newpatch->return_register    = NO_REGISTER;
	newpatch->parameters         = pl_initialize();

	return newpatch;
}

/** Initialize a patch that preserves the original code, stores the return address in 
 * ecx (note that ecx gets whacked), and saves the registers. This is probably the most
 * common patch. */
t_memorypatch *mp_initialize_useful(void *address, void *call_function, int length)
{
	t_memorypatch *newpatch = mp_initialize(address, call_function, length);
	newpatch->preserve_original = TRUE;
	newpatch->preserve_registers = TRUE;
	newpatch->return_register = ECX;

	return newpatch;
}

void mp_destroy(t_memorypatch *patch)
{
	if(patch)
	{
		if(is_applied(patch))
			mp_remove(patch);
		pl_destroy(patch->parameters);
		buffer_destroy(patch->original);
		buffer_destroy(patch->patch);
		buffer_destroy(patch->wrapper);
		memset(patch, 0, sizeof(patch));
		free(patch);
	}
}

void mp_set_preserve_original(t_memorypatch *patch, BOOL preserve_original)
{
	patch->preserve_original = preserve_original;
}

void mp_set_preserve_registers(t_memorypatch *patch, BOOL preserve_registers)
{
	patch->preserve_registers = preserve_registers;
}

void mp_set_return_register(t_memorypatch *patch, t_register return_register)
{
	patch->return_register = return_register;
}

void mp_add_register_parameter(t_memorypatch *patch, t_register parameter)
{
	pl_add_register(patch->parameters, parameter);
}

void mp_add_memoryoffset_parameter(t_memorypatch *patch, t_register parameter, char offset)
{
	pl_add_memoryoffset(patch->parameters, parameter, offset);
}

void mp_add_pointeroffset_parameter(t_memorypatch *patch, t_register baseregister, t_register tempregister, char offset)
{
	pl_add_pointeroffset(patch->parameters, baseregister, tempregister, offset);
}

void mp_add_custom_parameter(t_memorypatch *patch, char *patch_string, int patch_length)
{
	pl_add_custom(patch->parameters, patch_string, patch_length);
}

BOOL mp_apply(t_memorypatch *patch)
{
	char *original;
	HANDLE hProcess = GetCurrentProcess();
	char call_buffer[5];
	BOOL success = FALSE;

	log_add("Applying patch");
	log_indent();

	if(patch && !patch->is_applied)
	{
		/* Back up the original data */
		original = malloc(sizeof(char) * patch->length + 1);
		ReadProcessMemory(hProcess, patch->address, original, patch->length, NULL);
		buffer_clear(patch->original);
		buffer_insert_bytes(patch->original, original, patch->length);
		free(original);
		
		/* Create the wrapper.  The contents of the wrapper depends on which varibles 
		 * are set, but the maximum wrapper will do the following:
		 * - Save the return address    (pop [reg])
		 * - Run the original code      ([varies])
		 * - Preserve registers         (pushad)
		 * - Call the function          (call [func])
		 * - Restore registers          (popad)
		 * - Restore the return address (push [reg])
		 * - Return                     (ret) */
		buffer_clear(patch->wrapper);
		/* Pop the return address, if applicable. */
		if(patch->return_register != NO_REGISTER)
			buffer_insert_byte(patch->wrapper, asm_pop(patch->return_register));
		/* Insert the original code, if applicable. */
		if(patch->preserve_original)
			buffer_insert_buffer(patch->wrapper, patch->original);
		/* Preserve all registers, if applicable. */
		if(patch->preserve_registers)
		{
			buffer_insert_byte(patch->wrapper, asm_pushad());
			buffer_insert_byte(patch->wrapper, asm_pushfd());
		}
		/* Push the parameters, if applicable. */
		pl_add_pushes(patch->parameters, patch->wrapper);
		/* Call the actual function. Also allocate enough space so that the buffer isn't 
		 * re-allocated. */
		buffer_allocate(patch->wrapper, 1024);
		buffer_insert_bytes(patch->wrapper, asm_call(buffer_gettipaddress(patch->wrapper), patch->call_function, call_buffer), 5);
		/* Restore all registers, if applicable. */
		if(patch->preserve_registers)
		{
			buffer_insert_byte(patch->wrapper, asm_popfd());
			buffer_insert_byte(patch->wrapper, asm_popad());
		}
		/* Restore the return address, if applicable. */
		if(patch->return_register != NO_REGISTER)
			buffer_insert_byte(patch->wrapper, asm_push(patch->return_register));
		/* Finally, return. */
		buffer_insert_byte(patch->wrapper, asm_ret());

		/* Create the patch. */
		buffer_clear(patch->patch);
		buffer_allocate(patch->patch, 256);
		buffer_insert_bytes(patch->patch, asm_call(patch->address, buffer_get_cstring(patch->wrapper), call_buffer), 5);

		/* Pad the patch up to the proper length. */
		while(buffer_get_length(patch->patch) < patch->length)
			buffer_insert_byte(patch->patch, asm_nop());

		/* Can be uncommented for debugging: */
		mp_print(patch);
		/* Apply the patch. */
		success = WriteProcessMemory(GetCurrentProcess(), patch->address, buffer_get_cstring(patch->patch), patch->length, NULL);

		/* Set the patch to applied. */
		patch->is_applied = success;
	}

	if(success)
		log_add("--> Patch Successful!");
	else
		log_add("--> Patch Failed!");
	log_undent();

	return success;
}

void mp_remove(t_memorypatch *patch)
{
	if(patch && patch->is_applied)
	{
		/* Restore the original memory. */
		WriteProcessMemory(GetCurrentProcess(), patch->address, buffer_get_cstring(patch->original), patch->length, NULL);

		/* Clear the buffers. */
		buffer_clear(patch->original);
		buffer_clear(patch->wrapper);
		buffer_clear(patch->patch);

		patch->is_applied = FALSE;
	}
}

void mp_print(t_memorypatch *patch)
{
	if(patch)
	{
		log_add("Address: %p",                       patch->address);
		log_add("Length: %d bytes",                  patch->length);
		log_add("Call function: %p",                 patch->call_function);
		log_add("Preserve original: %d",             patch->preserve_original);
		log_add("Preserve registers: %d",            patch->preserve_registers);
		log_add("Return register: %s",               t_register_list[patch->return_register]);
		log_add("Original code (if applicable): ");
		log_indent();
		buffer_print(patch->original);
		log_undent();
		log_add("Parameters:");
		log_indent();
		pl_print(patch->parameters);
		log_undent();
	}
}

BOOL is_applied(t_memorypatch *patch)
{
	return patch && patch->is_applied;
}