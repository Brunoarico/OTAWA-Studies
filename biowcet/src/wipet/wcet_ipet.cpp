#include "wcet_ipet.h"

/**
 * @brief Constructs a WCETCalculator object with the given WorkSpace.
 *
 * @param ws The WorkSpace object to use for calculating the WCET.
 */
WCETCalculator::WCETCalculator(otawa::WorkSpace *ws) {
    this->ws = ws;
}

/**
 * [HRIT01]
 * @brief Calculates the worst-case execution time (WCET) of the program.
 *
 * @throws elm::option::OptionException if the script file cannot be found.
 */
void WCETCalculator::calculateWCET() {
    wcet = otawa::ipet::WCET(ws);
}

/**
 * [HRIT01]
 * @brief Returns the worst-case execution time (WCET) of the program.
 *
 * @return The WCET in cpu cycles.
 */
uint32_t WCETCalculator::getWCET() {
    return wcet;
}