#pragma once
#include <vector>
#include <string>
#include <climits>
#include "Graph.h"

// klasa  definiująca wspólny interfejs dla wszystkich algorytmów

class TSPAlgorithm {
public:
    // konstruktor inicjalizuje stan poczatkowy
    TSPAlgorithm();

    // dla poprawnego polimorfizmu
    virtual ~TSPAlgorithm() {}

    // do zaimplementowania przez klasy pochodne.
    virtual void solve(const Graph& graph) = 0;

    // zwraca nazwe algorytmu
    virtual std::string getName() const = 0;

    // zwraca najlepsza znaleziona sciezke
    const std::vector<int>& getBestPath() const;

    // zwraca koszt najlepszej znalezionej sciezki
    int getBestCost() const;

    // zwraca czas wykonania ostatniego solve() w nanosekundach
    long long getLastExecutionTimeNs() const;

    // wyswietla wyniki: trase, koszt i czas wykonania
    void printResults() const;

    // resetuje wyniki (sciezka, koszt, czas)
    void reset();

protected:
    std::vector<int> bestPath;   // najlepsza znaleziona sciezka
    int bestCost;                // koszt najlepszej sciezki
    long long executionTimeNs;   // czas wykonania w nanosekundach

    // oblicza koszt podanej sciezki w grafie.
    int calculatePathCost(const std::vector<int>& path, const Graph& graph) const;
};