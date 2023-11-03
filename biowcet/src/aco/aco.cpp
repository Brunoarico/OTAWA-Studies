#include "aco.h"

ACO::ACO(CfgMatrix graph, int antNo, int firstNode, int maxIter, double alpha, double beta, double rho) {
    this->graph = graph;
    this->nodeNo = graph.getSize() + 1;
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
        int randomIndex = rand() % nodeNo;                       // Gera um índice aleatório de 0 a 15
        matriz_resultante[i] = graph.getCycles(i, randomIndex);  // Seleciona o elemento correspondente ao índice aleatório
    }

    tau0 = 10 * 1 / (nodeNo * mean(graph, nodeNo));
    // tau0 = 1 / (mean(graph, nodeNo));
    // tau0 = 1;

    tau.resize(nodeNo);
    for (int i = 0; i < nodeNo; i++) tau[i].resize(nodeNo);

    eta.resize(nodeNo);
    for (int i = 0; i < nodeNo; i++) eta[i].resize(nodeNo);
    for (int i = 0; i < nodeNo; i++) {
        int sum = 0;
        for (int j = 0; j < nodeNo; j++) sum += graph.getCycles(i, j);
        for (int j = 0; j < nodeNo; j++) {
            eta[i][j] = float(graph.getCycles(i, j));
            if (eta[i][j]) tau[i][j] = tau0;
        }
    }

    colony.queen.tour;
    colony.queen.wcet;
    colony.queen.fitness = 0;
    colony.ant.resize(antNo);

    for (int i = 0; i < antNo; i++) {
        colony.ant[i].fitness = 0;
    }

    // printTau();
    // printEta();
}

double ACO::mean(CfgMatrix array, int size) {
    double sum = 0.0;
    for (size_t i = 0; i < size; i++)
        for (size_t j = 0; j < size; j++) sum += array.getCycles(i, j);
    return sum / pow(size, 2);
}

/**
 * @brief Initializes the ants for the ACO algorithm.
 *
 * This function initializes the ants for the ACO algorithm by setting their fitness to 0 and clearing their tour and wcet vectors.
 *
 */
void ACO::initializeAnts() {
    for (int i = 0; i < antNo; i++) {
        colony.ant[i].fitness = 0;
        colony.ant[i].tour.clear();
        colony.ant[i].wcet.clear();
    }
}

void ACO::printAnt(int antNo) {
    printf("\tAnt: %d\n", antNo);
    printf("\tTour: ");
    for (int j = 0; j < colony.ant[antNo].tour.size(); j++)
        printf("%d ", colony.ant[antNo].tour[j]);
    printf("\n");
    uint32_t sum = 0;
    for (int j = 0; j < colony.ant[antNo].wcet.size(); j++)
        sum += colony.ant[antNo].wcet[j];
    printf("\twcet:: %d\n", sum);
}

/**
 * @brief Selects the next node to be visited by an ant, based on the probability vector P.
 *
 * This function selects the next node to be visited by an ant, based on the probability vector P.
 * It uses the roulette wheel selection method to choose the next node.
 *
 * @param P Vector of probabilities for each node.
 * @return The index of the next node to be visited.
 */

int ACO::rouletteWheel(std::vector<double> P) {
    std::vector<double> cumsumP(nodeNo);
    cumsumP[0] = P[0];

    // printf("\ncumsumP: ");
    //  Compute cumulative sum
    for (int i = 1; i < nodeNo; ++i) {
        cumsumP[i] = cumsumP[i - 1] + P[i];
        // printf("%f ", cumsumP[i]);
    }
    // printf("\n");

    double r = (double)rand() / RAND_MAX;
    int nextNode = -1;

    // Roulette wheel selection
    for (int i = 0; i < nodeNo; i++) {
        if (r <= cumsumP[i]) {
            // printf("here:\n");
            nextNode = i;
            break;
        }
    }

    return nextNode;
}

/**
 * @brief Calculates the average number of next nodes for a given node.
 *
 * This function calculates the average number of next nodes for a given node, based on the eta matrix.
 *
 * @param node The node for which to calculate the average number of next nodes.
 * @return The average number of next nodes for the given node.
 */
int ACO::sumNextNodes(int node) {
    int sum = 0;
    int n = 0;
    for (int i = 0; i < eta.size(); ++i) {
        if (i > 0) n++;
        sum += eta[node][i];
    }
    return sum / n;
}

/**
 * @brief Selects the next node to be visited by an ant, based on the current node and the stack of loops.
 *
 * @param s Stack of loops.
 * @param currentNode Current node being visited by the ant.
 * @param P Vector of probabilities for each node.
 */
