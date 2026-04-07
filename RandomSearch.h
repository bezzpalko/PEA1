#pragma once
#include "TSPAlgorithm.h"

// algorytm losowy dla ATSP.

class RandomSearch : public TSPAlgorithm {
public:
    // opcjonalne ustawienie ziarna (seed) dla generatora liczb pseudolosowych
    explicit RandomSearch(unsigned int seed = 0);

    // uruchamia algorytm losowy z liczba permutacji = 10 * N
    void solve(const Graph& graph) override;

    // uruchamia algorytm z niestandardowa liczba permutacji (dla menu)
    void solveWithCustomIterations(const Graph& graph, int iterations);

    std::string getName() const override;

    // zwraca liczbe permutacji uzywan w ostatnim uruchomieniu
    int getLastIterationCount() const;

private:
    unsigned int seed;          // seed generatora (0 = losowy z random_device)
    int lastIterationCount;     // liczba permutacji z ostatniego uruchomienia

    // wspolna logika dla solve() i solveWithCustomIterations()
    void runSearch(const Graph& graph, int iterations);
};