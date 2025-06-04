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

// Algorytm Johnsona dla dwóch maszyn
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

    std::vector<int> wynik;

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


    wynik.push_back(sumaIndex[0].second); // Pierwsze zadanie w kolejności

    std::vector<std::vector<std::pair<int, int>>> czasy; //tablica czasow (first -> przod, second -> w tyl)

    czasy.resize(1); // [pozycja zadania][maszyna]
    czasy[0].resize(m);

    //uzupelnienie pierwszego zadania w przod
    for (int i = 0; i<m; i++){
        if(i==0) czasy[0][i] = std::make_pair(z.czasy[sumaIndex[0].second][i], 0);
        else czasy[0][i] = std::make_pair(z.czasy[sumaIndex[0].second][i] + czasy[0][i-1].first, 0);
    }

    //uzupelnienie pierwszego zadania w tyl
    for(int i = m-1; i>=0; i--){
        if(i==m-1) czasy[0][i].second = z.czasy[sumaIndex[0].second][i];
        else czasy[0][i].second = z.czasy[sumaIndex[0].second][i] + czasy[0][i+1].second;
    }

    for( int p = 1; p<n; p++){ // p -> kolejny indeks z sumaIndex
        int zadanie = sumaIndex[p].second; // indeks nowego zadania
        int C = INT16_MAX;
        int poz = 0; //pozycja w ktorej dodamy nowe zadanie
        std::vector<std::pair<int,int>> czasyNaj(m); //czasy dla nowego zadania, na najlepszej pozycji

        for(int k = 0; k<wynik.size()+1; k++){ // k -> pozycja w ktorej dodamy nowe zadanie
            std::vector<std::pair<int, int>> czasyNowe(m); //czasy dla nowego zadania
            int Cnowe=0;
            //przeliczmy w przod
            if(k==wynik.size()){ //dodajemy na koniec -> cmax = max(suma pj + auktualne pj), nie trzeba liczyc w tyl
                for(int i = 0; i<m; i++){
                    if(i==0) czasyNowe[i].first = czasy[k-1][i].first + z.czasy[sumaIndex[p].second][i];
                    else czasyNowe[i].first = std::max(czasy[k-1][i].first, czasyNowe[i-1].first) + z.czasy[sumaIndex[p].second][i];
                    Cnowe = std::max(Cnowe, czasyNowe[i].first);
                }
            }
            else{
                //liczymy w przod
                for(int i = 0; i<m; i++){
                    if(i==0) czasyNowe[i].first = (k==0 ? 0 : czasy[k-1][i].first) + z.czasy[sumaIndex[p].second][i]; //jesli jest to pierwsze zadanie to tylko czas zadania, inaczej czas skonczenia poprzedniego plus obecne zadanie
                    else czasyNowe[i].first = std::max(czasyNowe[i-1].first, (k==0 ? 0 : czasy[k-1][i].first)) + z.czasy[sumaIndex[p].second][i]; //maks z poprzedniego zadania i zadania na poprzedniej maszynie (jesli na pozycji 0 -> czas skonczenia zadania na poprzedniej maszynie)
                }

                //obliczmy cmax
                for(int i = 0; i<m; i++){
                    if(k==wynik.size()) Cnowe = std::max(Cnowe, czasyNowe[i].first); //na koncu -> najwiekszy czas skonczenia zadania
                    else Cnowe = std::max(Cnowe, czasy[k][i].second + czasyNowe[i].first); //wartosc w tyl dla maszyny poprzedniej i w przod dla maszyny obliczonej
                }
            }
            if(Cnowe < C){ //jesli mamy lepszy Cmax, zapamietujemy i zapamietujemy pozycje
                C = Cnowe;
                poz = k;
                czasyNaj = czasyNowe; //zapamietujemy czasy dla nowego zadania, na najlepszej pozycji
            }
        }
        //wstawiamy zadanie do wyniku na poz i przeliczmy czasy ogolne
        wynik.emplace(wynik.begin() + poz, zadanie);
        czasy.emplace(czasy.begin() + poz, czasyNaj);
        //przeliczamy czasy w przod od poz+1 do konca
        for(int k = poz +1 ; k<wynik.size(); k++){
            for(int i = 0; i<m; i++){
                // if(k==0 && i==0) czasy[k][i].first = z.czasy[wynik[k]][i]; //jesli pierwsze zadanie na pierwszej maszynie, bierzemy wynik w ciemno (jak nie zadziala to odkomentujemy i wrzucamy od poz)
                // else if(k==0) czasy[k][i].first = czasy[k][i-1].first + z.czasy[wynik[k]][i];
                 if(i==0) czasy[k][i].first = czasy[k-1][i].first + z.czasy[wynik[k]][i]; //pierwsza maszyna, czas skonczenia poprzedniego zadania na tej maszynie + czas obecnego
                else czasy[k][i].first = std::max(czasy[k-1][i].first, czasy[k][i-1].first) + z.czasy[wynik[k]][i]; //maks z poprzedniego zadania i zadania na poprzedniej maszyny
            }
        }

        //przeliczamy czasy w tyl od poz do 0
        for(int k = poz; k>=0; k--){
            for(int i = m-1; i>=0; i--){
                if(k == czasy.size()-1 && i == m-1) czasy[k][i].second = z.czasy[wynik[k]][i];
                else if(k == czasy.size()-1) czasy[k][i].second = z.czasy[wynik[k]][i] + czasy[k][i+1].second;
                else if(i == m-1) czasy[k][i].second = z.czasy[wynik[k]][i] + czasy[k+1][i].second; 
                else czasy[k][i].second = z.czasy[wynik[k]][i] + std::max(czasy[k+1][i].second, czasy[k][i+1].second);
            }
        }
    }

    return std::make_pair(wynik, calculateMakespan(wynik, z.czasy, m)); //zwracamy kolejność i Cmax
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
        Node obecny = queue.top(); //rozpatrujemy najlepsze LB
        queue.pop(); //wywalamy z kolejki

        if (obecny.LB >= UB) {
            continue; //jesli mamy lepszy wynik to robimy dalej -> bierzemy kolejny node
        }

        if (obecny.level == n) {  //jesli damy wszystkie zadania to liczymy Cmax
            int currentMakespan = calculateMakespan(obecny.path,z.czasy, m);
            if (currentMakespan < UB) { //jesli jest lepsze niz UB to updatujemy
                UB = currentMakespan;
                najlepszeKol = obecny.path;
            }
            continue; //kolejny node sprawdzamy
        }

        for (int j = 0; j < n; j++) { //dodajemy kolejne elementy -> kolejne węzły
            if (!obecny.assigned[j]) {
                Node child = obecny;
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