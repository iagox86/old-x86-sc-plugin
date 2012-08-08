#include "StarcraftDisplay.h"
#include "Logging.h"
#include "StarcraftPlayers.h"

#include "Commands.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	PARSE_START, /* This state comes at the beginning, or after a space. */
	PARSE_IN_WORD,
	PARSE_ON_BACKSLASH,
	PARSE_IN_SINGLE_QUOTES,
	PARSE_IN_DOUBLE_QUOTES,
	PARSE_ON_DOUBLE_QUOTES_BACKSLASH
} t_parse_state;


/** This function is the main multiplexor for different commands. argc will always be 
 * set to the number of arguments + 1, and argv[] is the name of the command, followed 
 * by a list of arguments. The cases here should be in alphabetical order by the command's
 * name, and should only be a line or two. If any parameter parsing is necessary, it should
 * be done in another function. 
 */
static BOOL go(int argc, char *argv[], t_command_source source)
{
	BOOL processed = FALSE;

/*	if(!_stricmp(argv[0], "/player") && source == SOURCE_INGAME)
	{
		if(argc == 2)
			display_player_by_num(atoi(argv[1]));
		else
			display_game(7000, "[red]Error: /player requires one parameter");
		processed = TRUE;
	}
	else if(!_stricmp(argv[0], "/players") && source == SOURCE_INGAME)
	{
		display_players();
		processed = TRUE;
	}
	else if(!_stricmp(argv[0], "/races") && source == SOURCE_INGAME)
	{
		display_races();
		processed = TRUE;
	}
	else if(!_stricmp(argv[0], "/gas") && source == SOURCE_INGAME)
	{
		if(argc == 2)
			change_gas(-1, atoi(argv[1]));
		else if(argc == 3)
			change_gas(atoi(argv[1]), atoi(argv[2]));
		else
			display_game(7000, "[red]Error: use /gas <amount> or /gas <player> <amount>");

		processed = TRUE;
	}
	else if(!_stricmp(argv[0], "/minerals") && source == SOURCE_INGAME)
	{
		if(argc == 2)
			change_minerals(-1, atoi(argv[1]));
		else if(argc == 3)
			change_minerals(atoi(argv[1]), atoi(argv[2]));
		else
			display_game(7000, "[red]Error: use /minerals <amount> or /minerals <player> <amount>");

		processed = TRUE;
	}
	else */
	
	if(!_stricmp(argv[0], "/test") && source == SOURCE_CHANNEL)
	{
//		display_channel("Test was successful! (channel)");
		int addr = 0x190011B0;
		int i = 0;
		int result;

		for(i = 0; i < 100; i++)
		{
			__asm
			{
				push 0
				mov eax, i
				
				call addr

				mov result, eax
			}

			display_channel("%d => %d => %d => %d", i, result, i % result, result - (i % result));
		}

		

		processed = TRUE;
	}
	else if(!_stricmp(argv[0], "/test") && source == SOURCE_PREGAME)
	{
		display_pregame("Test was successful! (pregame)");
	}
	else if(!_stricmp(argv[0], "/test") && source == SOURCE_INGAME)
	{
		display_game(10000, "Test was successful! (ingame)");
	}

	return processed;
}


/* This returns the number of spaces in a given string. It is used to obtain an upper-limit on
 * how many parameters a command will have. */
static int count_spaces(const char *str)
{
	const char *s = str;
	int count = 0;
	do
	{
		if(*s == ' ')
			count++;
	}
	while(*(++s));

	return count;
}

BOOL parse_command(const char *command, t_command_source source)
{
	/* An array of strings, each string being an argument. */
	int argc = 0;
	int argc_limit = count_spaces(command);
	char **argv = malloc((argc_limit + 1) * sizeof(char *));
	int i;
	char *current_command;
	int pos;
	t_parse_state state = PARSE_START;
	BOOL retval = FALSE;

	memset(argv, 0, (argc_limit + 1) * sizeof(char *));

	current_command = malloc((strlen(command) + 1) * sizeof(char));
	memset(current_command, '\0', (strlen(command) + 1) * sizeof(char));
	pos = 0;
	
	for(i = 0; i < (int) strlen(command); i++)
	{
		char c = command[i];

		switch(state)
		{
		case PARSE_START:
			if(c == '\\')
			{
				state = PARSE_ON_BACKSLASH;
			}
			else if(c == ' ' || c == '\r' || c == '\n' || c == '\t')
			{
			}
			else if(c == '"')
			{
				state = PARSE_IN_DOUBLE_QUOTES;
			}
			else if(c == '\'')
			{
				state = PARSE_IN_SINGLE_QUOTES;
			}
			else
			{
				current_command[pos++] = c;
				state = PARSE_IN_WORD;
			}
			break;

		case PARSE_IN_WORD:
			if(c == '\\')
			{
				state = PARSE_ON_BACKSLASH;
			}
			else if(c == ' ' || c == '\r' || c == '\n' || c == '\t')
			{
				argv[argc++] = current_command;
				current_command = malloc((strlen(command) + 1) * sizeof(char));
				memset(current_command, '\0', strlen(command) * sizeof(char));
				pos = 0;

				// TODO: Complete word
				state = PARSE_START;
			}
			else if(c == '"')
			{
				state = PARSE_IN_DOUBLE_QUOTES;
			}
			else if(c == '\'')
			{
				state = PARSE_IN_SINGLE_QUOTES;
			}
			else
			{
				current_command[pos++] = c;
			}
			break;

		case PARSE_ON_BACKSLASH:
			current_command[pos++] = c;
			state = PARSE_IN_WORD;
			break;

		case PARSE_IN_DOUBLE_QUOTES:
			if(c == '\\')
			{
				state = PARSE_ON_DOUBLE_QUOTES_BACKSLASH;
			}
			else if(c == '"')
			{
				state = PARSE_IN_WORD;
			}
			else
			{
				current_command[pos++] = c;
			}
			break;

		case PARSE_ON_DOUBLE_QUOTES_BACKSLASH:
			current_command[pos++] = c;
			state = PARSE_IN_DOUBLE_QUOTES;
			break;

		case PARSE_IN_SINGLE_QUOTES:
			if(c == '\'')
			{
				state = PARSE_IN_WORD;
			}
			else
			{
				current_command[pos++] = c;
			}
			break;
		}
	}
	if(pos)
		argv[argc++] = current_command;

	/* There's no sense going if there are no arguments. */
	if(argc > 0)
		retval = go(argc, argv, source);

	for(i = 0; i < argc; i++)
		free(argv[i]);
	free(argv);

	return retval;
}

/*int main(int argc, char *argv)
{
	parse_command("param1 \"param 2\" param\\ 3-- 'param 4\'", 0);

	system("pause");
}*/