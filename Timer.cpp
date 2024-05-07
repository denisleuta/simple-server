#include "Timer.h"
using namespace std;

Timer::Timer() {
    startTime = chrono::high_resolution_clock::now();
}

double Timer::GetElapsedTime() {
    auto currentTime = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsedTime = currentTime - startTime;
    return elapsedTime.count();
}

void Timer::Reset() {
    startTime = chrono::high_resolution_clock::now();
}