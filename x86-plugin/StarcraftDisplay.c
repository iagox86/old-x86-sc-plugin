#include "Buffer.h"
#include "Match.h"
#include "StarcraftAddresses.h"

#include <stdio.h>
#include <windows.h>

#include "StarcraftDisplay.h"

char *get_colour_string(t_colour colour, char buffer[2]);

void display_pregame(char *message, ...)
{
    char buffer[DISPLAY_MAX + 1];

    va_list argptr;
    va_start(argptr, message);

	memset(buffer, 0, DISPLAY_MAX);
    _vsnprintf_s(buffer, DISPLAY_MAX, DISPLAY_MAX, message, argptr);

	display_channel_simple(EID_INFO, "", buffer);;
}

void display_channel(char *message, ...)
{
    char buffer[DISPLAY_MAX + 1];

    va_list argptr;
    va_start(argptr, message);

	memset(buffer, 0, DISPLAY_MAX);
    _vsnprintf_s(buffer, DISPLAY_MAX, DISPLAY_MAX, message, argptr);

	display_channel_simple(EID_INFO, "", buffer);
}

void display_channel_simple(t_messagetype type, char *username, char *message)
{
	display_channel_raw(type, 0, 0, username, message);
}



char *get_colour_string(t_colour colour, char buffer[2])
{
	buffer[0] = (char) colour;
	buffer[1] = '\0';

	return buffer;
}

void display_game(int ms_duration, char *message, ...)
{
	char colour_buffer[2];
    char buffer[DISPLAY_MAX + 1];

    va_list argptr;
    va_start(argptr, message);

	memset(buffer, 0, DISPLAY_MAX);
    _vsnprintf_s(buffer, DISPLAY_MAX, DISPLAY_MAX, message, argptr);

	/* Do color replacements. */
	match_replaceall("[nocolor]",      get_colour_string(SC_NOCOLOR,      colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[unknown1]",     get_colour_string(SC_UNKNOWN1,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[paleblue]",     get_colour_string(SC_PALEBLUE,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[yellow]",       get_colour_string(SC_YELLOW,       colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[white]",        get_colour_string(SC_WHITE,        colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[grey]",         get_colour_string(SC_GREY,         colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[darkred]",      get_colour_string(SC_DARKRED,      colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[green]",        get_colour_string(SC_GREEN,        colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[lightred]",     get_colour_string(SC_LIGHTRED,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[tab]",          get_colour_string(SC_TAB,          colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[cr]",           get_colour_string(SC_CR,           colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[unknown2]",     get_colour_string(SC_UNKNOWN2,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[lf]",           get_colour_string(SC_LF,           colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[unknown3]",     get_colour_string(SC_UNKNOWN3,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[darkblue]",     get_colour_string(SC_DARKBLUE,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[teal]",         get_colour_string(SC_TEAL,         colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[purple]",       get_colour_string(SC_PURPLE,       colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[orange]",       get_colour_string(SC_ORANGE,       colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[right]",        get_colour_string(SC_RIGHT,        colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[center]",       get_colour_string(SC_CENTER,       colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[unknown4]",     get_colour_string(SC_UNKNOWN4,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[brown]",        get_colour_string(SC_BROWN,        colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[offwhite]",     get_colour_string(SC_OFFWHITE,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[brightyellow]", get_colour_string(SC_BRIGHTYELLOW, colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[darkgreen]",    get_colour_string(SC_DARKGREEN,    colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[paleyellow]",   get_colour_string(SC_PALEYELLOW,   colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[unknown5]",     get_colour_string(SC_UNKNOWN5,     colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[peach]",        get_colour_string(SC_PEACH,        colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[blue]",         get_colour_string(SC_BLUE,         colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[palegreen]",    get_colour_string(SC_PALEGREEN,    colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[mauve]",        get_colour_string(SC_MAUVE,        colour_buffer), buffer, DISPLAY_MAX);
	match_replaceall("[lightblue]",    get_colour_string(SC_LIGHTBLUE,    colour_buffer), buffer, DISPLAY_MAX);
 


	display_game_raw(ms_duration, SC_PALEBLUE, buffer);
}
