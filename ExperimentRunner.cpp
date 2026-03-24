#include "ExperimentRunner.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
#include <numeric>

// -----------------------------------------------------------------------
// Pomocnicze
// -----------------------------------------------------------------------

double ExperimentRunner::relativeError(int cost, int optimum) const {
    if (optimum <= 0 || optimum == INT_MAX) return -1.0;
    if (cost == INT_MAX) return -1.0;
    return (static_cast<double>(cost - optimum) / static_cast<double>(optimum)) * 100.0;
}

const std::vector<ExperimentResult>& ExperimentRunner::getErrorResults() const {
    return errorResults;
}

const std::vector<BFTimeResult>& ExperimentRunner::getBFTimeResults() const {
    return bfTimeResults;
}

// -----------------------------------------------------------------------
// Glowny eksperyment – blad wzgledny dla N in {10,11,12,13,14}
// -----------------------------------------------------------------------

void ExperimentRunner::runErrorExperiment(const std::string& outputCsvPath) {
    errorResults.clear();

    const std::vector<int> nValues = {10, 11, 12, 13, 14};
    const int repetitions = 100;

    BruteForce     bf;
    NearestNeighbour nn;
    RepetitiveNN   rnn;
    RandomSearch   rndSearch;

    std::cout << "Eksperyment: sredni blad wzgledny (100 instancji)" << std::endl;

    for (int n : nValues) {
        std::cout << "[N = " << n << "] trwa obliczanie..." << std::flush;

        double sumErrorNN     = 0.0;
        double sumErrorRNN    = 0.0;
        double sumErrorRandom = 0.0;
        double sumTimeBF      = 0.0;
        double sumTimeNN      = 0.0;
        double sumTimeRNN     = 0.0;
        double sumTimeRandom  = 0.0;
        int    validCount     = 0;

        for (int rep = 0; rep < repetitions; ++rep) {
            // Generuj losowa instancje
            Graph g;
            g.generateRandom(n, 1, 100);

            // --- Brute Force (optimum) ---
            bf.solve(g);
            int optimum = bf.getBestCost();

            // Pomijamy instancje, dla ktorych BF nie znalazl rozwiazania
            if (optimum == INT_MAX) continue;

            sumTimeBF += static_cast<double>(bf.getLastExecutionTimeNs());

            // --- Nearest Neighbour ---
            nn.solve(g);
            int costNN = nn.getBestCost();
            double errNN = relativeError(costNN, optimum);
            if (errNN >= 0.0) sumErrorNN += errNN;
            sumTimeNN += static_cast<double>(nn.getLastExecutionTimeNs());

            // --- Repetitive Nearest Neighbour ---
            rnn.solve(g);
            int costRNN = rnn.getBestCost();
            double errRNN = relativeError(costRNN, optimum);
            if (errRNN >= 0.0) sumErrorRNN += errRNN;
            sumTimeRNN += static_cast<double>(rnn.getLastExecutionTimeNs());

            // --- Random Search (10*N permutacji) ---
            rndSearch.solve(g);
            int costRandom = rndSearch.getBestCost();
            double errRandom = relativeError(costRandom, optimum);
            if (errRandom >= 0.0) sumErrorRandom += errRandom;
            sumTimeRandom += static_cast<double>(rndSearch.getLastExecutionTimeNs());

            ++validCount;

            // Postep co 10 instancji
            if ((rep + 1) % 10 == 0) {
                std::cout << " " << (rep + 1) << "/" << repetitions << std::flush;
            }
        }

        if (validCount == 0) {
            std::cout << "Brak waznych wynikow dla N=" << n << std::endl;
            continue;
        }

        ExperimentResult result;
        result.n             = n;
        result.avgErrorNN    = sumErrorNN    / validCount;
        result.avgErrorRNN   = sumErrorRNN   / validCount;
        result.avgErrorRandom= sumErrorRandom/ validCount;
        result.avgTimeBF_ns  = sumTimeBF     / validCount;
        result.avgTimeNN_ns  = sumTimeNN     / validCount;
        result.avgTimeRNN_ns = sumTimeRNN    / validCount;
        result.avgTimeRandom_ns = sumTimeRandom / validCount;

        errorResults.push_back(result);

        std::cout << "\n  BF  czas avg:  " << std::fixed << std::setprecision(2)
                  << result.avgTimeBF_ns / 1e6 << " ms";
        std::cout << "\n  NN  blad avg:  " << result.avgErrorNN    << " %";
        std::cout << "\n  RNN blad avg:  " << result.avgErrorRNN   << " %";
        std::cout << "\n  Rnd blad avg:  " << result.avgErrorRandom << " %" << std::endl;
    }

    saveErrorResultsToCSV(outputCsvPath);
    std::cout << "Wyniki zapisano do: " << outputCsvPath << std::endl;
}

// -----------------------------------------------------------------------
// Eksperyment czasu BF
// -----------------------------------------------------------------------

