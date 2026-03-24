#include "ExperimentRunner.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
#include <numeric>

using namespace std;

// -----------------------------------------------------------------------
// Pomocnicze
// -----------------------------------------------------------------------

double ExperimentRunner::relativeError(int cost, int optimum) const {
    if (optimum <= 0 || optimum == INT_MAX) return -1.0;
    if (cost == INT_MAX) return -1.0;
    return (static_cast<double>(cost - optimum) / static_cast<double>(optimum)) * 100.0;
}

const vector<ExperimentResult>& ExperimentRunner::getErrorResults() const {
    return errorResults;
}

const vector<BFTimeResult>& ExperimentRunner::getBFTimeResults() const {
    return bfTimeResults;
}

// -----------------------------------------------------------------------
// Glowny eksperyment – blad wzgledny dla N in {10,11,12,13,14}
// -----------------------------------------------------------------------

void ExperimentRunner::runErrorExperiment(const string& outputCsvPath) {
    errorResults.clear();

    const vector<int> nValues = {10, 11, 12, 13, 14};
    const int repetitions = 100;

    BruteForce     bf;
    NearestNeighbour nn;
    RepetitiveNN   rnn;
    RandomSearch   rndSearch;

    cout << "Eksperyment: sredni blad wzgledny (100 instancji)" << endl;

    for (int n : nValues) {
        cout << "[N = " << n << "] trwa obliczanie...";

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
                cout << " " << (rep + 1) << "/" << repetitions;
            }
        }

        if (validCount == 0) {
            cout << "Brak waznych wynikow dla N=" << n << endl;
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

        cout << "\n  BF  czas avg:  " << fixed << setprecision(2)
                  << result.avgTimeBF_ns / 1e6 << " ms";
        cout << "\n  NN  blad avg:  " << result.avgErrorNN    << " %";
        cout << "\n  RNN blad avg:  " << result.avgErrorRNN   << " %";
        cout << "\n  Rnd blad avg:  " << result.avgErrorRandom << " %" << endl;
    }

    saveErrorResultsToCSV(outputCsvPath);
    cout << "Wyniki zapisano do: " << outputCsvPath << endl;
}

// -----------------------------------------------------------------------
// Eksperyment czasu BF
// -----------------------------------------------------------------------

void ExperimentRunner::runBFTimeExperiment(const vector<int>& nValues,
                                            int repetitions,
                                            const string& outputCsvPath) {
    bfTimeResults.clear();
    BruteForce bf;

    cout << "Eksperyment: Czas BF vs N" << endl;
    cout << setw(6) << "N"
              << setw(18) << "Avg czas [ms]"
              << setw(18) << "Avg czas [s]" << endl;
    cout << string(42, '-') << endl;

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

        cout << setw(6) << n
                  << setw(18) << fixed << setprecision(4)
                  << result.avgTime_ns / 1e6
                  << setw(18) << setprecision(6)
                  << result.avgTime_s << endl;
    }

    saveBFTimeResultsToCSV(outputCsvPath);
    cout << "Wyniki czasu BF zapisano do: " << outputCsvPath << endl;
}

// -----------------------------------------------------------------------
// Szukanie N dla czasu ~2 minut
// -----------------------------------------------------------------------

int ExperimentRunner::findNForTargetTime(double targetSeconds) {
    BruteForce bf;
    const int repetitions = 3; // Malo powtorzen – wieksze N trwa dlugo
    const double toleranceFactor = 0.1; // 10% tolerancja

    cout << "Szukanie N dla czasu BF = "
              << fixed << setprecision(0)
              << targetSeconds << " s" << endl;

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

        cout << "  N = " << setw(3) << n
                  << " | Avg czas: " << fixed << setprecision(4)
                  << measuredTime << " s" << endl;

        // Sprawdz czy jestesmy w przedziale docelowym
        if (abs(measuredTime - targetSeconds) / targetSeconds <= toleranceFactor) {
            cout << "[OK] Znaleziono: N = " << n
                      << " (~" << measuredTime << " s)" << endl;
            return n;
        }

        // Przekroczylismy cel – poprzednie N bylo blizej
        if (measuredTime > targetSeconds) {
            cout << "[OK] Najblizsze N = " << n
                      << " (czas: " << measuredTime << " s > "
                      << targetSeconds << " s)" << endl;
            return n;
        }

        ++n;

        // Zabezpieczenie przed zbyt dlugim dzialaniem
        if (n > 20) {
            cout << "[Uwaga] Przekroczono N=20 bez osiagniecia celu." << endl;
            return n;
        }
    }
}

// -----------------------------------------------------------------------
// Zapis do CSV
// -----------------------------------------------------------------------

void ExperimentRunner::saveErrorResultsToCSV(const string& path) const {
    ofstream file(path);
    if (!file.is_open()) {
        cerr << "[Blad] Nie mozna zapisac do pliku: " << path << endl;
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

    file << fixed << setprecision(6);
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

void ExperimentRunner::saveBFTimeResultsToCSV(const string& path) const {
    ofstream file(path);
    if (!file.is_open()) {
        cerr << "[Blad] Nie mozna zapisac do pliku: " << path << endl;
        return;
    }

    file << "N,AvgTime_ms,AvgTime_s\n";
    file << fixed << setprecision(6);
    for (const auto& r : bfTimeResults) {
        file << r.n                   << ","
             << r.avgTime_ns / 1e6    << ","
             << r.avgTime_s           << "\n";
    }

    file.close();
}