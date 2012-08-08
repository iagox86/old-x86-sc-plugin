#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#define DISPLAY_MAX 256

/** Functions here are used for displaying messages, either in the game or in the channel. 
 * The actual raw display calls are defined in StarcraftAddresses.h, the functions here are
 * simply wrappers that allow formatting. */



/* These are colours that can be used in in-game messages. I haven't tried to use them 
 * in channels, but it might work. Where there was no obvious colour change, I documented
 * what the code actually did. */
typedef enum
{
	SC_NOCOLOR      = 0x00, /* This takes the color of the previous line. */
	SC_UNKNOWN1     = 0x01, /* Same as SC_NOCOLOR. */
	SC_PALEBLUE     = 0x02, /* This is the default color. A pale blue, almost mauve-ish. */
	SC_YELLOW       = 0x03,
	SC_WHITE        = 0x04,
	SC_GREY         = 0x05, /* This also prevents any other colors on the line. */
	SC_DARKRED      = 0x06,
	SC_GREEN        = 0x07,
	SC_LIGHTRED     = 0x08,
	SC_TAB          = 0x09, /* Appears to be a tab with 4-space columns. */
	SC_CR           = 0x0A, /* New line. I'm assuming CR. */
	SC_UNKNOWN2     = 0x0B, /* Terminates output, like a NULL. */
	SC_LF           = 0x0C, /* New line. I'm assuming LF. */
	SC_UNKNOWN3     = 0x0D, /* Does nothing. */
	SC_DARKBLUE     = 0x0E,
	SC_TEAL         = 0x0F,
	SC_PURPLE       = 0x10,
	SC_ORANGE       = 0x11,
	SC_RIGHT        = 0x12, /* Text after will be aligned right. */
	SC_CENTER       = 0x13, /* Text after will be aligned center. */
	SC_UNKNOWN4     = 0x14, /* Teminates output, like a NULL. */
	SC_BROWN        = 0x15,
	SC_OFFWHITE     = 0x16, /* A greenish white... */
	SC_BRIGHTYELLOW = 0x17,
	SC_DARKGREEN    = 0x18,
	SC_PALEYELLOW   = 0x19,
	SC_UNKNOWN5     = 0x1A, /* Does nothing. */
	SC_PEACH        = 0x1B,
	SC_BLUE         = 0x1C,
	SC_PALEGREEN    = 0x1D,
	SC_MAUVE        = 0x1E,
	SC_LIGHTBLUE    = 0x1F  /* A nice, light blue color. A very different shade than "pale blue" */
} t_colour;

/** These codes can be used in channel messages. */
typedef enum
{
	EID_SHOWUSER            = 0x01,
	EID_JOIN                = 0x02,
	EID_LEAVE               = 0x03,
	EID_WHISPER             = 0x04,
	EID_TALK                = 0x05,
	EID_BROADCAST           = 0x06,
	EID_CHANNEL             = 0x07,
	EID_UNKNOWN1            = 0x08,
	EID_USERFLAGS           = 0x09,
	EID_WHISPERSENT         = 0x0A,
	EID_UNKNOWN2            = 0x0B,
	EID_UNKNOWN3            = 0x0C,
	EID_CHANNELFULL         = 0x0D,
	EID_CHANNELDOESNOTEXIST = 0x0E,
	EID_CHANNELRESTRICTED   = 0x0F,
	EID_UNKNOWN4            = 0x10,
	EID_UNKNOWN5            = 0x11,
	EID_INFO                = 0x12,
	EID_ERROR               = 0x13,
	EID_UNKNOWN6            = 0x14,
	EID_IGNORE              = 0x15,
	EID_UNIGNORE            = 0x16,
	EID_EMOTE               = 0x17
} t_messagetype;


/* Display a formatted message, which uses printf-style formatting. Colour codes can also
 * be used in this string, which is the name of a colour surrounded by square brackets. For
 * example, [mauve] will make everything after mauve, likewise [blue], [peach], etc. */
void display_game(int ms_duration, char *message, ...);

/* Display a formatted message in pregame. */
void display_pregame(char *message, ...);

/* Display a formatted message in the channel. */
void display_channel(char *message, ...);

/* Display any EID_ packet in the channel, with the specified username and ping. */
void display_channel_simple(t_messagetype type, char *username, char *message);

#endif