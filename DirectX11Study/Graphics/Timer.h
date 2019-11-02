#pragma once
#include <chrono>

class Timer {
private:
	bool mIsRunning = false;
#ifdef _WIN32
	std::chrono::time_point<std::chrono::steady_clock> mStart;
	std::chrono::time_point<std::chrono::steady_clock> mStop;
#else
	std::chrono::time_point<std::chrono::system_clock> mStart;
	std::chrono::time_point<std::chrono::system_clock> mStop;
#endif

public:
	Timer();
	double GetMilesecondsElapsed();
	void Restart();
	bool Stop();
	bool Start();
};