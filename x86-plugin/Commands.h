#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include <windows.h>

/** This module takes care of parsing users' commands and sending them to the appropriate handler,
 * if necessary. This is a good lesson in using a finite state machine to parse strings, so 
 * anybody who doesn't know how to do that should have a look at Commands.c. It's valuable to
 * know. */

typedef enum
{
	SOURCE_CHANNEL,
	SOURCE_PREGAME,
	SOURCE_INGAME,
	SOURCE_OTHER,
} t_command_source;

/* When a user types a command, either in channel, pregame, or ingame, this is called. 
 * The last parameter indicates where the command was sent from. */
BOOL parse_command(const char *command, t_command_source source);

#endif