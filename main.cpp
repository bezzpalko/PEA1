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

// Czysci bufor wejscia po blednym odczycie
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Wczytuje liczbe calkowita z walidacja
int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) return value;
        std::cout << "Blad: podaj liczbe calkowita.\n";
        clearInputBuffer();
    }
}

// Wyswietla glowne menu
void printMenu() {
    std::cout << "1. Wczytaj dane z pliku"<< std::endl;
    std::cout << "2. Generuj losowe dane" << std::endl;
    std::cout << "3. Wyswietl aktualne dane (macierz)" << std::endl;
    std::cout << "4. Uruchom algorytm" << std::endl;
    std::cout << "0. Wyjscie" << std::endl;
    std::cout << "Wybor: ";
}

// Wyswietla podmenu wyboru algorytmu
void printAlgorithmMenu() {
    std::cout << "Wybor algorytmu:" << std::endl;
    std::cout << "1. Brute-Force (przeglad zupelny)" << std::endl;
    std::cout << "2. Nearest Neighbour (NN)" << std::endl;
    std::cout << "3. Repetitive Nearest Neighbour (RNN)" << std::endl;
    std::cout << "4. Algorytm losowy (Random Search)" << std::endl;
    std::cout << "Wybor algorytmu: ";
}

// -----------------------------------------------------------------------
// Opcje menu
// -----------------------------------------------------------------------

// Opcja 1: Wczytanie grafu z pliku
void menuLoadFromFile(Graph& graph) {
    std::string filename;
    std::cout << "Podaj nazwe pliku: ";
    std::cin >> filename;
    graph.loadFromFile(filename);
}

// Opcja 2: Generowanie losowego grafu
void menuGenerateRandom(Graph& graph) {
    int n        = readInt("Podaj rozmiar N: ");
    int minW     = readInt("Podaj minimalna wage krawedzi: ");
    int maxW     = readInt("Podaj maksymalna wage krawedzi: ");
    graph.generateRandom(n, minW, maxW);
}

// Opcja 3: Wyswietlenie macierzy
void menuDisplay(const Graph& graph) {
    graph.display();
}

// Opcja 4: Uruchomienie wybranego algorytmu
void menuRunAlgorithm(const Graph& graph) {
    if (!graph.isValid()) {
        std::cout << "Blad: brak wczytanych danych." << std::endl;
        return;
    }

    printAlgorithmMenu();
    int choice;
    std::cin >> choice;

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
            // Zapytaj uzytkownika o liczbe permutacji (zgodnie ze specyfikacja menu)
            std::cout << "Podaj liczbe permutacji (0 = domyslnie 10*N = "
                      << 10 * graph.getSize() << "): ";
            int iters;
            std::cin >> iters;
            if (iters <= 0) {
                rnd.solve(graph);
            } else {
                rnd.solveWithCustomIterations(graph, iters);
            }
            std::cout << "Liczba permutacji: "
                      << rnd.getLastIterationCount() << std::endl;
            rnd.printResults();
            break;
        }
        default:
            std::cout << "Blad: nieznany algorytm." << std::endl;
    }
}

// Opcja ukryta (99): Testy automatyczne ExperimentRunner
void menuRunExperiments() {
    std::cout << "TRYB TESTOW AUTOMATYCZNYCH" << std::endl;

    ExperimentRunner runner;

    std::cout << "Wybierz testy do przeprowadzenia:" << std::endl;
    std::cout << "1. Blad wzgledny NN/RNN/Random dla N=10..14 (100 instancji)" << std::endl;
    std::cout << "2. Czas BF dla wybranych N" << std::endl;
    std::cout << "3. Znajdz N dla czasu BF = 2 minuty" << std::endl;
    std::cout << "4. Wszystkie powyzsze" << std::endl;
    std::cout << "Wybor: ";

    int testChoice;
    std::cin >> testChoice;

    if (testChoice == 1 || testChoice == 4) {
        runner.runErrorExperiment("error_results.csv");
    }

    if (testChoice == 2 || testChoice == 4) {
        // Reprezentatywne wartosci N dla pomiaru czasu BF
        // (skupiamy sie na wiekszych N zgodnie z wymaganiem projektu)
        std::vector<int> nValues;
        std::cout << "Podaj wartosci N do pomiaru czasu BF (oddzielone spacjami, zakoncz 0):\n";
        std::cout << "Sugerowane: 6 7 8 9 10 11 12\n";
        std::cout << "N: ";

        int val;
        while (std::cin >> val && val != 0) {
            nValues.push_back(val);
        }

        if (nValues.empty()) {
            // Domyslny zestaw jesli uzytkownik nic nie podal
            nValues = {6, 7, 8, 9, 10, 11, 12};
        }

        int reps = readInt("Liczba powtorzen dla kazdego N (np. 5): ");
        runner.runBFTimeExperiment(nValues, reps, "bf_time_results.csv");
    }

    if (testChoice == 3 || testChoice == 4) {
        int foundN = runner.findNForTargetTime(120.0);
        std::cout << "\nWynik: N = " << foundN
                  << " daje czas BF okolo 2 minuty." << std::endl;
    }
}

// -----------------------------------------------------------------------
// Punkt wejscia programu
// -----------------------------------------------------------------------

int main() {
    Graph graph; // Aktualnie wczytany/wygenerowany graf

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {
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
                std::cout << "Do widzenia!" << std::endl;
                break;
            // Ukryta opcja testow automatycznych
            case 99:
                menuRunExperiments();
                break;
            default:
                std::cout << "Blad: nieznana opcja." << std::endl;
        }
    }

    return 0;
}