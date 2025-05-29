#pragma once
#include "Zadanie.h"


struct Node {
    vector<int> path;       // czesc zadan dodanych do szeregowania
    vector<bool> assigned;  // ktore zadania juz sa uszeregowane
    int level;             // obecna pozycja zadania (dodanego?)
    int LB;              // Lower bound of the partial schedule
    
    // Constructor
    Node(int n) : assigned(n, false), level(0), LB(0) {}
    
    // For priority queue (min-heap based on cost)
    bool operator>(const Node& other) const {
        return LB > other.LB;
    }
};

class Rozwiazania {
    const Zadanie &z;

public:
    Rozwiazania(Zadanie &z) : z(z){};
    // Oblicza Cmax dla danej permutacji zadań
    static int calculateMakespan(const vector<int>& perm, const Matrix& times, int m);

    // Algorytm przeglądu zupełnego (brute-force)
    int bruteForceMakespan();
    //Algorytm NEH
    pair<vector<int>, int> neh();


    // Algorytm Johnsona (dla 2 maszyn)
    //vector<int> johnsonAlgorithm();
   std::pair<std::vector<int>, int> johnsonAlgorithm();

    //FNEH
    std::pair<std::vector<int>, int> fneh();

    //bound and branch algorithm
    std::pair<std::vector<int>, int> bound();

    //oblicznie LB dla Bound
    int obliczLB(const Node& node, int m);
};
