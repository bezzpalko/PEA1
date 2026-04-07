#pragma once
#include "NearestNeighbour.h"
#include <stack>

// algorytm Repetitive Nearest Neighbour (RNN)

class RepetitiveNN : public NearestNeighbour {
public:
    RepetitiveNN() = default;

    // uruchamienie RNN z kazdego wierzcholka startowego
    void solve(const Graph& graph) override;

    std::string getName() const override;

private:
    // stan wezla na stosie poszukiwan
    struct SearchState {
        std::vector<int> path;       // aktualna czesc sciezki
        std::vector<bool> visited;   // ktore wierzcholki zostaly odwiedzone
        int currentVertex;           // aktualny wierzcholek
        int currentCost;             // aktualny koszt sciezki do tej pory
        int startVertex;             // wierzcholek startowy tej sciezki
    };

    // Uruchomienie poszukiwanie NN z rozgalezianiem remisow startujac od podanego wierzcholka, zwraca najlepsza znaleziona sciezke
    // i jej koszt przez parametry referencyjne.
    void runWithTieBreaking(const Graph& graph, int startVertex, std::vector<int>& outBestPath, int& outBestCost) const;
};