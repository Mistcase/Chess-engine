#include "Magic.h"

uint64_t sliding_funcs::GetMaxValue(unsigned int bitsCount)
{
	uint64_t result = 0;
	for (size_t i = 0; i < bitsCount; i++)
		result |= 1ULL << i;
	return result;
}

uint64_t sliding_funcs::findBishopsAttacks(uint64_t & allFiguresBothMask, size_t sq)
{
	return GetRay(sq, Rays::NorthWest) & ~GetRay(bitScanForward(GetRay(sq, Rays::NorthWest) & allFiguresBothMask), Rays::NorthWest) |
		GetRay(sq, Rays::NorthEast) & ~GetRay(bitScanForward(GetRay(sq, Rays::NorthEast) & allFiguresBothMask), Rays::NorthEast) |
		GetRay(sq, Rays::SouthWest) & ~GetRay(bitScanReverse(GetRay(sq, Rays::SouthWest) & allFiguresBothMask), Rays::SouthWest) |
		GetRay(sq, Rays::SouthEast) & ~GetRay(bitScanReverse(GetRay(sq, Rays::SouthEast) & allFiguresBothMask), Rays::SouthEast);
}

uint64_t sliding_funcs::findRookAttacks(uint64_t & allFiguresBothMask, size_t sq)
{
	return GetRay(sq, Rays::North) & ~GetRay(bitScanForward(GetRay(sq, Rays::North) & allFiguresBothMask), Rays::North) |
		GetRay(sq, Rays::East) & ~GetRay(bitScanForward(GetRay(sq, Rays::East) & allFiguresBothMask), Rays::East) |
		GetRay(sq, Rays::South) & ~GetRay(bitScanReverse(GetRay(sq, Rays::South) & allFiguresBothMask), Rays::South) |
		GetRay(sq, Rays::West) & ~GetRay(bitScanReverse(GetRay(sq, Rays::West) & allFiguresBothMask), Rays::West);
}

void sliding_funcs::initBishopMasks()
{
	uint64_t disablerMask = FILE_A | FILE_H | RANK_1 | RANK_8;
	for (size_t sq = 0; sq < 64; sq++)
	{
		bishopMasks[sq] = 0;
		bishopMasks[sq] |= (GetRay(sq, Rays::NorthWest) & disablerMask) ^ GetRay(sq, Rays::NorthWest);
		bishopMasks[sq] |= (GetRay(sq, Rays::NorthEast) & disablerMask) ^ GetRay(sq, Rays::NorthEast);
		bishopMasks[sq] |= (GetRay(sq, Rays::SouthWest) & disablerMask) ^ GetRay(sq, Rays::SouthWest);
		bishopMasks[sq] |= (GetRay(sq, Rays::SouthEast) & disablerMask) ^ GetRay(sq, Rays::SouthEast);
	}
}

void sliding_funcs::initRookMasks()
{
	for (size_t sq = 0; sq < 64; sq++)
	{
		rookMasks[sq] = 0;
		rookMasks[sq] |= (GetRay(sq, Rays::North) & RANK_8) ^ GetRay(sq, Rays::North);
		rookMasks[sq] |= (GetRay(sq, Rays::South) & RANK_1) ^ GetRay(sq, Rays::South);
		rookMasks[sq] |= (GetRay(sq, Rays::West) & FILE_A) ^ GetRay(sq, Rays::West);
		rookMasks[sq] |= (GetRay(sq, Rays::East) & FILE_H) ^ GetRay(sq, Rays::East);
	}
}

void sliding_funcs::initMagicBishops()
{
	//Zero memory
	for (size_t sq = 0; sq < BOARD_SIZE; sq++)
	{
		for (size_t i = 0; i < BISHOP_ANY_SQ_MAX_MAGIC_ATTACKS; i++)
			bishopTable[sq][i] = 0;
	}

	for (size_t sq = 0; sq < 64; sq++)
	{
		//_bishopIndexBits[sq] --------> usable count of bits (fields)

		uint64_t maxValue = GetMaxValue(_bishopIndexBits[sq]);
		int arrayOfRedirection[BISHOP_MAGIC_INDEXES_MAX];
		size_t counter = 0;

		for (size_t i = 0; i < 64; i++)
		{
			if (bishopMasks[sq] & (1ULL << i))
				arrayOfRedirection[counter++] = i;
		}

		for (uint64_t val = 0; val < maxValue; val++)
		{
			uint64_t currentOccBitMask = 0;
			for (size_t i = 0; i < BISHOP_MAGIC_INDEXES_MAX; i++)
			{
				if (val & (1ULL << i))
					currentOccBitMask |= (1ULL << arrayOfRedirection[i]);
			}

			size_t tableIndex = (currentOccBitMask * _bishopMagics[sq]) >> 54;
			bishopTable[sq][tableIndex] = findBishopsAttacks(currentOccBitMask, sq);
		}
	}
}

void sliding_funcs::initMagicRooks()
{
	//Zero memory
	for (size_t sq = 0; sq < BOARD_SIZE; sq++)
	{
		for (size_t i = 0; i < ROOK_ANY_SQ_MAX_MAGIC_ATTACKS; i++)
			rookTable[sq][i] = 0;
	}

	for (size_t sq = 0; sq < 64; sq++)
	{
		//_rookIndexBits[sq] --------> usable count of bits (fields)

		uint64_t maxValue = GetMaxValue(_rookIndexBits[sq]);
		int arrayOfRedirection[ROOK_MAGIC_INDEXES_MAX];
		size_t counter = 0;

		for (size_t i = 0; i < 64; i++)
		{
			if (rookMasks[sq] & (1ULL << i))
				arrayOfRedirection[counter++] = i;
		}

		for (uint64_t val = 0; val < maxValue; val++)
		{
			uint64_t currentOccBitMask = 0;
			for (size_t i = 0; i < ROOK_MAGIC_INDEXES_MAX; i++)
			{
				if (val & (1ULL << i))
					currentOccBitMask |= (1ULL << arrayOfRedirection[i]);
			}

			size_t tableIndex = (currentOccBitMask * _rookMagics[sq]) >> 52;
			rookTable[sq][tableIndex] = findRookAttacks(currentOccBitMask, sq);
		}
	}
}

uint64_t sliding_funcs::bishopMasks[BOARD_SIZE];
uint64_t sliding_funcs::bishopTable[BOARD_SIZE][BISHOP_ANY_SQ_MAX_MAGIC_ATTACKS];
uint64_t sliding_funcs::rookMasks[BOARD_SIZE];
uint64_t sliding_funcs::rookTable[BOARD_SIZE][ROOK_ANY_SQ_MAX_MAGIC_ATTACKS];