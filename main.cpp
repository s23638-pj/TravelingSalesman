#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "tsp.h"

using namespace std;

void displayRoute(const vector<int>& cities, const vector<string>& cityNames) {
    for (size_t i = 0; i < cities.size(); ++i) {
        if (cities[i] < 0 || cities[i] >= cityNames.size()) {
            cerr << "Błąd: Indeks miasta poza zakresem: " << cities[i] << endl;
            exit(1);
        }
        cout << cityNames[cities[i]];
        if (i + 1 < cities.size()) {
            cout << " -> ";
        } else {
            cout << " -> " << cityNames[cities[0]]; // Wraca do pierwszego miasta
        }
    }
    cout << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Użycie: " << argv[0] << " <plik_z_danymi> [-tabu <rozmiar_tabu>]" << endl;
        return 1;
    }

    string filename = argv[1];
    int tabuSize = 10; // Domyślny rozmiar listy tabu to 10

    // Przetwarzanie argumentów linii komend
    for (int i = 2; i < argc; ++i) {
        if (string(argv[i]) == "-tabu" && i + 1 < argc) {
            tabuSize = atoi(argv[++i]);
        }
    }

    pair<vector<string>, vector<vector<double>>> data;
    if (filename.substr(filename.find_last_of(".") + 1) == "csv") {
        data = read_csv(filename);
    } else {
        cerr << "Nieobsługiwany format pliku: " << filename << endl;
        return 1;
    }

    auto [cityNames, distanceMatrix] = data;

    // Generowanie losowego rozwiązania
    Route initialRoute = generate_random_solution(distanceMatrix.size(), distanceMatrix);

    cout << "Losowa trasa:\n";
    displayRoute(initialRoute.cities, cityNames);
    cout << "Koszt: " << initialRoute.cost << " km" << endl;

    // Algorytm wspinaczkowy
    Route hillClimbingRoute = solve_hill_climbing(distanceMatrix);
    cout << "\nTrasa po algorytmie wspinaczkowym:\n";
    displayRoute(hillClimbingRoute.cities, cityNames);
    cout << "Koszt: " << hillClimbingRoute.cost << " km" << endl;

    // Algorytm pełnego przeglądu
    Route bestRoute = solve_tsp(distanceMatrix);
    cout << "\nTrasa po algorytmie pełnego przeglądu:\n";
    displayRoute(bestRoute.cities, cityNames);
    cout << "Koszt: " << bestRoute.cost << " km" << endl;

    // Algorytm Tabu Search
    Route tabuRoute = solve_tabu(distanceMatrix, tabuSize);
    cout << "\nTrasa po algorytmie Tabu (rozmiar tabu: " << tabuSize << "):\n";
    displayRoute(tabuRoute.cities, cityNames);
    cout << "Koszt: " << tabuRoute.cost << " km" << endl;

    return 0;
}