#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Zadanie.h"
#include "Rozwiazania.h"



void readFile(Matrix &czasy, const std::string& filename)
{
    ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        return;
    }
    std::string line;

    getline(file, line);
    int n, m;
    istringstream header(line);
    header >> n >> m;

    for (int i = 0; i < n; ++i) {
        getline(file, line);
        istringstream iss(line);
        vector<int> czasZadania;
        int czas;
        
        while (iss >> czas) {
            czasZadania.push_back(czas);
        }
        
        czasy.push_back(czasZadania);
    }
    file.close();
    
}
/*
int main() {

    //Przykładowa macierz czasów

    // Matrix czasy = {
    //     {54, 79},  // zadanie 0
    //     {83, 3},  // zadanie 1
    //     {15, 11},  // zadanie 2
    //     {71, 99},
    //     {77, 56},
    //     {4, 79} 
   
    // };

    Matrix czasy;

    readFile(czasy, "test_daneGosia.txt");

    Zadanie zadanie(czasy);

    Rozwiazania roz(zadanie);

    std::cout << "=== Johnson (m=2) ===\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto wynik_john = roz.johnsonAlgorithm();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Kolejnosc: ";
    for (int j : wynik_john.first)
        std::cout << j << " ";
    std::cout << "\nCmax = " << wynik_john.second << " (Czas: " << duration.count() << " μs)" <<std::endl;

    std::cout << "\n=== Brute-force ===\n";
    start = std::chrono::high_resolution_clock::now();
    int cmax = roz.bruteForceMakespan();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Najlepsze Cmax = " << cmax << " (Czas: " << duration.count() << " μs)" << std::endl <<std::endl;


    std::cout << "\n=== NEH ===\n" << std::endl;
    start= std::chrono::high_resolution_clock::now();
    auto wynik = roz.neh();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Kolejnosc: ";
    for (int zad : wynik.first)
        std::cout << zad << " ";
    std::cout << std::endl;
    std::cout << "Cmax = " << wynik.second << " (Czas: " << duration.count() << " μs)"<< std::endl;


    std::cout << "\n=== FNEH ===" << std::endl;
    start= std::chrono::high_resolution_clock::now();
    auto wynik_fneh = roz.fneh();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Kolejnosc: ";
    for (int zad : wynik_fneh.first)
        std::cout << zad << " ";
    std::cout << "\nCmax = " << wynik_fneh.second << " (Czas: " << duration.count() << " μs)"<< std::endl;


    std::cout << "\n=== BOUND and BRANCH ===" << std::endl;
    start= std::chrono::high_resolution_clock::now();
    auto wynik_bb = roz.bound();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Kolejnosc: ";
    for (int zad : wynik_bb.first)
        std::cout << zad << " ";
    std::cout << "\nCmax = " << wynik_bb.second << " (Czas: " << duration.count() << " μs)"<< std::endl;

    return 0;
}
*/
int main() {
    Matrix czasy;
    readFile(czasy, "test.txt");
  
    Zadanie zadanie(czasy);
    Rozwiazania roz(zadanie);
/*
    
    std::cout << "\n--- Algorytm brute-force (dokładny) ---\n";
    auto start_bf = std::chrono::high_resolution_clock::now();
    int cmax = roz.bruteForceMakespan();  // C_opt
    auto end_bf = std::chrono::high_resolution_clock::now();
    auto duration_bf = std::chrono::duration_cast<std::chrono::microseconds>(end_bf - start_bf);
    std::cout << "Brute force Cmax = " << cmax << " (Czas: " << duration_bf.count() << " μs)\n";
*/
    std::cout << "\n--- Algorytm NEH ---\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto wynik = roz.neh();  // C_heur
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //double rpd_neh = ((wynik.second - cmax) / (double)cmax) * 100.0;
    std::cout << "NEH Cmax = " << wynik.second << " (Czas: " << duration.count() << " μs)\n";
   // std::cout << "Błąd RPD = " << rpd_neh << " %\n";

    std::cout << "\n--- Algorytm FNEH ---\n";
    start = std::chrono::high_resolution_clock::now();
    auto wynik_fneh = roz.fneh();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    //double rpd_fneh = ((wynik_fneh.second - cmax) / (double)cmax) * 100.0;
    std::cout << "FNEH Cmax = " << wynik_fneh.second << " (Czas: " << duration.count() << " μs)\n";
   // std::cout << "Błąd RPD = " << rpd_fneh << " %\n";

    std::cout << "\n--- Algorytm Johnsona ---\n";
    start = std::chrono::high_resolution_clock::now();
    auto wynik_john = roz.johnsonAlgorithm();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
   // double rpd_john = ((wynik_john.second - cmax) / (double)cmax) * 100.0;
    std::cout << "Johnson Cmax = " << wynik_john.second << " (Czas: " << duration.count() << " μs)\n";
    //std::cout << "Błąd RPD = " << rpd_john << " %\n";


/*
    std::cout << "\n=== BOUND and BRANCH ===" << std::endl;
    start= std::chrono::high_resolution_clock::now();
    auto wynik_bb = roz.bound();
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double rpd_bound = ((wynik_bb.second - cmax) / (double)cmax) * 100.0;
    std::cout << "Kolejnosc: ";
    for (int zad : wynik_bb.first)
        std::cout << zad << " ";
    std::cout << "\nCmax = " << wynik_bb.second << " (Czas: " << duration.count() << " μs)"<< std::endl;
    std::cout << "Błąd RPD = " << rpd_bound << " %\n";
*/

    return 0;
}

