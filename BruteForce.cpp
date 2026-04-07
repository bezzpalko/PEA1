#include "BruteForce.h"
#include "Timer.h"
#include <algorithm>
#include <numeric>
#include <climits>

using namespace std;

string BruteForce::getName() const {
    // zwraca nazwę algorytmu, która będzie wyświetlana w głównym menu programu
    return "Brute-Force (przeglad zupelny)";
}

void BruteForce::solve(const Graph& graph) {
    // przywrócenie stanu początkowego zmiennych przed nowym uruchomieniem algorytmu
    reset();
    const int n = graph.getSize();
    if (n <= 0) return;

    // wierzchołek startowy to zawsze 0, więc permutacjom podlegają tylko pozostałe wierzchołki
    // badana przestrzeń rozwiązań wynosi dokładnie (n-1)!
    vector<int> perm(n - 1);
    iota(perm.begin(), perm.end(), 1); // wypełnia wektor wartościami

    // przygotowanie struktury dla pełnej ścieżki (start, permutacja miast, powrót do startu)
    vector<int> currentPath(n + 1);
    currentPath[0] = 0;
    currentPath[n] = 0; // gwarantuje zamknięcie cyklu i powrót do wierzchołka początkowego

    Timer timer;
    timer.start();

    // funkcja next_permutation generuje kolejne permutacje
    // pętla główna wykona się dokładnie (n-1)! razy
    do {
        // uzupełnienie środkowej części pełnej ścieżki aktualnie badaną permutacją wierzchołków
        for (int i = 0; i < n - 1; ++i) {
            currentPath[i + 1] = perm[i];
        }

        // obliczanie kosztu ścieżki bez stosowania technik odcinania,
        int cost = 0;
        bool valid = true;
        for (int i = 0; i < n; ++i) {
            int edge = graph.getEdge(currentPath[i], currentPath[i + 1]);
            if (edge == -1) {
                valid = false;
                // badana krawędź nie istnieje, więc ta konkretna trasa jest nielegalna;
                // przerywamy sumowanie dla tej ścieżki, ale algorytm przejdzie do kolejnej permutacji
                cost = INT_MAX;
                break;
            }
            cost += edge;
        }

        // nadpisanie najlepszego wyniku
        if (valid && cost < bestCost) {
            bestCost = cost;
            bestPath = currentPath;
        }

    } while (next_permutation(perm.begin(), perm.end()));

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}