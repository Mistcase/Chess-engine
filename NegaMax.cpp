#include "NegaMax.h"

int Search(Position* position, int depth, int ply, int alpha, int beta, LinePV* pv, const bool* stopFlag)
{
	int32_t score = -INFINITY_VALUE, currentValue;
	bool activeColor = position->activePlayer, oppColor = !position->activePlayer;
	MovesList movesLists[move_types::MovesTypesCount];
	LinePV tempLine;

	if (depth <= 0 || ply == MAX_PLY || *stopFlag)
		return Evaluate(position, GetEvaluationSide(position));//ForceSearch(position, alpha, beta, &tempLine, stopFlag);

	move_generation::GeneratePossibleTransformations(position, &movesLists[0]);
	move_generation::GeneratePossiblePawnSuperCaptures(position, &movesLists[1]);
	move_generation::GeneratePossibleCaptures(position, &movesLists[2]);
	move_generation::GeneratePossibleCastlings(position, &movesLists[3]);
	move_generation::GeneratePossibleMoves(position, &movesLists[4]);

	move_sorting::pick(position, &movesLists[4]);
	move_sorting::mvv_lva(&movesLists[2]);

	for (uint_fast8_t moveType = 0; moveType < move_types::MovesTypesCount; moveType++)
	{
		ChessMove* ptrMove = nullptr;
		while (alpha < beta && (ptrMove = movesLists[moveType].nextMove()) != nullptr)
		{
			MakeChessMove(position, *ptrMove);
			if (position->figures[INDEX_KING][activeColor] & ~position->allAttacks[oppColor])
			{
				tempLine.reset();

				int newSearhDepth = depth - 1;
				currentValue = -Search(position, newSearhDepth, ply + 1, -(alpha + 1), -alpha, &tempLine, stopFlag);
				if (currentValue > alpha && currentValue < beta)
					currentValue = -Search(position, newSearhDepth, ply + 1, -beta, -currentValue, &tempLine, stopFlag);

				if (currentValue > score)
				{
					score = currentValue;
					if (score > alpha)
						alpha = score;
					if (alpha <= beta)
						pv->writeLine(&tempLine, ptrMove->moveMask);
				}
			}
			UndoChessMove(position, *ptrMove);
		}
	}

	if (score == -INFINITY_VALUE && position->figures[INDEX_KING][activeColor] & position->allAttacks[oppColor])
		score = VALUE_MATE - ply;
	else if (score == -INFINITY_VALUE)
		score = 0;

	return score;
}

int ForceSearch(Position * position, int alpha, int beta, LinePV * pv, const bool * stopFlag)
{
	bool activeColor = position->activePlayer, oppColor = !position->activePlayer;
	MovesList capturesList;
	LinePV tempLine;

	int val = Evaluate(position, GetEvaluationSide(position));
	if (val > alpha)
		alpha = val;

	if (*stopFlag)
		return val;

	move_generation::GeneratePossibleCaptures(position, &capturesList);

	ChessMove* ptrMove = nullptr;
	while (alpha < beta && (ptrMove = capturesList.nextMove()) != nullptr)
	{
		MakeChessMove(position, *ptrMove);
		if (position->figures[INDEX_KING][activeColor] & ~position->allAttacks[oppColor])
		{
			tempLine.reset();
			val = -ForceSearch(position, -beta, -alpha, &tempLine, stopFlag);
		}
		UndoChessMove(position, *ptrMove);

		if (val > alpha)
		{
			alpha = val;
			if (alpha <= beta)
				pv->writeLine(&tempLine, ptrMove->moveMask);
		}
	}
	return alpha;
}
