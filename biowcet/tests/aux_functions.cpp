#include <iostream>
#include <string>
#include <cstdio>
#include "../src/utils/utilities.h"
#define TO_FF "../din//xml2ff.py"
#define BUILD_FOLDER "../din/build"
#define DINAMIC_FOLDER "../din"
#define MAKE "make"

void genff() {
    printf("Smart Generating of ff file...");
    std::string command = std::string(TO_FF) +" "+ std::string(BUILD_FOLDER);
    FILE* in = popen(command.c_str(), "r");
    if (!in)
        printf("Failed to execute command: ");
    else {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), in) != NULL) printf("%s", buffer);
        pclose(in);
    }
}

void compile() {
    printInfo("Compiling...");
    if (directoryChange(DINAMIC_FOLDER))
        executeAndLog(MAKE);
    else {
        printError("Error in directory change");
        exit(1);
    }
}

void clearDir() {
    printInfo("Cleaning directory");
    if (directoryChange(DINAMIC_FOLDER)) {
        std::string cmd = MAKE + std::string(" clean");
        executeAndLog(cmd);
    } else
        exit(1);
}


