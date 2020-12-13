#pragma once
#ifndef _ENGINE_INCLUDED_
#define _ENGINE_INCLUDED_

#define COMPUTING_UNITS_LIMIT 32

#define ENGINE_OPTION_THREADS 1
#define ENGINE_OPTION_MULTIPV 2
#define ENGINE_OPTION_MINIMUM_THINKING_TIME 3
#define ENGINE_OPTION_SKILL_LEVEL 4
#define ENGINE_OPTION_DEBUG_FILE_NAME 5
#define ENGINE_OPTION_PONDER 6

#include <cstdint>
#include <string>
#include <iostream>
#include <thread>

#include "Thread.h"
#include "ParsingFunctional.h"
#include "Coods.h"
#include "ChessMove.h"
#include "Bitscan.h"
#include "Moves.h"

class Engine
{
public:
	void startCalculations(int depth);
	void stopCalculations();

	bool setOption(int optionType, void* value);

	void setPosition(Position pos);
	bool setPosition(std::string fenString);
	bool makeMove(std::string moveStringNotation);

	Position* getPosition();
private:
	Thread units[COMPUTING_UNITS_LIMIT];

	Position position;

	unsigned int threadsCounter		 = 1,
				 multiPv			 = 1,
				 skillLevel			 = 10,
				 minimumThinkingTime = 20;
	std::string debugFileName;
	bool ponder = false;

	std::thread monitoringThread;
};

#endif