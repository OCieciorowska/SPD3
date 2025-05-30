#include "Rozwiazania.h"
#include <algorithm>
#include <limits>
#include <deque>
#include <queue>

// Obliczanie wartości Cmax dla danej permutacji zadań
//kade zadanie kończy sie na kazdej maszynie, czas ostaniego zadnia na ostaniej maszynie i zachowuje permutacje z najmniejszym cmax
int Rozwiazania::calculateMakespan(const vector<int>& perm, const Matrix& times, int m) {
    int n = perm.size();
    vector<vector<int> > C(n, vector<int>(m, 0)); // tablica czasów zakończenia

    for (int i = 0; i < n; ++i) {
        int job = perm[i];
        for (int j = 0; j < m; ++j) {
            if (i == 0 && j == 0)
                C[i][j] = times[job][j]; // pierwsze zadanie na pierwszej maszynie i to maszyna, j to pozycja zadania w permutacji
            else if (i == 0)
                C[i][j] = C[i][j - 1] + times[job][j]; // pierwsze zadanie na kolejnej maszynie
            else if (j == 0)
                C[i][j] = C[i - 1][j] + times[job][j]; // kolejne zadanie na pierwszej maszynie
            else
                C[i][j] = max(C[i - 1][j], C[i][j - 1]) + times[job][j]; // reszta
        }
    }

    return C[n - 1][m - 1]; // czas zakończenia ostatniego zadania na ostatniej maszynie
}

// Brute-force: przegląd wszystkich permutacji
int Rozwiazania::bruteForceMakespan() {
    int n = z.liczbaZadan();
    int m = z.liczbaMaszyn();
    vector<int> perm(n);
    for (int i = 0; i < n; ++i)
        perm[i] = i;

    int bestCmax = numeric_limits<int>::max();
    do {
        int cmax = calculateMakespan(perm, z.czasy, m);
        bestCmax = min(bestCmax, cmax); // aktualizacja najlepszego Cmax
    } while (next_permutation(perm.begin(), perm.end()));

    return bestCmax;
}


//NEH 
std::pair<std::vector<int>, int> Rozwiazania::neh() {
    int n = z.liczbaZadan();
    int m = z.liczbaMaszyn();

    std::vector<std::pair<int, int> > sumaIndex;
    for (int i = 0; i < n; ++i) {
        int suma = 0;
        for (int j = 0; j < m; ++j) {
            suma += z.czasy[i][j];
        }
        sumaIndex.push_back({-suma, i});
    }

    std::sort(sumaIndex.begin(), sumaIndex.end());

    std::vector<int> kolejnosc;

    for (int k = 0; k < n; ++k) {
        int zadanie = sumaIndex[k].second;
        std::vector<int> najlepsza;
        int minCmax = std::numeric_limits<int>::max();

        for (size_t pos = 0; pos <= kolejnosc.size(); ++pos) {
            std::vector<int> tymczasowa = kolejnosc;
            tymczasowa.insert(tymczasowa.begin() + pos, zadanie);

            Matrix C(tymczasowa.size(), std::vector<int>(m, 0));
            for (size_t i = 0; i < tymczasowa.size(); ++i) {
                for (int j = 0; j < m; ++j) {
                    int czas = z.czasy[tymczasowa[i]][j];
                    if (i == 0 && j == 0)
                        C[i][j] = czas;
                    else if (i == 0)
                        C[i][j] = C[i][j - 1] + czas;
                    else if (j == 0)
                        C[i][j] = C[i - 1][j] + czas;
                    else
                        C[i][j] = std::max(C[i - 1][j], C[i][j - 1]) + czas;
                }
            }

            int cmax = C[tymczasowa.size() - 1][m - 1];
            if (cmax < minCmax) {
                minCmax = cmax;
                najlepsza = tymczasowa;
            }
        }
        kolejnosc = najlepsza;
    }

    int finalCmax = 0;
    {
        Matrix C(kolejnosc.size(), std::vector<int>(m, 0));
        for (size_t i = 0; i < kolejnosc.size(); ++i) {
            for (int j = 0; j < m; ++j) {
                int czas = z.czasy[kolejnosc[i]][j];
                if (i == 0 && j == 0)
                    C[i][j] = czas;
                else if (i == 0)
                    C[i][j] = C[i][j - 1] + czas;
                else if (j == 0)
                    C[i][j] = C[i - 1][j] + czas;
                else
                    C[i][j] = std::max(C[i - 1][j], C[i][j - 1]) + czas;
            }
        }
        finalCmax = C[kolejnosc.size() - 1][m - 1];
    }

    return {kolejnosc, finalCmax};
}


