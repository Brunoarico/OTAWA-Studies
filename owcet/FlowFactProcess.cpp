#include "FlowFactProcess.h"
#include <iostream>
#include <fstream>
#include <string>
#include <regex>

void FlowFactProcess::load(const std::string& filename) {
    size_t lastSlashPos = filename.find_last_of('/');
    std::string result = filename.substr(lastSlashPos, filename.size()-1);

    std::ifstream file(filename + result + ".ff");
    std::regex loopRegex(R"(loop\s+"[^"]+"\s+\+\s0x[0-9a-fA-F]+\s(\d+)\s;\s\/\/\s(0x[0-9a-fA-F]+))");

    if (!file.is_open()) {
        std::cerr << "Cannot found the .ff file" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::smatch match;
        if (std::regex_search(line, match, loopRegex)) {
            unsigned int addr = std::stoul(match[2], nullptr, 16);
            dict[addr] = std::stoul(match[1], nullptr, 16);
        }
    }
    file.close();
}

void FlowFactProcess::print() {
    for (auto it = dict.begin(); it != dict.end(); ++it) {
        std::cout << it->first << " " << it->second << std::endl;
    }
}

int FlowFactProcess::get(unsigned int addr) {
    auto it = dict.find(addr);
    if (it != dict.end()) {
        //std::cout << "Found: " << it->first << " " << it->second << std::endl;
        return it->second;
    }
    return 0;
}

