#include <iostream>
#include "Zadanie.h"
#include "Rozwiazania.h"

int main() {
    Rozwiazania roz;

    // Przykładowa macierz czasów: 4 zadania, 2 maszyny
    Matrix czasy = {
        {5, 2},  // zadanie 0
        {3, 6},  // zadanie 1
        {7, 4},  // zadanie 2
        {4, 3}   // zadanie 3
    };

    Zadanie zadanie(czasy);

    std::cout << "=== Johnson (m=2) ===\n";
    auto kolejnosc = roz.johnsonAlgorithm(zadanie);
    std::cout << "Kolejnosc: ";
    for (int j : kolejnosc)
        std::cout << j << " ";
    std::cout << "\nCmax = " << roz.calculateMakespan(kolejnosc, zadanie.czasy, 2) << "\n";

    std::cout << "\n=== Brute-force ===\n";
    int cmax = roz.bruteForceMakespan(zadanie);
    std::cout << "Najlepsze Cmax = " << cmax << "\n";


    std::cout << "=== NEH ===" << std::endl;
     auto wynik = roz.neh(czasy);
    std::cout << "Kolejnosc: ";
    for (int zad : wynik.first)
        std::cout << zad << " ";
    std::cout << std::endl;
    std::cout << "Cmax = " << wynik.second << std::endl;


    std::cout << "\n=== FNEH ===" << std::endl;
    auto wynik_fneh = roz.fneh(czasy);
    std::cout << "Kolejnosc: ";
    for (int zad : wynik_fneh.first)
        std::cout << zad << " ";
    std::cout << "\nCmax = " << wynik_fneh.second << std::endl;

    return 0;
}
