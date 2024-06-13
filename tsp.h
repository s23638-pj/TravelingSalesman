#ifndef TSP_H
#define TSP_H

#include <vector>
#include <string>

using namespace std;

struct Route {
    vector<int> cities;
    double cost;
};

double check_cost(const vector<int>& route, const vector<vector<double>>& distanceMatrix);

vector<Route> generate_neighborhood(const Route& currentRoute, const vector<vector<double>>& distanceMatrix);

Route generate_random_solution(int numberOfCities, const vector<vector<double>>& distanceMatrix);

Route check_tsp(const vector<vector<double>>& distanceMatrix);

Route solve_hill_climbing(const vector<vector<double>>& distanceMatrix);

pair<vector<string>, vector<vector<double>>> read_csv(const string& filename);

#endif // TSP_H
