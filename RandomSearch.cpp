#include "RandomSearch.h"
#include "Timer.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <climits>

using namespace std;

RandomSearch::RandomSearch(unsigned int seed) {
    this->seed = seed;
    this->lastIterationCount = 0;
}

string RandomSearch::getName() const {
    return "Random Search (algorytm losowy)";
}

int RandomSearch::getLastIterationCount() const {
    return lastIterationCount;
}

void RandomSearch::solve(const Graph& graph) {
    const int n = graph.getSize();
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

    // inicjalizacja generatora
    if (seed == 0) {
        random_device rd;
        rng.seed(rd());
    } else {
        rng.seed(seed);
    }

    // permutacja robocza
    vector<int> perm(n);
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    Timer timer;
    timer.start();

    for (int iter = 0; iter < iterations; ++iter) {
        shuffle(perm.begin(), perm.end(), rng);

        // obliczanie kosztt sciezki
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

        // krawedz powrotu do wierzcholka startowego
        if (valid) {
            int returnEdge = graph.getEdge(perm[n - 1], perm[0]);
            if (returnEdge == -1) {
                valid = false;
            } else {
                cost += returnEdge;
            }
        }

        // aktualizacja najlepszego wyniku
        if (valid && cost < bestCost) {
            bestCost = cost;
            // zapis pełnej sciezki z powrotem do startu
            bestPath.assign(perm.begin(), perm.end());
            bestPath.push_back(perm[0]);
        }
    }

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}