#pragma once
#ifndef _POSITION_INCLUDED_
#define  _POSITION_INCLUDED_

#include <cstdint>
#include <string>

#include "types.h"
#include "Consts.h"
#include "Bitscan.h"
#include "ConstAttacks.h"
#include "ConstAttacks.h"
#include "Magic.h"

struct Position
{
	uint64_t figures[FIGURES_TYPES_COUNT][PLAYERS_COUNT];
	uint64_t allFigures[PLAYERS_COUNT], allFiguresBothMask = 0;
	uint64_t allAttacks[PLAYERS_COUNT];
	uint32_t castlings[PLAYERS_COUNT] = { 0, 0 };
	uint32_t drawMoves = 0, movesCount = 0;
	uint64_t brokenField = 0;
	bool activePlayer;

	void refresh();

	int32_t value = 0;
	Key key;
};

#endif