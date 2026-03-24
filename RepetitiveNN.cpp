#include "RepetitiveNN.h"
#include "Timer.h"
#include <climits>
#include <algorithm>

std::string RepetitiveNN::getName() const {
    return "Repetitive Nearest Neighbour (z obsluga remisow)";
}

void RepetitiveNN::solve(const Graph& graph) {
    reset();
    const int n = graph.getSize();
    if (n <= 0) return;

    Timer timer;
    timer.start();

    // Uruchamiamy poszukiwanie z kazdego wierzcholka startowego
    for (int startVertex = 0; startVertex < n; ++startVertex) {
        std::vector<int> candidatePath;
        int candidateCost = INT_MAX;

        runWithTieBreaking(graph, startVertex, candidatePath, candidateCost);

        // Aktualizuj globalnie najlepszy wynik
        if (candidateCost < bestCost) {
            bestCost = candidateCost;
            bestPath = candidatePath;
        }
    }

    timer.stop();
    executionTimeNs = timer.getElapsedNanoseconds();
}

void RepetitiveNN::runWithTieBreaking(const Graph& graph, int startVertex,
                                       std::vector<int>& outBestPath,
                                       int& outBestCost) const {
    const int n = graph.getSize();
    outBestCost = INT_MAX;
    outBestPath.clear();

    // Inicjalizacja stosu – poczatkowy stan poszukiwania
    std::stack<SearchState> stateStack;

    SearchState initialState;
    initialState.path.push_back(startVertex);
    initialState.visited.assign(n, false);
    initialState.visited[startVertex] = true;
    initialState.currentVertex = startVertex;
    initialState.currentCost = 0;
    initialState.startVertex = startVertex;

    stateStack.push(std::move(initialState));

    // Iteracyjne przeszukiwanie z rozgalezianiem przy remisach
    while (!stateStack.empty()) {
        // Pobierz kopie stanu (move dla wydajnosci)
        SearchState state = std::move(stateStack.top());
        stateStack.pop();

        // Jesli odwiedzilismy wszystkie wierzcholki – zamknij cykl
        if (static_cast<int>(state.path.size()) == n) {
            int returnEdge = graph.getEdge(state.currentVertex, state.startVertex);

            // Krawedz powrotna musi istniec
            if (returnEdge != -1) {
                int totalCost = state.currentCost + returnEdge;

                if (totalCost < outBestCost) {
                    outBestCost = totalCost;
                    outBestPath = state.path;
                    outBestPath.push_back(state.startVertex); // Zamkniecie cyklu
                }
            }
            continue;
        }

        // Znajdz minimalny koszt wsrod dostepnych krawedzi
        int minEdgeCost = INT_MAX;
        for (int candidate = 0; candidate < n; ++candidate) {
            if (state.visited[candidate]) continue;
            int edgeCost = graph.getEdge(state.currentVertex, candidate);
            if (edgeCost != -1 && edgeCost < minEdgeCost) {
                minEdgeCost = edgeCost;
            }
        }

        // Jesli nie ma zadnej dostepnej krawedzi – sciezka jest slepa uliczka
        if (minEdgeCost == INT_MAX) continue;

        // Zbierz WSZYSTKICH kandydatow o koszcie rownym minimalnemu (remis)
        // Kazdy z nich trafi na stos jako osobna galaz poszukiwan
        for (int candidate = 0; candidate < n; ++candidate) {
            if (state.visited[candidate]) continue;
            int edgeCost = graph.getEdge(state.currentVertex, candidate);
            if (edgeCost != minEdgeCost) continue;

            // Tworzymy nowy stan dla tej galezi
            SearchState newState;
            newState.path = state.path;               // Kopia aktualnej sciezki
            newState.visited = state.visited;         // Kopia maski odwiedzonych
            newState.currentVertex = candidate;
            newState.currentCost = state.currentCost + edgeCost;
            newState.startVertex = state.startVertex;

            newState.path.push_back(candidate);
            newState.visited[candidate] = true;

            stateStack.push(std::move(newState));
        }
    }
}