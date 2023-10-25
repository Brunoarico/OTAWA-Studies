
#include "wcet_dyn.h"

WCETCalculatorDyn::WCETCalculatorDyn(const std::string scriptPath, const std::string elfPath) {
    this->scriptPath = scriptPath;
    this->elfPath = elfPath;
}

/**
 * Flashes the binary file to the microcontroller by executing a command that takes two arguments: the path of the st-flash tool and the path of the binary file.
 * The output of the command is logged to the console.
 * If the command fails to execute, an error message is printed to the console.
 */
void WCETCalculatorDyn::flash() {
    printInfo("Flashing in hardware...");
    std::string cmd = std::string(STFLASH) + " write " + getFolder(BIN_FOLDER) + " 0x8000000";
    executeAndLog(cmd, DEBUG);
}

/**
 * Starts the OpenOCD server by executing a command that takes two arguments: the path of the OpenOCD executable and the path of the OpenOCD configuration file.
 * The output of the command is logged to the console.
 */
void WCETCalculatorDyn::startOpenocd() {
    printInfo("Starting OpenOCD...");
    std::string comandoOpenOCD = std::string(OPENOCD) + " -f " + getFolder(OPENOCD_CONFIG_FILE);
    executeAndLog(comandoOpenOCD.c_str(), DEBUG, false);
}

/**
 * Ends the OpenOCD server by executing a command that kills all running instances of OpenOCD.
 * The output of the command is not logged to the console.
 */
void WCETCalculatorDyn::endOpenocd() {
    std::string comandoOpenOCD = std::string(KILLALL) + " " + std::string(OPENOCD);
    executeAndLog(comandoOpenOCD.c_str(), DEBUG, false);
}

/**
 * Finds a specific substring in a given text and returns the integer value that follows it.
 * @param texto The text to search for the substring.
 * @param substring The substring to search for in the text.
 * @return The integer value that follows the substring if it is found in the text, otherwise -1.
 */
uint32_t WCETCalculatorDyn::findWcetWord(const std::string& texto, const std::string& substring) {
    size_t pos = texto.find(substring);
    if (pos != std::string::npos) {
        pos += substring.length();
        int wcet = std::stoi(texto.substr(pos));
        return wcet;
    } else
        return -1;
}

/**
 * Runs the GDB debugger to measure the dynamic worst-case execution time (WCET) of the program.
 * The GDB debugger is executed with a script file and the path of the ELF file to be debugged.
 * The output of the GDB debugger is parsed to find the WCET value.
 * @return The dynamic WCET value of the program.
 */
uint32_t WCETCalculatorDyn::runGDB() {
    printInfo("Running Dynamic mesure [it may take a time, go have a coffee]...");
    std::string cdm = std::string(GDB) + " -q -x " + getFolder(scriptPath) + " " + getFolder(elfPath);
    std::string out = executeAndLog(cdm, DEBUG);
    return findWcetWord(out, KEY_WORD_PARSE);
}

/**
 * Calculates the dynamic worst-case execution time (WCET) of the program by flashing the binary file to the microcontroller, starting the OpenOCD server, running the GDB debugger, and ending the OpenOCD server.
 * The WCET value is stored in the wcet member variable.
 */
void WCETCalculatorDyn::calculateWCET() {
    flash();
    std::thread openocd(startOpenocd);
    wcet = runGDB();
    endOpenocd();
    openocd.join();
}

/**
 * Returns the dynamic worst-case execution time (WCET) of the program.
 * @return The dynamic WCET value of the program in cpu cycles.
 */
uint32_t WCETCalculatorDyn::getWCET() {
    return wcet;
}