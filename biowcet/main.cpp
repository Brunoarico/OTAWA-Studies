#include <iostream>
#include <getopt.h>

#include <unistd.h>
#include <thread>

#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include <boost/asio.hpp>
#include <csignal>
#include "src/wipet/wcet_ipet.h"
#include "src/wbio/wcet_bio.h"
#include "src/wdyn/wcet_dyn.h"
#include "src/utils/utilities.h"



//System Utilitaries
#define MAKE "make"

//Scripts
#define DEBUG_FILE "/hwdebug.py"
#define TO_FF "/xml2ff.py" 
//Files
#define DINAMIC_FOLDER "/din"
#define MAKE_FOLDER "/din/Makefile"
#define DEST_FOLDER "/src/main.c"
#define BUILD_FOLDER "/build"  
#define ELF_OTAWA_FOLDER "/build/main_otawa.elf"
#define ELF_FOLDER "/build/main.elf"

/**
 * Shows the usage of the program.
 * @param programName The name of the program.
 */
void showUsage(const char* programName) {
    printf("Uso: %s -s archPath -p elfPath [-f function]\n", programName);
}

/**
 * Compiles the project by changing the current working directory to the project directory and executing the make command.
 * If the directory change is successful, the make command is executed and the output is logged to the console.
 * If the directory change fails, the program exits with an error code.
 */
void compile() {
    printInfo("Compiling...");
    if(directoryChange(getFolder(DINAMIC_FOLDER))) executeAndLog(MAKE, DEBUG);
    else {
        printError("Error in directory change");
        exit(1);
    }
}

/**
 * Cleans the project directory by changing the current working directory to the project directory and executing the make clean command.
 * If the directory change is successful, the make clean command is executed and the output is logged to the console.
 * If the directory change fails, the program exits with an error code.
 */
void clearDir() {
    printInfo("Cleaning directory");
    if(directoryChange(getFolder(DINAMIC_FOLDER))) {
        std::string cmd = MAKE +  std::string(" clean");
        executeAndLog(cmd, DEBUG);
    } 
    else exit(1);
}

/**
 * Generates an ff file by executing a command that takes two arguments: the path of the ff file generator (a python script) and the path of the build folder.
 * The output of the command is logged to the console.
 * If the command fails to execute, an error message is printed to the console.
 */
void genff() {
    printInfo("Smart Generating of ff file...");
    std::string command = std::string(getFolder(TO_FF)) + " " + std::string(getFolder(BUILD_FOLDER));
    FILE* in = popen(command.c_str(), "r");
    if (!in) printError("Failed to execute command: " + command);
    else {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), in) != NULL) printf("%s", buffer);
        pclose(in);
    }
}

int main(int argc, char* argv[]) {
    std::string scriptPath;
    std::string entry = "main";
    std::string cPath;
    bool dyn = false;

    uint32_t wcet_d = -1;
    uint32_t wcet_i = -1;
    uint32_t wcet_b = -1;

    int option;
    while ((option = getopt(argc, argv, "s:p:f:d")) != -1) {
        switch (option) {
            case 's':
                scriptPath = optarg;
                break;
            case 'p':
                cPath = optarg;
                break;
            case 'f':
                entry = optarg;
                break;
            case 'd':
                dyn = true;
                break;
            default:
                showUsage(argv[0]);
                return 1;
        }
    }
    clearDir();
    copyToDir(cPath, DEST_FOLDER);
    compile();
    genff();
    

    WCETCalculator wcetIpet(scriptPath, entry, getFolder(ELF_OTAWA_FOLDER));
    wcetIpet.calculateWCET();
    wcet_i = wcetIpet.getWCET();
    
    WCETCalculatorBio wcetBio(scriptPath, entry, getFolder(ELF_OTAWA_FOLDER));
    wcetBio.calculateWCET();
    wcet_b = wcetBio.getWCET();
    
    if(dyn) {
        WCETCalculatorDyn wcetDyn(DEBUG_FILE, ELF_FOLDER);
        wcetDyn.calculateWCET();
        wcet_d = wcetDyn.getWCET();
    }

    if (wcet_i == -1) printError("No WCET_IPET computed");
    else printResult("WCET_IPET[" + std::string(entry) + "] = " + std::to_string(wcet_i) + " cycles");
    
    if (wcet_b == -1) printError("No WCET_BIO computed");
    else printResult("WCET_BIO[" + std::string(entry) + "] = " + std::to_string(wcet_b) + " cycles");
    
    if (wcet_d == -1) printError("No WCET_DYNAMIC computed");
    else printResult("WCET_DYNAMIC[" + std::string(entry) + "] = " + std::to_string(wcet_d) + " cycles");
    
    return 0;
}