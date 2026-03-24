#pragma once
#include <chrono>

// Klasa do precyzyjnego pomiaru czasu wykonania algorytmow
// Oparta na std::chrono::high_resolution_clock
class Timer {
public:
    // Rozpoczyna pomiar czasu
    void start();

    // Zatrzymuje pomiar czasu
    void stop();

    // Zwraca zmierzony czas w nanosekundach
    long long getElapsedNanoseconds() const;

    // Zwraca zmierzony czas w mikrosekundach
    long long getElapsedMicroseconds() const;

    // Zwraca zmierzony czas w milisekundach
    long long getElapsedMilliseconds() const;

    // Zwraca zmierzony czas w sekundach (jako double dla precyzji)
    double getElapsedSeconds() const;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
};