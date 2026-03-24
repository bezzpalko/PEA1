#pragma once
#include <vector>
#include <string>
#include <climits>
#include "Graph.h"

// Klasa bazowa (interfejs) dla algorytmow rozwiazujacych ATSP.
// Realizuje wzorzec strategii (Strategy Pattern) – kazdy algorytm
// implementuje metode solve() i przechowuje wynik w ujednolicony sposob.
class TSPAlgorithm {
public:
    // Konstruktor inicjalizuje stan poczatkowy
    TSPAlgorithm();

    // Wirtualny destruktor – wymagany dla poprawnego polimorfizmu
    virtual ~TSPAlgorithm() {}

    // Glowna metoda algorytmu – do zaimplementowania przez klasy pochodne.
    // Przyjmuje graf, zapisuje najlepsza sciezke i jej koszt.
    virtual void solve(const Graph& graph) = 0;

    // Zwraca nazwe algorytmu (do wyswietlania w menu i raportach)
    virtual std::string getName() const = 0;

    // Zwraca najlepsza znaleziona sciezke (sekwencje wierzcholkow)
    const std::vector<int>& getBestPath() const;

    // Zwraca koszt najlepszej znalezionej sciezki
    int getBestCost() const;

    // Zwraca czas wykonania ostatniego solve() w nanosekundach
    long long getLastExecutionTimeNs() const;

    // Wyswietla wyniki: trase, koszt i czas wykonania
    void printResults() const;

    // Resetuje wyniki (sciezka, koszt, czas) do stanu poczatkowego
    void reset();

protected:
    std::vector<int> bestPath;   // Najlepsza znaleziona sciezka
    int bestCost;                // Koszt najlepszej sciezki
    long long executionTimeNs;   // Czas wykonania w nanosekundach

    // Oblicza koszt podanej sciezki w grafie.
    // Sciezka musi byc pelnym cyklem (powrot do wierzcholka startowego).
    // Zwraca INT_MAX jesli sciezka zawiera nielegalna krawedz (-1).
    int calculatePathCost(const std::vector<int>& path, const Graph& graph) const;
};