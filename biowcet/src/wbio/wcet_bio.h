#ifndef WCET_BIO_H
#define WCET_BIO_H

#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include <otawa/app/Application.h>
#include <otawa/cfg/LoopUnroller.h>
#include <otawa/cfg/features.h>
#include <otawa/etime/features.h>
#include <otawa/events/features.h>
#include <otawa/flowfact/FlowFactLoader.h>
#include <otawa/hard/Processor.h>
#include <otawa/ilp.h>
#include <otawa/ilp/System.h>
#include <otawa/ipet/BasicObjectFunctionBuilder.h>
#include <otawa/ipet/WCETComputation.h>
#include <otawa/ipet/features.h>
#include <otawa/script/Script.h>
#include <otawa/stats/BBStatCollector.h>
#include <otawa/stats/StatInfo.h>
#include <otawa/util/BBRatioDisplayer.h>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <vector>

#include "../aco/aco.h"
#include "../cfgmatrix/CfgMatrix.h"

using namespace otawa;

class WCETCalculatorBio {
   public:
    WCETCalculatorBio(const std::string scriptPath, const std::string entryFunction, const std::string elfPath);
    void calculateWCET();
    uint32_t getWCET();

   private:
    bool replaceDependencies(CfgMatrix *c);
    uint32_t blockTime(WorkSpace *ws, Block *b);
    void cfg2Matrix(WorkSpace *ws);
    elm::string scriptPath;
    elm::string entryFunction;
    elm::string elfPath;
    uint32_t wcet;
    WorkSpace *ws;
    std::queue<CfgMatrix> pq;
    std::unordered_map<int, int> cfgMap;

    double alpha = 1;  // Phromone exponential parameters
    double beta = 1;   // Desirability exponential parameter
};

#endif  // WCET_CALCULATOR_H
