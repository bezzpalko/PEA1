#pragma once
#include "TSPAlgorithm.h"

// Algorytm Brute-Force (przeglad zupelny) dla ATSP.
// Zlozonosc: O(n!) – sprawdzane sa WSZYSTKIE permutacje wierzcholkow.
//
// Uwaga: Zgodnie z wymaganiami projektu, algorytm NIE stosuje
// zadnego przycinania galezi (branch and bound). Kazda permutacja
// jest w pelni obliczana, nawet jesli jej czatkowy koszt przekracza
// aktualnie najlepszy wynik.
class BruteForce : public TSPAlgorithm {
public:
    BruteForce() = default;

    // Przeszukuje wszystkie (n-1)! permutacji wierzcholkow (startujac od 0).
    // Uzywa std::next_permutation do generowania kolejnych permutacji.
    void solve(const Graph& graph) override;

    std::string getName() const override;
};