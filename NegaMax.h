#pragma once
#ifndef _NEGAMAX_INCLUDED_
#define _NEGAMAX_INCLUDED_

#include <cstdint>

#include "Consts.h"
#include "Position.h"
#include "Evaluation.h"
#include "Moves.h"
#include "PawnsMoves.h"
#include "Bitscan.h"
#include "PrincipalVariation.h"
#include "Coods.h"

#include <iostream>

int Search(Position* position, int depth, int ply, int alpha, int beta, LinePV* pv, const bool* stopFlag);
int ForceSearch(Position* position, int alpha, int beta, LinePV* pv, const bool* stopFlag);

//Subfunctions
#define EnemyKingIsAttacked(positionPtr, activeColor, oppColor) positionPtr->figures[INDEX_KING][oppColor] & positionPtr->allAttacks[activeColor]
#define GetEvaluationSide(positionPtr) positionPtr->activePlayer == WHITE ? EVALUATION_SIDE_WHITE : EVALUATION_SIDE_BLACK

#endif