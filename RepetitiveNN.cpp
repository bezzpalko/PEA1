#include "RepetitiveNN.h"
#include "Timer.h"
#include <climits>
#include <algorithm>

using namespace std;

string RepetitiveNN::getName() const {
    return "Repetitive Nearest Neighbour (z obsluga remisow)";
}

void RepetitiveNN::solve(const Graph& graph) {
    reset();
    const int n = graph.getSize();
    if (n <= 0) return;

    Timer timer;
    timer.start();

    // uruchamiamy poszukiwanie z kazdego wierzcholka startowego
    for (int startVertex = 0; startVertex < n; ++startVertex) {
        vector<int> candidatePath;
        int candidateCost = INT_MAX;

        runWithTieBreaking(graph, startVertex, candidatePath, candidateCost);

        // aktualizuj globalnie najlepszy wynik
        if (candidateCost < bestCost) {
            bestCost = candidateCost;
            bestPath = candidatePath;
        }
    }

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}

void RepetitiveNN::runWithTieBreaking(const Graph& graph, int startVertex, vector<int>& outBestPath, int& outBestCost) const {
    const int n = graph.getSize();
    outBestCost = INT_MAX;
    outBestPath.clear();

    // inicjalizacja stosu, poczatkowy stan poszukiwania
    stack<SearchState> stateStack;

    SearchState initialState;
    initialState.path.push_back(startVertex);
    initialState.visited.assign(n, false);
    initialState.visited[startVertex] = true;
    initialState.currentVertex = startVertex;
    initialState.currentCost = 0;
    initialState.startVertex = startVertex;

    stateStack.push(initialState);

    // iteracyjne przeszukiwanie z rozgalezianiem przy remisach
    while (!stateStack.empty()) {
        // pobierz kopie stanu
        SearchState state = stateStack.top();
        stateStack.pop();

        // jesli odwiedzilismy wszystkie wierzcholki to zamknij cykl
        if (state.path.size() == n) {
            int returnEdge = graph.getEdge(state.currentVertex, state.startVertex);

            // krawedz powrotna
            if (returnEdge != -1) {
                int totalCost = state.currentCost + returnEdge;

                if (totalCost < outBestCost) {
                    outBestCost = totalCost;
                    outBestPath = state.path;
                    outBestPath.push_back(state.startVertex); // zamkniecie cyklu
                }
            }
            continue;
        }

        // znajdz minimalny koszt wsrod dostepnych krawedzi
        int minEdgeCost = INT_MAX;
        for (int candidate = 0; candidate < n; ++candidate) {
            if (state.visited[candidate]) continue;
            int edgeCost = graph.getEdge(state.currentVertex, candidate);
            if (edgeCost != -1 && edgeCost < minEdgeCost) {
                minEdgeCost = edgeCost;
            }
        }

        // jesli nie ma zadnej dostepnej krawedzi to sciezka jest slepa uliczka
        if (minEdgeCost == INT_MAX) continue;

        // zbierz wszystkich kandydatow o koszcie rownym minimalnemu (remis), kazdy z nich  na stos jako osobna galaz poszukiwan
        for (int candidate = 0; candidate < n; ++candidate) {
            if (state.visited[candidate]) continue;
            int edgeCost = graph.getEdge(state.currentVertex, candidate);
            if (edgeCost != minEdgeCost) continue;

            // nowy stan dla tej galezi
            SearchState newState;
            newState.path = state.path;               // kopia aktualnej sciezki
            newState.visited = state.visited;         // kopia maski odwiedzonych
            newState.currentVertex = candidate;
            newState.currentCost = state.currentCost + edgeCost;
            newState.startVertex = state.startVertex;

            newState.path.push_back(candidate);
            newState.visited[candidate] = true;

            stateStack.push(newState);
        }
    }
}