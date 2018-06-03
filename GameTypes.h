#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <iostream>
#include <vector>

#include "cd_game.h"
#include "TArray.h"

enum compass_directions	{NORTH = 0, EAST, SOUTH, WEST};
enum entity_types		{EMPTY = 0, FLOOR, WALL, ICE, FIRE, HERO, END, START, COIN};
enum game_states		{LOSE = 0, RUNNING};

typedef entity_types		ENTITY;

typedef struct _tile
{
	ENTITY	type;
} TILE, *LPTILE;

typedef game_states			GAMESTATE;
typedef compass_directions	DIRECTION;
typedef TArray2D<TILE>		GAMEBOARD;

#endif