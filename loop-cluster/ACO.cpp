#include <stdio.h>
#include <stdlib.h>
#include <cmath>

struct Ant {
    int *tour;
    int *wcet;
    int *tourRow;
    int *tourColumn;
    int fitness;
    // ... other ant properties
};

struct Colony {
    struct Ant *ant;
    // ... other colony properties
};

void updatePhromone(double **tau, struct Colony colony, int nodeNo, int antNo) {
    int i, j;
    int currentNode, nextNode;

    for (i = 0; i < antNo; ++i) {
        for (j = 0; j < nodeNo - 1; ++j) {
            currentNode = colony.ant[i].tour[j];
            nextNode = colony.ant[i].tour[j + 1];

            tau[currentNode][nextNode] += 1.0 / colony.ant[i].fitness;
            tau[nextNode][currentNode] += 1.0 / colony.ant[i].fitness;
        }
    }
}


int rouletteWheel(double *P, int length) {
    double *cumsumP = malloc(length * sizeof(double));
    cumsumP[0] = P[0];

    // Compute cumulative sum
    for (int i = 1; i < length; ++i) {
        cumsumP[i] = cumsumP[i - 1] + P[i];
    }

    double r = (double)rand() / RAND_MAX;
    int nextNode = -1;

    // Roulette wheel selection
    for (int i = 0; i < length; ++i) {
        if (r <= cumsumP[i]) {
            nextNode = i;
            break;
        }
    }

    free(cumsumP);
    return nextNode;
}

int calculateFitness(int *tour, int tourLength) {
    int fitness = 0;
    for (int i = 0; i < tourLength; ++i) {
        fitness += tour[i];
    }
    return fitness;
}

int fitnessFunction(int *tour, int tourLength) {
    int fitness = calculateFitness(tour, tourLength);
    return fitness;
}

void createColony(int **tau, int **eta, int length_edges, struct Colony *colony, int antNo, double alpha, double beta) {
    int nodeNo = length_edges;

    for (int i = 0; i < antNo; ++i) {
        int initial_node = rand() % nodeNo + 1; // select a random node
        colony->ant[i].tour[0] = initial_node;
        colony->ant[i].wcet[0] = 0;
        colony->ant[i].tourRow[0] = 0;
        colony->ant[i].tourColumn[0] = 0;
        int nextNode = 0;

        while (nextNode <= length_edges - 1) { // to choose the rest of nodes
            int currentNode = colony->ant[i].tour[nextNode];

            double *P_allNodes = malloc(nodeNo * sizeof(double));
            for (int j = 0; j < nodeNo; ++j) {
                P_allNodes[j] = pow(tau[currentNode][j], alpha) * pow(eta[currentNode][j], beta);
            }

            // Calculate sum of P_allNodes
            double sumP = 0;
            for (int j = 0; j < nodeNo; ++j) {
                sumP += P_allNodes[j];
            }

            double *P = malloc(nodeNo * sizeof(double));
            for (int j = 0; j < nodeNo; ++j) {
                P[j] = P_allNodes[j] / sumP;
            }

            free(P_allNodes);

            nextNode = rouletteWheel(P, nodeNo);

            colony->ant[i].wcet[nextNode] = eta[currentNode][nextNode];
            colony->ant[i].tour[nextNode] = nextNode;
            colony->ant[i].tourRow[nextNode] = currentNode;
            colony->ant[i].tourColumn[nextNode] = nextNode;

            free(P);
        }
    }
}