#include "Graph.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

using namespace std;

Graph::Graph() {
    this->size = 0;
    this->valid = false;
}

Graph::Graph(int n) {
    this->size = n;
    this->valid = false;
    initMatrix(n);
}

void Graph::initMatrix(int n) {
    // Alokacja dynamiczna macierzy N x N
    matrix.assign(n, vector<int>(n, 0));
    // Ustawienie przekatnej na -1 (brak polaczenia z samym soba)
    for (int i = 0; i < n; ++i) {
        matrix[i][i] = -1;
    }
    size = n;
}

bool Graph::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Blad: nie mozna otworzyc pliku: " << filename << endl;
        valid = false;
        return false;
    }

    int n = 0;
    file >> n;

    if (n <= 0) {
        cerr << "Blad: nieprawidlowy rozmiar grafu w pliku: " << n << endl;
        valid = false;
        return false;
    }

    initMatrix(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
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

    // Uzywamy Mersenne Twister dla dobrej jakosci losowosci
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(minWeight, maxWeight);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                matrix[i][j] = dist(rng);
            }
            // matrix[i][i] pozostaje -1 (ustawione przez initMatrix)
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

    cout << "Macierz kosztow (N = " << size << "):\n";
    cout << string(size * 6 + 4, '-') << "\n";

    // Naglowek kolumn
    cout << "     ";
    for (int j = 0; j < size; ++j) {
        cout << setw(5) << j;
    }
    cout << "\n" << string(size * 6 + 4, '-') << "\n";

    // Wiersze macierzy
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
    if (from < 0 || from >= size || to < 0 || to >= size) {
        throw out_of_range("Blad: indeks krawedzi poza zakresem macierzy.");
    }
    return matrix[from][to];
}

int Graph::getSize() const {
    return size;
}

const vector<vector<int>>& Graph::getMatrix() const {
    return matrix;
}

bool Graph::isValid() const {
    return valid;
}