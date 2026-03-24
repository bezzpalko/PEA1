#include "TSPAlgorithm.h"
#include "Timer.h"
#include <iostream>
#include <iomanip>
#include <climits>

TSPAlgorithm::TSPAlgorithm()
    : bestCost(INT_MAX), executionTimeNs(0) {}

const std::vector<int>& TSPAlgorithm::getBestPath() const {
    return bestPath;
}

int TSPAlgorithm::getBestCost() const {
    return bestCost;
}

long long TSPAlgorithm::getLastExecutionTimeNs() const {
    return executionTimeNs;
}

void TSPAlgorithm::printResults() const {
    if (bestPath.empty()) {
        std::cout << "Blad: algorytm nie zostal jeszcze uruchomiony." << std::endl;
        return;
    }

    std::cout << "Wyniki: " << getName() << std::endl;
    std::cout << "Trasa: ";
    for (int i = 0; i < static_cast<int>(bestPath.size()); ++i) {
        std::cout << bestPath[i];
        if (i < static_cast<int>(bestPath.size()) - 1) std::cout << " -> ";
    }
    std::cout << std::endl;
    std::cout << "Koszt:  " << bestCost << std::endl;

    // Wyswietlenie czasu w najbardziej czytelnej jednostce
    if (executionTimeNs < 1000LL) {
        std::cout << "Czas:   " << executionTimeNs << " ns" << std::endl;
    } else if (executionTimeNs < 1000000LL) {
        std::cout << "Czas:   " << std::fixed << std::setprecision(3)
                  << executionTimeNs / 1000.0 << " us" << std::endl;
    } else if (executionTimeNs < 1000000000LL) {
        std::cout << "Czas:   " << std::fixed << std::setprecision(3)
                  << executionTimeNs / 1000000.0 << " ms" << std::endl;
    } else {
        std::cout << "Czas:   " << std::fixed << std::setprecision(4)
                  << executionTimeNs / 1000000000.0 << " s" << std::endl;
    }
}

void TSPAlgorithm::reset() {
    bestPath.clear();
    bestCost = INT_MAX;
    executionTimeNs = 0;
}

int TSPAlgorithm::calculatePathCost(const std::vector<int>& path, const Graph& graph) const {
    int cost = 0;
    int n = static_cast<int>(path.size());

    for (int i = 0; i < n - 1; ++i) {
        int edge = graph.getEdge(path[i], path[i + 1]);
        // Krawedz -1 oznacza brak polaczenia – sciezka jest nielegalna
        if (edge == -1) return INT_MAX;
        cost += edge;
    }
    return cost;
}