#include "Thread.h"

bool Thread::isActive()
{
	return !this->threadStopFlag;
}

bool Thread::setPosition(Position position)
{
	uint64_t mask = 0;

	mask = position.figures[INDEX_PAWN][WHITE] & position.figures[INDEX_PAWN][BLACK] & position.figures[INDEX_KNIGHT][WHITE] & position.figures[INDEX_KNIGHT][BLACK]
		& position.figures[INDEX_BISHOP][WHITE] & position.figures[INDEX_BISHOP][BLACK] & position.figures[INDEX_ROOK][WHITE] & position.figures[INDEX_ROOK][BLACK]
		& position.figures[INDEX_QUEEN][WHITE] & position.figures[INDEX_QUEEN][BLACK] & position.figures[INDEX_KING][WHITE] & position.figures[INDEX_KING][BLACK];

	if (!mask && position.figures[INDEX_KING][WHITE] != 0 && position.figures[INDEX_KING][BLACK] != 0)
	{
		this->position = position;
		return true;
	}

	return false;
}

void Thread::go(Depth depth)
{
	this->threadStopFlag = false;
	this->searchDepth = depth;
	this->position.key = 0;

	this->nativeHandle = std::thread([&]()
	{
		for (Depth newDepth = 1; newDepth <= searchDepth; newDepth++)
		{
			this->posProcCount = 0;

			LinePV pv;
			Flag hashFound(false);
			Timer timer;
			int searchResult = AlphaBeta(newDepth, 0, -INFINITY_VALUE, INFINITY_VALUE, &pv);
			TimeValue elapsedTime = timer.elapsed(false);
			String movesLine = pv.createString(this->position);

			sync_cout << "info depth " + std::to_string(newDepth) + " multipv 1 score " + SearchValue(searchResult) + " nodes " +
				std::to_string(posProcCount) + " nps " + std::to_string(posProcCount / elapsedTime * 1000) + " time " + std::to_string(elapsedTime) + " pv " + movesLine << sync_endl;

			if (newDepth == searchDepth || abs(searchResult) > abs(VALUE_MATE))
			{
				String firstMove(""), secondMove("");
				Parser parser(pv.createString(this->position));
				firstMove = parser.getNextWord();
				secondMove = parser.getNextWord();

				sync_cout << "bestmove " + firstMove;
				if (!secondMove.empty())
					std::cout << " ponder " + secondMove;
				std::cout << sync_endl;
				break;
			}
		}
		this->threadStopFlag = true;
	});
	this->nativeHandle.detach();
}

void Thread::stop()
{
	this->threadStopFlag = true;
}


