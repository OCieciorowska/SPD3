#include <iostream>
#include "Zadanie.h"
#include "Rozwiazania.h"

int main() {

    // Przykładowa macierz czasów: 4 zadania, 2 maszyny
    Matrix czasy = {
        {54, 79},  // zadanie 0
        {83, 3},  // zadanie 1
        {15, 11},  // zadanie 2
        {71, 99},
        {77, 56},
        {4, 79} 
   
    };

    Zadanie zadanie(czasy);

    Rozwiazania roz(zadanie);

    std::cout << "=== Johnson (m=2) ===\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto kolejnosc = roz.johnsonAlgorithm(zadanie);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Kolejnosc: ";
    for (int j : kolejnosc)
        std::cout << j << " ";
    std::cout << "\nCmax = " << roz.calculateMakespan(kolejnosc, zadanie.czasy, 2) << " (Czas: " << duration.count() << " μs)" <<std::endl;

    std::cout << "\n=== Brute-force ===\n";
      start = std::chrono::high_resolution_clock::now();
    int cmax = roz.bruteForceMakespan(zadanie);
    end = std::chrono::high_resolution_clock::now();
     duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Najlepsze Cmax = " << cmax << " (Czas: " << duration.count() << " μs)" << std::endl <<std::endl;


    std::cout << "\n=== NEH ===\n" << std::endl;
    start= std::chrono::high_resolution_clock::now();
     auto wynik = roz.neh(czasy);
     end = std::chrono::high_resolution_clock::now();
      duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Kolejnosc: ";
    for (int zad : wynik.first)
        std::cout << zad << " ";
    std::cout << std::endl;
    std::cout << "Cmax = " << wynik.second << " (Czas: " << duration.count() << " μs)"<< std::endl;


    std::cout << "\n=== FNEH ===" << std::endl;
    start= std::chrono::high_resolution_clock::now();
    auto wynik_fneh = roz.fneh(czasy);
     end = std::chrono::high_resolution_clock::now();
     duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Kolejnosc: ";
    for (int zad : wynik_fneh.first)
        std::cout << zad << " ";
    std::cout << "\nCmax = " << wynik_fneh.second << " (Czas: " << duration.count() << " μs)"<< std::endl;

    return 0;
}
