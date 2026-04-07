#include "TSPAlgorithm.h"
#include "Timer.h"
#include <iostream>
#include <iomanip>
#include <climits>

using namespace std;

TSPAlgorithm::TSPAlgorithm()
    // domyślny konstruktor inicjalizujący stan początkowy
    : bestCost(INT_MAX), executionTimeNs(0) {}

const vector<int>& TSPAlgorithm::getBestPath() const {
    // udostępnia stałą referencję do wektora przechowującego optymalną sekwencję wierzchołków
    return bestPath;
}

int TSPAlgorithm::getBestCost() const {
    // zwraca wyliczony koszt dla najlepszej znalezionej trasy
    return bestCost;
}

long long TSPAlgorithm::getLastExecutionTimeNs() const {
    // zwraca dokładny czas pracy ostatnio uruchomionego algorytmu wyrażony w nanosekundach
    return executionTimeNs;
}

void TSPAlgorithm::printResults() const {
    // czy algorytm wygenerował jakiekolwiek rozwiązanie przed próbą jego wypisania
    if (bestPath.empty()) {
        cout << "Blad: algorytm nie zostal jeszcze uruchomiony." << endl;
        return;
    }

    cout << "Wyniki: " << getName() << endl;
    cout << "Trasa: ";
    
    // formatowanie i wypisywanie kolejnych miast na trasie
    for (int i = 0; i < bestPath.size(); ++i) {
        cout << bestPath[i];
        if (i < bestPath.size() - 1) cout << " -> ";
    }
    cout << endl;
    cout << "Koszt:  " << bestCost << endl;

    // dynamiczne dostosowanie jednostki czasu (ns, us, ms, s)
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
    // przywraca obiekt do stanu początkowego przed kolejnym uruchomieniem, czyszczenie zapisanej trasy i reset liczników
    bestPath.clear();
    bestCost = INT_MAX;
    executionTimeNs = 0;
}

int TSPAlgorithm::calculatePathCost(const vector<int>& path, const Graph& graph) const {
    // funkcja  iterująca po kolejnych wierzchołkach i sumująca wagi krawędzi między nimi
    int cost = 0;
    int n = (path.size());

    for (int i = 0; i < n - 1; ++i) {
        int edge = graph.getEdge(path[i], path[i + 1]);
        
        // wartość -1 oznacza brak połączenia
        if (edge == -1) return INT_MAX;
        
        cost += edge;
    }
    return cost;
}