void ExperimentRunner::runBFTimeExperiment(const std::vector<int>& nValues,
                                            int repetitions,
                                            const std::string& outputCsvPath) {
    bfTimeResults.clear();
    BruteForce bf;

    std::cout << "Eksperyment: Czas BF vs N" << std::endl;
    std::cout << std::setw(6) << "N"
              << std::setw(18) << "Avg czas [ms]"
              << std::setw(18) << "Avg czas [s]" << std::endl;
    std::cout << std::string(42, '-') << std::endl;

    for (int n : nValues) {
        double sumTime_ns = 0.0;
        int validCount = 0;

        for (int rep = 0; rep < repetitions; ++rep) {
            Graph g;
            g.generateRandom(n, 1, 100);
            bf.solve(g);
            if (bf.getBestCost() != INT_MAX) {
                sumTime_ns += static_cast<double>(bf.getLastExecutionTimeNs());
                ++validCount;
            }
        }

        if (validCount == 0) continue;

        BFTimeResult result;
        result.n          = n;
        result.avgTime_ns = sumTime_ns / validCount;
        result.avgTime_s  = result.avgTime_ns / 1e9;
        bfTimeResults.push_back(result);

        std::cout << std::setw(6) << n
                  << std::setw(18) << std::fixed << std::setprecision(4)
                  << result.avgTime_ns / 1e6
                  << std::setw(18) << std::setprecision(6)
                  << result.avgTime_s << std::endl;
    }

    saveBFTimeResultsToCSV(outputCsvPath);
    std::cout << "Wyniki czasu BF zapisano do: " << outputCsvPath << std::endl;
}

// -----------------------------------------------------------------------
// Szukanie N dla czasu ~2 minut
// -----------------------------------------------------------------------

int ExperimentRunner::findNForTargetTime(double targetSeconds) {
    BruteForce bf;
    const int repetitions = 3; // Malo powtorzen – wieksze N trwa dlugo
    const double toleranceFactor = 0.1; // 10% tolerancja

    std::cout << "Szukanie N dla czasu BF = "
              << std::fixed << std::setprecision(0)
              << targetSeconds << " s" << std::endl;

    // Startujemy od N=10 i zwiekszamy az przekroczymy cel
    int n = 10;
    double measuredTime = 0.0;

    while (true) {
        double sumTime = 0.0;
        for (int rep = 0; rep < repetitions; ++rep) {
            Graph g;
            g.generateRandom(n, 1, 100);
            bf.solve(g);
            sumTime += bf.getLastExecutionTimeNs() / 1e9;
        }
        measuredTime = sumTime / repetitions;

        std::cout << "  N = " << std::setw(3) << n
                  << " | Avg czas: " << std::fixed << std::setprecision(4)
                  << measuredTime << " s" << std::endl;

        // Sprawdz czy jestesmy w przedziale docelowym
        if (std::abs(measuredTime - targetSeconds) / targetSeconds <= toleranceFactor) {
            std::cout << "[OK] Znaleziono: N = " << n
                      << " (~" << measuredTime << " s)" << std::endl;
            return n;
        }

        // Przekroczylismy cel – poprzednie N bylo blizej
        if (measuredTime > targetSeconds) {
            std::cout << "[OK] Najblizsze N = " << n
                      << " (czas: " << measuredTime << " s > "
                      << targetSeconds << " s)" << std::endl;
            return n;
        }

        ++n;

        // Zabezpieczenie przed zbyt dlugim dzialaniem
        if (n > 20) {
            std::cout << "[Uwaga] Przekroczono N=20 bez osiagniecia celu." << std::endl;
            return n;
        }
    }
}

// -----------------------------------------------------------------------
// Zapis do CSV
// -----------------------------------------------------------------------

void ExperimentRunner::saveErrorResultsToCSV(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Blad] Nie mozna zapisac do pliku: " << path << std::endl;
        return;
    }

    // Naglowek
    file << "N,"
         << "AvgError_NN_%,"
         << "AvgError_RNN_%,"
         << "AvgError_Random_%,"
         << "AvgTime_BF_ms,"
         << "AvgTime_NN_ms,"
         << "AvgTime_RNN_ms,"
         << "AvgTime_Random_ms\n";

    file << std::fixed << std::setprecision(6);
    for (const auto& r : errorResults) {
        file << r.n                       << ","
             << r.avgErrorNN              << ","
             << r.avgErrorRNN             << ","
             << r.avgErrorRandom          << ","
             << r.avgTimeBF_ns  / 1e6     << ","
             << r.avgTimeNN_ns  / 1e6     << ","
             << r.avgTimeRNN_ns / 1e6     << ","
             << r.avgTimeRandom_ns / 1e6  << "\n";
    }

    file.close();
}

void ExperimentRunner::saveBFTimeResultsToCSV(const std::string& path) const {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "[Blad] Nie mozna zapisac do pliku: " << path << std::endl;
        return;
    }

    file << "N,AvgTime_ms,AvgTime_s\n";
    file << std::fixed << std::setprecision(6);
    for (const auto& r : bfTimeResults) {
        file << r.n                   << ","
             << r.avgTime_ns / 1e6    << ","
             << r.avgTime_s           << "\n";
    }

    file.close();
}