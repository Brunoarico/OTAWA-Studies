#include <iostream>
#include <string>
#include <cstdio>
#define TO_FF "../din//xml2ff.py"
#define BUILD_FOLDER "../din/build"

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


