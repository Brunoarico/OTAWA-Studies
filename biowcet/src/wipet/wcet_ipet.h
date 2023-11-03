#ifndef WCET_IPET_H
#define WCET_IPET_H

#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include <otawa/ipet/WCETComputation.h>
#include <otawa/ipet/features.h>

#include <iostream>

#include "../utils/utilities.h"

class WCETCalculator {
   public:
    WCETCalculator(otawa::WorkSpace *ws);
    void calculateWCET();
    uint32_t getWCET();

   private:
    otawa::WorkSpace *ws;
    uint32_t wcet;
};

#endif  // WCET_CALCULATOR_H