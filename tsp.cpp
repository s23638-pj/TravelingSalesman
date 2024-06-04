#include "tsp.h"
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <limits>

double calculateCost(const std::vector<int>& route, const std::vector<std::vector<double>>& distanceMatrix) {
    double totalCost = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        totalCost += distanceMatrix[route[i]][route[i + 1]];
    }
    totalCost += distanceMatrix[route.back()][route.front()]; // Powrót do punktu początkowego
    return totalCost;
}

std::vector<Route> generateNeighborhood(const Route& currentRoute, const std::vector<std::vector<double>>& distanceMatrix) {
    std::vector<Route> neighborhood;
    for (size_t i = 1; i < currentRoute.cities.size() - 1; ++i) {
        for (size_t j = i + 1; j < currentRoute.cities.size(); ++j) {
            Route newRoute = currentRoute;
            std::swap(newRoute.cities[i], newRoute.cities[j]);
            newRoute.cost = calculateCost(newRoute.cities, distanceMatrix);
            neighborhood.push_back(newRoute);
        }
    }
    return neighborhood;
}

Route generateRandomSolution(int numberOfCities, const std::vector<std::vector<double>>& distanceMatrix) {
    Route randomRoute;
    randomRoute.cities.resize(numberOfCities);
    std::iota(randomRoute.cities.begin(), randomRoute.cities.end(), 0); // Wypełnij kolejno 0, 1, 2, ..., numberOfCities-1
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(randomRoute.cities.begin(), randomRoute.cities.end(), g);
    randomRoute.cost = calculateCost(randomRoute.cities, distanceMatrix);
    return randomRoute;
}
