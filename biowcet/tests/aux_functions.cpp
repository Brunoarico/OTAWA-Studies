#include <iostream>
#include <string>
#include <cstdio>
#include "../src/utils/utilities.h"
#include "../src/cfgGen/cfgGen.h"
#define TO_FF "../din//xml2ff.py"
#define BUILD_FOLDER "../din/build"
#define DINAMIC_FOLDER "../din"
#define MAKE "make"
#define RESULT_FILE "/results.txt"

void genff()
{
    std::string command = std::string(TO_FF) + " " + std::string(BUILD_FOLDER);
    FILE *in = popen(command.c_str(), "r");
    if (!in)
        printf("Failed to execute command: ");
    else
    {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), in) != NULL)
            printf("%s", buffer);
        pclose(in);
    }
}

void compile()
{
    printInfo("Compiling...");
    if (directoryChange(DINAMIC_FOLDER))
        executeAndLog(MAKE);
    else
    {
        printError("Error in directory change");
        exit(1);
    }
}

void clearDir()
{
    printInfo("Cleaning directory");
    if (directoryChange(DINAMIC_FOLDER))
    {
        std::string cmd = MAKE + std::string(" clean");
        executeAndLog(cmd);
    }
    else
        exit(1);
}

void toFile(std::string dir, std::string entry, uint32_t wceti, uint32_t wcetb, uint32_t wcetd, bool dyn) {
    std::string outname = dir + RESULT_FILE;
    printInfo("Writing results to " + outname);
    FILE* outfile = fopen(outname.c_str(), "w");


    if(wceti >= 0) fprintf(outfile, "WCET_IPET[%s] = %d cycles\n", entry.c_str(), wceti);

    if(wcetb >= 0) fprintf(outfile, "WCET_BIO[%s] = %d cycles\n", entry.c_str(), wcetb);

    if(wcetd >= 0 && dyn) fprintf(outfile, "WCET_DYNAMIC[%s] = %d cycles\n", entry.c_str(), wcetd);

    fclose(outfile);
}




