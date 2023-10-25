#include "utilities.h"

/**
 * Prints an error message to the console with red text.
 * @param msg The error message to print.
 */
void printError(std::string msg) {
    printf("\033[1;31m[ERROR]:\033[0m %s\n", msg.c_str());
}

/**
 * Prints an informational message to the console with yellow text.
 * @param msg The informational message to print.
 */
void printInfo(std::string msg) {
    printf("\033[0;33m[INFO]:\033[0m %s\n", msg.c_str());
}

/**
 * Prints a result message to the console with green text.
 * @param msg The result message to print.
 */
void printResult(std::string msg) {
    printf("\033[0;32m[RESULT]:\033[0m %s\n", msg.c_str());
}

/**
 * Executes a command and returns the output as a string.
 * @param cmd The command to execute.
 * @param getError Whether to print the error output to the console or not. Default is true.
 * @return The output of the command as a string.
 */
std::string execute(std::string cmd, bool getError) {
    std::string data;
    FILE* stream;
    const int max_buffer = 2048;
    char buffer[max_buffer];
    cmd.append(" 2>&1");
    stream = popen(cmd.c_str(), "r");
    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        if (pclose(stream) && getError) {
            printError("Error in: " + cmd);
            printError(data);
            exit(1);
        }
    }
    return data;
}

/**
 * Executes a command and returns the output as a string.
 * @param command The command to execute.
 * @param print Whether to print the command to the console or not. Default is true.
 * @param getError Whether to print the error output to the console or not. Default is true.
 * @return The output of the command as a string.
 */
std::string executeAndLog(std::string command, bool print, bool getError) {
    std::string buffer;
    if (print) printInfo("command: " + command);
    buffer = execute(command, getError);
    return buffer;
}

/**
 * Copies a file to a directory relative to the parent directory of the current working directory.
 * @param source The path of the file to copy.
 */
void copyToDir(std::string source, std::string dest) {
    char cwd[256];
    getcwd(cwd, 256);
    std::string newDir = cwd + std::string("/..");
    if (chdir(newDir.c_str()) == 0) {
        std::string cwd_str = std::string(cwd);
        std::string cmd = CP + std::string(" '") + source + std::string("' '") + cwd_str + dest + std::string("'");
        executeAndLog(cmd.c_str(), DEBUG);
    }
}

/**
 * Changes the current working directory to a folder relative to the current working directory.
 * @return True if the directory change was successful, false otherwise.
 */
bool directoryChange(std::string newDir) {
    if (chdir(newDir.c_str()) == 0) {
        printInfo("Directory changed");
        return 1;
    } else {
        printError("Error in directory change");
        return 0;
    }
}

/**
 * Returns the full path of a folder relative to the current working directory.
 * @param str The relative path of the folder.
 * @return The full path of the folder.
 */
std::string getFolder(std::string str) {
    char cwd[256];
    std::string cmd;
    getcwd(cwd, 256);
    return std::string(cwd) + std::string(str.c_str());
}
