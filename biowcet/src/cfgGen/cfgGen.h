#ifndef CFGOTAWA_H
#define CFGOTAWA_H

#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include <otawa/etime/features.h>
#include <otawa/flowfact/FlowFactLoader.h>
#include <otawa/ilp/System.h>
#include <otawa/ipet/features.h>
#include <otawa/script/Script.h>

#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>

#include "../cfgmatrix/CfgMatrix.h"
#include "../utils/utilities.h"

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
