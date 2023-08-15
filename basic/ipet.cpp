#include <elm/io.h>
#include <otawa/cfg.h>
#include <otawa/ipet.h>
#include <otawa/otawa.h>

#include <elm/sys/System.h>

#include <otawa/app/Application.h>
#include <otawa/cfg/features.h>
#include <otawa/ilp/System.h>
#include <otawa/ipet/IPET.h>
#include <otawa/script/Script.h>
#include <otawa/stats/StatInfo.h>
#include <otawa/util/BBRatioDisplayer.h>
#include <otawa/flowfact/FlowFactLoader.h>


#include <iostream>

#include "msgLogger.h"

#define ARGS_EXPECTED 2

using namespace elm;
using namespace otawa;
using namespace std;


PropList hardwareDef(String hardware_path) {
    PropList props;
    PROCESSOR_PATH(props) = hardware_path+"/processor.xml";
    MEMORY_PATH(props) = hardware_path+"/memory.xml";
    VERBOSE(props) = false;
    return props;
}

WorkSpace *openElf(String path, PropList props) {
    msgLogger::echoInfo("Opening file...");
    WorkSpace *ws = MANAGER.load(path, props);
    if (ws == NULL) msgLogger::echoError("Could not open file " + String(path));
    return ws;
}


void process(PropList props) {
    Processor::COLLECT_STATS(props) = true;
}


int main(int argc, char **argv) {
    if (argc > ARGS_EXPECTED) {
        String elfPath = argv[1];
        String hardPath = argv[2];
        WorkSpace *ws = openElf(elfPath, hardwareDef(hardPath));
        if (ws == NULL) return EXIT_FAILURE;
        const otawa::CFGCollection *cfgs = INVOLVED_CFGS(ws);

        if (cfgs == NULL) {
            msgLogger::echoError("Could not get CFGs");
            return EXIT_FAILURE;
        }

        msgLogger::echoInfo("File opened successfully");
        return EXIT_SUCCESS;

    } else {
        msgLogger::echoError("Error: Usage- ipet <elf path> <hardware_folder path>");
        return EXIT_FAILURE;
    }
}