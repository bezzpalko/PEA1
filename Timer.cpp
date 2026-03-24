#include "Timer.h"

void Timer::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

void Timer::stop() {
    endTime = std::chrono::high_resolution_clock::now();
}

long long Timer::getElapsedNanoseconds() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
}

long long Timer::getElapsedMicroseconds() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
}

long long Timer::getElapsedMilliseconds() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
}

double Timer::getElapsedSeconds() const {
    return std::chrono::duration<double>(endTime - startTime).count();
}