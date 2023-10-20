#include "CfgMatrix.h"
#include "SparseMatrix.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <functional>
#include <cmath>

/*------------------- set functions -------------------*/
void CfgMatrix::setConv(int nodeA, int nodeB, int cycles){
    adjMatrixTotal.setCycle(nodeA, nodeB, cycles);
}

void CfgMatrix::setLoop(int nodeA, int nodeB, int cycles){
    adjMatrixLoop.setCycle(nodeA, nodeB, cycles);
}

void CfgMatrix::setIteration(int node, int iterations){
    this->iterations[node] = iterations;
}

void CfgMatrix::setFuncName(int node, std::string name){
    adjMatrixTotal.setName(node, name);
}

/*------------------- get functions -------------------*/
int CfgMatrix::getCycles(int nodeA, int nodeB){
    return adjMatrixTotal.getCycle(nodeA, nodeB);
}

int CfgMatrix::getOuts(int node){
    return adjMatrixTotal.getOuts(node);
}

std::string CfgMatrix::getBlockName(int node){
    return adjMatrixTotal.getName(node);
}

int CfgMatrix::getBlockNameHash(int node){
    std::hash<std::string> hasher;
    return -1 * std::abs(int(hasher(getBlockName(node))));
}

int CfgMatrix::getIteration(int node) {
    auto it = iterations.find(node);
    if (it != iterations.end()) {
        return it->second;
    }
    return 0;
}

std::vector<std::vector<int>> CfgMatrix::get_cycles_from_node(int start_node) {
    std::vector<std::vector<int>> cycles;
    std::vector<int> path;
    int num_nodes = adjMatrixTotal.size();

    dfs_cycles(start_node, start_node, path, cycles);

    return cycles;
}

/*------------------- print functions -------------------*/
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

void CfgMatrix::printOuts() {
    for (int i = 0; i < adjMatrixTotal.size(); ++i) {
        std::cout << "Outs(" << i << ") = " << adjMatrixTotal.getOuts(i) << std::endl;
    }
}

void CfgMatrix::print_all_cycles() {
    std::vector<std::vector<int>> cycles;
    for (int start_node = 0; start_node < adjMatrixTotal.size(); ++start_node) {
        if(getIteration(start_node) ){
            cycles = get_cycles_from_node(start_node);
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


/*------------------- export functions -------------------*/
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

/*------------------- private functions -------------------*/

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