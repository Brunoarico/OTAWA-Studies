#include "aco.h"
#include <iostream>
#include <vector>
#define SIZE 16
//#define SIZE 5

void allocate(std::vector<std::vector<int>>& m) {

    /*int values[SIZE][SIZE] = {
        {0, 70, 0, 0, 0},
        {0, 0, 25, 25, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 25, 0, 10},
        {0, 0, 25, 0, 0}
    };*/

    int values[SIZE][SIZE] = {
        {0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 4, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 4, 2, 3, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    m.resize(SIZE);
    for (int i = 0; i < SIZE; i++) {
        m[i].resize(SIZE);
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            m[i][j] = values[i][j];
        }
    }

}

void printMatrix(const std::vector<std::vector<int>>& matrix) {
    for (int i = 0; i < SIZE; ++i) {
        std::cout << "[";
        for (int j = 0; j < SIZE; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "]\n";
    }
}

int main () {
    int maxIter = 30;
    int antNo = 2;
    double rho = 0.5; // Evaporation rate 
    double alpha = 1; // Phromone exponential parameters 
    double beta = 1;  // Desirability exponential parameter 
    std::vector<std::vector<int>> graph;

    allocate(graph);
    printMatrix(graph);
    ACO ACO(graph, SIZE, antNo, maxIter, alpha, beta, rho);
    ACO.simulate();
    printf("End\n");
    return 0;
}