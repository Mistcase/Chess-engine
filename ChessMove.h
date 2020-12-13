#pragma once
#ifndef _CHESS_MOVE_INCLUDED_
#define _CHESS_MOVE_INCLUDED_

#include <cstdint>
#include <stdio.h>

#include "types.h"
#include "Consts.h"

namespace move_types
{
	enum : uint_fast8_t
	{
		Transformation,
		PawnSuperCapture,
		Capture,
		Castling,
		Quiet,
		MovesTypesCount,

		MoveEmpty
	};
}

struct ChessMove
{
	uint_fast8_t figureIndex, figureColor, moveType;
	uint64_t moveMask;
	int value;

	uint_fast8_t victimIndex, victimColor, castlingType, castlingColor, brokenFieldIndex, transFieldIndex, transNewFigureIndex;
	Flag needMoreSearch = false;

	//Old position data
	uint32_t oldCastlings[PLAYERS_COUNT];
	uint64_t oldBrokenFieldValue;
	uint32_t oldDrawMoves;
};

class MovesList
{
public:
	void push(ChessMove& chessMove);
	ChessMove* nextMove();

	ChessMove* getVariant(size_t);
	size_t getCounterFilled() const;
	size_t getCounterReturned() const;
	void cutTo(size_t);
	Flag find(Bitboard moveMask);

private:
	ChessMove variants[MAX_BRANCHING_RATIO];
	size_t counterFilled = 0, counterReturned = 0;
};

#endif