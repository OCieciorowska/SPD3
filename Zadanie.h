#pragma once
#include <vector>

using namespace std;

// Typ macierzy do przechowywania czasów zadań
using Matrix =  vector<vector<int> >;

// Klasa reprezentująca dane wejściowe problemu FP||Cmax
class Zadanie {
public:
    Matrix czasy; // czasy[job][machine] - macierz czasów przetwarzania

    Zadanie(const Matrix& czasy); // Konstruktor

    int liczbaZadan() const;     // Zwraca liczbę zadań
    int liczbaMaszyn() const;    // Zwraca liczbę maszyn
};