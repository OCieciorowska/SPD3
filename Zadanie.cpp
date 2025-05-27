#include "Zadanie.h"

// Konstruktor inicjalizujący macierz czasów
Zadanie::Zadanie(const Matrix& czasy) : czasy(czasy) {}

// Liczba zadań = liczba wierszy
int Zadanie::liczbaZadan() const {
    return static_cast<int>(czasy.size());
}

// Liczba maszyn = liczba kolumn
int Zadanie::liczbaMaszyn() const {
    return static_cast<int>(czasy[0].size());
}
