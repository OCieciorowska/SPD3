#pragma once
#include "Zadanie.h"

class Rozwiazania {
public:
    // Oblicza Cmax dla danej permutacji zadań
    static int calculateMakespan(const vector<int>& perm, const Matrix& times, int m);

    // Algorytm przeglądu zupełnego (brute-force)
    static int bruteForceMakespan(const Zadanie& z);
    //Algorytm NEH
    pair<vector<int>, int> neh(const Matrix& czasy);


    // Algorytm Johnsona (dla 2 maszyn)
    static vector<int> johnsonAlgorithm(const Zadanie& z);

    //FNEH
    std::pair<std::vector<int>, int> fneh(const Matrix& czasy);

};
