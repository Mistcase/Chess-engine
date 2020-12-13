#include "ChessMove.h"

void MovesList::push(ChessMove& chessMove)
{
	if (counterFilled == MAX_BRANCHING_RATIO - 1)
		printf("Array overflow\n");
	this->variants[counterFilled++] = chessMove;
}

ChessMove * MovesList::nextMove()
{
	return this->counterReturned < this->counterFilled ? &this->variants[counterReturned++] : nullptr;
}

ChessMove * MovesList::getVariant(size_t index)
{
	return &this->variants[index];
}

size_t MovesList::getCounterFilled() const
{
	return this->counterFilled;
}

size_t MovesList::getCounterReturned() const
{
	return this->counterReturned;
}

void MovesList::cutTo(size_t bestBranchesCount)
{
	this->counterFilled = bestBranchesCount > this->counterFilled ? this->counterFilled : bestBranchesCount;
}

Flag MovesList::find(Bitboard moveMask)
{
	for (int i = 0; i < this->counterFilled; i++)
		if (this->variants[i].moveMask == moveMask)
			return true;
	return false;
}
