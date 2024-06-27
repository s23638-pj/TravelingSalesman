#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <sys/resource.h> // for getrusage
#include "tsp.h"

using namespace std;
using namespace std::chrono;

// Funkcja do pobierania zużycia pamięci
long getCurrentMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

void displayRoute(const vector<int>& cities, const vector<string>& cityNames) {
    for (size_t i = 0; i < cities.size(); ++i) {
        cout << cityNames[cities[i]];
        if (i + 1 < cities.size()) {
            cout << " -> ";
        } else {
            cout << " -> " << cityNames[cities[0]];
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

    pair<vector<string>, vector<vector<double>>> data;
    data = read_csv(filename);

    auto [cityNames, distanceMatrix] = data;

    // Generowanie losowego rozwiązania
    auto start_random = high_resolution_clock::now();
    long mem_before_random = getCurrentMemoryUsage();
    Route initialRoute = generate_random_solution(distanceMatrix.size(), distanceMatrix);
    long mem_after_random = getCurrentMemoryUsage();
    auto end_random = high_resolution_clock::now();
    duration<double, milli> duration_random = end_random - start_random;

    cout << "Losowa trasa:\n";
    displayRoute(initialRoute.cities, cityNames);
    cout << "Koszt: " << initialRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_random.count() << " ms" << endl;
    cout << "Zużycie pamięci: " << (mem_after_random - mem_before_random) << " KB" << endl;

    // Algorytm wspinaczkowy
    int hill_climbing_iterations;
    auto start_hill = high_resolution_clock::now();
    long mem_before_hill = getCurrentMemoryUsage();
    Route hillClimbingRoute = solve_hill_climbing(distanceMatrix, maxIterations, hill_climbing_iterations);
    long mem_after_hill = getCurrentMemoryUsage();
    auto end_hill = high_resolution_clock::now();
    duration<double, milli> duration_hill = end_hill - start_hill;

    cout << "\nTrasa po algorytmie wspinaczkowym:\n";
    displayRoute(hillClimbingRoute.cities, cityNames);
    cout << "Koszt: " << hillClimbingRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_hill.count() << " ms" << endl;
    cout << "Liczba iteracji: " << hill_climbing_iterations << endl;
    cout << "Zużycie pamięci: " << (mem_after_hill - mem_before_hill) << " KB" << endl;

    // Algorytm wspinaczkowy z losowym wyborem sąsiada
    int random_hill_climbing_iterations;
    auto start_random_hill = high_resolution_clock::now();
    long mem_before_random_hill = getCurrentMemoryUsage();
    Route randomHillClimbingRoute = solve_random_hill_climbing(distanceMatrix, maxIterations, random_hill_climbing_iterations);
    long mem_after_random_hill = getCurrentMemoryUsage();
    auto end_random_hill = high_resolution_clock::now();
    duration<double, milli> duration_random_hill = end_random_hill - start_random_hill;

    cout << "\nTrasa po algorytmie wspinaczkowym z losowym wyborem sąsiada:\n";
    displayRoute(randomHillClimbingRoute.cities, cityNames);
    cout << "Koszt: " << randomHillClimbingRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_random_hill.count() << " ms" << endl;
    cout << "Liczba iteracji: " << random_hill_climbing_iterations << endl;
    cout << "Zużycie pamięci: " << (mem_after_random_hill - mem_before_random_hill) << " KB" << endl;

    // Algorytm Tabu Search
    int tabu_iterations;
    auto start_tabu = high_resolution_clock::now();
    long mem_before_tabu = getCurrentMemoryUsage();
    Route tabuRoute = solve_tabu(distanceMatrix, tabuSize, maxIterations, tabu_iterations);
    long mem_after_tabu = getCurrentMemoryUsage();
    auto end_tabu = high_resolution_clock::now();
    duration<double, milli> duration_tabu = end_tabu - start_tabu;

    cout << "\nTrasa po algorytmie Tabu (rozmiar tabu: " << tabuSize << "):\n";
    displayRoute(tabuRoute.cities, cityNames);
    cout << "Koszt: " << tabuRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_tabu.count() << " ms" << endl;
    cout << "Liczba iteracji: " << tabu_iterations << endl;
    cout << "Zużycie pamięci: " << (mem_after_tabu - mem_before_tabu) << " KB" << endl;

    // Algorytm wyżarzania
    int sa_iterations;
    auto T = [](int iteration) -> double { return 10000.0 / iteration; }; // Funkcja temperatury
    auto start_sa = high_resolution_clock::now();
    long mem_before_sa = getCurrentMemoryUsage();
    Route saRoute = solve_simulated_annealing(distanceMatrix, T, maxIterations, sa_iterations);
    long mem_after_sa = getCurrentMemoryUsage();
    auto end_sa = high_resolution_clock::now();
    duration<double, milli> duration_sa = end_sa - start_sa;

    cout << "\nTrasa po algorytmie wyżarzania:\n";
    displayRoute(saRoute.cities, cityNames);
    cout << "Koszt: " << saRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_sa.count() << " ms" << endl;
    cout << "Liczba iteracji: " << sa_iterations << endl;
    cout << "Zużycie pamięci: " << (mem_after_sa - mem_before_sa) << " KB" << endl;

    // Algorytm pełnego przeglądu
    int tsp_iterations;
    auto start_tsp = high_resolution_clock::now();
    long mem_before_tsp = getCurrentMemoryUsage();
    Route bestRoute = solve_full_review(distanceMatrix, tsp_iterations);
    long mem_after_tsp = getCurrentMemoryUsage();
    auto end_tsp = high_resolution_clock::now();
    duration<double, milli> duration_tsp = end_tsp - start_tsp;

    cout << "\nTrasa po algorytmie pełnego przeglądu:\n";
    displayRoute(bestRoute.cities, cityNames);
    cout << "Koszt: " << bestRoute.cost << " km" << endl;
    cout << "Czas wykonania: " << duration_tsp.count() << " ms" << endl;
    cout << "Liczba iteracji: " << tsp_iterations << endl;
    cout << "Zużycie pamięci: " << (mem_after_tsp - mem_before_tsp) << " KB" << endl;

    return 0;
}
