#include "NearestNeighbour.h"
#include "Timer.h"
#include <climits>
#include <vector>

using namespace std;

string NearestNeighbour::getName() const {
    // zwraca czytelną nazwę algorytmu
    return "Nearest Neighbour (zachlanny)";
}

void NearestNeighbour::solve(const Graph& graph) {
    // wyczyszczenie poprzednich wyników
    reset();
    if (graph.getSize() <= 0) return;

    Timer timer;
    timer.start();

    // poszukiwania zawsze rozpoczynamy od pierwszego wierzchołka (indeks 0)
    runFromVertex(graph, 0, bestPath, bestCost);

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}

void NearestNeighbour::runFromVertex(const Graph& graph, int startVertex,
                                      vector<int>& outPath, int& outCost) const {
    const int n = graph.getSize();

    // przygotowanie struktur śledzących odwiedzone miasta oraz budowaną trasę
    vector<bool> visited(n, false);
    vector<int> path;

    int currentVertex = startVertex;
    visited[currentVertex] = true;
    path.push_back(currentVertex);

    // w każdym z n-1 kroków algorytm podejmuje lokalnie optymalną (zachłanną) decyzję o przejściu do najbliższego miasta
    for (int step = 0; step < n - 1; ++step) {
        int nearestVertex = -1;
        int minCost = INT_MAX;

        // przegląd wszystkich potencjalnych wierzchołków w poszukiwaniu najtańszego przejścia
        for (int candidate = 0; candidate < n; ++candidate) {
            if (visited[candidate]) continue;

            int edgeCost = graph.getEdge(currentVertex, candidate);

            // pominięcie krawędzi o wadze -1
            if (edgeCost == -1) continue;

            // wybór najlepszego kandydata
            if (edgeCost < minCost) {
                minCost = edgeCost;
                nearestVertex = candidate;
            }
        }

        // zabezpieczenie na wypadek grafu niespójnego
        if (nearestVertex == -1) {
            outPath.clear();
            outCost = INT_MAX;
            return;
        }

        visited[nearestVertex] = true;
        path.push_back(nearestVertex);
        currentVertex = nearestVertex;
    }

    // domknięcie cyklu poprzez dopisanie wierzchołka startowego na sam koniec wektora trasy
    path.push_back(startVertex);

    // podsumowanie kosztów wyznaczonej ścieżki
    int totalCost = 0;
    for (int i = 0; i < n; ++i) {
        int edge = graph.getEdge(path[i], path[i + 1]);
        if (edge == -1) {
            // czy krawędź powrotna z ostatniego miasta do pierwszego na pewno istnieje
            outPath.clear();
            outCost = INT_MAX;
            return;
        }
        totalCost += edge;
    }

    outPath = path;
    outCost = totalCost;
}