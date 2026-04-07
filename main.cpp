#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "Graph.h"
#include "Timer.h"
#include "BruteForce.h"
#include "NearestNeighbour.h"
#include "RepetitiveNN.h"
#include "RandomSearch.h"
#include "ExperimentRunner.h"

using namespace std;

// czysci bufor wejscia po blednym odczycie
void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

// wczytuje liczbe calkowita z walidacja
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Blad: podaj liczbe calkowita.\n";
        clearInputBuffer();
    }
}

// wyswietla glowne menu
void printMenu() {
    cout << "1. Wczytaj dane z pliku"<< endl;
    cout << "2. Generuj losowe dane" << endl;
    cout << "3. Wyswietl aktualne dane (macierz)" << endl;
    cout << "4. Uruchom algorytm" << endl;
    cout << "0. Wyjscie" << endl;
    cout << "Wybor: ";
}

// wyswietla podmenu wyboru algorytmu
void printAlgorithmMenu() {
    cout << "Wybor algorytmu:" << endl;
    cout << "1. Brute-Force (przeglad zupelny)" << endl;
    cout << "2. Nearest Neighbour (NN)" << endl;
    cout << "3. Repetitive Nearest Neighbour (RNN)" << endl;
    cout << "4. Algorytm losowy (Random Search)" << endl;
    cout << "Wybor algorytmu: ";
}


// wczytanie grafu z pliku
void menuLoadFromFile(Graph& graph) {
    string filename;
    cout << "Podaj nazwe pliku: ";
    cin >> filename;
    graph.loadFromFile(filename);
}

// generowanie losowego grafu
void menuGenerateRandom(Graph& graph) {
    int n        = readInt("Podaj rozmiar N: ");
    int minW     = readInt("Podaj minimalna wage krawedzi: ");
    int maxW     = readInt("Podaj maksymalna wage krawedzi: ");
    graph.generateRandom(n, minW, maxW);
}

// wyswietlenie macierzy
void menuDisplay(const Graph& graph) {
    graph.display();
}

// uruchomienie wybranego algorytmu
void menuRunAlgorithm(const Graph& graph) {
    if (!graph.isValid()) {
        cout << "Blad: brak wczytanych danych." << endl;
        return;
    }

    printAlgorithmMenu();
    int choice;
    cin >> choice;

    switch (choice) {
        case 1: {
            BruteForce bf;
            bf.solve(graph);
            bf.printResults();
            break;
        }
        case 2: {
            NearestNeighbour nn;
            nn.solve(graph);
            nn.printResults();
            break;
        }
        case 3: {
            RepetitiveNN rnn;
            rnn.solve(graph);
            rnn.printResults();
            break;
        }
        case 4: {
            RandomSearch rnd;
            // liczba permutacji
            cout << "Podaj liczbe permutacji (0 = domyslnie 10*N = "
                      << 10 * graph.getSize() << "): ";
            int iters;
            cin >> iters;
            if (iters <= 0) {
                rnd.solve(graph);
            } else {
                rnd.solveWithCustomIterations(graph, iters);
            }
            cout << "Liczba permutacji: "
                      << rnd.getLastIterationCount() << endl;
            rnd.printResults();
            break;
        }
        default:
            cout << "Blad: nieznany algorytm." << endl;
    }
}

// opcja ukryta (99): testy automatyczne ExperimentRunner
void menuRunExperiments() {
    cout << "TRYB TESTOW AUTOMATYCZNYCH" << endl;

    ExperimentRunner runner;

    cout << "Wybierz testy do przeprowadzenia:" << endl;
    cout << "1. Blad wzgledny NN/RNN/Random dla N=10..14 (100 instancji)" << endl;
    cout << "2. Czas BF dla wybranych N" << endl;
    cout << "3. Znajdz N dla czasu BF = 2 minuty" << endl;
    cout << "4. Wszystkie powyzsze" << endl;
    cout << "Wybor: ";

    int testChoice;
    cin >> testChoice;

    if (testChoice == 1 || testChoice == 4) {
        runner.runErrorExperiment("error_results.csv");
    }

    if (testChoice == 2 || testChoice == 4) {
        // reprezentatywne wartosci N dla pomiaru czasu BF
        vector<int> nValues;
        cout << "Podaj wartosci N do pomiaru czasu BF (oddzielone spacjami, zakoncz 0):\n";
        cout << "Sugerowane: 6 7 8 9 10 11 12\n";
        cout << "N: ";

        int val;
        while (cin >> val && val != 0) {
            nValues.push_back(val);
        }

        if (nValues.empty()) {
            // domyslny zestaw jesli uzytkownik nic nie podal
            nValues = {6, 7, 8, 9, 10, 11, 12};
        }

        int reps = readInt("Liczba powtorzen dla kazdego N: ");
        runner.runBFTimeExperiment(nValues, reps, "bf_time_results.csv");
    }

    if (testChoice == 3 || testChoice == 4) {
        int foundN = runner.findNForTargetTime(120.0);
        cout << "\nWynik: N = " << foundN
                  << " daje czas BF okolo 2 minuty." << endl;
    }
}

// punkt wejscia programu

int main() {
    Graph graph; // aktualnie wczytany/wygenerowany graf

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(cin >> choice)) {
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                menuLoadFromFile(graph);
                break;
            case 2:
                menuGenerateRandom(graph);
                break;
            case 3:
                menuDisplay(graph);
                break;
            case 4:
                menuRunAlgorithm(graph);
                break;
            case 0:
                break;
            // ukryta opcja testow automatycznych
            case 99:
                menuRunExperiments();
                break;
            default:
                cout << "Blad: nieznana opcja" << endl;
        }
    }

    return 0;
}