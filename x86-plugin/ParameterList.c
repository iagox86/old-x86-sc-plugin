#include "Assembly.h"
#include "Buffer.h"
#include "Logging.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ParameterList.h"

t_parameterlist *pl_initialize()
{
	t_parameterlist *newlist = (t_parameterlist *) malloc(sizeof(t_parameterlist));
	
	if(!newlist)
	{
		fprintf(stderr, "Out of memory!\n");
		exit(1);
	}
	memset(newlist, 0, sizeof(t_parameterlist));

	return newlist;
}

void pl_destroy(t_parameterlist *list)
{
	t_parameterlist *current = list;
	t_parameterlist *next = NULL;
	while(current)
	{
		next = current->next;
		memset(current, 0, sizeof(t_parameterlist));
		if(current->custom)
			free(current->custom);
		free(current);
		current = next;
	}
}

/* This actually adds the node to the beginning of the linked list, because the parameters
 * have to be pushed in reverse order, from the last to the first. */
static void pl_add_node(t_parameterlist *list, t_parameterlist *newnode)
{
	newnode->next = NULL;
	if(list->next)
		newnode->next = list->next;

	list->next = newnode;
}

void pl_add_register(t_parameterlist *list, t_register reg)
{
	t_parameterlist *newnode = pl_initialize();

	if(reg <= NO_REGISTER || reg >= TOP_REGISTER)
		return;

	newnode->type   = PL_REGISTER;
	newnode->reg    = reg;
	newnode->offset = 0;

	pl_add_node(list, newnode);
}

void pl_add_memoryoffset(t_parameterlist *list, t_register reg, char offset)
{
	t_parameterlist *newnode = pl_initialize();

	if(reg <= NO_REGISTER || reg >= TOP_REGISTER)
		return;

	newnode->type   = PL_MEMORYOFFSET;
	newnode->reg    = reg;
	newnode->offset = offset;

	pl_add_node(list, newnode);
}

void pl_add_pointeroffset(t_parameterlist *list, t_register baseregister, t_register tempregister, char offset)
{
	t_parameterlist *newnode = pl_initialize();

	if(baseregister <= NO_REGISTER || baseregister >= TOP_REGISTER)
		return;
	if(tempregister <= NO_REGISTER || tempregister >= TOP_REGISTER)
		return;

	newnode->type         = PL_POINTEROFFSET;
	newnode->reg          = baseregister;
	newnode->tempregister = tempregister;
	newnode->offset       = offset;

	pl_add_node(list, newnode);
}

/* This allows the user to add a custom parameter, for weirder stuff. */
void pl_add_custom(t_parameterlist *list, char *patch, int patch_length)
{
	t_parameterlist *newnode = pl_initialize();

	newnode->type = PL_CUSTOM;
	newnode->custom = malloc(patch_length);
	memcpy(newnode->custom, patch, patch_length);
	newnode->custom_length = patch_length;

	pl_add_node(list, newnode);
}

void pl_print(t_parameterlist *list)
{
	int i = 0;
	t_parameterlist *current = list;

	while(current->next)
	{
		current = current->next;

		switch(current->type)
		{
		case PL_REGISTER:
			log_add("%d: push %s", i++, t_register_list[current->reg]);
			break;

		case PL_MEMORYOFFSET:
			log_add("%d: push [%s + %d]", i++, t_register_list[current->reg], current->offset);
			break;

		case PL_POINTEROFFSET:
			log_add("%d: lea %s, [%s + %d] / push %s", i++, t_register_list[current->tempregister], t_register_list[current->reg], current->offset, t_register_list[current->tempregister]);
			break;

		case PL_CUSTOM:
			log_add("%d: custom, %d bytes", i++, current->custom_length);
		}
	}
}

void pl_add_pushes(t_parameterlist *list, t_buffer *buffer)
{
	t_parameterlist *current = list;
	char pushoffset_buffer[3];

	while(current->next)
	{
		current = current->next;

		switch(current->type)
		{
		case PL_REGISTER:
			buffer_insert_byte(buffer, asm_push(current->reg));
			break;

		case PL_MEMORYOFFSET:
			buffer_insert_bytes(buffer, asm_pushregisteroffset(current->reg, current->offset, (char *)&pushoffset_buffer), 3);
			break;

		case PL_POINTEROFFSET:
			buffer_insert_bytes(buffer, asm_leaoffset(current->tempregister, current->reg, current->offset, (char *)&pushoffset_buffer), 3);
			buffer_insert_byte(buffer, asm_push(current->tempregister));
			break;

		case PL_CUSTOM:
			buffer_insert_bytes(buffer, current->custom, current->custom_length);
			break;
		}
	}
}