// Algorytm Johnsona (dla 2 maszyn)
// vector<int> Rozwiazania::johnsonAlgorithm() {
//     int n = z.liczbaZadan();
//     const Matrix& times = z.czasy;
//     vector<bool> used(n, false); // które zadania już dodano
//     deque<int> sequence;         // kolejność wynikowa

//     while (sequence.size() < n) {
//         int minTime = numeric_limits<int>::max();
//         int selectedJob = -1;
//         bool firstMachine = true;

//         // Szukanie zadania z minimalnym czasem na maszynie 1 lub 2
//         for (int j = 0; j < n; ++j) {
//             if (used[j]) continue;
//             if (min(times[j][0], times[j][1]) < minTime) {
//                 minTime = min(times[j][0], times[j][1]);
//                 selectedJob = j;
//                 firstMachine = (times[j][0] <= times[j][1]);
//             }
//         }

//         // Dodajemy zadanie na początek lub koniec kolejki
//         if (firstMachine)
//             sequence.push_front(selectedJob);
//         else
//             sequence.push_back(selectedJob);

//         used[selectedJob] = true;
//     }

//     return vector<int>(sequence.begin(), sequence.end());
// }
// Algorytm Johnsona (dla 2 maszyn)
std::pair<std::vector<int>, int> Rozwiazania::johnsonAlgorithm(){
    vector<int> gr1, gr2;
    int n = z.liczbaZadan();

    for(int i = 0; i < n; ++i) {
        if (z.czasy[i][0] < z.czasy[i][1]) {
            gr1.push_back(i); // Zadanie do pierwszej grupy (czas na m1 < czas na m2)
        } else {
            gr2.push_back(i); // Zadanie do drugiej grupy (czas na m1 >= czas na m2)
        }
    }

    // Sortowanie grup
    sort(gr1.begin(), gr1.end(), [this](int a, int b) {
        return z.czasy[a][0] < z.czasy[b][0]; // Sortowanie rosnąco po czasie na m1
    });

    sort(gr2.begin(), gr2.end(), [this](int a, int b) {
        return z.czasy[a][1] > z.czasy[b][1]; // Sortowanie malejąco po czasie na m2
    });

    // Łączenie grup
    vector<int> kolejnosc;
    for (auto zadanie : gr1) kolejnosc.push_back(zadanie);
    for (auto zadanie : gr2) kolejnosc.push_back(zadanie);

    return {kolejnosc, calculateMakespan(kolejnosc, z.czasy, z.liczbaMaszyn())};
}



//FNEH
//Nie oblicza za kadym razem makespan tylko wykorzystuje wczesniej obliczone i aktualizuje tylko te co uległy zmianie

std::pair<std::vector<int>, int> Rozwiazania::fneh() {
    int n = z.liczbaZadan();
    int m = z.liczbaMaszyn();

    //Sortowanie zadań  malejaco dla sumy czasów wykonania
    std::vector<std::pair<int, int> > sumaIndex;
    for (int i = 0; i < n; ++i) {
        int suma = 0;
        for (int j = 0; j < m; ++j) {
            suma += z.czasy[i][j];
        }
        sumaIndex.push_back({-suma, i}); // Używamy ujemnej wartości dla sortowania malejącego
    }
    std::sort(sumaIndex.begin(), sumaIndex.end());

    //pierwsze zadanie w kolejności
    std::vector<int> kolejnosc = {sumaIndex[0].second};
    
    //tablica czasów zakończenia dla 1. zadania
    std::vector<std::vector<int> > C(1, std::vector<int>(m, 0));
    for (int j = 0; j < m; ++j) {
        if (j == 0)
            C[0][j] = z.czasy[kolejnosc[0]][j];
        else
            C[0][j] = C[0][j-1] + z.czasy[kolejnosc[0]][j];
    }

    // Wstawianie kolejnych zadań w optymalne pozycje
    for (int k = 1; k < n; ++k) {
        int zadanie = sumaIndex[k].second;
        std::vector<int> najlepsza_kolejnosc;
        int minCmax = std::numeric_limits<int>::max();
        std::vector<std::vector<int> > najlepsza_C;

        // Próba wstawienia bieżącego zadania na wszystkie możliwe pozycje
        for (size_t pos = 0; pos <= kolejnosc.size(); ++pos) {
            // Tworzenie tymczasowej kolejności
            std::vector<int> tymczasowa_kolejnosc = kolejnosc;
            tymczasowa_kolejnosc.insert(tymczasowa_kolejnosc.begin() + pos, zadanie);

            // Obliczanie nowej tablicy C z akceleracją
            std::vector<std::vector<int> > nowa_C(tymczasowa_kolejnosc.size(), std::vector<int>(m, 0));
            
            // Kopiowanie wcześniejszych wartości dla zadań przed wstawionym
            for (size_t i = 0; i < pos; ++i) {
                for (int j = 0; j < m; ++j) {
                    nowa_C[i][j] = C[i][j];
                }
            }
            
            // Obliczanie dla wstawionego zadania
            for (int j = 0; j < m; ++j) {
                if (pos == 0 && j == 0)
                    nowa_C[pos][j] = z.czasy[zadanie][j];
                else if (pos == 0)
                    nowa_C[pos][j] = nowa_C[pos][j-1] + z.czasy[zadanie][j];
                else if (j == 0)
                    nowa_C[pos][j] = nowa_C[pos-1][j] + z.czasy[zadanie][j];
                else
                    nowa_C[pos][j] = std::max(nowa_C[pos][j-1], nowa_C[pos-1][j]) + z.czasy[zadanie][j];
            }
            
            // Obliczanie dla zadań po wstawionym
            for (size_t i = pos+1; i < tymczasowa_kolejnosc.size(); ++i) {
                for (int j = 0; j < m; ++j) {
                    if (j == 0)
                        nowa_C[i][j] = nowa_C[i-1][j] + z.czasy[tymczasowa_kolejnosc[i]][j];
                    else
                        nowa_C[i][j] = std::max(nowa_C[i][j-1], nowa_C[i-1][j]) + z.czasy[tymczasowa_kolejnosc[i]][j];
                }
            }
            
            int cmax = nowa_C.back().back();
            if (cmax < minCmax) {
                minCmax = cmax;
                najlepsza_kolejnosc = tymczasowa_kolejnosc;
                najlepsza_C = nowa_C;
            }
        }
        
        kolejnosc = najlepsza_kolejnosc;
        C = najlepsza_C;
    }

    return {kolejnosc, C.back().back()};
}


