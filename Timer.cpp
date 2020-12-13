#include "Timer.h"

Timer::Timer(bool startNowFlag)
{
	if (startNowFlag)
		startNow();
}

void Timer::startNow()
{
	this->startPoint = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
	this->endPoint = std::chrono::high_resolution_clock::now();
}

TimeValue Timer::elapsed(bool restartFlag)
{
	stop();
	TimeValue elapsedMilliseconds = static_cast<TimeValue>((endPoint - startPoint).count() / 1000000);
	if (restartFlag)
		startNow();
	return elapsedMilliseconds != 0 ? elapsedMilliseconds : 1;
}
