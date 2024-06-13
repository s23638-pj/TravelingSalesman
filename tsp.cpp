#include "tsp.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <limits>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

// Funkcja celu
double check_cost(const vector<int>& route, const vector<vector<double>>& distanceMatrix) {
    double totalCost = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        totalCost += distanceMatrix[route[i]][route[i + 1]];
    }
    totalCost += distanceMatrix[route.back()][route.front()];
    return totalCost;
}

// Funkcja generująca sąsiedztwo
vector<Route> generate_neighborhood(const Route& currentRoute, const vector<vector<double>>& distanceMatrix) {
    vector<Route> neighborhood;
    for (size_t i = 1; i < currentRoute.cities.size() - 1; ++i) {
        for (size_t j = i + 1; j < currentRoute.cities.size(); ++j) {
            Route newRoute = currentRoute;
            swap(newRoute.cities[i], newRoute.cities[j]);
            newRoute.cost = check_cost(newRoute.cities, distanceMatrix);
            neighborhood.push_back(newRoute);
        }
    }
    return neighborhood;
}

// Funkcja generująca losowe rozwiązanie
Route generate_random_solution(int numberOfCities, const vector<vector<double>>& distanceMatrix) {
    Route randomRoute;
    randomRoute.cities.resize(numberOfCities);
    iota(randomRoute.cities.begin(), randomRoute.cities.end(), 0);
    random_device rd;
    mt19937 g(rd());
    shuffle(randomRoute.cities.begin(), randomRoute.cities.end(), g);
    randomRoute.cost = check_cost(randomRoute.cities, distanceMatrix);
    return randomRoute;
}

// Algorytm pełnego przeglądu
Route check_tsp(const vector<vector<double>>& distanceMatrix) {
    int numberOfCities = distanceMatrix.size();
    vector<int> cities(numberOfCities);
    iota(cities.begin(), cities.end(), 0);

    Route bestRoute;
    bestRoute.cost = numeric_limits<double>::infinity();

    // Generowanie wszystkich możliwych permutacji miast
    do {
        double currentCost = check_cost(cities, distanceMatrix);
        if (currentCost < bestRoute.cost) {
            bestRoute.cities = cities;
            bestRoute.cost = currentCost;
        }
    } while (next_permutation(cities.begin(), cities.end()));

    return bestRoute;
}

// Algorytm wspinaczkowy
Route solve_hill_climbing(const vector<vector<double>>& distanceMatrix) {
    int numberOfCities = distanceMatrix.size();
    Route currentRoute = generate_random_solution(numberOfCities, distanceMatrix);
    bool improvement = true;

    while (improvement) {
        improvement = false;
        auto neighborhood = generate_neighborhood(currentRoute, distanceMatrix);
        for (const auto& neighbor : neighborhood) {
            if (neighbor.cost < currentRoute.cost) {
                currentRoute = neighbor;
                improvement = true;
            }
        }
    }

    return currentRoute;
}

// Funkcja wczytująca dane z pliku CSV
pair<vector<string>, vector<vector<double>>> read_csv(const string& filename) {
    vector<vector<double>> matrix;
    vector<string> cityNames;
    ifstream file(filename);
    if (!file) {
        cerr << "Nie można otworzyć pliku: " << filename << endl;
        exit(1);
    }

    string line;
    // Wczytaj nazwy miast z pierwszej linii
    if (getline(file, line)) {
        istringstream iss(line);
        string cityName;
        while (getline(iss, cityName, ';')) {
            if (!cityName.empty()) {
                cityNames.push_back(cityName);
            }
        }
        cityNames.erase(cityNames.begin()); // Usuń pierwszy pusty element
    }

    // Wczytaj macierz odległości
    while (getline(file, line)) {
        istringstream iss(line);
        string cityName;
        getline(iss, cityName, ';'); // Pomijamy nazwę miasta w wierszu
        vector<double> row;
        string value;
        while (getline(iss, value, ';')) {
            if (!value.empty()) {
                row.push_back(stod(value));
            }
        }
        matrix.push_back(row);
    }

    if (matrix.empty()) {
        cerr << "Plik " << filename << " jest pusty lub niepoprawny." << endl;
        exit(1);
    }

    return {cityNames, matrix};
}
