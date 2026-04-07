#pragma once
#include <vector>
#include <string>
#include "Graph.h"
#include "BruteForce.h"
#include "NearestNeighbour.h"
#include "RepetitiveNN.h"
#include "RandomSearch.h"

// klasa zarządzająca automatycznym środowiskiem testowym;

// struktura przechowujaca wyniki pojedynczego eksperymentu dla danego N
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

// struktura wynikow pomiaru czasu BF dla roznych rozmiarow N
struct BFTimeResult {
    int n;                        // Rozmiar problemu
    double avgTime_ns;            // Sredni czas BF [ns]
    double avgTime_s;             // Sredni czas BF [s]
};

class ExperimentRunner {
public:
    // domyślny konstruktor klasy
    ExperimentRunner() = default;

    // główna metoda badawcza
    void runErrorExperiment(const std::string& outputCsvPath = "error_results.csv");

    // moduł badający złożoność czasową przeglądu zupełnego;
    void runBFTimeExperiment(const std::vector<int>& nValues,
                             int repetitions,
                             const std::string& outputCsvPath = "bf_time_results.csv");

    // funkcja poszukująca takiej wielkości grafu, dla której czas pracy algorytmu brute-force wynosi około podanego progu (domyślnie 120 sekund);
    int findNForTargetTime(double targetSeconds = 120.0);

    // udostępnia wektor z wynikami testów jakości (błędów) z ostatniego uruchomienia eksperymentu
    const std::vector<ExperimentResult>& getErrorResults() const;

    // udostępnia wektor z wynikami testów wydajnościowych (czasu BF) z ostatniej sesji testowej
    const std::vector<BFTimeResult>& getBFTimeResults() const;

private:
    std::vector<ExperimentResult> errorResults;   // wewnętrzny kontener na wyniki pomiarów błędów względnych
    std::vector<BFTimeResult> bfTimeResults;      // wewnętrzny kontener na wyniki pomiarów czasowych algorytmu dokładnego

    // funkcja obliczająca procentowy błąd względny uzyskanego kosztu w odniesieniu do wartości optymalnej;
    double relativeError(int cost, int optimum) const;

    // moduł eksportujący zgromadzone dane o błędach do ustrukturyzowanego pliku tekstowego (CSV)
    void saveErrorResultsToCSV(const std::string& path) const;

    // moduł eksportujący zgromadzone dane o czasie wykonania algorytmu BF do pliku tekstowego (CSV)
    void saveBFTimeResultsToCSV(const std::string& path) const;
};