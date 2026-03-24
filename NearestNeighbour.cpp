#include "NearestNeighbour.h"
#include "Timer.h"
#include <climits>
#include <vector>

std::string NearestNeighbour::getName() const {
    return "Nearest Neighbour (zachlanny)";
}

void NearestNeighbour::solve(const Graph& graph) {
    reset();
    if (graph.getSize() <= 0) return;

    Timer timer;
    timer.start();

    runFromVertex(graph, 0, bestPath, bestCost);

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}

void NearestNeighbour::runFromVertex(const Graph& graph, int startVertex,
                                      std::vector<int>& outPath, int& outCost) const {
    const int n = graph.getSize();

    std::vector<bool> visited(n, false);
    std::vector<int> path;
    path.reserve(n + 1);

    int currentVertex = startVertex;
    visited[currentVertex] = true;
    path.push_back(currentVertex);

    // Wykonujemy n-1 krokow, za kazdym razem wybierajac najblizszego sasiada
    for (int step = 0; step < n - 1; ++step) {
        int nearestVertex = -1;
        int minCost = INT_MAX;

        // Przeszukaj wszystkich nieodwiedzonych sasiadow
        for (int candidate = 0; candidate < n; ++candidate) {
            if (visited[candidate]) continue;

            int edgeCost = graph.getEdge(currentVertex, candidate);

            // Pomijamy krawedzie nielegalne (-1)
            if (edgeCost == -1) continue;

            // Wybieramy kandydata o nizszym koszcie.
            // W przypadku remisu – zachowujemy wierzcholek o nizszym indeksie
            // (deterministyczny wybor, nizszy indeks zachodzi przez warunek '<').
            if (edgeCost < minCost) {
                minCost = edgeCost;
                nearestVertex = candidate;
            }
        }

        // Jesli nie znaleziono sasiada – graf nie jest spojny, przerywamy
        if (nearestVertex == -1) {
            outPath.clear();
            outCost = INT_MAX;
            return;
        }

        visited[nearestVertex] = true;
        path.push_back(nearestVertex);
        currentVertex = nearestVertex;
    }

    // Powrot do wierzcholka startowego – zamkniecie cyklu
    path.push_back(startVertex);

    // Oblicz calkowity koszt sciezki
    int totalCost = 0;
    for (int i = 0; i < n; ++i) {
        int edge = graph.getEdge(path[i], path[i + 1]);
        if (edge == -1) {
            outPath.clear();
            outCost = INT_MAX;
            return;
        }
        totalCost += edge;
    }

    outPath = path;
    outCost = totalCost;
}