int Thread::AlphaBeta(Depth depth, Depth ply, int alpha, int beta, LinePV* pv)
{
	int32_t score = -INFINITY_VALUE, currentValue = -INFINITY_VALUE;
	bool activeColor = position.activePlayer, oppColor = !position.activePlayer;
	MovesList movesLists[move_types::MovesTypesCount];
	LinePV tempLine;
	int evalSide = activeColor == WHITE ? EVALUATION_SIDE_WHITE : EVALUATION_SIDE_BLACK;
	int valueStartPos = Evaluate(&position, evalSide);
	ChessMove* ptrMove = nullptr;

	if (depth <= 0 || ply == MAX_PLY || threadStopFlag)
	{
		return Quies(alpha, beta, &tempLine);
	}

	move_generation::GeneratePossibleTransformations(&position, &movesLists[0]);
	move_generation::GeneratePossiblePawnSuperCaptures(&position, &movesLists[1]);
	move_generation::GeneratePossibleCaptures(&position, &movesLists[2]);
	move_generation::GeneratePossibleCastlings(&position, &movesLists[3]);
	move_generation::GeneratePossibleMoves(&position, &movesLists[4]);

	for (size_t i = 0; i < movesLists[4].getCounterFilled(); i++)
		movesLists[4].getVariant(i)->value = EvaluateFast(&position, movesLists[4].getVariant(i));
	std::sort(movesLists[4].getVariant(0), movesLists[4].getVariant(movesLists[4].getCounterFilled()), [](const ChessMove& first, const ChessMove& second)
	{
		return first.value > second.value;
	});
	move_sorting::mvv_lva(&movesLists[2]);
	if (depth >= 4)
		movesLists[4].cutTo(FutilityMoveCount[ply < 9 ? ply : 8]);

	for (uint_fast8_t moveType = 0; moveType < move_types::MovesTypesCount; moveType++)
	{
		while (alpha < beta && (ptrMove = movesLists[moveType].nextMove()) != nullptr)
		{
			position.refresh();
			Flag ourKingChecked = position.figures[INDEX_KING][activeColor] & position.allAttacks[oppColor];

			MakeChessMove(&position, *ptrMove);
			if (position.figures[INDEX_KING][activeColor] & ~position.allAttacks[oppColor])
			{
				int newSearhDepth;
				tempLine.reset();

				//Furtility pruning
				int margin = _evaluation_::values[INDEX_PAWN] / 4;
				int valueNow = Evaluate(&position, evalSide);
				/*if (depth - 1 <= 2
					&& ptrMove->moveType != move_types::Capture
					&& position.figures[INDEX_KING][oppColor] & ~position.allAttacks[activeColor]
					&& !ourKingChecked
					&& !ptrMove->needMoreSearch
					&& valueNow + margin <= alpha)
					goto undo;*/
					//Razoring from Vulkanus
					if (ply >= 4
						&& ptrMove->moveType != move_types::Capture
						&& !ourKingChecked
						&& !ptrMove->needMoreSearch
						&& position.figures[INDEX_KING][oppColor] & ~position.allAttacks[activeColor]
						&& valueNow < valueStartPos)
						newSearhDepth = depth / 2 - 1;
					else
					{
						newSearhDepth = depth - 1;
						/*if (depth == 1)
						{
							if (ptrMove->figureIndex == INDEX_PAWN && (((ptrMove->moveMask & RANK_7) && ptrMove->figureColor == WHITE) ||
								(((ptrMove->moveMask & RANK_2) && ptrMove->figureColor == BLACK))))
								newSearhDepth = 2;
						}*/
					}

				currentValue = -AlphaBeta(newSearhDepth, ply + 1, -(alpha + 1), -alpha, &tempLine);
				if (currentValue > alpha && currentValue < beta)
				{
					tempLine.reset();
					currentValue = -AlphaBeta(newSearhDepth, ply + 1, -beta, -currentValue, &tempLine);
				}

			//undo:
				if (currentValue > score)
				{
					score = currentValue;
					if (score > alpha)
						alpha = score;
					if (alpha <= beta)
						pv->writeLine(&tempLine, ptrMove->moveMask);
				}
			}
			UndoChessMove(&position, *ptrMove);
		}
	}

	if (score == -INFINITY_VALUE && position.figures[INDEX_KING][activeColor] & position.allAttacks[oppColor])
		score = VALUE_MATE - ply;
	else if (score == -INFINITY_VALUE)
		score = 0;

	return score;
}

int Thread::Quies(int alpha, int beta, LinePV* pv)
{
	bool activeColor = position.activePlayer, oppColor = !position.activePlayer;
	MovesList capturesList;
	LinePV tempLine;

	int val = Evaluate(&position, position.activePlayer == WHITE ? EVALUATION_SIDE_WHITE : EVALUATION_SIDE_BLACK);
	this->posProcCount++;
	if (val > alpha)
	{
		tempLine.reset();
		alpha = val;
	}

	move_generation::GeneratePossibleCaptures(&position, &capturesList);

	ChessMove* ptrMove = nullptr;
	while (alpha < beta && (ptrMove = capturesList.nextMove()) != nullptr)
	{
		MakeChessMove(&position, *ptrMove);
		if (position.figures[INDEX_KING][activeColor] & ~position.allAttacks[oppColor])
		{
			tempLine.reset();
			val = -Quies(-beta, -alpha, &tempLine);
			this->posProcCount++;

			if (val > alpha)
			{
				alpha = val;
				if (alpha <= beta)
					pv->writeLine(&tempLine, ptrMove->moveMask);
			}

		}
		UndoChessMove(&position, *ptrMove);
	}
	return alpha;
}

String Thread::SearchValue(int value)
{
	int absVal = abs(value);
	return (absVal > abs(VALUE_MATE)) ? ("mate " + std::to_string(value < 0 ? (value - VALUE_MATE) : (value + VALUE_MATE))) : "cp " + std::to_string(value / 10);
}
