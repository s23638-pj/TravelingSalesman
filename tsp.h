#ifndef TSP_H
#define TSP_H

#include <vector>

struct Route {
    std::vector<int> cities;
    double cost;
};

double calculateCost(const std::vector<int>& route, const std::vector<std::vector<double>>& distanceMatrix);

std::vector<Route> generateNeighborhood(const Route& currentRoute, const std::vector<std::vector<double>>& distanceMatrix);

Route generateRandomSolution(int numberOfCities, const std::vector<std::vector<double>>& distanceMatrix);

#endif