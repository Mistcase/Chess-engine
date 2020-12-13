#include "PrincipalVariation.h"

void LinePV::writeLine(LinePV* sourceLine, uint64_t& bestMove)
{
	for (size_t i = 0; i < sourceLine->count; i++)
		this->moves[i + 1] = sourceLine->moves[i];
	this->moves[0] = bestMove;
	this->count = sourceLine->count + 1;
}

void LinePV::reset()
{
	this->count = 0;
}

std::string LinePV::createString(Position position)
{
	std::string pv("");

	for (size_t i = 0; i < this->count; i++)
	{
		//std::cout << "i = " << i << ": " << this->moves[i] << std::endl;

		uint64_t from = position.allFigures[position.activePlayer] & this->moves[i];
		pv += (GetBoardPosition(bitScanForward(from)) + GetBoardPosition(bitScanForward(this->moves[i] ^ from)) + " ");
		position.allFigures[position.activePlayer] ^= this->moves[i];
		if (position.allFigures[!position.activePlayer] & this->moves[i])
			position.allFigures[!position.activePlayer] &= ~this->moves[i];
		position.activePlayer = !position.activePlayer;
	}

	return pv;
}

Flag LinePV::find(Bitboard moveMask)
{
	for (int i = 0; i < this->count; i++)
	{
		if (this->moves[i] == moveMask)
			return true;
	}
	return false;
}
