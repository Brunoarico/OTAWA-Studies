#ifndef UTILITIES_H
#define UTILITIES_H

#include <unistd.h>

#include <iostream>

// system utilitaries
#define CP "cp"

#define DEBUG 1

void printError(std::string msg);
void printInfo(std::string msg);
void printResult(std::string msg);
std::string executeAndLog(std::string command, bool print = true, bool getError = true);
void copyToDir(std::string source, std::string dest);
bool directoryChange(std::string newDir);
std::string getFolder(std::string str);
std::string execute(std::string cmd, bool getError = true);

#endif  // UTILITIES_H
