#pragma once
#ifndef _COMPUTING_UNIT_INCLUDED_
#define _COMPUTING_UNIT_INCLUDED_

#include <thread>
#include <string>
#include <ctime>
#include <cmath>

#include "types.h"
#include "Consts.h"
#include "sync_cout.h"
#include "Timer.h"
#include "Parser.h"
#include "Position.h"
#include "Evaluation.h"
#include "Moves.h"
#include "PawnsMoves.h"
#include "Bitscan.h"
#include "PrincipalVariation.h"
#include "Coods.h"
#include "Table.h"

class Thread
{
public:
	bool isActive();
	bool setPosition(Position position);
	void go(Depth depth);
	void stop();

private:
	int AlphaBeta(Depth depth, Depth ply, int alpha, int beta, LinePV* pv);
	int Quies(int alpha, int beta, LinePV* pv);
	String SearchValue(int value);

	Position position;
	Flag threadStopFlag = true;
	Depth searchDepth;
	unsigned int posProcCount;
	//HashTable hashTable;

	NativeThreadHandle nativeHandle;
};

#endif