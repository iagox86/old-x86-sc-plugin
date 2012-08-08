#if 0

#include "Logging.h"
#include "StarcraftAddresses.h"
#include "StarcraftDisplay.h"

#include <stdio.h>
#include <windows.h>

#include "StarcraftPlayers.h"

/* The size of the player struct is 36-bytes. */
#define PLAYERSIZE 36

/* The first byte is the status, followed by race, team, and then the 33-byte name. */
#define STATUS_INDEX 0
#define RACE_INDEX   1
#define TEAM_INDEX   2
#define NAME_INDEX   3

/* These are the three codes used for the three races. */
typedef enum
{
    RACE_ZERG = 0,
    RACE_TERRAN = 1,
    RACE_PROTOSS = 2,
    NUM_RACES = 3
} t_races;
static char *raceList[] = { "zerg", "terran", "protoss", "unknown" };

/* These are all the statuses I could determine for players. */
typedef enum
{
    NOTINGAME  = 0x00,
    COMPUTER   = 0x01,
    HUMAN      = 0x02,
    DEAD       = 0x0B,
    NUM_STATUS = 0x0C
} t_statuses;
static char *statusList[] = { "not in game", "computer", "human", "unknown", "unknown", "unknown", "unknown", "unknown", "unknown", "unknown", "unknown", "dead", "unknown" };

static unsigned char *getPlayerData(int num)
{
	/* addr_var_players is the start of the player struct. We index in multiples of PLAYERSIZE
	 * to jump to the start of the appropriate one. */
    return addr_var_players + ((num - 1) * PLAYERSIZE);
}

static unsigned char getStatus(int num)
{
    return getPlayerData(num)[STATUS_INDEX];
}

static unsigned char getRace(int num)
{
    return getPlayerData(num)[RACE_INDEX];
}

static unsigned char getTeam(int num)
{
    return getPlayerData(num)[TEAM_INDEX];
}

static char *getName(int num)
{
    return (char*)(getPlayerData(num) + NAME_INDEX);
}

static int getMinerals(int num)
{
	/* Minerals is a simple array. */
    return addr_var_minerals[num - 1];
}

static void setMinerals(int num, int val)
{
	addr_var_minerals[num - 1] = val;
}

static int getGas(int num)
{
	/* Gas is a simple array. */
    return addr_var_gas[num - 1];
}

static void setGas(int num, int val)
{
	addr_var_gas[num - 1] = val;
}

t_colour getPlayerColour(int number)
{
	return get_player_colour(number - 1);
}

char *getNameFromNumber(int number)
{
    if(number < 1 || number > 8)
        return NULL;

    return getName(number);
}

int getNumberFromName(char *name)
{
	int i;

	/* A linear search over 8 elements isn't the greatest, but it's not bad. */
    for(i = 1; i <= 8 ; i++)
        if(_stricmp(name, getName(i)) == 0)
            return i;

    return -1;
}

void display_players()
{
	int i;

	log_add("Displaying players");
	log_indent();
    for(i = 1; i <= 8; i++)
	{
        if(getStatus(i) != NOTINGAME)
            display_player_by_num(i);
	}
	log_undent();
}

/* Player number is 1..8. */
void display_player_by_num(int playerNum)
{
    if(playerNum < 1 || playerNum > 8)
        display_game(7000, "[red]Invalid player number.");
    else
    {
        int iRace = getRace(playerNum);
        int iStatus = getStatus(playerNum);
        char *name = getName(playerNum);
        int iMinerals = getMinerals(playerNum);
        int iGas = getGas(playerNum);
		char cColour = getPlayerColour(playerNum);

        if(iRace > NUM_RACES)
            iRace = NUM_RACES;
        if(iStatus > NUM_STATUS)
            iStatus = NUM_STATUS;

		log_add("%d: %16s is %7s, %12s, and has %4u/%4u.", playerNum, name, raceList[iRace], statusList[iStatus], iMinerals, iGas);
		display_game(15000, "[green]%d[white]: %c%s [white]is [yellow]%s[white], [yellow]%s[white], and has [paleblue]%u[white]/[green]%u[white].", playerNum, cColour, name, raceList[iRace], statusList[iStatus], iMinerals, iGas);
    }
}

void display_player_by_name(char *playerName)
{
    int playerNum = getNumberFromName(playerName);
	if(playerNum < 1 || playerNum > 8)
        display_game(7000, "[red]Unknown player.");
    else
		display_player_by_num(playerNum);
}

/* This function was more or less for demonstration. */
void display_races()
{
	int races[NUM_RACES];
	int i;

	races[RACE_TERRAN]  = 0;
	races[RACE_ZERG]    = 0;
	races[RACE_PROTOSS] = 0;

	for(i = 1; i <= 8; i++)
		if(getStatus(i) == COMPUTER || getStatus(i) == HUMAN)
			races[getRace(i)]++;

	display_game(10000, "Racial counts:");
	display_game(10000, "[white]%d [paleblue]players are [grey]Terran",       races[RACE_TERRAN]);
	display_game(10000, "[white]%d [paleblue]players are [purple]Zerg",       races[RACE_ZERG]);
	display_game(10000, "[white]%d [paleblue]players are [lightblue]Protoss", races[RACE_PROTOSS]);
}

void display_minerals(int player)
{
	display_game(7000, "[yellow]%s [white]has [paleblue]%d[white] minerals", getName(player), getMinerals(player));
}

void display_gas(int player)
{
	display_game(7000, "[yellow]%s [white]has [green]%d[white] gas", getName(player), getGas(player));
}

void change_minerals(int player, int value)
{
	if(player < 0)
		player = 1 + *MyPlayerNumber;
	setMinerals(player, value);
	
	display_minerals(player);
}

void change_gas(int player, int value)
{
	if(player < 0)
		player = 1 + *MyPlayerNumber;

	setGas(player, value);

	display_gas(player);
}

#endif // #if 0