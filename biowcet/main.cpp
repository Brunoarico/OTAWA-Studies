#include <iostream>
#include <getopt.h>
#include <elm/option/StringList.h>
#include <elm/sys/System.h>
#include "wipet/wcet_ipet.h"
#include "wbio/wcet_bio.h"


void showUsage(const char* programName) {
    std::cerr << "Uso: " << programName << " -s archPath -p elfPath [-f function]" << std::endl;
}

int main(int argc, char* argv[]) {
    elm::string scriptPath;
    elm::string entry = "main";
    elm::string elfPath;

    // Processar argumentos da linha de comando
    int option;
    while ((option = getopt(argc, argv, "s:p:f:")) != -1) {
        switch (option) {
            case 's':
                scriptPath = optarg;
                break;
            case 'p':
                elfPath = optarg;
                break;
            case 'f':
                entry = optarg;
                break;
            default:
                showUsage(argv[0]);
                return 1;
        }
    }

    WCETCalculator wcetIpet(scriptPath, entry, elfPath);
    wcetIpet.calculateWCET();
    uint32_t wcet_i = wcetIpet.getWCET();

    WCETCalculatorBio wcetBio(scriptPath, entry, elfPath);
    wcetBio.calculateWCET();
    uint32_t wcet_b = wcetBio.getWCET();

    if (wcet_i == -1) std::cerr << "ERROR: no WCET_IPET computed" << std::endl;
    else std::cout << "WCET_IPET[" << entry << "] = " << wcet_i << " cycles" << std::endl;
    

    

    if (wcet_b == -1) std::cerr << "ERROR: no WCET_BIO computed" << std::endl;
    else std::cout << "WCET_BIO[" << entry << "] = " << wcet_b << " cycles" << std::endl;

    return 0;
}