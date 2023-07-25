#include <elm/io.h>
#include <otawa/cfg.h>
#include <otawa/otawa.h>

#include <iostream>

#include "msgLogger.h"

#define ARGS_EXPECTED 2

using namespace elm;
using namespace otawa;
using namespace std;

PropList hardwareDef(String proc_path, String mem_path) {
    PropList props;
    PROCESSOR_PATH(props) = proc_path;
    MEMORY_PATH(props) = mem_path;
    return props;
}

WorkSpace *openElf(String path) {
    msgLogger::echoInfo("Opening file...");
    WorkSpace *ws = MANAGER.load(path);
    if (ws == NULL) msgLogger::echoError("Could not open file " + String(path));
    return ws;
}

int main(int argc, char **argv) {
    if (argc > ARGS_EXPECTED) {
        String path = argv[1];
        WorkSpace *ws = openElf(path);
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