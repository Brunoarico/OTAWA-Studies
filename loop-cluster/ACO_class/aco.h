#ifndef ACO_H
#define ACO_H
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <vector>

typedef struct _Ant {
    std::vector<int> tour;
    std::vector<int> wcet;
    std::vector<int> tourRow;
    std::vector<int> tourColumn;
    int fitness;
}Ant;

typedef struct _Colony {
    std::vector<Ant> ant;
    Ant queen;
}Colony;

class ACO {

    public:
    ACO(std::vector<std::vector<int>> graph, int nodeNo, int firstNode, int antNo, int maxIter, double alpha, double beta, double rho);
    void runColony();
    void printAnt(int antNo);
    void simulate();
    int getResults();

    private:
    double mean(std::vector<std::vector<int>> array, int size);
    void initializeAnts();
    int rouletteWheel(std::vector<double> P);
    void calculateFitness();
    int fitnessFunction(int antNo);
    void updatePhromone();
    int findQueen();
    void printWCEP(Ant a);
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<double>> tau;
    std::vector<std::vector<double>> eta;
    std::vector<int> matriz_resultante;
    std::vector<int> wcet;

    Colony colony;
    int nodeNo;
    int firstNode;
    int antNo;
    int maxIter;
    double tau0;
    double alpha; 
    double beta;
    double rho;
    double bestFitness;
};


#endif // ACO_H