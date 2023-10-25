#ifndef WCET_DYNAMIC_H
#define WCET_DYNAMIC_H

#include <getopt.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "../utils/utilities.h"

#define KEY_WORD_PARSE "Wcet_Dynamic: "

// System Utilitaries
#define GDB "gdb-multiarch"
#define OPENOCD "openocd"
#define STFLASH "st-flash"
#define KILLALL "killall"

// Files
#define BIN_FOLDER "/build/main.bin"
#define OPENOCD_CONFIG_FILE "/stlink_bluepill.cfg"

class WCETCalculatorDyn {
   public:
    WCETCalculatorDyn(const std::string scriptPath, const std::string elfPath);
    void calculateWCET();
    uint32_t getWCET();

   private:
    void flash();
    static void startOpenocd();
    void endOpenocd();
    uint32_t runGDB();
    uint32_t findWcetWord(const std::string& texto, const std::string& substring);
    uint32_t wcet;
    std::string scriptPath;
    std::string elfPath;
};

#endif  // WCET_DYNAMIC_H