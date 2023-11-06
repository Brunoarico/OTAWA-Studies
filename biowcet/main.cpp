#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include <getopt.h>
#include <unistd.h>

#include <boost/asio.hpp>
#include <libgen.h>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <chrono>

#include "src/utils/utilities.h"
#include "src/wbio/wcet_bio.h"
#include "src/wdyn/wcet_dyn.h"
#include "src/wipet/wcet_ipet.h"
#include "src/cfgGen/cfgGen.h"


const char *asciiArt =
    " _______   ______   ______   __       __   ______   ________  ________ \n"
    "/       \\ /      | /      \\ /  |  _  /  | /      \\ /        |/        |\n"
    "$$$$$$$  |$$$$$$/ /$$$$$$  |$$ | / \\ $$ |/$$$$$$  |$$$$$$$$/ $$$$$$$$/ \n"
    "$$ |__$$ |  $$ |  $$ |  $$ |$$ |/$  \\$$ |$$ |  $$ |$$ |__       $$ |   \n"
    "$$    $$<   $$ |  $$ |  $$ |$$ /$$$  $$ |$$ |      $$    |      $$ |   \n"
    "$$$$$$$  |  $$ |  $$ |  $$ |$$ $$/$$ $$ |$$ |   __ $$$$$/       $$ |   \n"
    "$$ |__$$ | _$$ |_ $$ \\__$$ |$$$$/  $$$$ |$$ \\__/  |$$ |_____    $$ |   \n"
    "$$    $$/ / $$   |$$    $$/ $$$/    $$$ |$$    $$/ $$       |   $$ |   \n"
    "$$$$$$/  $$$$$$/  $$$$$$/  $$/      $$/  $$$$$$/  $$$$$$$$/    $$/    \n"
    "   v1.1                                                                \n"
    "                                                                       \n";

// System Utilitaries
#define MAKE "make"
#define REALPATH "realpath "

// Scripts
#define DEBUG_FILE "/hwdebug.py"
#define TO_FF "/xml2ff.py"
// Files
#define DINAMIC_FOLDER "/din"
#define MAKE_FOLDER "/din/Makefile"
#define DEST_FOLDER "/src/main.c"
#define BUILD_FOLDER "/build"
#define ELF_OTAWA_FOLDER "/build/main_otawa.elf"
#define ELF_FOLDER "/build/main.elf"
#define RESULT_FILE "/results.txt"

//bool VERBOSE = false;

/**
 * Shows the usage of the program.
 * @param programName The name of the program.
 */
void showUsage(const char* programName) {
    //printf("Usage: %s -s [archPath] -p [elfPath] [-f function]\n", programName);
    printf("Usage: %s -s archPath -p cFile [-d] [-v] [-h]\n", programName);
    printf("FLAGS:\n");
    printf("\t-s\t\tPath to the architecture description file\n");
    printf("\t-p\t\tPath to the .c file\n");
    printf("\t-d\t\tEnable dynamic analysis\n");
    printf("\t-v\t\tEnable verbose mode\n");
    printf("\t-h\t\tShow this help message\n");
    printf("EXAMPLES:\n");
    printf("\t%s -s trivial -p /test_files/if/main.c\n", programName);
    printf("\t%s -s trivial -p /test_files/if/main.c -v \n", programName);
    printf("\t%s -s trivial -p /test_files/if/main.c -v -d\n", programName);
}

/**
 * Compiles the project by changing the current working directory to the project directory and executing the make command.
 * If the directory change is successful, the make command is executed and the output is logged to the console.
 * If the directory change fails, the program exits with an error code.
 */
void compile() {
    printInfo("Compiling...");
    if (directoryChange(getFolder(DINAMIC_FOLDER)))
        executeAndLog(MAKE);
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
    if (directoryChange(getFolder(DINAMIC_FOLDER))) {
        std::string cmd = MAKE + std::string(" clean");
        executeAndLog(cmd);
    } else
        exit(1);
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
    if (!in)
        printError("Failed to execute command: " + command);
    else {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), in) != NULL) printf("%s", buffer);
        pclose(in);
    }
}

std::string absolutePath(std::string path) {
    return executeAndLog(REALPATH + path);
}

/**
 * Writes the results of the WCET analysis to a file.
 * 
 * @param dir The directory where the results file will be saved.
 * @param wceti The WCET value obtained by the IPET analysis.
 * @param wcetb The WCET value obtained by the BIO analysis.
 * @param wcetd The WCET value obtained by the dynamic analysis.
 */
