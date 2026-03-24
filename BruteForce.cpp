#include "BruteForce.h"
#include "Timer.h"
#include <algorithm>
#include <numeric>
#include <climits>

using namespace std;

string BruteForce::getName() const {
    return "Brute-Force (przeglad zupelny)";
}

void BruteForce::solve(const Graph& graph) {
    reset();
    const int n = graph.getSize();
    if (n <= 0) return;

    // Wierzcholek startowy jest zawsze 0 – permutujemy pozostale wierzcholki
    // Velicina przestrzeni: (n-1)! permutacji
    vector<int> perm(n - 1);
    iota(perm.begin(), perm.end(), 1); // Wypelnia wartosciami 1, 2, ..., n-1

    // Budujemy pelna sciezke: [0, permutacja..., 0]
    vector<int> currentPath(n + 1);
    currentPath[0] = 0;
    currentPath[n] = 0; // Powrot do wierzcholka startowego

    Timer timer;
    timer.start();

    // next_permutation iteruje przez wszystkie permutacje w kolejnosci leksykograficznej.
    // Petla wykonuje sie dokladnie (n-1)! razy.
    do {
        // Przepisanie aktualnej permutacji do pelnej sciezki
        for (int i = 0; i < n - 1; ++i) {
            currentPath[i + 1] = perm[i];
        }

        // Obliczenie kosztu CALEJ sciezki – bez zadnego przycinania,
        // zgodnie z wymaganiem klasycznego przegladu zupelnego.
        int cost = 0;
        bool valid = true;
        for (int i = 0; i < n; ++i) {
            int edge = graph.getEdge(currentPath[i], currentPath[i + 1]);
            if (edge == -1) {
                valid = false;
                // Brak przerwania petli – klasyczny przeglad zupelny wymaga
                // sprawdzenia calej sciezki nawet gdy jest nielegalna
                cost = INT_MAX;
                break;
            }
            cost += edge;
        }

        // Aktualizacja najlepszego wyniku
        if (valid && cost < bestCost) {
            bestCost = cost;
            bestPath = currentPath;
        }

    } while (next_permutation(perm.begin(), perm.end()));

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}