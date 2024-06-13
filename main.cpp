#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "tsp.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Użycie: " << argv[0] << " <plik_z_danymi>" << endl;
        return 1;
    }

    string filename = argv[1];
    pair<vector<string>, vector<vector<double>>> data;
    if (filename.substr(filename.find_last_of(".") + 1) == "csv") {
        data = read_csv(filename);
    } else {
        cerr << "Nieobsługiwany format pliku: " << filename << endl;
        return 1;
    }

    auto [cityNames, distanceMatrix] = data;
    int numberOfCities = distanceMatrix.size();

    // Generowanie losowego rozwiązania
    Route initialRoute = generate_random_solution(numberOfCities, distanceMatrix);

    cout << "Początkowa trasa:\n";
    for (int city : initialRoute.cities) {
        cout << cityNames[city] << " ";
    }
    cout << "\nKoszt: " << initialRoute.cost << endl;

    // Algorytm wspinaczkowy
    Route hillClimbingRoute = solve_hill_climbing(distanceMatrix);
    cout << "Trasa po algorytmie wspinaczkowym:\n";
    for (int city : hillClimbingRoute.cities) {
        cout << cityNames[city] << " ";
    }
    cout << "\nKoszt: " << hillClimbingRoute.cost << endl;

    // Algorytm pełnego przeglądu
    Route bestRoute = check_tsp(distanceMatrix);
    cout << "Najlepsza trasa (pełny przegląd):\n";
    for (int city : bestRoute.cities) {
        cout << cityNames[city] << " ";
    }
    cout << "\nKoszt: " << bestRoute.cost << endl;

    return 0;
}
