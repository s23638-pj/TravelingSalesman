#include "tsp.h"
#include <algorithm>
#include <numeric>
#include <random>
#include <limits>
#include <sstream>
#include <fstream>
#include <iostream>
#include <deque>
#include <unordered_set>
#include <list>
#include <set>
#include <cmath>

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
    for (int i = 0; i < numberOfCities; ++i) {
        randomRoute.cities[i] = i;
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(randomRoute.cities.begin(), randomRoute.cities.end(), g);
    randomRoute.cost = check_cost(randomRoute.cities, distanceMatrix);
    return randomRoute;
}

// Algorytm wspinaczkowy
Route solve_hill_climbing(const vector<vector<double>>& distanceMatrix, int maxIterations, int& iteration_count) {
    int numberOfCities = distanceMatrix.size();
    Route currentRoute = generate_random_solution(numberOfCities, distanceMatrix);
    bool improvement = true;
    iteration_count = 0;

    while (improvement && iteration_count < maxIterations) {
        improvement = false;
        auto neighborhood = generate_neighborhood(currentRoute, distanceMatrix);
        for (const auto& neighbor : neighborhood) {
            if (neighbor.cost < currentRoute.cost) {
                currentRoute = neighbor;
                improvement = true;
            }
        }
        iteration_count++;
    }

    return currentRoute;
}

// Algorytm wspinaczkowy z losowym wyborem sąsiada
Route solve_random_hill_climbing(const vector<vector<double>>& distanceMatrix, int maxIterations, int& iteration_count) {
    int numberOfCities = distanceMatrix.size();
    Route currentRoute = generate_random_solution(numberOfCities, distanceMatrix);

    random_device rd;
    mt19937 rgen(rd());
    iteration_count = 0;

    for (int i = 0; i < maxIterations; ++i) {
        auto neighborhood = generate_neighborhood(currentRoute, distanceMatrix);
        uniform_int_distribution<int> dist(0, neighborhood.size() - 1);
        Route newRoute = neighborhood[dist(rgen)];

        if (newRoute.cost < currentRoute.cost) {
            currentRoute = newRoute;
        }
        iteration_count++;
    }

    return currentRoute;
}

// Algorytm pełnego przeglądu
Route solve_full_review(const vector<vector<double>>& distanceMatrix, int& iteration_count) {
    int numberOfCities = distanceMatrix.size();
    vector<int> cities(numberOfCities);
    for (int i = 0; i < numberOfCities; ++i) {
        cities[i] = i;
    }

    Route bestRoute;
    bestRoute.cities = cities;
    bestRoute.cost = check_cost(cities, distanceMatrix);
    iteration_count = 0;

    do {
        Route currentRoute;
        currentRoute.cities = cities;
        currentRoute.cost = check_cost(cities, distanceMatrix);

        if (currentRoute.cost < bestRoute.cost) {
            bestRoute = currentRoute;
        }
        iteration_count++;
    } while (next_permutation(cities.begin(), cities.end()));

    return bestRoute;
}

// Algorytm Tabu Search
Route solve_tabu(const vector<vector<double>>& distanceMatrix, int tabuSize, int maxIterations, int& iteration_count) {
    int numberOfCities = distanceMatrix.size();
    Route currentRoute = generate_random_solution(numberOfCities, distanceMatrix);
    list<Route> tabuList;
    set<vector<int>> tabuSet;

    tabuList.push_back(currentRoute);
    tabuSet.insert(currentRoute.cities);

    Route bestRoute = currentRoute;
    iteration_count = 0;

    for (int i = 0; i < maxIterations; ++i) {
        auto neighborhood = generate_neighborhood(currentRoute, distanceMatrix);

        neighborhood.erase(remove_if(neighborhood.begin(), neighborhood.end(), [&](Route& neighbor) {
            return tabuSet.count(neighbor.cities) > 0;
        }), neighborhood.end());

        if (neighborhood.empty()) {
            // Brak dostępnych sąsiadów, koniec algorytmu
            break;
        }

        auto nextRoute = *min_element(neighborhood.begin(), neighborhood.end(), [](const Route& lhs, const Route& rhs) {
            return lhs.cost < rhs.cost;
        });

        if (nextRoute.cost < bestRoute.cost) {
            bestRoute = nextRoute;
        }

        tabuList.push_back(nextRoute);
        tabuSet.insert(nextRoute.cities);

        if (tabuList.size() > tabuSize) {
            tabuSet.erase(tabuList.front().cities);
            tabuList.pop_front();
        }

        currentRoute = nextRoute;
        iteration_count++;
    }

    return bestRoute;
}

// Algorytm wyżarzania
Route solve_simulated_annealing(const vector<vector<double>>& distanceMatrix, function<double(int)> T, int maxIterations, int& iteration_count) {
    int numberOfCities = distanceMatrix.size();
    Route bestRoute = generate_random_solution(numberOfCities, distanceMatrix);
    Route currentRoute = bestRoute;

    random_device rd;
    mt19937 rgen(rd());
    iteration_count = 0;

    for (int i = 0; i < maxIterations; ++i) {
        auto neighborhood = generate_neighborhood(currentRoute, distanceMatrix);
        uniform_int_distribution<int> dist(0, neighborhood.size() - 1);
        Route newRoute = neighborhood[dist(rgen)];

        if (newRoute.cost < currentRoute.cost) {
            currentRoute = newRoute;
            if (newRoute.cost < bestRoute.cost) {
                bestRoute = newRoute;
            }
        } else {
            uniform_real_distribution<double> u(0.0, 1.0);
            if (u(rgen) < exp(-abs(newRoute.cost - currentRoute.cost) / T(i))) {
                currentRoute = newRoute;
            }
        }
        iteration_count++;
    }

    return bestRoute;
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
