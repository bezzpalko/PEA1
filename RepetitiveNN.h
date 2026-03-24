#pragma once
#include "NearestNeighbour.h"
#include <stack>

// Algorytm Repetitive Nearest Neighbour (RNN) dla ATSP.
// Zlozonosc: O(n^3) w przypadku bez remisow, wyzej przy remisach.
//
// Zasada dzialania:
//   1. Uruchamia NN z kazdego mozliwego wierzcholka startowego (0..n-1).
//   2. Zapamietuje globalnie najlepsza znaleziona sciezke.
//
// Obsluga remisow (kluczowe wymaganie projektu):
//   Jesli w danym kroku istnieja 2 lub wiecej krawedzi o tym samym,
//   minimalnym koszcie, algorytm rozgalezia poszukiwania i sprawdza
//   KAZDA z nich. Realizowane jest to iteracyjnie za pomoca stosu
//   (symulacja rekurencji), co eliminuje ryzyko przepelnienia stosu
//   dla duzych instancji.
class RepetitiveNN : public NearestNeighbour {
public:
    RepetitiveNN() = default;

    // Uruchamia RNN z kazdego wierzcholka startowego.
    // Dla kazdego startu stosuje rozgalezianie przy remisach.
    void solve(const Graph& graph) override;

    std::string getName() const override;

private:
    // Stan wezla na stosie poszukiwan (do obslugi rozgalezien przy remisach)
    struct SearchState {
        std::vector<int> path;       // Aktualna czesc sciezki
        std::vector<bool> visited;   // Ktore wierzcholki zostaly odwiedzone
        int currentVertex;           // Aktualny wierzcholek
        int currentCost;             // Aktualny koszt sciezki do tej pory
        int startVertex;             // Wierzcholek startowy tej sciezki
    };

    // Uruchamia poszukiwanie NN z rozgalezianiem remisow startujac
    // od podanego wierzcholka. Zwraca najlepsza znaleziona sciezke
    // i jej koszt przez parametry referencyjne.
    void runWithTieBreaking(const Graph& graph, int startVertex,
                            std::vector<int>& outBestPath, int& outBestCost) const;
};