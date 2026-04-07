#pragma once
#include <vector>
#include <string>
#include <random>

// klasa reprezentująca graf skierowany w postaci macierzy kosztów

class Graph {
public:
    // inicjalizacja pustego obiektu grafu
    Graph();

    // graf o zadanym rozmiarze n i od razu przygotowujący dla niego struktury w pamięci
    explicit Graph(int n);

    // wczytywanie danych wejściowych z pliku tekstowego;
    bool loadFromFile(const std::string& filename);

    // generuje losową, asymetryczną macierz kosztów
    void generateRandom(int n, int minWeight = 1, int maxWeight = 100);

    // wyświetla zawartość macierzy kosztów
    void display() const;

    // zwraca koszt przejścia (wagę krawędzi) pomiędzy wskazanym wierzchołkiem początkowym a końcowym
    int getEdge(int from, int to) const;

    // zwraca aktualny rozmiar grafu
    int getSize() const;

    // udostępnia stałą referencję do wewnętrznej macierzy kosztów
    const std::vector<std::vector<int>>& getMatrix() const;

    // sprawdza, czy graf został pomyślnie wczytany lub wygenerowany
    bool isValid() const;

private:
    int size;                             // aktualna liczba miast (wierzchołków)
    std::vector<std::vector<int>> matrix; // struktura przechowująca wagi krawędzi
    bool valid;                           // stan gotowości obiektu do przeprowadzania obliczeń

    // pamięć dla macierzy n x n i ustawiająca wartość -1 na jej głównej przekątnej
    void initMatrix(int n);
};