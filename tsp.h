#ifndef TSP_H
#define TSP_H

#include <vector>
#include <string>
#include <functional>

using namespace std;

struct Route {
    vector<int> cities;
    double cost;
};

double check_cost(const vector<int>& route, const vector<vector<double>>& distanceMatrix);

vector<Route> generate_neighborhood(const Route& currentRoute, const vector<vector<double>>& distanceMatrix);

Route generate_random_solution(int numberOfCities, const vector<vector<double>>& distanceMatrix);

Route solve_hill_climbing(const vector<vector<double>>& distanceMatrix, int maxIterations, int& iteration_count);

Route solve_random_hill_climbing(const vector<vector<double>>& distanceMatrix, int maxIterations, int& iteration_count);

Route solve_full_review(const vector<vector<double>>& distanceMatrix, int maxIterations, int& iteration_count);

Route solve_tabu(const vector<vector<double>>& distanceMatrix, int tabuSize, int maxIterations, int& iteration_count);

Route solve_simulated_annealing(const vector<vector<double>>& distanceMatrix, function<double(int)> T, int maxIterations, int& iteration_count);

pair<vector<string>, vector<vector<double>>> read_csv(const string& filename);

size_t hash_pair(int a, int b);

#endif // TSP_H
