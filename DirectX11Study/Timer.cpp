#include "Timer.h"

Timer::Timer() 
	:
	mStart(std::chrono::high_resolution_clock::now()),
	mStop(std::chrono::high_resolution_clock::now())
{
}

double Timer::GetMilesecondsElapsed() {
	if (mIsRunning) {
		auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - mStart);
		return elapsed.count();
	}
	else {
		auto elapsed = std::chrono::duration<double, std::milli>(mStop - mStart);
		return elapsed.count();
	}
}

void Timer::Restart() {
	mIsRunning = true;
	mStart = std::chrono::high_resolution_clock::now();
}

bool Timer::Stop() {
	if (!mIsRunning) {
		return false;
	}
	else {
		mIsRunning = false;
		mStop = std::chrono::high_resolution_clock::now();
		return true;
	}
}

bool Timer::Start() {
	if (mIsRunning) {
		return false;
	}
	else {
		mIsRunning = true;
		mStart = std::chrono::high_resolution_clock::now();
		return true;
	}
}
