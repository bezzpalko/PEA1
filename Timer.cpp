#include "Timer.h"
using namespace std;

void Timer::start() {
    // rozpoczyna pomiar czasu, zapisując aktualny stan zegara o najwyższej dostępnej rozdzielczości
    startTime = chrono::high_resolution_clock::now();
}

void Timer::stop() {
    // zatrzymuje pomiar, zapisując moment końcowy
    endTime = chrono::high_resolution_clock::now();
}

long long Timer::getElapsedNanoseconds() const {
    // zwraca różnicę między czasem końcowym a początkowym, wyrażoną w nanosekundach
    return chrono::duration_cast<chrono::nanoseconds>(endTime - startTime).count();
}

long long Timer::getElapsedMicroseconds() const {
    // zwraca różnicę między czasem końcowym a początkowym, przeliczoną na mikrosekundy
    return chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
}

long long Timer::getElapsedMilliseconds() const {
    // zwraca zmierzony upływ czasu w milisekundach
    return chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
}

double Timer::getElapsedSeconds() const {
    // zwraca zmierzony czas w sekundach w postaci liczby zmiennoprzecinkowej
    return chrono::duration<double>(endTime - startTime).count();
}