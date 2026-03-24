#pragma once
#include <vector>
#include <string>
#include <random>

// Klasa reprezentujaca asymetryczny graf skierowany jako macierz kosztow.
// Przekatna macierzy zawiera wartosc -1 (brak polaczenia z samym soba).
// Odleglosci sa liczbami calkowitymi.
class Graph {
public:
    // Konstruktor domyslny - tworzy pusty graf
    Graph();

    // Konstruktor z rozmiarem - tworzy graf N x N
    explicit Graph(int n);

    // Wczytuje macierz kosztow z pliku tekstowego.
    // Format: pierwsza linia to N, kolejne N linii to wiersze macierzy
    // (liczby oddzielone spacjami, -1 na przekatnej).
    // Zwraca true jesli wczytanie sie powiodlo.
    bool loadFromFile(const std::string& filename);

    // Generuje losowa asymetryczna macierz kosztow rozmiaru n x n.
    // Wagi krawedzi sa losowane z zakresu [minWeight, maxWeight].
    // Przekatna ustawiana jest na -1.
    void generateRandom(int n, int minWeight = 1, int maxWeight = 100);

    // Wyswietla macierz kosztow w czytelnym formacie
    void display() const;

    // Zwraca koszt krawedzi (i -> j)
    int getEdge(int from, int to) const;

    // Zwraca rozmiar grafu (liczbe wierzcholkow)
    int getSize() const;

    // Zwraca cala macierz kosztow (tylko do odczytu)
    const std::vector<std::vector<int>>& getMatrix() const;

    // Sprawdza czy graf zostal poprawnie zaladowany/wygenerowany
    bool isValid() const;

private:
    int size;                             // Liczba wierzcholkow N
    std::vector<std::vector<int>> matrix; // Macierz kosztow N x N
    bool valid;                           // Flaga poprawnosci grafu

    // Inicjalizuje macierz rozmiaru n x n, przekatna = -1
    void initMatrix(int n);
};