#include "msgLogger.h"

#include <elm/io.h>
#include <iostream>

using namespace elm;


void msgLogger::echoError(String msg) {
    elm::cerr << "[!] Error: " << msg << io::endl;  
}

void msgLogger::echoInfo(String msg) {
    elm::cout << "[+] Info: " << msg << io::endl;  
}

void msgLogger::echoWarning(String msg) {
    elm::cout << "[!] Warning: " << msg << io::endl;  
}
