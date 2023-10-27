#include "aco.h"

ACO::ACO(std::vector<std::vector<int>> graph, int nodeNo, int firstNode, int antNo, int maxIter, double alpha, double beta, double rho) {
    this->graph = graph;
    this->nodeNo = nodeNo;
    this->antNo = antNo;
    this->maxIter = maxIter;
    this->alpha = alpha;
    this->beta = beta;
    this->rho = rho;
    this->firstNode = firstNode;
    bestFitness = -INFINITY;

    wcet.resize(maxIter);

    srand(time(NULL));

    matriz_resultante.resize(nodeNo);

    for (int i = 0; i < nodeNo; ++i) {
        int randomIndex = rand() % nodeNo;  // Gera um índice aleatório de 0 a 15
        matriz_resultante[i] = graph[i][randomIndex]; // Seleciona o elemento correspondente ao índice aleatório
    }

    tau0 = 10 * 1 / (nodeNo * mean(graph, nodeNo)); 

    tau.resize(nodeNo);
    for (int i = 0; i < nodeNo; i++) tau[i].resize(nodeNo); 

    //initialize tau with tau0
    for (int i = 0; i < nodeNo; i++) 
        for (int j = 0; j < nodeNo; j++) tau[i][j] = tau0;

    eta.resize(nodeNo);
    for (int i = 0; i < nodeNo; i++) eta[i].resize(nodeNo);

    //initialize eta with graph values
    for (int i = 0; i < nodeNo; i++) 
        for (int j = 0; j < nodeNo; j++) eta[i][j] = graph[i][j];


    colony.queen.tour.resize(nodeNo);
    colony.queen.wcet.resize(nodeNo);
    colony.queen.tourRow.resize(nodeNo);
    colony.queen.tourColumn.resize(nodeNo);
    colony.queen.fitness = 0;
    colony.ant.resize(antNo);
    
    for (int i = 0; i < antNo; i++) {
        colony.ant[i].tour.resize(nodeNo);
        colony.ant[i].wcet.resize(nodeNo);
        colony.ant[i].tourRow.resize(nodeNo);
        colony.ant[i].tourColumn.resize(nodeNo);
        colony.ant[i].fitness = 0;
    }
}

double ACO::mean(std::vector<std::vector<int>> array, int size) {
    double sum = 0.0;
    for (size_t i = 0; i < size; i++) 
        for (size_t j = 0; j < size; j++) sum += array[i][j];
    return sum / pow(size, 2);
}

void setToZero(std::vector<int> array) {

}

void ACO::initializeAnts() {
    for (int i = 0; i < antNo; i++) {
        colony.ant[i].tour.assign(nodeNo, 0);
        colony.ant[i].wcet.assign(nodeNo, 0);
        colony.ant[i].tourRow.assign(nodeNo, 0);
        colony.ant[i].tourColumn.assign(nodeNo, 0);
        colony.ant[i].fitness = 0;
    }
}

int ACO::rouletteWheel(std::vector<double> P) {
    std::vector<double> cumsumP(nodeNo);
    cumsumP[0] = P[0];

    // Compute cumulative sum
    for (int i = 1; i < nodeNo; ++i) {
        cumsumP[i] = cumsumP[i - 1] + P[i];
    }

    double r = (double)rand() / RAND_MAX;
    int nextNode = -1;

    // Roulette wheel selection
    for (int i = 0; i < nodeNo; ++i) {
        if (r <= cumsumP[i]) {
            nextNode = i;
            break;
        }
    }
    return nextNode;
}

void ACO::printAnt(int antNo){
    printf("Ant: %d\n", antNo);
    for (int j = 0; j < nodeNo; j++) {
        printf("tour[%d]: %d\n", j, colony.ant[antNo].tour[j]);
        printf("wcet[%d]: %d\n", j, colony.ant[antNo].wcet[j]);
        printf("tourRow[%d]: %d\n", j, colony.ant[antNo].tourRow[j]);
        printf("tourColumn[%d]: %d\n", j, colony.ant[antNo].tourColumn[j]);
    }
}



