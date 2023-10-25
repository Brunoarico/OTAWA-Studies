#include "wcet_ipet.h"

using namespace otawa;
using namespace elm::option;

/**
 * @brief Constructor for the WCETCalculator class.
 * 
 * @param scriptPath The path to the script file.
 * @param entryFunction The name of the entry function.
 * @param elfPath The path to the ELF file.
 */
WCETCalculator::WCETCalculator(const std::string scriptPath, const std::string entryFunction, const std::string elfPath) {
        this->scriptPath = elm::string(scriptPath.c_str());
        this->entryFunction = elm::string(entryFunction.c_str());
        this->elfPath = elm::string(elfPath.c_str());
}

/**
 * @brief Calculates the worst-case execution time (WCET) of the program.
 * 
 * @throws elm::option::OptionException if the script file cannot be found.
 */
void WCETCalculator::calculateWCET() {
        elm::Path path = this->scriptPath;
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
                throw elm::option::OptionException(_ << "cannot find script " << script);
        }
        TASK_ENTRY(props) = entry;
        script::PATH(props) = path;
        script::Script *scr = new script::Script();
        otawa::WorkSpace *ws = MANAGER.load(this->elfPath, props);
        ws->run(scr, props);
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
