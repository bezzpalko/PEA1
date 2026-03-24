#pragma once
#include <vector>
#include <string>
#include "Graph.h"
#include "BruteForce.h"
#include "NearestNeighbour.h"
#include "RepetitiveNN.h"
#include "RandomSearch.h"

// Struktura przechowujaca wyniki pojedynczego eksperymentu dla danego N
struct ExperimentResult {
    int n;                        // Rozmiar problemu
    double avgErrorNN;            // Sredni blad wzgledny NN [%]
    double avgErrorRNN;           // Sredni blad wzgledny RNN [%]
    double avgErrorRandom;        // Sredni blad wzgledny Random [%]
    double avgTimeBF_ns;          // Sredni czas BF [ns]
    double avgTimeNN_ns;          // Sredni czas NN [ns]
    double avgTimeRNN_ns;         // Sredni czas RNN [ns]
    double avgTimeRandom_ns;      // Sredni czas Random [ns]
};

// Struktura wynikow pomiaru czasu BF dla roznych rozmiarow N
struct BFTimeResult {
    int n;                        // Rozmiar problemu
    double avgTime_ns;            // Sredni czas BF [ns]
    double avgTime_s;             // Sredni czas BF [s]
};

// Klasa odpowiedzialna za automatyczne przeprowadzenie eksperymentow
// i zapis wynikow do plikow CSV zgodnie z wymaganiami projektu.
//
// Eksperymenty:
//   1. Dla N in {10,11,12,13,14}: 100 losowych macierzy, blad wzgledny NN/RNN/Random
//   2. Pomiar czasu BF dla roznych N, wyznaczenie N dla czasu ~2 minuty
class ExperimentRunner {
public:
    ExperimentRunner() = default;

    // Glowny eksperyment: dla N in {10,11,12,13,14}, 100 instancji kazda.
    // Oblicza sredni blad wzgledny wzgledem optimum (BF).
    // Zapisuje wyniki do pliku CSV.
    void runErrorExperiment(const std::string& outputCsvPath = "error_results.csv");

    // Eksperyment pomiaru czasu BF dla podanych wartosci N.
    // Dla kazdego N uruchamia BF na 'repetitions' losowych instancjach i uśrednia czas.
    // Zapisuje wyniki do pliku CSV.
    void runBFTimeExperiment(const std::vector<int>& nValues,
                             int repetitions,
                             const std::string& outputCsvPath = "bf_time_results.csv");

    // Szuka N dla ktorego BF dziala okolo targetSeconds (domyslnie 120s = 2 minuty).
    // Uzywa binarnego/sekwencyjnego poszukiwania z pomiarami czasu.
    int findNForTargetTime(double targetSeconds = 120.0);

    // Zwraca ostatnio obliczone wyniki bledow (po runErrorExperiment)
    const std::vector<ExperimentResult>& getErrorResults() const;

    // Zwraca ostatnio obliczone wyniki czasu BF (po runBFTimeExperiment)
    const std::vector<BFTimeResult>& getBFTimeResults() const;

private:
    std::vector<ExperimentResult> errorResults;
    std::vector<BFTimeResult> bfTimeResults;

    // Oblicza blad wzgledny [%]: (koszt - optimum) / optimum * 100
    // Zwraca 0.0 jesli koszt == optimum, -1.0 jesli optimum == 0 lub INT_MAX
    double relativeError(int cost, int optimum) const;

    // Zapisuje wyniki bledow do pliku CSV
    void saveErrorResultsToCSV(const std::string& path) const;

    // Zapisuje wyniki czasu BF do pliku CSV
    void saveBFTimeResultsToCSV(const std::string& path) const;
};