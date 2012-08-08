#ifndef __PARAMETER_LIST_H__
#define __PARAMETER_LIST_H__

/** The purpose of parameterlist is to, obviously, store a list of parameters. These are 
 * parameters that will eventually be used in calling a memory patch. 
 * 
 * There are several types of parameters, that will be explained in the enum below. Adding
 * more is fairly simple, add another entry to the enum, add a function, and add the type to
 * all the switches. The code can be as complicated as you want, since it is run in a wrapper
 * anyways. 
 */

#include "Assembly.h"
#include "Buffer.h"

typedef enum
{
	/* A simple "push [reg]". */
	PL_REGISTER,
	/* A memory offset from the register, "push [reg + x]". */
	PL_MEMORYOFFSET,
	/* A pointer to an offset from the register, "lea temp, [reg + x] / push temp". */
	PL_POINTEROFFSET,
	/* A custom pointer, the programmer specifies the machine code to use. */
	PL_CUSTOM
} t_parameterlist_types;

/* Most of the entries in this structure are dependant on the type. This would be 
 * well-suited to being a union, but I forget how to use unions so I didn't bother. */
typedef struct _parameterlist
{
	t_parameterlist_types type;
	t_register reg;
	char offset; /* Not necessarily used. */
	char tempregister; /* Not necessarily used. */
	char *custom;
	int custom_length;
	struct _parameterlist *next;
} t_parameterlist;

/* Create a new instance. */
t_parameterlist *pl_initialize();

/* Destroy the instance, and free up any memory. */
void pl_destroy(t_parameterlist *list);

/* Add a simple register parameter. */
void pl_add_register(t_parameterlist *list, t_register reg);

/* Add a memory offset parameter. */
void pl_add_memoryoffset(t_parameterlist *list, t_register reg, char offset);

/* Add a pointer offset parameter. */
void pl_add_pointeroffset(t_parameterlist *list, t_register baseregister, t_register tempregister, char offset);

/* Add a custom parameter. */
void pl_add_custom(t_parameterlist *list, char *patch, int patch_length);

/* Print the list of parameters to the log. */
void pl_print(t_parameterlist *list);

/* Add the machinecode for the parameters to the end of the buffer. */
void pl_add_pushes(t_parameterlist *list, t_buffer *buffer);

#endif