#include "Coods.h"

std::string GetBoardPosition(size_t square)
{
	unsigned int x, y;
	y = square / 8;
	x = square % 8;
	return std::string() + chess_board::coordsX[x] + chess_board::coordsY[y];
}

uint64_t GetBoardPosition(std::string position)
{
	unsigned int x, y, index;

	for (int i = 0; i < BOARD_SIZE_ANY_SIDE; i++)
	{
		if (position[0] == chess_board::coordsX[i])
			x = i + 1;
		if (position[1] == chess_board::coordsY[i])
			y = i + 1;
	}

	index = (y - 1) * 8 + (x - 1);

	return (1ULL << index);
}
