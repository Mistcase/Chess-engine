#pragma once
#ifndef _MAGIC_FUNCTIONAL_INCLUDED_
#define _MAGIC_FUNCTIONAL_INCLUDED_

#include <cstdint>

#include "Consts.h"
#include "Rays.h"
#include "Bitscan.h"

#define GetBishopAttacks(sq, allFiguresBothMask) (sliding_funcs::bishopTable[sq][((allFiguresBothMask & sliding_funcs::bishopMasks[sq]) * _bishopMagics[sq]) >> 54])
#define GetRookAttacks(sq, allFiguresBothMask) (sliding_funcs::rookTable[sq][((allFiguresBothMask & sliding_funcs::rookMasks[sq]) * _rookMagics[sq]) >> 52])

namespace sliding_funcs
{
	uint64_t GetMaxValue(unsigned int bitsCount);

	uint64_t findBishopsAttacks(uint64_t& allFiguresBothMask, size_t sq);
	uint64_t findRookAttacks(uint64_t& allFiguresBothMask, size_t sq);

	void initBishopMasks();
	void initRookMasks();

	void initMagicBishops();
	void initMagicRooks();

	extern uint64_t bishopMasks[BOARD_SIZE];
	extern uint64_t bishopTable[BOARD_SIZE][BISHOP_ANY_SQ_MAX_MAGIC_ATTACKS];
	extern uint64_t rookMasks[BOARD_SIZE];
	extern uint64_t rookTable[BOARD_SIZE][ROOK_ANY_SQ_MAX_MAGIC_ATTACKS];
}
#endif