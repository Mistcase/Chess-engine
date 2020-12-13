#include "Evaluation.h"

int32_t CountOnes(uint64_t n)
{
	n = ((n >> 1) & 0x5555555555555555llu) + (n & 0x5555555555555555llu);
	n = ((n >> 2) & 0x3333333333333333llu) + (n & 0x3333333333333333llu);
	n = ((n >> 4) & 0x0F0F0F0F0F0F0F0Fllu) + (n & 0x0F0F0F0F0F0F0F0Fllu);
	n = ((n >> 8) & 0x00FF00FF00FF00FFllu) + (n & 0x00FF00FF00FF00FFllu);
	n = ((n >> 16) & 0x0000FFFF0000FFFFllu) + (n & 0x0000FFFF0000FFFFllu);
	n = ((n >> 32) & 0x00000000FFFFFFFFllu) + (n & 0x00000000FFFFFFFFllu);

	return (int32_t)n;
}

int Evaluate(Position * position, int evaluationSide)
{
	int value = 0;
	unsigned int countOfValuableFigures = 0;

	//Phase of game
	for (int index = INDEX_KNIGHT; index < INDEX_KING; ++index)
		countOfValuableFigures += (CountOnes(position->figures[index][WHITE]) + CountOnes(position->figures[index][BLACK]));

	//Material
	for (int index = INDEX_PAWN; index < INDEX_KING; ++index)
	{
		uint64_t whiteFigures = position->figures[index][WHITE],
			blackFigures = position->figures[index][BLACK];

		while (whiteFigures)
		{
			size_t bitIndex = bitScanForward(whiteFigures);
			value += _evaluation_::positionEval[index][_evaluation_::posRedirectionArray[WHITE][bitIndex]];
			whiteFigures &= whiteFigures - 1;
		}
		while (blackFigures)
		{
			size_t bitIndex = bitScanForward(blackFigures);
			value -= _evaluation_::positionEval[index][_evaluation_::posRedirectionArray[BLACK][bitIndex]];
			blackFigures &= blackFigures - 1;
		}

		value += (CountOnes(position->figures[index][WHITE]) - CountOnes(position->figures[index][BLACK])) *
			_evaluation_::values[index];
	}

	Bitboard file = FILE_A;
	for (int i = 0; i < 8; i++)
	{
		unsigned int cb = CountOnes(position->figures[INDEX_PAWN][BLACK] & file), cw = CountOnes(position->figures[INDEX_PAWN][WHITE] & file);
		if (cb > 0)
			value += (cb - 1) * 50;
		if (cw > 0)
			value -= (cw - 1) * 50;
		file <<= 1;
	}

	if (countOfValuableFigures < 3)
	{
		value += _evaluation_::kingPositionEval[GAME_PHASE_ENDGAME][_evaluation_::posRedirectionArray[WHITE][bitScanForward(position->figures[INDEX_KING][WHITE])]];
		value -= _evaluation_::kingPositionEval[GAME_PHASE_ENDGAME][_evaluation_::posRedirectionArray[BLACK][bitScanForward(position->figures[INDEX_KING][BLACK])]];
		value *= ENDGAME_VALUE_MULTIPLIER;
		value /= 1000;
	}
	else
	{
		value += _evaluation_::kingPositionEval[GAME_PHASE_MIDGAME_OR_OPENING][_evaluation_::posRedirectionArray[WHITE][bitScanForward(position->figures[INDEX_KING][WHITE])]];
		value -= _evaluation_::kingPositionEval[GAME_PHASE_MIDGAME_OR_OPENING][_evaluation_::posRedirectionArray[BLACK][bitScanForward(position->figures[INDEX_KING][BLACK])]];
	}

	return value * evaluationSide;
}

int EvaluateFast(Position* startpos, ChessMove* chessMove)
{
	size_t fromF = bitScanForward(startpos->figures[chessMove->figureIndex][chessMove->figureColor] & chessMove->moveMask);
	size_t from = _evaluation_::posRedirectionArray[chessMove->figureColor][fromF],
		to = _evaluation_::posRedirectionArray[chessMove->figureColor][bitScanForward(chessMove->moveMask ^ (1ULL << fromF))];

	/*if (chessMove->moveMask == 2097216)
		std::cout << (startpos->figures[chessMove->figureIndex][chessMove->figureColor] & chessMove->moveMask) << std::endl;*/

	return static_cast<int>(_evaluation_::positionEval[chessMove->figureIndex][to] - _evaluation_::positionEval[chessMove->figureIndex][from]);
}

void _evaluation_::initEvaluationConsts()
{
	for (int sqFrom = 0, sqTo = 63; sqFrom < 64; sqFrom++, sqTo--)
		posRedirectionArray[WHITE][sqFrom] = sqTo;
	for (int sq = 0; sq < 64; sq++)
		posRedirectionArray[BLACK][sq] = sq;
}

size_t _evaluation_::posRedirectionArray[2][64];