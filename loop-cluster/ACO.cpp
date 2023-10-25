#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>

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
    struct Ant queen;
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
    double *cumsumP = (double *) malloc(length * sizeof(double));
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

void createColony(int length_edges, double** tau, double** eta, struct Colony *colony, int antNo, double alpha, double beta) {
    int nodeNo = length_edges;
    double *P_allNodes = (double *) calloc(nodeNo, sizeof(double));
    double *P = (double *) calloc(nodeNo, sizeof(double));
    for (int i = 0; i < antNo; ++i) {
        printf("Ant: %d\n", i);
        int initial_node = rand() % nodeNo; // select a random node
        colony->ant[i].tour[0] = initial_node;
        colony->ant[i].wcet[0] = 0;
        colony->ant[i].tourRow[0] = 0;
        colony->ant[i].tourColumn[0] = 0;
        int nextNode = 0;
        //while (nextNode <= length_edges - 1) { // to choose the rest of nodes
        while (nextNode != -1) {
            int currentNode = colony->ant[i].tour[nextNode];
            printf("current node: %d\n", initial_node);
            
            for (int j = 0; j < nodeNo; ++j) {
                P_allNodes[j] = pow(tau[currentNode][j], alpha) * pow(eta[currentNode][j], beta);
            }

            // Calculate sum of P_allNodes
            double sumP = 0;
            for (int j = 0; j < nodeNo; ++j) {
                sumP += P_allNodes[j];
            }

            
            for (int j = 0; j < nodeNo; ++j) {
                P[j] = P_allNodes[j] / sumP;
            }

            

            nextNode = rouletteWheel(P, nodeNo);
            printf("next node: %d\n", nextNode);

            colony->ant[i].wcet[nextNode] = eta[currentNode][nextNode];
            colony->ant[i].tour[nextNode] = nextNode;
            colony->ant[i].tourRow[nextNode] = currentNode;
            colony->ant[i].tourColumn[nextNode] = nextNode;

            
        }
    }
    free(P);
    free(P_allNodes);
}

double mean(int *array, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += array[i];
    }
    return sum / size;
}

int main()
{
    
    int graph[16][16] = {
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

    int length_edges = sizeof(graph) / sizeof(graph[0]);
    srand(time(NULL));

    // Inicializa a matriz resultante
    int matriz_resultante[length_edges];

    
    // Gera um índice aleatório dentro do número de colunas para cada linha
    for (int i = 0; i < length_edges; ++i) {
        int indice_aleatorio = rand() % 16;  // Gera um índice aleatório de 0 a 15
        matriz_resultante[i] = graph[i][indice_aleatorio]; // Seleciona o elemento correspondente ao índice aleatório
    }


    int maxIter = 30;
    int antNo = 1;

    double tau0 = 10 * 1 / (length_edges * mean(&graph[0][0], length_edges * length_edges)); 

    // Phromone matrix initialization
    double** tau = (double**)malloc(length_edges * sizeof(double*));
    for (int i = 0; i < length_edges; i++) tau[i] = (double*)malloc(length_edges * sizeof(double));
    
    for (int i = 0; i < length_edges; ++i) {
        for (int j = 0; j < length_edges; ++j) {
            tau[i][j] = tau0;
        }
    }
    

    // Desirability of each edge (eta)
    double** eta = (double**)malloc(length_edges * sizeof(double*));
    for (int i = 0; i < length_edges; i++) eta[i] = (double*)malloc(length_edges * sizeof(double));
    
    for (int i = 0; i < length_edges; ++i) {
        for (int j = 0; j < length_edges; ++j) {
            eta[i][j] = graph[i][j];
        }
    }

    

    double rho = 0.5; // Evaporation rate 
    double alpha = 1; // Phromone exponential parameters 
    double beta = 1;  // Desirability exponential parameter 

    double bestFitness = -INFINITY;
    int bestTour[2] = {0}; // Assuming tourRow and tourColumn are integers

    struct Colony colony;
    colony.ant = (struct Ant *) malloc(antNo * sizeof(struct Ant));
    printf("Ants: %d\n", antNo);
    for(int i = 0; i < antNo; i++) {
        colony.ant[i].tour = (int *) malloc(length_edges * sizeof(int));
        colony.ant[i].wcet = (int *) malloc(length_edges * sizeof(int));
        colony.ant[i].tourRow = (int *) malloc(length_edges * sizeof(int));
        colony.ant[i].tourColumn = (int *) malloc(length_edges * sizeof(int));
    }
    printf("Queen: %d\n", antNo);
    colony.queen.tour = (int *) malloc(length_edges * sizeof(int));
    colony.queen.wcet = (int *) malloc(length_edges * sizeof(int));
    colony.queen.tourRow = (int *) malloc(length_edges * sizeof(int));
    colony.queen.tourColumn = (int *) malloc(length_edges * sizeof(int));

    double Wcet[maxIter];

    for (int t = 0; t < maxIter; ++t) {
        // Create Ants

        createColony(length_edges, tau, eta, &colony, antNo, alpha, beta);
        
        // Calculate the fitness values of all ants
        for (int i = 0; i < antNo; ++i) {
            colony.ant[i].fitness = fitnessFunction(colony.ant[i].wcet, length_edges);
        }
        

        // Find the best ant (queen)
        double maxVal = -INFINITY;
        int maxIndex = -1;
        for (int i = 0; i < antNo; ++i) {
            if (colony.ant[i].fitness > maxVal) {
                maxVal = colony.ant[i].fitness;
                maxIndex = i;
            }
        }

        if (maxVal > bestFitness) {
            bestFitness = maxVal;
            bestTour[0] = *colony.ant[maxIndex].tourRow;
            bestTour[1] = *colony.ant[maxIndex].tourColumn;
        }

        colony.queen.fitness = bestFitness;
        // Assuming queen.tour is an array of size 2
        colony.queen.tour[0] = bestTour[0];
        colony.queen.tour[1] = bestTour[1];

        // Update pheromone matrix
        updatePhromone(tau, colony, length_edges, antNo);

        // Evaporation
        for (int i = 0; i < length_edges; ++i) {
            for (int j = 0; j < length_edges; ++j) {
                tau[i][j] *= (1 - rho);
            }
        }

        // Display the results
        printf("Iteration #%d: Longest length = %d\n", t + 1, colony.queen.fitness);
        Wcet[t] = colony.queen.fitness;
    }

    for (int i = 0; i < length_edges; i++) free(tau[i]);
    free(tau);
    for (int i = 0; i < length_edges; i++) free(eta[i]);
    free(eta);
    
    return 0;
}