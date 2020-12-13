#pragma once
#ifndef _MOVES_H_INCLUDED_
#define _MOVES_H_INCLUDED_

#include <cstdint>
#include <iostream>
#include <algorithm>

#include "Consts.h"
#include "Position.h"
#include "Bitscan.h"
#include "ChessMove.h"
#include "PawnsMoves.h"
#include "Evaluation.h"
#include "Table.h"

#define CreateMove(fistMask_uint64_t, secondMask_uint64_t) fistMask_uint64_t | secondMask_uint64_t

namespace Castling
{
	enum : size_t
	{
		SHORT,
		LONG,
		CASTLINGS_TYPES_COUNT
	};

	void MakeCastlingWS(Position* position);
	void MakeCastlingWL(Position* position);
	void MakeCastlingBS(Position* position);
	void MakeCastlingBL(Position* position);

	bool CastlingIsPossibleWS(Position* position);
	bool CastlingIsPossibleWL(Position* position);
	bool CastlingIsPossibleBS(Position* position);
	bool CastlingIsPossibleBL(Position* position);

	void initCastlingsPointers();
	extern void(*castlingsPointersMakeOrUndo[PLAYERS_COUNT][CASTLINGS_TYPES_COUNT])(Position*);
	extern bool(*castlingIsPossible[PLAYERS_COUNT][CASTLINGS_TYPES_COUNT])(Position*);
	extern uint64_t castlingsKingMoveMasks[PLAYERS_COUNT][CASTLINGS_TYPES_COUNT];
}

namespace move_generation
{
	void GeneratePossibleMoves(Position* position, MovesList* list);
	void GeneratePossibleCaptures(Position* position, MovesList* list);
	void GeneratePossibleCastlings(Position* position, MovesList* list);
	void GeneratePossiblePawnSuperCaptures(Position* position, MovesList* list);
	void GeneratePossibleTransformations(Position* position, MovesList* list);
}

namespace move_sorting
{
	void mvv_lva(MovesList* list);
	void pick(Position* position, MovesList* list);
}

void MakeChessMove(Position* position, ChessMove& chessMove);

void UndoChessMove(Position* position, ChessMove& chessMove);

#endif