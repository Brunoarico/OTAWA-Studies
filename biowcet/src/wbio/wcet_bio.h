#ifndef WCET_BIO_H
#define WCET_BIO_H

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <vector>

#include "../aco/aco.h"
#include "../cfgmatrix/CfgMatrix.h"


class WCETCalculatorBio {
   public:
    WCETCalculatorBio(std::set<CfgMatrix> cfgSet);
    void calculateWCET();
    uint32_t getWCET();

   private:
    bool replaceDependencies(CfgMatrix *c);
    uint32_t wcet;
    std::set<CfgMatrix> cfgSet;
    std::queue<CfgMatrix> pq;
    std::unordered_map<int, int> cfgMap;

    double alpha = 1;  // Phromone exponential parameters
    double beta = 1;   // Desirability exponential parameter
};

#endif  // WCET_CALCULATOR_H