//oblicznie LB
int Rozwiazania::obliczLB(const Node& node, int m) //m -> ilosc maszyn
{
    vector<int> czasyKoniec(m, 0);
    //najpierw liczymy dla zadan uszeregowanych z przerwami
    for (int i = 0; i < node.level; i++) {
        int job = node.path[i];
        czasyKoniec[0] += z.czasy[job][0];
        for (int k = 1; k < m; k++) {
            czasyKoniec[k] = max(czasyKoniec[k], czasyKoniec[k-1]) + z.czasy[job][k];
        }
    }

    //doklejamy wszystkie zadania, zakladajac brak przerw, tylko dla ostatniej maszyny, wczesniejsze czasy zakonczenia nie maja znaczenia
    for (int j = 0; j < z.liczbaZadan(); j++) {
        if (!node.assigned[j]) {
            czasyKoniec[m-1] += z.czasy[j][m-1];
        }
    }

    return czasyKoniec[m-1];
}

std::pair<std::vector<int>, int> Rozwiazania::bound(){
    int n = z.liczbaZadan();
    int m = z.liczbaMaszyn();

    std::priority_queue<Node, vector<Node>, greater<Node>> queue; //min heap

    Node root(n); //tworzymy "korzen", brak zadan na maszynach
    root.LB = obliczLB(root, m); //liczymy najlepszy mozliwy Cmax
    queue.push(root); //dodajemy do kolejki -> namniejsze LB na gorze, czyli zaczynamy od najlepszych rozwiazan

    vector<int> najlepszeKol;
    int UB = neh().second;
    
    while (!queue.empty()) {
        Node current = queue.top(); //rozpatrujemy najlepsze LB
        queue.pop(); //wywalamy z kolejki

        if (current.LB >= UB) {
            continue; //jesli mamy lepszy wynik to robimy dalej -> bierzemy kolejny node
        }

        if (current.level == n) {  //jesli damy wszystkie zadania to liczymy Cmax
            int currentMakespan = calculateMakespan(current.path,z.czasy, m);
            if (currentMakespan < UB) { //jesli jest lepsze niz UB to updatujemy
                UB = currentMakespan;
                najlepszeKol = current.path;
            }
            continue; //kolejny node sprawdzamy
        }

        for (int j = 0; j < n; j++) { //dodajemy kolejne elementy -> kolejne węzły
            if (!current.assigned[j]) {
                Node child = current;
                child.assigned[j] = true;
                child.path.push_back(j);
                child.level++;
                child.LB = obliczLB(child, m);
            
                if (child.LB < UB) { //jesli nowy LB jest mniejszy od UB to dodajemy do kolejki
                    queue.push(child);
                }
            }
        }
    }

    return {najlepszeKol, UB};
}