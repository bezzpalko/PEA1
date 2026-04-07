#include "ExperimentRunner.h"
#include "Timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <climits>
#include <numeric>

using namespace std;

// pomocnicze
double ExperimentRunner::relativeError(int cost, int optimum) const {
    // oblicza błąd względny wyrażony w procentach w stosunku do optymalnego rozwiązania (brute-force)
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


// blad wzgledny dla N in {10,11,12,13,14}
void ExperimentRunner::runErrorExperiment(const string& outputCsvPath) {
    // czyszczenie poprzednich wyników przed rozpoczęciem nowej serii badań
    errorResults.clear();

    const vector<int> nValues = {10, 11, 12, 13, 14};
    const int repetitions = 100; // dla każdego n generowane jest po 100 tablic odległości

    BruteForce     bf;
    NearestNeighbour nn;
    RepetitiveNN   rnn;
    RandomSearch   rndSearch;

    cout << "Eksperyment: sredni blad wzgledny (100 instancji)" << endl;

    for (int n : nValues) {
        cout << "[N = " << n << "] trwa obliczanie...";

        // zmienne agregujące wyniki ze wszystkich powtórzeń dla zadanego n
        double sumErrorNN     = 0.0;
        double sumErrorRNN    = 0.0;
        double sumErrorRandom = 0.0;
        double sumTimeBF      = 0.0;
        double sumTimeNN      = 0.0;
        double sumTimeRNN     = 0.0;
        double sumTimeRandom  = 0.0;
        int    validCount     = 0;

        for (int rep = 0; rep < repetitions; ++rep) {
            // wygenerowanie nowej, losowej macierzy kosztów o podanym rozmiarze
            Graph g;
            g.generateRandom(n, 1, 100);

            // Brute Force
            // uruchomienie algorytmu dokładnego, aby ustalić bezwzględne optimum do wyliczania błędów
            bf.solve(g);
            int optimum = bf.getBestCost();

            // pominięcie wygenerowanej instancji, jeżeli badany graf nie posiada poprawnej ścieżki
            if (optimum == INT_MAX) continue;

            sumTimeBF += static_cast<double>(bf.getLastExecutionTimeNs());

            // Nearest Neighbour
            nn.solve(g);
            int costNN = nn.getBestCost();
            double errNN = relativeError(costNN, optimum);
            if (errNN >= 0.0) sumErrorNN += errNN;
            sumTimeNN += static_cast<double>(nn.getLastExecutionTimeNs());

            // Repetitive Nearest Neighbour
            rnn.solve(g);
            int costRNN = rnn.getBestCost();
            double errRNN = relativeError(costRNN, optimum);
            if (errRNN >= 0.0) sumErrorRNN += errRNN;
            sumTimeRNN += static_cast<double>(rnn.getLastExecutionTimeNs());

            // Random Search (10*N permutacji)
            rndSearch.solve(g);
            int costRandom = rndSearch.getBestCost();
            double errRandom = relativeError(costRandom, optimum);
            if (errRandom >= 0.0) sumErrorRandom += errRandom;
            sumTimeRandom += static_cast<double>(rndSearch.getLastExecutionTimeNs());

            ++validCount;

            // wyświetlanie komunikatu o postępie co 10 instancji
            if ((rep + 1) % 10 == 0) {
                cout << " " << (rep + 1) << "/" << repetitions;
            }
        }

        if (validCount == 0) {
            cout << "Brak waznych wynikow dla N = " << n << endl;
            continue;
        }

        // uśrednienie skumulowanych wartości błędów względnych oraz czasu wykonania dla aktualnego n
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

    // eksport przygotowanych danych do pliku
    saveErrorResultsToCSV(outputCsvPath);
    cout << "Wyniki zapisano do: " << outputCsvPath << endl;
}

// eksperyment czasu BF
void ExperimentRunner::runBFTimeExperiment(const vector<int>& nValues, int repetitions, const string& outputCsvPath) {
    // przygotowanie środowiska pod kątem pomiaru wydajności samego algorytmu brute-force
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
            // brany jest pod uwagę wyłącznie czas działania dla spójnych instancji
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

// szukanie N dla czasu ~2 minut
int ExperimentRunner::findNForTargetTime(double targetSeconds) {
    BruteForce bf;
    // redukcja liczby powtórzeń jest konieczna
    const int repetitions = 3; 
    const double toleranceFactor = 0.1; // określenie akceptowalnego progu odchylenia względem docelowych 2 minut

    cout << "Szukanie N dla czasu BF = "
              << fixed << setprecision(0)
              << targetSeconds << " s" << endl;

    // poszukiwania bezpiecznie startują od najmniejszej sprawdzanej wcześniej wartości n
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

        // sprawdzenie, czy zmierzony czas pracy algorytmu łapie się w bezpiecznym zakresie tolerancji
        if (abs(measuredTime - targetSeconds) / targetSeconds <= toleranceFactor) {
            cout << "Znaleziono: N = " << n
                      << " (~" << measuredTime << " s)" << endl;
            return n;
        }

        // przerwanie pętli, gdy czas znacząco przewyższył zakładany próg, uznając poprzednie n za wystarczające
        if (measuredTime > targetSeconds) {
            cout << "Najblizsze N = " << n
                      << " (czas: " << measuredTime << " s > "
                      << targetSeconds << " s)" << endl;
            return n;
        }

        ++n;

        // dodatkowe zabezpieczenie chroniące program przed ewentualnym uśpieniem w bardzo długich obliczeniach
        if (n > 20) {
            cout << "Przekroczono N=20 bez osiagniecia celu." << endl;
            return n;
        }
    }
}

// zapis do CSV

void ExperimentRunner::saveErrorResultsToCSV(const string& path) const {
    // nawiązanie połączenia ze strumieniem plikowym wraz z weryfikacją poprawności dostępu
    ofstream file(path);
    if (!file.is_open()) {
        cerr << "[Blad] Nie mozna zapisac do pliku: " << path << endl;
        return;
    }

    // wygenerowanie stałego nagłówka opisującego poszczególne pola wymagane w tabeli wyników
    file << "N,"
         << "AvgError_NN_%,"
         << "AvgError_RNN_%,"
         << "AvgError_Random_%,"
         << "AvgTime_BF_ms,"
         << "AvgTime_NN_ms,"
         << "AvgTime_RNN_ms,"
         << "AvgTime_Random_ms\n";

    // rygorystyczne formatowanie danych zmiennoprzecinkowych poprawia ich czytelność w narzędziach analitycznych
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

    // zrzut uśrednionych danych do str
    file << "N,AvgTime_ms,AvgTime_s\n";
    file << fixed << setprecision(6);
    for (const auto& r : bfTimeResults) {
        file << r.n                   << ","
             << r.avgTime_ns / 1e6    << ","
             << r.avgTime_s           << "\n";
    }

    file.close();
}