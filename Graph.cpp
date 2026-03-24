#include "Graph.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>

Graph::Graph() : size(0), valid(false) {}

Graph::Graph(int n) : size(n), valid(false) {
    initMatrix(n);
}

void Graph::initMatrix(int n) {
    // Alokacja dynamiczna macierzy N x N
    matrix.assign(n, std::vector<int>(n, 0));
    // Ustawienie przekatnej na -1 (brak polaczenia z samym soba)
    for (int i = 0; i < n; ++i) {
        matrix[i][i] = -1;
    }
    size = n;
}

bool Graph::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Blad: nie mozna otworzyc pliku: " << filename << std::endl;
        valid = false;
        return false;
    }

    int n = 0;
    file >> n;

    if (n <= 0) {
        std::cerr << "Blad: nieprawidlowy rozmiar grafu w pliku: " << n << std::endl;
        valid = false;
        return false;
    }

    initMatrix(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (!(file >> matrix[i][j])) {
                std::cerr << "Blad: nieoczekiwany koniec pliku podczas wczytywania macierzy." << std::endl;
                valid = false;
                return false;
            }
        }
    }

    file.close();
    valid = true;
    std::cout << "Wczytano graf rozmiaru " << n << " x " << n
              << " z pliku: " << filename << std::endl;
    return true;
}

void Graph::generateRandom(int n, int minWeight, int maxWeight) {
    if (n <= 0 || minWeight > maxWeight) {
        std::cerr << "Blad: nieprawidlowe parametry generowania grafu." << std::endl;
        valid = false;
        return;
    }

    initMatrix(n);

    // Uzywamy Mersenne Twister dla dobrej jakosci losowosci
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(minWeight, maxWeight);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                matrix[i][j] = dist(rng);
            }
            // matrix[i][i] pozostaje -1 (ustawione przez initMatrix)
        }
    }

    valid = true;
    std::cout << "Wygenerowano losowy graf asymetryczny rozmiaru "
              << n << " x " << n << std::endl;
}

void Graph::display() const {
    if (!valid) {
        std::cout << "Blad: graf nie zostal wczytany ani wygenerowany." << std::endl;
        return;
    }

    std::cout << "Macierz kosztow (N = " << size << "):\n";
    std::cout << std::string(size * 6 + 4, '-') << "\n";

    // Naglowek kolumn
    std::cout << "     ";
    for (int j = 0; j < size; ++j) {
        std::cout << std::setw(5) << j;
    }
    std::cout << "\n" << std::string(size * 6 + 4, '-') << "\n";

    // Wiersze macierzy
    for (int i = 0; i < size; ++i) {
        std::cout << std::setw(3) << i << " |";
        for (int j = 0; j < size; ++j) {
            if (matrix[i][j] == -1) {
                std::cout << std::setw(5) << "-1";
            } else {
                std::cout << std::setw(5) << matrix[i][j];
            }
        }
        std::cout << "\n";
    }
    std::cout << std::string(size * 6 + 4, '-') << "\n";
}

int Graph::getEdge(int from, int to) const {
    if (from < 0 || from >= size || to < 0 || to >= size) {
        throw std::out_of_range("Blad: indeks krawedzi poza zakresem macierzy.");
    }
    return matrix[from][to];
}

int Graph::getSize() const {
    return size;
}

const std::vector<std::vector<int>>& Graph::getMatrix() const {
    return matrix;
}

bool Graph::isValid() const {
    return valid;
}