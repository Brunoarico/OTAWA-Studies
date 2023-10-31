#ifndef UTILITIES_H
#define UTILITIES_H

#include <unistd.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

// system utilitaries
#define CP "cp"

void printError(std::string msg);
void printInfo(std::string msg, bool force = false);
void printResult(std::string msg);
void updateProgressBar(int progress, int total);
std::string executeAndLog(std::string command, bool getError = true);
void copyToDir(std::string source, std::string dest);
bool directoryChange(std::string newDir);
std::string getFolder(std::string str);
std::string execute(std::string cmd, bool getError = true);
void setVerbose(bool verbose);
bool getVerbose();
int getTerminalWidth();


#endif  // UTILITIES_H
