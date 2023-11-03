#ifndef CFGOTAWA_H
#define CFGOTAWA_H

#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include <otawa/app/Application.h>
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
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <vector>

#include "../utils/utilities.h"
#include "../cfgmatrix/CfgMatrix.h"

class cfgGen {
   public:
    cfgGen(const std::string scriptPath, const std::string entryFunction, const std::string elfPath);
    std::set<CfgMatrix> cfg2Matrix();
    otawa::WorkSpace *workspaceGenerator();

   private:
    uint32_t blockTime(otawa::Block *b);
    elm::string scriptPath;
    elm::string entryFunction;
    elm::string elfPath;
    otawa::WorkSpace *ws;
};

#endif
