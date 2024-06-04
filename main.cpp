#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "tsp.h"

// Wczytanie tras
std::vector<std::vector<double>> loadDistanceMatrix(const std::string& filename) {
    std::vector<std::vector<double>> matrix;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<double> row;
        double value;
        while (iss >> value) {
            row.push_back(value);
        }
        matrix.push_back(row);
    }
    return matrix;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Użycie: " << argv[0] << " <plik_z_danymi>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    auto distanceMatrix = loadDistanceMatrix(filename);
    int numberOfCities = distanceMatrix.size();

    // Generowanie losowego rozwiązania
    Route initialRoute = generateRandomSolution(numberOfCities, distanceMatrix);

    std::cout << "Początkowa trasa: ";
    for (int city : initialRoute.cities) {
        std::cout << city << " ";
    }
    std::cout << "\nKoszt: " << initialRoute.cost << std::endl;

    // Generowanie sąsiedztwa
    auto neighborhood = generateNeighborhood(initialRoute, distanceMatrix);
    std::cout << "Liczba sąsiednich tras: " << neighborhood.size() << std::endl;

    return 0;
}