void ACO::selectNextNode(std::stack<loop> s, int currentNode, std::vector<double> *P) {
    std::vector<double> P_allNodes(nodeNo);
    double sumP = 0;
    for (int j = 0; j < nodeNo; ++j) {
        if (!s.empty() && j == s.top().loophead) {
            // if iterations are 0 set probability to 0 to that loop
            if (s.top().iters == 0) P_allNodes[j] = 0;
            // else set high probability to 1 to that loop
            else {
                P_allNodes[j] = pow(eta[currentNode][j], 30);
            }
        } else
            P_allNodes[j] = pow(tau[currentNode][j], alpha) * pow((eta[currentNode][j] + sumNextNodes(j)) / 2, beta);
        sumP += P_allNodes[j];
    }

    for (int j = 0; j < nodeNo; ++j) (*P)[j] = P_allNodes[j] / sumP;
}

/**
 * @brief Runs the Ant Colony Optimization algorithm for a given number of iterations.
 *
 * This function runs the Ant Colony Optimization algorithm for a given number of iterations.
 * It initializes the ants, runs the colony, calculates the fitness of each ant's tour, updates the pheromone matrix, and finds the ant with the highest fitness.
 *
 */
void ACO::runColony() {
    std::vector<double> P(nodeNo);

    for (int i = 0; i < antNo; ++i) {
        colony.ant[i].wcet.push_back(0);
        colony.ant[i].tour.push_back(firstNode);
        int currentNode = firstNode;
        std::stack<loop> s;
        while (true) {
            if (!s.empty() && currentNode == s.top().ref) {
                s.top().iters -= 1;
            }

            // if pass by a loophead push to stack
            if (graph.isAloop(currentNode)) {
                loop l = {currentNode, graph.loopHead(currentNode), graph.getIteration(currentNode)};
                if (s.empty() || s.top().ref != currentNode) {
                    s.push(l);
                }
            }

            // set node probabilities
            selectNextNode(s, currentNode, &P);

            int nextNode = rouletteWheel(P);

            if (!s.empty() && s.top().iters == 0 && currentNode == s.top().ref) {
                s.pop();
            }

            if (nextNode == -1) break;
            colony.ant[i].wcet.push_back(graph.getCycles(currentNode, nextNode));
            colony.ant[i].tour.push_back(nextNode);
            currentNode = nextNode;
        }
        for (int j = 0; j < colony.ant[i].tour.size(); ++j) {
            fprintf(f, "%d", colony.ant[i].tour[j]);
            if (j < colony.ant[i].tour.size() - 1) fprintf(f, "->");
        }
        fprintf(f, "\n");
    }
}

/**
 * @brief Calculates the fitness of an ant's tour.
 *
 * This function calculates the fitness of an ant's tour by summing the WCET (worst-case execution time) of each node in the tour.
 *
 * @param antNo The index of the ant whose tour's fitness will be calculated.
 * @return The fitness of the ant's tour.
 */
int ACO::fitnessFunction(int antNo) {
    int fitness = 0;
    for (int i = 0; i < colony.ant[antNo].wcet.size(); ++i)
        fitness += colony.ant[antNo].wcet[i];
    return fitness;
}

/**
 * @brief Calculates the fitness of each ant's tour and finds the ant with the highest fitness.
 *
 * This function calculates the fitness of each ant's tour using the fitness function.
 * It then finds the ant with the highest fitness and updates the `bestAnt` variable.
 *
 */
void ACO::calculateFitness() {
    float maxFitness = -INFINITY;
    for (int i = 0; i < antNo; ++i) {
        colony.ant[i].fitness = fitnessFunction(i);
        if (maxFitness < colony.ant[i].fitness) maxFitness = colony.ant[i].fitness;
    }
}

/**
 * @brief Prints the tour and WCET of an ant.
 *
 * This function prints the tour and WCET (worst-case execution time) of an ant.
 * It prints the tour as a sequence of nodes and the corresponding WCET for each node.
 *
 * @param a The ant whose tour and WCET will be printed.
 */
void ACO::printWCEP(Ant a) {
    printf("\nWCEP: ");
    for (int i = 0; i < a.tour.size(); ++i) printf("%d -> ", a.tour[i]);
    printf("end\n");
    printf("WCET: ");
    int sum = 0;
    for (int i = 0; i < a.wcet.size(); ++i) sum += a.wcet[i];
    printf("%d\n", sum);
    printf("\nPath: ");
    for (int i = 0; i < a.tour.size(); ++i) {
        printf("%d ", a.wcet[i]);
        if (i < a.tour.size() - 1) printf("-> ");
    }
    printf("end\n");
}

