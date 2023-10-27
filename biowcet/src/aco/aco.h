#ifndef ACO_H
#define ACO_H
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <vector>
#include "../cfgmatrix/CfgMatrix.h"
#include "../aco/aco.h"

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
    ACO(CfgMatrix graph, int antNo, int maxIter, double alpha, double beta, double rho);
    void printAnt(int antNo);
    uint32_t simulate(bool verbose = false);

    private:
    double mean(CfgMatrix array, int size);
    void initializeAnts();
    int rouletteWheel(std::vector<double> P);
    void calculateFitness();
    int fitnessFunction(int antNo);
    void updatePhromone();
    int findQueen();
    void printWCEP(Ant a);
    void runColony();

    CfgMatrix graph;
    std::vector<std::vector<double>> tau;
    std::vector<std::vector<double>> eta;
    std::vector<int> matriz_resultante;
    std::vector<int> wcet;

    Colony colony;
    int nodeNo;
    int antNo;
    int maxIter;
    double tau0;
    double alpha; 
    double beta;
    double rho;
    double bestFitness;
};


#endif // ACO_H