#pragma once
#include "TSPAlgorithm.h"

// zachłanny algorytm najbliższego sąsiada (nearest neighbour)

class NearestNeighbour : public TSPAlgorithm {
public:
    // domyślny konstruktor klasy
    NearestNeighbour() = default;

    // główna metoda
    void solve(const Graph& graph) override;

    // udostępnia tekstową nazwę algorytmu
    std::string getName() const override;

protected:
    // kluczowa metoda pomocnicza implementująca rdzeń logiki poszukiwania ścieżki od zadanego miasta startowego
    void runFromVertex(const Graph& graph, int startVertex, std::vector<int>& outPath, int& outCost) const;
};