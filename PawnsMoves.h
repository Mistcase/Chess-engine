#pragma once
#ifndef _PAWNS_MOVES_INCLUDED_
#define _PAWNS_MOVES_INCLUDED_

#include "Consts.h"

#define GetPawnMoves(color, sq, blockers) ((pawns_moves::moves[color][sq][0] & blockers) ? 0 : (pawns_moves::moves[color][sq][0] | \
((pawns_moves::moves[color][sq][1] & blockers) ? 0 : pawns_moves::moves[color][sq][1])))\

namespace pawns_moves
{
	void initPawnsMovesMasks();

	extern uint64_t moves[PLAYERS_COUNT][BOARD_SIZE][2];
	extern uint64_t bfAttackPositions[64];
	extern uint64_t bfDisabledPawnsMasks[64];
}
#endif