void toFile(std::string dir, std::string entry, uint32_t wceti, uint32_t wcetb, uint32_t wcetd, bool dyn) {
    std::string outname = dir + RESULT_FILE;
    printInfo("Writing results to " + outname);
    FILE* outfile = fopen(outname.c_str(), "w");


    if(wceti >= 0) fprintf(outfile, "WCET_IPET[%s] = %d cycles\n", entry.c_str(), wceti);

    if(wcetb >= 0) fprintf(outfile, "WCET_BIO[%s] = %d cycles\n", entry.c_str(), wcetb);

    if(wcetd >= 0 && dyn) fprintf(outfile, "WCET_DYNAMIC[%s] = %d cycles\n", entry.c_str(), wcetd);

    fclose(outfile);
}


/**
 * The main function of the program.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line arguments.
 * @return An integer representing the exit status of the program.
 */
int main(int argc, char* argv[]) {
    std::string scriptPath;
    std::string entry = "main";
    std::string cPath;
    std::string resolvedPath;
    bool dyn = false;
    char* dir;
    otawa::WorkSpace *ws;
    std::chrono::high_resolution_clock::time_point startI, endI, startB, endB, startD, endD;
    std::chrono::duration<double, std::milli> elapsedD, elapsedI, elapsedB;


    uint32_t wcet_d = -1;
    uint32_t wcet_i = -1;
    uint32_t wcet_b = -1;

    int option;
    
    while ((option = getopt(argc, argv, "s:p:vdh")) != -1) {
        switch (option) {
            case 's':
                scriptPath = optarg;
                break;
            case 'p':
                cPath = optarg;
                break;
            case 'd':
                dyn = true;
                break;
            case 'v':
                setVerbose(true);
                break;
            case 'h':
                showUsage(argv[0]);
                return 0;
            default:
                showUsage(argv[0]);
                return 1;
        }
    }
    printf("%s\n",asciiArt);
    clearDir();
    copyToDir(cPath, DEST_FOLDER);
    resolvedPath = absolutePath(cPath).c_str();
    dir = dirname(&resolvedPath[0]);
    compile();
    genff();

    cfgGen otawaInstance(scriptPath, entry, getFolder(ELF_OTAWA_FOLDER));

    WCETCalculator wcetIpet(otawaInstance.workspaceGenerator());
    printInfo("Calculating WCET using IPET...", true);
    startI = std::chrono::high_resolution_clock::now();
    wcetIpet.calculateWCET();
    endI = std::chrono::high_resolution_clock::now();
    wcet_i = wcetIpet.getWCET();
    elapsedI = endI - startI;

    WCETCalculatorBio wcetBio(otawaInstance.cfg2Matrix());
    printInfo("Calculating WCET using ACO...", true);
    startB = std::chrono::high_resolution_clock::now();
    wcetBio.calculateWCET();
    endB = std::chrono::high_resolution_clock::now();
    wcet_b = wcetBio.getWCET();
    elapsedB = endB - startB;

    if (dyn) {
        WCETCalculatorDyn wcetDyn(DEBUG_FILE, ELF_FOLDER);
        printInfo("Calculating Execution Time using the Hardware...", true);
        startD = std::chrono::high_resolution_clock::now();
        wcetDyn.calculateWCET();
        endD = std::chrono::high_resolution_clock::now();
        wcet_d = wcetDyn.getWCET();
        elapsedD = endD - startD;
    }
   
    
    

    if (wcet_i == -1)
        printError("No WCET_IPET computed");
    else{
        printResult("WCET_IPET[" + std::string(entry) + "] = " + std::to_string(wcet_i) + " cycles");
        printInfo("IPET consumed " + std::to_string(elapsedI.count()) + " ms", true);
    }

    if (wcet_b == -1)
        printError("No WCET_BIO computed");
    else{
        printResult("WCET_BIO[" + std::string(entry) + "] = " + std::to_string(wcet_b) + " cycles");
        printInfo("BIO consumed " + std::to_string(elapsedB.count()) + " ms", true);
    }


    if(dyn) {
        if (wcet_d == -1)
            printError("No WCET_DYNAMIC computed");
        else{
            printResult("WCET_DYNAMIC[" + std::string(entry) + "] = " + std::to_string(wcet_d) + " cycles");
            printInfo("DYNAMIC consumed " + std::to_string(elapsedD.count()) + " ms", true);
        }
    }

    toFile(dir, entry, wcet_i, wcet_b, wcet_d, dyn);

    return 0;
}