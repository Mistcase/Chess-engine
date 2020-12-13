#include "ConstAttacks.h"

void _const_attacks_funcs_::initConstAttacks()
{
	for (unsigned int x = 1; x <= 8; x++)
	{
		for (unsigned int y = 1; y <= 8; y++)
		{
			unsigned int currentIndex = getLocationIndex(x, y);
			pawnsAttacks[WHITE][currentIndex] = getPawnAttacks(x, y, WHITE);
			pawnsAttacks[BLACK][currentIndex] = getPawnAttacks(x, y, BLACK);

			knightsAttacks[currentIndex] = getKnightAttacks(x, y);
			kingsAttacks[currentIndex] = getKingAttacks(x, y);
		}
	}
}

uint64_t _const_attacks_funcs_::getKingAttacks(unsigned int x, unsigned int y)
{
	int bitFields[8];
	uint64_t result;
	bitFields[0] = getLocationIndex(x + 1, y);
	bitFields[1] = getLocationIndex(x - 1, y);
	bitFields[2] = getLocationIndex(x, y + 1);
	bitFields[3] = getLocationIndex(x, y - 1);
	bitFields[4] = getLocationIndex(x + 1, y + 1);
	bitFields[5] = getLocationIndex(x - 1, y - 1);
	bitFields[6] = getLocationIndex(x + 1, y - 1);
	bitFields[7] = getLocationIndex(x - 1, y + 1);

	_const_attacks_funcs_::EnableLegalBits(result, bitFields, 8);
	return result;
}

uint64_t _const_attacks_funcs_::getKnightAttacks(unsigned int x, unsigned int y)
{
	int bitFields[8];
	uint64_t result;
	bitFields[0] = getLocationIndex(x - 2, y + 1);
	bitFields[1] = getLocationIndex(x - 1, y + 2);
	bitFields[2] = getLocationIndex(x + 1, y + 2);
	bitFields[3] = getLocationIndex(x + 2, y + 1);
	bitFields[4] = getLocationIndex(x + 2, y - 1);
	bitFields[5] = getLocationIndex(x + 1, y - 2);
	bitFields[6] = getLocationIndex(x - 1, y - 2);
	bitFields[7] = getLocationIndex(x - 2, y - 1);

	_const_attacks_funcs_::EnableLegalBits(result, bitFields, 8);
	return result;
}

uint64_t _const_attacks_funcs_::getPawnAttacks(unsigned int x, unsigned int y, int color)
{
	int bitFields[2];
	uint64_t result;

	if (color == WHITE)
	{
		bitFields[0] = getLocationIndex(x + 1, y + 1);
		bitFields[1] = getLocationIndex(x - 1, y + 1);
	}
	else if (color == BLACK)
	{
		bitFields[0] = getLocationIndex(x + 1, y - 1);
		bitFields[1] = getLocationIndex(x - 1, y - 1);
	}
	_const_attacks_funcs_::EnableLegalBits(result, bitFields, 2);
	return result;
}

uint64_t _const_attacks_funcs_::getTablePawnMove(unsigned int x, unsigned int y, int color)
{
	int bitFields[2];
	uint64_t result;

	if (color == WHITE)
	{
		bitFields[0] = getLocationIndex(x, y + 1);
		if (y == 2)
		{
			bitFields[1] = getLocationIndex(x, y + 2);
		}
	}
	else if (color == BLACK)
	{
		bitFields[0] = getLocationIndex(x, y - 1);
		if (y == 7)
		{
			bitFields[1] = getLocationIndex(x, y - 2);
		}
	}
	_const_attacks_funcs_::EnableLegalBits(result, bitFields, 2);
	return result;
}

void _const_attacks_funcs_::EnableLegalBits(uint64_t & result, int * results, unsigned int count)
{
	result = 0;
	for (unsigned int i = 0; i < count; i++)
	{
		if (results[i] != -1)
			result |= (1ULL << results[i]);
	}
}

uint64_t pawnsAttacks[2][64], knightsAttacks[64], kingsAttacks[64];