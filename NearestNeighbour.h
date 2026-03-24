#pragma once
#include "TSPAlgorithm.h"

// Algorytm zachlanny Nearest Neighbour (NN) dla ATSP.
// Zlozonosc: O(n^2).
//
// Zasada dzialania:
//   1. Startuj od wierzcholka 0.
//   2. W kazdym kroku przejdz do najblizszego nieodwiedzonego sasiada.
//   3. Po odwiedzeniu wszystkich wierzcholkow wróc do startu.
//
// W przypadku remisu (kilka krawedzi o tym samym najmniejszym koszcie)
// wybierany jest wierzcholek o najnizszym indeksie (deterministycznie).
class NearestNeighbour : public TSPAlgorithm {
public:
    NearestNeighbour() = default;

    // Uruchamia algorytm NN startujac zawsze z wierzcholka 0.
    void solve(const Graph& graph) override;

    std::string getName() const override;

protected:
    // Pomocnicza metoda uruchamiajaca NN z podanego wierzcholka startowego.
    // Zwraca znaleziona sciezke i jej koszt przez parametry referencyjne.
    // Uzywana rowniez przez klase RepetitiveNN.
    void runFromVertex(const Graph& graph, int startVertex,
                       std::vector<int>& outPath, int& outCost) const;
};