#pragma once
#include <chrono>

// Klasa do precyzyjnego pomiaru czasu  oparta na std::chrono::high_resolution_clock
class Timer {
public:
    // rozpoczyna pomiar czasu
    void start();

    // zatrzymuje pomiar czasu
    void stop();

    // zwraca zmierzony czas w nanosekundach
    long long getElapsedNanoseconds() const;

    // zwraca zmierzony czas w mikrosekundach
    long long getElapsedMicroseconds() const;

    // zwraca zmierzony czas w milisekundach
    long long getElapsedMilliseconds() const;

    // zwraca zmierzony czas w sekundach
    double getElapsedSeconds() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
};