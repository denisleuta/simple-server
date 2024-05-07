#pragma once
#include <chrono>
class Timer {
public:
	Timer();
	double GetElapsedTime();
	void Reset();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};
