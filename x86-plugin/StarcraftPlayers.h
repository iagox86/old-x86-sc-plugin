#if 0
/* Players.h
 * by iago
 * Sept. 10/2003
 *
 * These are functions for displaying the characters in a game, as well as querying 
 * information about them (minerals, gas, etc.)
 */

#ifndef PLAYERS_H
#define PLAYERS_H

#include "StarcraftDisplay.h"

#include <stdio.h>
#include <windows.h>

/* Convert a player number to the corresponding name. */
char *getNameFromNumber(int number);

/* Convert a player name to the corresponding number. This is a linear search, so it's not
 * terribly efficient, but there are only 8 locations, so eh? */
int getNumberFromName(char *name);

/* Displays information on all players. */
void display_players();

/* Displays a single player, based on its number. */
void display_player_by_num(int playerNum);

/* Displays a single player, based on its name. */
void display_player_by_name(char *playerName);

/* Display the number of players who are each race. */
void display_races();

/* Display the minerals for a specific player. */
void display_minerals(int player);
/* Display the gas for a specific player. */
void display_gas(int player);

/* Change the value of a player's minerals. This will drop you in a multiplayer game. */
void change_minerals(int player, int value);
/* Change the value of a player's gas. This will drop you in a multiplayer game. */
void change_gas(int player, int value);

#endif
#endif // #if 0