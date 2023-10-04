#include "CfgMatrix.h"
#include "SparseMatrix.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>


void CfgMatrix::addConv(int nodeA, int nodeB, int cycles){
    adjMatrixTotal.setCycle(nodeA, nodeB, cycles);
}

void CfgMatrix::addLoop(int nodeA, int nodeB, int cycles){
    adjMatrixLoop.setCycle(nodeA, nodeB, cycles);
}

void CfgMatrix::addObrigatoryPass(int node, int iterations){
    this->iterations[node] = iterations;
}

void CfgMatrix::setFuncName(int node, std::string name){
    adjMatrixTotal.setName(node, name);
}

void CfgMatrix::exportCSVs(std::string funcname) {
    adjMatrixLoop.exportToCSV(funcname + "_loop.csv");
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.exportToCSV(funcname +"_conv.csv");
}

void CfgMatrix::exportDots(std::string funcname) {
    adjMatrixLoop.generateGraphImage(funcname+"_loop.dot");
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.generateGraphImage(funcname+"_conv.dot");
    adjMatrixTotal.generateGraphImage(funcname+"_full.dot");
}

void CfgMatrix::printCycles() {
    std::cout << "---Conventional---" << std::endl;
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.printCycles();
    std::cout << "---Loop---" << std::endl;
    adjMatrixLoop.printCycles();
    std::cout << "---Full---" << std::endl;
    adjMatrixTotal.printCycles();
}

void CfgMatrix::printFunctions() {
    for (int i = 0; i < adjMatrixTotal.size(); ++i) {
        std::cout << "Node(" << i << ") = " << adjMatrixTotal.getName(i) << std::endl;
    }
}

void CfgMatrix::printIterations() {
    for (auto it = iterations.begin(); it != iterations.end(); ++it) {
        std::cout << "Iterations of node " << it->first << ": " << it->second << std::endl;
    }
}

int CfgMatrix::getObrigatoryPass(int node) {
    auto it = iterations.find(node);
    if (it != iterations.end()) {
        return it->second;
    }
    return 0;
}

void CfgMatrix::printOuts() {
    for (int i = 0; i < adjMatrixTotal.size(); ++i) {
        std::cout << "Outs(" << i << ") = " << adjMatrixTotal.getOuts(i) << std::endl;
    }
}

void CfgMatrix::dfs_cycles(int node, int start_node, std::vector<int>& path, std::vector<std::vector<int>>& cycles) {

    path.push_back(node);

    for (int neighbor = 0; neighbor < adjMatrixTotal.size(); ++neighbor) {
        if (adjMatrixTotal.getCycle(node,neighbor) > 0) {
            if (neighbor == start_node && path.size() > 1) {
                // Encontramos um ciclo dirigido
                cycles.push_back(path);
            }
            else if (find(path.begin(), path.end(), neighbor) == path.end()) {
                dfs_cycles(neighbor, start_node, path, cycles);
            }
        }
    }

    path.pop_back();
}

std::vector<std::vector<int>> CfgMatrix::find_cycles_from_node(int start_node) {
    std::vector<std::vector<int>> cycles;
    std::vector<int> path;
    int num_nodes = adjMatrixTotal.size();


    dfs_cycles(start_node, start_node, path, cycles);

    return cycles;
}

void CfgMatrix::print_all_cycles() {
    std::vector<std::vector<int>> cycles;
    for (int start_node = 0; start_node < adjMatrixTotal.size(); ++start_node) {
        if(getObrigatoryPass(start_node) ){
            cycles = find_cycles_from_node(start_node);
            std::cout << "Cycles Found:" << std::endl;
            for (const std::vector<int>& cycle : cycles) {
                for (int node : cycle) {
                    std::cout << node << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}