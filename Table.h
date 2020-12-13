#pragma once
#ifndef _TABLE_H_INCLUDED_
#define _TABLE_H_INCLUDED_

#include <ctime>
#include <random>
#include <iostream>

#include "types.h"
#include "Consts.h"
#include "ChessMove.h"
#include "PrincipalVariation.h"

enum HashFlags
{
	FLAG_EMPTY,
	FLAG_EXACT,
};

struct HashItem
{
	Key key;
	int depth = -1;
	int flags = FLAG_EMPTY;
	int score = -INFINITY_VALUE;
	ChessMove bestMove;
	LinePV pv;
};
class HashTable
{
public:
	HashTable(size_t sizeofTableinBytes = 0);
	size_t getSize() const;
	size_t filled() const;

	void recordHash(Depth depth, int score, int flag, Key key, ChessMove* bestMove, LinePV* pv, Depth ply);
	HashItem* getItem(Key key) const;
	void clear();

private:
	HashItem* items = nullptr;
	size_t itemsCapacity = 0, filled_ = 0;
};

namespace chessTables
{
	void InitZobristKeys();

	extern Key ZobristKeys[FIGURES_TYPES_COUNT][PLAYERS_COUNT][BOARD_SIZE];
	extern Key ZobristKeyMove;//ключ смены хода
	extern Key ZobristKeyNullMove;//ключ нулевого хода
}
#endif