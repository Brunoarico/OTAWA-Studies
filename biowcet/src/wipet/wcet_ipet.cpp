#include "wcet_ipet.h"

using namespace otawa;
using namespace elm::option;


/**
 * @brief Constructs a WCETCalculator object with the given WorkSpace.
 * 
 * @param ws The WorkSpace object to use for calculating the WCET.
 */
WCETCalculator::WCETCalculator(otawa::WorkSpace *ws) {
    this->ws = ws;
}

/**
 * @brief Calculates the worst-case execution time (WCET) of the program.
 * 
 * @throws elm::option::OptionException if the script file cannot be found.
 */
void WCETCalculator::calculateWCET() {
        /*elm::Path path = this->scriptPath;
        elm::string script = this->scriptPath;
        elm::string entry = this->entryFunction;
        PropList props;

        if (!path.exists() && !path.isAbsolute()) {
            elm::Path file = script;
            if (file.extension() != "osx")
                file = file.setExtension("osx");
            bool found = false;
            elm::string paths = MANAGER.buildPaths("../../share/Otawa/scripts", "");
            for (elm::Path::PathIter p(paths); p(); p++) {
                path = elm::Path(*p) / file;
                if (path.exists()) {
                    found = true;
                    break;
                }
            }
            if (!found)
                printError("Architecture description file not found");
        }
        else printError("Architecture description file not found");
        TASK_ENTRY(props) = entry;
        script::PATH(props) = path;
        script::Script *scr = new script::Script();
        otawa::WorkSpace *ws = MANAGER.load(this->elfPath, props);
        ws->run(scr, props);*/
        wcet = ipet::WCET(ws);
}

/**
 * @brief Returns the worst-case execution time (WCET) of the program.
 * 
 * @return The WCET in cpu cycles.
 */
uint32_t WCETCalculator::getWCET() {
        return wcet;
}
