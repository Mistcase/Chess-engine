#pragma once
#ifndef _ROOK_MASKS_INCLUDED_
#define _ROOK_MASKS_INCLUDED_

#include <cstdint>

#include "Consts.h"
#include "Rays.h"
#include "ConstAttacks.h"
#include "Bitscan.h"
#include "Ranks_Files.h"

namespace sliding_funcs
{
	void initRookMasks();
}

uint64_t findRookAttacks(uint64_t& allFigures, size_t sq);

extern uint64_t rookMasks[BOARD_SIZE];
extern uint64_t rookTable[BOARD_SIZE][ROOK_ANY_SQ_MAX_MAGIC_ATTACKS];

#endif