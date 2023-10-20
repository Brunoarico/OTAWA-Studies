#ifndef WCET_BIO_H
#define WCET_BIO_H

#include <elm/option/StringList.h>
#include <elm/sys/System.h>

#include <otawa/app/Application.h>
#include <otawa/cfg/features.h>
#include <otawa/ilp/System.h>

#include <otawa/events/features.h>
#include <otawa/hard/Processor.h>
#include <otawa/ilp.h>
#include <otawa/ipet/features.h>
#include <otawa/stats/BBStatCollector.h>

#include <otawa/etime/features.h>
#include <otawa/ipet/WCETComputation.h>

#include <otawa/ipet/BasicObjectFunctionBuilder.h>
#include <otawa/script/Script.h>
#include <otawa/stats/StatInfo.h>
#include <otawa/util/BBRatioDisplayer.h>
#include <otawa/flowfact/FlowFactLoader.h>        

#include <unordered_set>
#include <vector>
#include "../cfgmatrix/CfgMatrix.h"

using namespace otawa;

class WCETCalculatorBio {
public:
    WCETCalculatorBio(const elm::string scriptPath, const elm::string entryFunction, const elm::string elfPath);
    void calculateWCET();
    uint32_t getWCET();

private:
    uint32_t blockTime(WorkSpace *ws, Block *b);
    void cfg2Matrix(WorkSpace *ws);
    elm::string scriptPath;
    elm::string entryFunction;
    elm::string elfPath;
    uint32_t wcet;
    WorkSpace *ws;
};

#endif // WCET_CALCULATOR_H
