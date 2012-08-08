

#include "Commands.h"
#include "Logging.h"
#include "MemoryPatch.h"
#include "StarcraftPlayers.h"
#include "StarcraftAddresses.h"
#include "StarcraftState.h"

#include "MemoryPatches_StarcraftHooks.h"

static t_memorypatch *outgoing_channel_message = NULL;
static t_memorypatch *incoming_channel_message = NULL;

static t_memorypatch *outgoing_pregame_message = NULL;
static t_memorypatch *outgoing_ingame_message = NULL;

static void patch_outgoing_pregame_message()
{
	log_add("Patching outgoing pregame messages.");
	log_indent();
	if(!outgoing_pregame_message)
	{
		outgoing_pregame_message = mp_initialize(addr_patch_outgoing_pregame_message, (void *) patchfunc_outgoingpregamemessage, 5);
		mp_set_return_register(outgoing_pregame_message, EDX);
	}
	mp_apply(outgoing_pregame_message);

	log_undent();
}

static void unpatch_outgoing_pregame_message()
{
	log_add("Unpatching outgoing pregame messages");
	log_indent();
	if(outgoing_pregame_message)
		mp_remove(outgoing_pregame_message);
	log_undent();
}

static void patch_outgoing_ingame_message()
{
	log_add("Patching outgoing ingame messages.");
	log_indent();
	if(!outgoing_ingame_message)
	{
		outgoing_ingame_message = mp_initialize(addr_patch_outgoing_ingame_message, (void *) patchfunc_outgoingingamemessage, 5);
		mp_set_return_register(outgoing_ingame_message, EDX);
	}
	mp_apply(outgoing_ingame_message);

	log_undent();
}

static void unpatch_outgoing_ingame_message()
{
	log_add("Unpatching outgoing ingame messages");
	log_indent();
	if(outgoing_ingame_message)
		mp_remove(outgoing_ingame_message);
	log_undent();
}

static void patch_outgoing_channel_message()
{
	/* The number of bytes to patch over. */
	log_add("Patching outgoing channel messages");
	log_indent();
	if(!outgoing_channel_message)
	{
		outgoing_channel_message = mp_initialize(addr_patch_outgoing_channel_message, (void*) patchfunc_outgoingchannelmessage, 5);
		/* I tried EBP here, but it pooched the frame pointer. 
		 * I tried ESI, but it pooched an important value. 
		 * EBX, on the other hand, is useless! */
		mp_set_return_register(outgoing_channel_message, EBX);
		//mp_set_preserve_registers(outgoing_channel_message, TRUE);
	}
	mp_apply(outgoing_channel_message);
	log_undent();
}

static void unpatch_outgoing_channel_message()
{
	log_add("Unpatching outgoing channel messages");
	log_indent();
	if(outgoing_channel_message)
		mp_remove(outgoing_channel_message);
	log_undent();
}

static void patch_incoming_channel_message()
{
	log_add("Patching incoming channel messages.");
	log_indent();
	if(!incoming_channel_message)
	{
		incoming_channel_message = mp_initialize_useful((void *)addr_patch_incoming_channel_message, patchfunc_incomingchannelmessage, 9);
		mp_add_register_parameter(incoming_channel_message, EAX);
		mp_add_register_parameter(incoming_channel_message, ECX);
		mp_set_return_register(incoming_channel_message, EDX);
	}
	mp_apply(incoming_channel_message);

	log_undent();
}

static void unpatch_incoming_channel_message()
{
	log_add("Unpatching incoming channel messages");
	log_indent();
	if(incoming_channel_message)
		mp_remove(incoming_channel_message);
	log_undent();
}

void patches_addstarcraftpatches()
{
//	t_memorypatch *patch = mp_initialize_useful((void *)0x0048C6ED, patchfunc_display_one_parameter, 5);
//	mp_add_register_parameter(patch, EAX);
//	mp_apply(patch);

	log_add("Adding Starcraft-specific patches.");
	log_indent();
	patch_outgoing_pregame_message();
	patch_outgoing_ingame_message();
	patch_outgoing_channel_message();
	patch_incoming_channel_message();
	log_undent();
}

/* This function removes patches when exiting Battle.net. */
void patches_removestarcraftpatches()
{
	log_add("Removing Starcraft-specific patches.");
	log_indent();
	unpatch_outgoing_pregame_message();
	unpatch_outgoing_ingame_message();
	unpatch_outgoing_channel_message();
	unpatch_incoming_channel_message();
	log_undent();
}

BOOL __stdcall patchfunc_outgoingchannelmessage(int code, char *message)
{
	log_add("Sending [channel]: code = %02x; message = %s", code, message);

	if(message)
	{
		if(!parse_command(message, SOURCE_CHANNEL))
			send_tcp(code, message, (int) strlen(message) + 1);
	}

	return TRUE;
}

/** Return value: FALSE displays the message, TRUE cancels it. */
BOOL __stdcall patchfunc_outgoingpregamemessage(char *message)
{
	int retval = FALSE;

	__asm { push edx } // Has to be preserved. 

	if(message)
	{
		log_add("Pregame: %s", message);

		retval = parse_command(message, SOURCE_PREGAME);

		if(!retval)
			retval = send_command_from_game(message);
	}

	__asm { pop edx } // Has to be preserved. 
	return retval;
}

/** Return value: FALSE displays the message, TRUE cancels it. */
BOOL __stdcall patchfunc_outgoingingamemessage(char *message)
{
	int retval = FALSE;
	__asm { push edx } // Has to be preserved. 

	if(message)
	{
		log_add("Ingame: %s", message);

		retval = parse_command(message, SOURCE_INGAME);
		if(!retval)
			retval = send_command_from_game(message);
	}

	__asm { pop edx } // Has to be preserved. 
	return retval;
}

void __stdcall patchfunc_incomingchannelmessage(char *data, int length)
{
	int  *code     = (int*) data;

	char *username = data + 0x18;
	char *message  = data + 0x18 + strlen(username) + 1;

	log_add("[%02x] %s: %s", code, username, message);

	/* To stop a packet from being processed (for whatever reason), set the code to 0: */
	// *code = 0;
}
