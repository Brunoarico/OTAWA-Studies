#ifndef WCET_IPET_H
#define WCET_IPET_H

#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include <otawa/ipet/features.h>
#include <otawa/ipet/WCETComputation.h>
#include <otawa/script/Script.h>


class WCETCalculator {
public:
    WCETCalculator(const elm::string scriptPath, const elm::string entryFunction, const elm::string elfPath);
    void calculateWCET();
    uint32_t getWCET();

private:
    elm::string scriptPath;
    elm::string entryFunction;
    elm::string elfPath;
    uint32_t wcet;
};

#endif // WCET_CALCULATOR_H