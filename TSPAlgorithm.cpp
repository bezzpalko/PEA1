#include "TSPAlgorithm.h"
#include "Timer.h"
#include <iostream>
#include <iomanip>
#include <climits>

using namespace std;

TSPAlgorithm::TSPAlgorithm()
    : bestCost(INT_MAX), executionTimeNs(0) {}

const vector<int>& TSPAlgorithm::getBestPath() const {
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
        cout << "Blad: algorytm nie zostal jeszcze uruchomiony." << endl;
        return;
    }

    cout << "Wyniki: " << getName() << endl;
    cout << "Trasa: ";
    for (int i = 0; i < bestPath.size(); ++i) {
        cout << bestPath[i];
        if (i < bestPath.size() - 1) cout << " -> ";
    }
    cout << endl;
    cout << "Koszt:  " << bestCost << endl;

    // Wyswietlenie czasu w najbardziej czytelnej jednostce
    if (executionTimeNs < 1000LL) {
        cout << "Czas:   " << executionTimeNs << " ns" << endl;
    } else if (executionTimeNs < 1000000LL) {
        cout << "Czas:   " << fixed << setprecision(3)
                  << executionTimeNs / 1000.0 << " us" << endl;
    } else if (executionTimeNs < 1000000000LL) {
        cout << "Czas:   " << fixed << setprecision(3)
                  << executionTimeNs / 1000000.0 << " ms" << endl;
    } else {
        cout << "Czas:   " << fixed << setprecision(4)
                  << executionTimeNs / 1000000000.0 << " s" << endl;
    }
}

void TSPAlgorithm::reset() {
    bestPath.clear();
    bestCost = INT_MAX;
    executionTimeNs = 0;
}

int TSPAlgorithm::calculatePathCost(const vector<int>& path, const Graph& graph) const {
    int cost = 0;
    int n = (path.size());

    for (int i = 0; i < n - 1; ++i) {
        int edge = graph.getEdge(path[i], path[i + 1]);
        // Krawedz -1 oznacza brak polaczenia – sciezka jest nielegalna
        if (edge == -1) return INT_MAX;
        cost += edge;
    }
    return cost;
}