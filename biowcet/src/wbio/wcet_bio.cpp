#include "wcet_bio.h"

/**
 * @brief Constructor for the WCETCalculatorBio class.
 *
 * This constructor initializes the cfgSet member variable with the provided set of Control Flow Graph (CFG) matrices.
 *
 * @param cfgSet The set of CFG matrices.
 */
WCETCalculatorBio::WCETCalculatorBio(std::set<CfgMatrix> cfgSet) {
    this->cfgSet = cfgSet;
}

/**
 * @brief Calculates the WCET (Worst-Case Execution Time) for each function in the program.
 *
 * This function calculates the WCET for each function in the program using the Ant Colony Optimization (ACO) algorithm.
 * It first converts the Control Flow Graph (CFG) information to a matrix and sets the conversion time for each block.
 * Then, it iterates over the priority queue of CFG matrices and applies the ACO algorithm to each matrix.
 * Finally, it stores the WCET for each function in the cfgMap.
 */
void WCETCalculatorBio::calculateWCET() {
    for (const CfgMatrix& elem : cfgSet) pq.push(elem);
    while (!pq.empty()) {
        CfgMatrix c = pq.front();
        pq.pop();
        int maxIter = pow(c.getSize(), 0.5);
        int antNo = 10;
        float rho = 0.5;
        if (c.getPriority() > 0 && !replaceDependencies(&c)) {
            pq.push(c);
        } else {
            c.printCycles();
            ACO aco(c, antNo, 0, maxIter, alpha, beta, rho);
            aco.simulate();
            wcet = aco.getResults();
            cfgMap[c.getMyHashName()] = wcet;
        }
    }
}

/**
 * @brief Replaces the dependencies in the Control Flow Graph (CFG) matrix with their corresponding function names.
 *
 * This function replaces the dependencies in the CFG matrix with their corresponding function names using the cfgMap.
 * It iterates through the matrix and checks if the value is negative, indicating a function call.
 * If it is a function call, it replaces the value with the corresponding function name from the cfgMap.
 *
 * @param c The CfgMatrix pointer.
 */
bool WCETCalculatorBio::replaceDependencies(CfgMatrix* c) {
    for (int i = 0; i < c->getSize(); i++) {
        for (int j = 0; j < c->getSize(); j++) {
            int value = c->getCycles(i, j);
            if (value < 0) {                  // isfunc
                if (cfgMap.count(value) > 0)  // issolved
                    c->setConv(i, j, cfgMap[value]);
                else
                    return false;
            }
        }
    }
    return true;
}

/**
 * @brief Returns the Worst Case Execution Time (WCET) calculated by the calculateWCET() function.
 *
 * @return The WCET value.
 */
uint32_t WCETCalculatorBio::getWCET() {
    return wcet;
}