#include "wcet_ipet.h"

using namespace otawa;
using namespace elm::option;

WCETCalculator::WCETCalculator(const elm::string scriptPath, const elm::string entryFunction, const elm::string elfPath) {
        this->scriptPath = scriptPath;
        this->entryFunction = entryFunction;
        this->elfPath = elfPath;
}

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

uint32_t WCETCalculator::getWCET() {
        return wcet;
}


