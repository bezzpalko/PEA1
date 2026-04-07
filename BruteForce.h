#pragma once
#include "TSPAlgorithm.h"

// Algorytm Brute-Force (przeglad zupelny).

class BruteForce : public TSPAlgorithm {
public:
    // domyślny konstruktor klasy
    BruteForce() = default;

    // główna metoda
    void solve(const Graph& graph) override;

    // zwraca czytelną nazwę algorytmu
    std::string getName() const override;
};