void ACO::runColony() {
    std::vector<double> P_allNodes(nodeNo);
    std::vector<double> P(nodeNo);

    for (int i = 0; i < antNo; ++i) {
        //printf("\nAnt: %d\n", i);
        int initial_node = this->firstNode; // select a random node
        colony.ant[i].tour[0] = initial_node;
        colony.ant[i].wcet[0] = 0;
        colony.ant[i].tourRow[0] = 0;
        colony.ant[i].tourColumn[0] = 0;
        int nextNode = 0;
        while (nextNode != -1) {
            double sumP = 0;
            int currentNode = colony.ant[i].tour[nextNode];
            

            for (int j = 0; j < nodeNo; ++j) {
                P_allNodes[j] = pow(tau[currentNode][j], alpha) * pow(eta[currentNode][j], beta);
            }

            // Calculate sum of P_allNodes
            for (int j = 0; j < nodeNo; ++j) sumP += P_allNodes[j];
            for (int j = 0; j < nodeNo; ++j) P[j] = P_allNodes[j] / sumP;

            //print P_allNodes
            //printf("P_allNodes: ");
            //for (int j = 0; j < nodeNo; ++j) printf("%f ", P_allNodes[j]);

            nextNode = rouletteWheel(P);
            
            if(nextNode == -1) break;

            colony.ant[i].wcet[nextNode] = eta[currentNode][nextNode];
            colony.ant[i].tour[nextNode] = nextNode;
            colony.ant[i].tourRow[nextNode] = currentNode;
            colony.ant[i].tourColumn[currentNode] = nextNode;

            if (nextNode == this->firstNode) break;

        }
    }
}

int ACO::fitnessFunction(int antNo) {
    int fitness = 0;
    for (int i = 0; i < colony.ant[antNo].wcet.size(); ++i) 
        fitness += colony.ant[antNo].wcet[i];
    return fitness;
}

void ACO::calculateFitness() {
    for (int i = 0; i < antNo; ++i) colony.ant[i].fitness = fitnessFunction(i);
}

void ACO::printWCEP(Ant a) {
    printf("WCEP: 0 -> ");
    int i = 0;
    printf("%d -> ", a.tourColumn[i]);
    i = a.tourColumn[i];
    while(i !=0 ){
        printf("%d -> ", a.tourColumn[i]);
        i = a.tourColumn[i];
    }
    printf("end\n");
}

void ACO::simulate() {
    for (int t = 0; t < maxIter; ++t) {
        initializeAnts();
        runColony();
        calculateFitness();
        updatePhromone();
       
        wcet[t] = findQueen();
        for (int j = 0; j < antNo; ++j){
            printf("\nAnt #%d: %d\n", j, colony.ant[j].fitness);
            printWCEP(colony.ant[j]);
            colony.ant[j].fitness = 0;
            colony.ant[j].tourColumn.clear();
        }
        printf("\nIteration #%d: Longest length = %d\n", t + 1, wcet[t]);
        printf("---------------------------------------------------\n");
    }

    printf("Results:\n");
    printf("Longest length = %d\n", colony.queen.fitness);
    printWCEP(colony.queen);

}

int ACO::getResults() {
    return colony.queen.fitness;
}

void ACO::updatePhromone() {
    int i, j;
    int currentNode, nextNode;
    for (i = 0; i < antNo; ++i) {
        for (j = 0; j < nodeNo - 1; ++j) {
            currentNode = colony.ant[i].tour[j];
            nextNode = colony.ant[i].tour[j + 1];

            tau[currentNode][nextNode] += 1.0 / colony.ant[i].fitness;
            tau[nextNode][currentNode] += 1.0 / colony.ant[i].fitness;

            //printf("tau[%d][%d]: %f\n", currentNode, nextNode, tau[currentNode][nextNode]);
        }
    }
    for (int i = 0; i < nodeNo; ++i) 
        for (int j = 0; j < nodeNo; ++j) tau[i][j] *= (1 - rho);
}

int ACO::findQueen() {
    double maxVal = -INFINITY;
    int maxIndex = 0;
    for (int i = 0; i < antNo; ++i) {
        if (colony.ant[i].fitness > maxVal) {
            maxVal = colony.ant[i].fitness;
            maxIndex = i;
        }
    }
    
    if(maxVal > colony.queen.fitness) {
        colony.queen.fitness = maxVal;
        for (int j = 0; j < nodeNo; j++)
            colony.queen.tourColumn[j] = colony.ant[maxIndex].tourColumn[j];
    }
    return maxVal;
}