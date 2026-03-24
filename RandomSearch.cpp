#include "RandomSearch.h"
#include "Timer.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <climits>

RandomSearch::RandomSearch(unsigned int seed)
    : seed(seed), lastIterationCount(0) {}

std::string RandomSearch::getName() const {
    return "Random Search (algorytm losowy)";
}

int RandomSearch::getLastIterationCount() const {
    return lastIterationCount;
}

void RandomSearch::solve(const Graph& graph) {
    const int n = graph.getSize();
    // Liczba permutacji zgodna z wymaganiem projektu: 10 * N
    const int iterations = 10 * n;
    runSearch(graph, iterations);
}

void RandomSearch::solveWithCustomIterations(const Graph& graph, int iterations) {
    runSearch(graph, iterations);
}

void RandomSearch::runSearch(const Graph& graph, int iterations) {
    reset();
    const int n = graph.getSize();
    if (n <= 0) return;

    lastIterationCount = iterations;

    // Inicjalizacja generatora:
    // Jesli seed == 0, uzywamy std::random_device dla pełnej losowosci.
    // Uzywamy Mersenne Twister (mt19937) – bardzo dobra jakosc losowosci,
    // ogromny okres (2^19937-1), co minimalizuje ryzyko powtorzen permutacji.
    std::mt19937 rng;
    if (seed == 0) {
        std::random_device rd;
        rng.seed(rd());
    } else {
        rng.seed(seed);
    }

    // Permutacja robocza: wierzcholki 0..n-1
    // Wierzcholek startowy (0) rowniez jest losowany – w ATSP kolejnosc
    // wszystkich wierzcholkow ma znaczenie, wiec permutujemy caly wektor,
    // a jako punkt startowy traktujemy pierwszy element po przetasowaniu.
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0); // {0, 1, 2, ..., n-1}

    Timer timer;
    timer.start();

    for (int iter = 0; iter < iterations; ++iter) {
        // std::shuffle z mt19937 – tasowanie Fisher-Yates,
        // kazda permutacja jest jednakowo prawdopodobna.
        std::shuffle(perm.begin(), perm.end(), rng);

        // Oblicz koszt sciezki: perm[0] -> perm[1] -> ... -> perm[n-1] -> perm[0]
        int cost = 0;
        bool valid = true;

        for (int i = 0; i < n - 1; ++i) {
            int edge = graph.getEdge(perm[i], perm[i + 1]);
            if (edge == -1) {
                valid = false;
                break;
            }
            cost += edge;
        }

        // Krawedz powrotu do wierzcholka startowego
        if (valid) {
            int returnEdge = graph.getEdge(perm[n - 1], perm[0]);
            if (returnEdge == -1) {
                valid = false;
            } else {
                cost += returnEdge;
            }
        }

        // Aktualizuj najlepszy wynik
        if (valid && cost < bestCost) {
            bestCost = cost;
            // Zapisz pelna sciezke z powrotem do startu
            bestPath.assign(perm.begin(), perm.end());
            bestPath.push_back(perm[0]);
        }
    }

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}