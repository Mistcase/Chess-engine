#include "Table.h"

void chessTables::InitZobristKeys()
{
	srand(static_cast<unsigned int>(time(NULL)));

	for (size_t figureType = INDEX_PAWN; figureType < FIGURES_TYPES_COUNT; figureType++)
		for (int color = 0; color < PLAYERS_COUNT; color++)
			for (size_t sq = 0; sq < BOARD_SIZE; sq++)
				ZobristKeys[figureType][color][sq] = rand() ^ ((Key)rand() << 15) ^ ((Key)rand() << 31)
				^ ((Key)rand() << 47) ^ ((Key)rand() << 59);
}

HashTable::HashTable(size_t sizeofTable)
{
	this->clear();
	this->itemsCapacity = sizeofTable / sizeof(HashItem);
	if (itemsCapacity != 0)
		this->items = new HashItem[itemsCapacity];
}

size_t HashTable::getSize() const
{
	return this->itemsCapacity;
}

size_t HashTable::filled() const
{
	return this->filled_;
}

void HashTable::recordHash(Depth depth, int score, int flag, Key key, ChessMove* bestMove, LinePV* pv, Depth ply)
{
	//Mutex!
	HashItem* ptrItem = &this->items[key & (this->itemsCapacity - 1)];

	//std::cout << (key & (this->itemsCapacity - 1)) << std::endl;
	//Корректировка мата
	//
	//
	//
	//Корректировка мата

	ptrItem->depth = depth;
	ptrItem->score = score;
	ptrItem->flags = flag;
	ptrItem->key = key;
	ptrItem->bestMove = *bestMove;
	ptrItem->pv = *pv;
	this->filled_++;
}

HashItem * HashTable::getItem(Key key) const
{
	return &this->items[key & (this->itemsCapacity - 1)];
}

void HashTable::clear()
{
	if (this->items != nullptr)
	{
		delete[] this->items;
		this->items = nullptr;
	}
}


Key chessTables::ZobristKeys[FIGURES_TYPES_COUNT][PLAYERS_COUNT][BOARD_SIZE];
Key chessTables::ZobristKeyMove = 0x54ca3eb5b5f3cb5bULL;
Key chessTables::ZobristKeyNullMove = 0x08d9bc25bebf91b1ULL;