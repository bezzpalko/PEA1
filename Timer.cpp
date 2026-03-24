#include "Timer.h"
using namespace std;

void Timer::start() {
    startTime = chrono::high_resolution_clock::now();
}

void Timer::stop() {
    endTime = chrono::high_resolution_clock::now();
}

long long Timer::getElapsedNanoseconds() const {
    return chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();
}

long long Timer::getElapsedMicroseconds() const {
    return chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
}

long long Timer::getElapsedMilliseconds() const {
    return chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
}

double Timer::getElapsedSeconds() const {
    return chrono::duration<double>(endTime - startTime).count();
}