#include "Position.h"

void Position::refresh()
{
	this->allFigures[WHITE] = 0;
	this->allFigures[BLACK] = 0;
	this->allAttacks[BLACK] = 0;
	this->allAttacks[WHITE] = 0;

	uint64_t figures[FIGURES_TYPES_COUNT][PLAYERS_COUNT];
	for (size_t index = INDEX_PAWN; index < FIGURES_TYPES_COUNT; index++)
	{
		figures[index][WHITE] = this->figures[index][WHITE];
		figures[index][BLACK] = this->figures[index][BLACK];

		this->allFigures[WHITE] |= this->figures[index][WHITE];
		this->allFigures[BLACK] |= this->figures[index][BLACK];
	}

	this->allFiguresBothMask = this->allFigures[WHITE] | this->allFigures[BLACK];

	for (int color = 0; color < 2; color++)
	{
		while (this->figures[INDEX_PAWN][color])
		{
			this->allAttacks[color] |= pawnsAttacks[color][bitScanForward(this->figures[INDEX_PAWN][color])];
			this->figures[INDEX_PAWN][color] &= this->figures[INDEX_PAWN][color] - 1;
		}
		while (this->figures[INDEX_KNIGHT][color])
		{
			this->allAttacks[color] |= knightsAttacks[bitScanForward(this->figures[INDEX_KNIGHT][color])];
			this->figures[INDEX_KNIGHT][color] &= this->figures[INDEX_KNIGHT][color] - 1;
		}
		while (this->figures[INDEX_BISHOP][color])
		{
			this->allAttacks[color] |= GetBishopAttacks(bitScanForward(this->figures[INDEX_BISHOP][color]), this->allFiguresBothMask);
			this->figures[INDEX_BISHOP][color] &= this->figures[INDEX_BISHOP][color] - 1;
		}
		while (this->figures[INDEX_ROOK][color])
		{
			this->allAttacks[color] |= GetRookAttacks(bitScanForward(this->figures[INDEX_ROOK][color]), this->allFiguresBothMask);
			this->figures[INDEX_ROOK][color] &= this->figures[INDEX_ROOK][color] - 1;
		}
		while (this->figures[INDEX_QUEEN][color])
		{
			this->allAttacks[color] |= (GetRookAttacks(bitScanForward(this->figures[INDEX_QUEEN][color]), this->allFiguresBothMask) |
				GetBishopAttacks(bitScanForward(this->figures[INDEX_QUEEN][color]), this->allFiguresBothMask));
			this->figures[INDEX_QUEEN][color] &= this->figures[INDEX_QUEEN][color] - 1;
		}
		this->allAttacks[WHITE] |= kingsAttacks[bitScanForward(this->figures[INDEX_KING][WHITE])];
		this->allAttacks[BLACK] |= kingsAttacks[bitScanForward(this->figures[INDEX_KING][BLACK])];
	}

	for (size_t index = INDEX_PAWN; index < FIGURES_TYPES_COUNT; index++)
	{
		this->figures[index][WHITE] = figures[index][WHITE];
		this->figures[index][BLACK] = figures[index][BLACK];
	}
}
