#pragma once
#include "TSPAlgorithm.h"

// Algorytm losowy dla ATSP.
// Zlozonosc: O(10N * n) gdzie N to rozmiar grafu.
//
// Zasada dzialania:
//   Wykonuje dokladnie 10*N losowan permutacji wierzcholkow.
//   Dla kazdej oblicza koszt i zapamietuje najlepsza.
//
// Generowanie permutacji:
//   Uzywa std::mt19937 (Mersenne Twister) z seed z std::random_device
//   oraz std::shuffle – gwarantuje rowomierne losowanie bez systematycznych
//   powtorzen, przy zachowaniu losowego charakteru (nie jest to
//   std::next_permutation, ktore generuje permutacje w sposob deterministyczny).
class RandomSearch : public TSPAlgorithm {
public:
    // Konstruktor – mozliwosc podania zewnetrznego seed (domyslnie losowy)
    explicit RandomSearch(unsigned int seed = 0);

    // Uruchamia algorytm losowy z liczba permutacji = 10 * N
    void solve(const Graph& graph) override;

    // Uruchamia algorytm z niestandardowa liczba permutacji (dla menu)
    void solveWithCustomIterations(const Graph& graph, int iterations);

    std::string getName() const override;

    // Zwraca liczbe permutacji uzywan w ostatnim uruchomieniu
    int getLastIterationCount() const;

private:
    unsigned int seed;          // Seed generatora (0 = losowy z random_device)
    int lastIterationCount;     // Liczba permutacji z ostatniego uruchomienia

    // Wspolna logika dla solve() i solveWithCustomIterations()
    void runSearch(const Graph& graph, int iterations);
};