#ifndef ACO_H
#define ACO_H
#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>
#include <thread>
#include "../cfgmatrix/CfgMatrix.h"
#include "../aco/aco.h"
#include "../utils/utilities.h"
#include <stack>

typedef struct _Ant {
    std::vector<int> tour;
    std::vector<int> wcet;
    int fitness;
}Ant;

typedef struct _Colony {
    std::vector<Ant> ant;
    Ant queen;
}Colony;

typedef struct _loop{
    int ref;
    int loophead;
    int iters;
}loop;

class ACO {
    public:
    ACO(CfgMatrix graph, int antNo, int firstNode, int maxIter, double alpha, double beta, double rho);
    void printAnt(int antNo);
    void simulate(bool verbose = false);
    uint32_t getResults();

    private:
    double mean(CfgMatrix array, int size);
    void initializeAnts();
    int rouletteWheel(std::vector<double> P);
    void calculateFitness();
    void antsRun(int antNo);
    int fitnessFunction(int antNo);
    void updatePhromone();
    int sumNextNodes(int node);
    int findQueen(); 
    void printWCEP(Ant a);
    void runColony();
    void printTau();
    void printEta();
    void selectNextNode(std::stack<loop> s, int currentNode, std::vector<double> *P);

    CfgMatrix graph;
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
    FILE *fp;
};


#endif // ACO_H