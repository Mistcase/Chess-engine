#pragma comment(linker, "/STACK:1677721600")

#include <iostream>
#include <bitset>
#include <ctime>

#include "Magic.h"
#include "Engine.h"
#include "uci.h"

#include "Table.h"

using namespace std;

int main()
{
	//Initialization
	Rays::init();
	sliding_funcs::initBishopMasks();
	sliding_funcs::initRookMasks();
	sliding_funcs::initMagicBishops();
	sliding_funcs::initMagicRooks();
	_const_attacks_funcs_::initConstAttacks();
	_evaluation_::initEvaluationConsts();
	pawns_moves::initPawnsMovesMasks();
	Castling::initCastlingsPointers();
	chessTables::InitZobristKeys();

	Engine engine;
	UciEngine uciEngine(&engine);
	uciEngine.start();

    //Main thread is useless
	while (true)
	{
		this_thread::sleep_for(chrono::seconds(1));
	}

	return 0;
}
