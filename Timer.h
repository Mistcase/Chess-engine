#pragma once
#ifndef _TIMER_INCLUDED_
#define _TIMER_INCLUDED_

#include <chrono>

#include "types.h"

class Timer
{
public:
	Timer(bool startNow = true);

	void startNow();
	void stop();
	TimeValue elapsed(bool restart = true);

private:
	std::chrono::high_resolution_clock::time_point startPoint, endPoint;
};

#endif