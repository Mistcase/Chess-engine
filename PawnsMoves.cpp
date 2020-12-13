#include "PawnsMoves.h"

void pawns_moves::initPawnsMovesMasks()
{
	for (size_t sq = 8; sq < 56; sq++)
	{
		pawns_moves::moves[WHITE][sq][0] = (1ULL << (sq + 8));
		pawns_moves::moves[WHITE][sq][1] = ((1ULL << sq) & RANK_2) ? (1ULL << (sq + 16)) : 0;

		pawns_moves::moves[BLACK][sq][0] = (1ULL << (sq - 8));
		pawns_moves::moves[BLACK][sq][1] = ((1ULL << sq) & RANK_7) ? (1ULL << (sq - 16)) : 0;
	}
	bfAttackPositions[16] = 0x2000000;
	bfAttackPositions[17] = 0x5000000;
	bfAttackPositions[18] = 0xA000000;
	bfAttackPositions[19] = 0x14000000;
	bfAttackPositions[20] = 0x28000000;
	bfAttackPositions[21] = 0x50000000;
	bfAttackPositions[22] = 0xA0000000;
	bfAttackPositions[23] = 0x40000000;
	bfAttackPositions[47] = 0x4000000000;
	bfAttackPositions[46] = 0xA000000000;
	bfAttackPositions[45] = 0x5000000000;
	bfAttackPositions[44] = 0x2800000000;
	bfAttackPositions[43] = 0x1400000000;
	bfAttackPositions[42] = 0xA00000000;
	bfAttackPositions[41] = 0x500000000;
	bfAttackPositions[40] = 0x200000000;

	for (int i = 16; i <= 23; i++)
		bfDisabledPawnsMasks[i] = (1ULL << (i + 8));
	for (int i = 40; i <= 47; i++)
		bfDisabledPawnsMasks[i] = (1ULL << (i - 8));
}

uint64_t pawns_moves::moves[PLAYERS_COUNT][BOARD_SIZE][2];
uint64_t pawns_moves::bfAttackPositions[64];
uint64_t pawns_moves::bfDisabledPawnsMasks[64];