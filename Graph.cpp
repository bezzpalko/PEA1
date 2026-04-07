#include "Graph.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

Graph::Graph() {
    // konstruktor przygotowujący pusty obiekt grafu
    this->size = 0;
    this->valid = false;
}

Graph::Graph(int n) {
    // konstruktor tworzący od razu graf o konkretnej liczbie miast
    this->size = n;
    this->valid = false;
    initMatrix(n);
}

void Graph::initMatrix(int n) {
    // dynamiczna alokacja pamięci dla dwuwymiarowej macierzy kosztów
    matrix.assign(n, vector<int>(n, 0));
    
    // odległość z danego miasta do niego samego musi wynosić -1
    for (int i = 0; i < n; ++i) {
        matrix[i][i] = -1;
    }
    size = n;
}

bool Graph::loadFromFile(const string& filename) {
    // wczytywania danych wejściowych z pliku tekstowego
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Blad: nie mozna otworzyc pliku: " << filename << endl;
        valid = false;
        return false;
    }

    int n = 0;
    // pierwsza linia w pliku zawsze definiuje liczbę miast
    file >> n;

    if (n <= 0) {
        cerr << "Blad: nieprawidlowy rozmiar grafu w pliku: " << n << endl;
        valid = false;
        return false;
    }

    // przygotowanie struktur w pamięci pod wczytany przed chwilą rozmiar n
    initMatrix(n);

    // sekwencyjne wczytywanie kolejnych wag krawędzi do macierzy
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // weryfikacja, czy plik nie kończy się przedwcześnie i czy format danych jest poprawny
            if (!(file >> matrix[i][j])) {
                cerr << "Blad: nieoczekiwany koniec pliku podczas wczytywania macierzy." << endl;
                valid = false;
                return false;
            }
        }
    }

    file.close();
    valid = true;
    cout << "Wczytano graf rozmiaru " << n << " x " << n
              << " z pliku: " << filename << endl;
    return true;
}

void Graph::generateRandom(int n, int minWeight, int maxWeight) {
    if (n <= 0 || minWeight > maxWeight) {
        cerr << "Blad: nieprawidlowe parametry generowania grafu." << endl;
        valid = false;
        return;
    }

    initMatrix(n);

    // generator mersenne twister 
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(minWeight, maxWeight);

    // generowanie asymetrycznego grafu
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                matrix[i][j] = dist(rng);
            }
            // wartość na przekątnej (i == j) pozostaje bez zmian
        }
    }

    valid = true;
    cout << "Wygenerowano losowy graf asymetryczny rozmiaru "
              << n << " x " << n << endl;
}

void Graph::display() const {
    if (!valid) {
        cout << "Blad: graf nie zostal wczytany ani wygenerowany." << endl;
        return;
    }

    // metoda formatująca wypisywanie macierzy
    cout << "Macierz kosztow (N = " << size << "):\n";
    cout << string(size * 6 + 4, '-') << "\n";

    // renderowanie nagłówka z indeksami kolumn
    cout << "     ";
    for (int j = 0; j < size; ++j) {
        cout << setw(5) << j;
    }
    cout << "\n" << string(size * 6 + 4, '-') << "\n";

    // renderowanie poszczególnych wierszy wraz z ich indeksami
    for (int i = 0; i < size; ++i) {
        cout << setw(3) << i << " |";
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] == -1) {
                cout << setw(5) << "-1";
            } else {
                cout << setw(5) << matrix[i][j];
            }
        }
        cout << "\n";
    }
    cout << string(size * 6 + 4, '-') << "\n";
}

int Graph::getEdge(int from, int to) const {
    // mechanizm chroniący przed próbą odczytania kosztu z indeksów wykraczających poza rozmiar grafu
    if (from < 0 || from >= size || to < 0 || to >= size) {
        throw out_of_range("Blad: indeks krawedzi poza zakresem macierzy.");
    }
    return matrix[from][to];
}

int Graph::getSize() const {
    // zwraca bieżącą liczbę wierzchołków
    return size;
}

const vector<vector<int>>& Graph::getMatrix() const {
    // udostępnia zawartość macierzy
    return matrix;
}

bool Graph::isValid() const {
    // czy graf w pamięci zawiera poprawne i gotowe do analizy dane
    return valid;
}