/**
 * @brief Simulates the Ant Colony Optimization algorithm for a given number of iterations.
 *
 * This function simulates the Ant Colony Optimization algorithm for a given number of iterations.
 * It initializes the ants, runs the colony, calculates the fitness of each ant's tour, updates the pheromone matrix, and finds the ant with the highest fitness.
 * It also writes the results to a file and updates a progress bar.
 *
 * @param verbose A boolean value indicating whether to print verbose output.
 */
void ACO::simulate(bool verbose) {
    std::string funcName = graph.getMyName();
    std::string path = "./build/paths_" + funcName + ".txt";
    f = fopen(path.c_str(), "w");
    for (int t = 0; t < maxIter; ++t) {
        initializeAnts();
        runColony();
        calculateFitness();
        updatePhromone();
        wcet[t] = findQueen();
        updateProgressBar(t, maxIter);
        // printf("WCET: %d\n", wcet[t]);
    }
    fclose(f);
}

uint32_t ACO::getResults() {
    return colony.queen.fitness;
}

/**
 * @brief Updates the pheromone matrix (tau) based on the ant's tours.
 *
 * This function updates the pheromone matrix (tau) based on the tours of the ants in the colony.
 * It decreases the pheromone level of the edges used by the ants in their tours and increases the pheromone level of the edges not used.
 * The pheromone level is updated based on the fitness value of the ant's tour.
 * Additionally, it applies the pheromone evaporation factor (rho) to all edges in the matrix.
 */
void ACO::updatePhromone() {
    int i, j;
    int currentNode, nextNode;
    for (i = 0; i < antNo; ++i) {
        for (j = 0; j < colony.ant[i].tour.size() - 1; ++j) {
            currentNode = colony.ant[i].tour[j];
            nextNode = colony.ant[i].tour[j + 1];
            if (tau[nextNode - 1][currentNode + 1]) tau[nextNode - 1][currentNode + 1] -= 1.0 / colony.ant[i].fitness;
            // if(tau[nextNode][currentNode]) tau[nextNode][currentNode] += 1.0 / colony.ant[i].fitness;
        }
    }
    for (int i = 0; i < nodeNo; ++i)
        for (int j = 0; j < nodeNo; ++j) tau[i][j] *= (1 - rho);
}

/**
 * @brief Finds the ant with the highest fitness and updates the queen ant if necessary.
 *
 * This function finds the ant with the highest fitness value and updates the queen ant if its fitness value is surpassed.
 * The queen ant is a special ant that keeps track of the best tour found so far by the colony.
 *
 * @return The fitness value of the ant with the highest fitness.
 */
int ACO::findQueen() {
    double maxVal = -INFINITY;
    int maxIndex = 0;
    for (int i = 0; i < antNo; ++i) {
        if (colony.ant[i].fitness > maxVal) {
            maxVal = colony.ant[i].fitness;
            maxIndex = i;
        }
    }

    if (maxVal > colony.queen.fitness) {
        colony.queen.fitness = maxVal;
        colony.queen.tour.resize(colony.ant[maxIndex].tour.size());
        for (int j = 0; j < colony.ant[maxIndex].tour.size(); j++)
            colony.queen.tour[j] = colony.ant[maxIndex].tour[j];
    }
    return maxVal;
}

/**
 * @brief Prints the matrix of pheromone information (tau).
 *
 * This function prints the matrix of pheromone information (tau) used by the Ant Colony Optimization algorithm.
 * The matrix contains the pheromone information for each pair of nodes in the graph.
 *
 */
void ACO::printTau() {
    printf("Tau:\n");
    for (int i = 0; i < nodeNo; ++i) {
        for (int j = 0; j < nodeNo; ++j) printf("%f ", tau[i][j]);
        printf("\n");
    }
    printf("\n");
}

/**
 * @brief Prints the matrix of heuristic information (eta).
 *
 * This function prints the matrix of heuristic information (eta) used by the Ant Colony Optimization algorithm.
 * The matrix contains the heuristic information for each pair of nodes in the graph.
 *
 */
void ACO::printEta() {
    printf("Eta:\n");
    for (int i = 0; i < nodeNo; ++i) {
        for (int j = 0; j < nodeNo; ++j) printf("%f ", eta[i][j]);
        printf("\n");
    }
    printf("\n");
}