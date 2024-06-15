#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include "tsp.h"

using namespace std;
using namespace std::chrono;

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

    string filename = argv[1];
    int tabuSize = 10;
    int maxIterations = 1000;

    // Przetwarzanie argumentów linii komend
    for (int i = 2; i < argc; ++i) {
        if (string(argv[i]) == "-tabu" && i + 1 < argc) {
            tabuSize = atoi(argv[++i]);
        } else if (string(argv[i]) == "-i" && i + 1 < argc) {
            maxIterations = atoi(argv[++i]);
        }
    }

    pair<vector<string>, vector<vector<double> >> data;
    data = read_csv(filename);

    auto [cityNames, distanceMatrix] = data;

    // Generowanie losowego rozwiązania
    auto start_random = high_resolution_clock::now();
    Route initialRoute = generate_random_solution(distanceMatrix.size(), distanceMatrix);
    auto end_random = high_resolution_clock::now();
    duration<double, milli> duration_random = end_random - start_random;

    cout << "Losowa trasa:\n";
    displayRoute(initialRoute.cities, cityNames);
    cout << "Koszt: " << initialRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_random.count() << " ms" << endl;

    // Algorytm wspinaczkowy
    int hill_climbing_iterations;
    auto start_hill = high_resolution_clock::now();
    Route hillClimbingRoute = solve_hill_climbing(distanceMatrix, maxIterations, hill_climbing_iterations);
    auto end_hill = high_resolution_clock::now();
    duration<double, milli> duration_hill = end_hill - start_hill;

    cout << "\nTrasa po algorytmie wspinaczkowym:\n";
    displayRoute(hillClimbingRoute.cities, cityNames);
    cout << "Koszt: " << hillClimbingRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_hill.count() << " ms" << endl;
    cout << "Liczba iteracji: " << hill_climbing_iterations << endl;

    // Algorytm Tabu Search
    int tabu_iterations;
    auto start_tabu = high_resolution_clock::now();
    Route tabuRoute = solve_tabu(distanceMatrix, tabuSize, maxIterations, tabu_iterations);
    auto end_tabu = high_resolution_clock::now();
    duration<double, milli> duration_tabu = end_tabu - start_tabu;

    cout << "\nTrasa po algorytmie Tabu (rozmiar tabu: " << tabuSize << "):\n";
    displayRoute(tabuRoute.cities, cityNames);
    cout << "Koszt: " << tabuRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_tabu.count() << " ms" << endl;
    cout << "Liczba iteracji: " << tabu_iterations << endl;

    // Algorytm pełnego przeglądu
    int tsp_iterations;
    auto start_tsp = high_resolution_clock::now();
    Route bestRoute = solve_full_review(distanceMatrix, maxIterations, tsp_iterations);
    auto end_tsp = high_resolution_clock::now();
    duration<double, milli> duration_tsp = end_tsp - start_tsp;

    cout << "\nTrasa po algorytmie pełnego przeglądu:\n";
    displayRoute(bestRoute.cities, cityNames);
    cout << "Koszt: " << bestRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_tsp.count() << " ms" << endl;
    cout << "Liczba iteracji: " << tsp_iterations << endl;

    return 0;
}
