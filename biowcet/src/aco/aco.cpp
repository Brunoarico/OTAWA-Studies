#include "aco.h"

ACO::ACO(CfgMatrix graph, int antNo, int firstNode, int maxIter, double alpha, double beta, double rho) {
    this->graph = graph;
    this->nodeNo = graph.getSize()+1;
    this->antNo = antNo;
    this->maxIter = maxIter;
    this->alpha = alpha;
    this->beta = beta;
    this->rho = rho;
    bestFitness = -INFINITY;
    this->firstNode = firstNode;

    wcet.resize(maxIter);

    srand(time(NULL));

    matriz_resultante.resize(nodeNo);

    for (int i = 0; i < nodeNo; ++i) {
        int randomIndex = rand() % nodeNo;  // Gera um índice aleatório de 0 a 15
        matriz_resultante[i] = graph.getCycles(i, randomIndex); // Seleciona o elemento correspondente ao índice aleatório
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
        for (int j = 0; j < nodeNo; j++) eta[i][j] = graph.getCycles(i, j);


    colony.queen.tour;
    colony.queen.wcet;
    colony.queen.fitness = 0;
    colony.ant.resize(antNo);
    
    for (int i = 0; i < antNo; i++) {
        colony.ant[i].fitness = 0;
    }
}

double ACO::mean(CfgMatrix array, int size) {
    double sum = 0.0;
    for (size_t i = 0; i < size; i++) 
        for (size_t j = 0; j < size; j++) sum += array.getCycles(i, j);
    return sum / pow(size, 2);
}


void ACO::initializeAnts() {
    for (int i = 0; i < antNo; i++) {
        colony.ant[i].fitness = 0;
        colony.ant[i].tour.clear();
        colony.ant[i].wcet.clear();
    }
}



void ACO::printAnt(int antNo){
    printf("\tAnt: %d\n", antNo);
    printf("\tTour: ");
    for (int j = 0; j < colony.ant[antNo].tour.size(); j++) 
        printf("%d ",  colony.ant[antNo].tour[j]);
    printf("\n");
    uint32_t sum = 0;
    for (int j = 0; j < colony.ant[antNo].wcet.size(); j++) 
        sum += colony.ant[antNo].wcet[j];
    printf("\twcet:: %d\n", sum);
    
}

int ACO::rouletteWheel(std::vector<double> P) {
    std::vector<double> cumsumP(nodeNo);
    cumsumP[0] = P[0];

    //printf("\ncumsumP: ");
    // Compute cumulative sum
    for (int i = 1; i < nodeNo; ++i) {
        cumsumP[i] = cumsumP[i - 1] + P[i];
        //printf("%f ", cumsumP[i]);
    }
    //printf("\n"); 

    double r = (double)rand() / RAND_MAX;
    int nextNode = -1;

    // Roulette wheel selection
    for (int i = 0; i < nodeNo; i++) {
        if (r <= cumsumP[i]) {
            //printf("here:\n");
            nextNode = i;
            break;
        }
    }

    return nextNode;
}

void ACO::selectNextNode(std::stack<loop> s, int currentNode, std::vector<double> *P) {
    std::vector<double> P_allNodes(nodeNo);
    double sumP = 0;
    for (int j = 0; j < nodeNo; ++j) {
        if(!s.empty() && j == s.top().loophead){
            //if iterations are 0 set probability to 0 to that loop 
            if(s.top().iters == 0) P_allNodes[j] = 0;
            //else set high probability to 1 to that loop
            else P_allNodes[j] = pow(tau[currentNode][j], 10) * pow(eta[currentNode][j], 10);
        }
        else P_allNodes[j] = pow(tau[currentNode][j], alpha) * pow(eta[currentNode][j], beta);
        sumP += P_allNodes[j];
    }

    for (int j = 0; j < nodeNo; ++j) {
        (*P)[j] = P_allNodes[j] / sumP;
    }
}

void ACO::runColony() {
    std::vector<double> P(nodeNo);
    //printf("Nodes: %d\n", nodeNo);
    //printf("Ants: %d\n", antNo);
    //graph.printIterations();
    for (int i = 0; i < antNo; ++i) {
        //printf("\nAnt: %d\n", i);
        colony.ant[i].wcet.push_back(0);
        colony.ant[i].tour.push_back(firstNode);
        int currentNode = firstNode;
        std::stack<loop> s;
        while (true) {
            
            //printf("currentNode: %d\n", currentNode);
    
            //if something in the stack reduce iterations if pass by the same node
            if(!s.empty() && currentNode == s.top().ref) { 
                s.top().iters -= 1;
                //printf("reducted: %d\n", s.top().iters);
            }

            //if pass by a loophead push to stack
            if(graph.isAloop(currentNode)) {
                loop l = {currentNode, graph.loopHead(currentNode), graph.getIteration(currentNode)};
                //printf("Loophead: %d\n", graph.loopHead(currentNode));
                if(s.empty() || s.top().ref != currentNode) {
                    //printf("pushEmpty: %d\n", currentNode);
                    s.push(l);
                }
            }

            //set node probabilities
            selectNextNode(s, currentNode, &P);
            

            if(!s.empty() && s.top().iters == 0 && currentNode == s.top().ref) {
                    //printf("top: %d\n", s.top().ref); printf("pop: %d\n", currentNode);
                    s.pop();
            }

 
            
            //printf("P_allNodes: "); for (int j = 0; j < nodeNo; ++j) printf("%f ", P_allNodes[j]); printf("\n");
            //printf("P: "); for (int j = 0; j < nodeNo; ++j) printf("%f ", P[j]); printf("\n");

            int nextNode = rouletteWheel(P);


            if(nextNode == -1) break;
            colony.ant[i].wcet.push_back(eta[currentNode][nextNode]);
            colony.ant[i].tour.push_back(nextNode);
            currentNode = nextNode;
        }
        
        //printWCEP(colony.ant[i]);
        //printf("End Ant\n");
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
    printf("\nWCEP: ");
    for (int i = 0; i < a.tour.size(); ++i) printf("%d -> ", a.tour[i]);
    printf("end\n");
    printf("WCET: ");
    int sum = 0;
    for (int i = 0; i < a.wcet.size(); ++i) sum += a.wcet[i];
    printf("%d\n", sum);
    printf("\nPath: ");
    for (int i = 0; i < a.tour.size(); ++i) printf("%d -> ", a.wcet[i]);
    printf("end\n");
}

void ACO::simulate(bool verbose) {
    for (int t = 0; t < maxIter; ++t) {
        //printf("Initializing ants...\n");
        initializeAnts();
        //printf("Running colony...\n");
        runColony();
        //printf("Calculating fitness...\n");
        calculateFitness();
        //printf("Updating phromone...\n");
        updatePhromone();
        //printf("Finding queen...\n");
        wcet[t] = findQueen();
        //printf("Longest length = %d\n", wcet[t]);
        updateProgressBar(t, maxIter);
        if(verbose) {
            for (int j = 0; j < antNo; ++j){
                printf("\nAnt #%d: %d\n", j, colony.ant[j].fitness);
                printWCEP(colony.ant[j]);
                colony.ant[j].fitness = 0;
            }
            printf("\nIteration #%d: Longest length = %d\n", t + 1, wcet[t]);
            printf("---------------------------------------------------\n");
        }
    }
    if(verbose) {
        printf("Results:\n");
        printf("Longest length = %d\n", colony.queen.fitness);
        printWCEP(colony.queen);
    }
    
}

uint32_t ACO::getResults() {
    return colony.queen.fitness;
}

void ACO::updatePhromone() {
    int i, j;
    int currentNode, nextNode;
    for (i = 0; i < antNo; ++i) {
        for (j = 0; j < colony.ant[i].tour.size()-1; ++j) {
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
        colony.queen.tour.resize(colony.ant[maxIndex].tour.size());
        for (int j = 0; j < colony.ant[maxIndex].tour.size(); j++)
            colony.queen.tour[j] = colony.ant[maxIndex].tour[j];
    }
    return maxVal;
}

void ACO::printTau() {
    printf("Tau:\n");
    for (int i = 0; i < nodeNo; ++i) {
        for (int j = 0; j < nodeNo; ++j) printf("%f ", tau[i][j]);
        printf("\n");
    }
    printf("\n");
}

void ACO::printEta() {
    printf("Eta:\n");
    for (int i = 0; i < nodeNo; ++i) {
        for (int j = 0; j < nodeNo; ++j) printf("%f ", eta[i][j]);
        printf("\n");
    }
    printf("\n");
}