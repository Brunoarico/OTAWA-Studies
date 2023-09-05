#include "CfgMatrix.h"
#include "SparseMatrix.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


void CfgMatrix::addConv(int nodeA, int nodeB, int cycles, unsigned long addr){
    adjMatrixTotal.setCycle(nodeA, nodeB, cycles);
    adjMatrixTotal.setAddr(nodeA, nodeB, addr);
}

void CfgMatrix::addLoop(int nodeA, int nodeB, int cycles, unsigned long addr){
    adjMatrixLoop.setCycle(nodeA, nodeB, cycles);
    adjMatrixLoop.setAddr(nodeA, nodeB, addr);
}

void CfgMatrix::addObrigatoryPass(int node, int iterations){
    this->iterations[node] = iterations;
}

void CfgMatrix::exportCSVs() {
    adjMatrixLoop.exportToCSV("loop.csv");
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.exportToCSV("conv.csv");
}

void CfgMatrix::exportDots() {
    adjMatrixLoop.generateGraphImage("loop.dot");
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.generateGraphImage("conv.dot");
    adjMatrixTotal.generateGraphImage("full.dot");
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

void CfgMatrix::printAddrs() {
    std::cout << "---Conventional---" << std::endl;
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.printAddrs();
    std::cout << "---Loop---" << std::endl;
    adjMatrixLoop.printAddrs();
    std::cout << "---Full---" << std::endl;
    adjMatrixTotal.printAddrs();
}

void CfgMatrix::printIterations() {
    for (auto it = iterations.begin(); it != iterations.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }
}

int CfgMatrix::getObrigatoryPass(int node) {
    auto it = iterations.find(node);
    if (it != iterations.end()) {
        return it->second;
    }
    return 0;
}






void CfgMatrix::dfs_inner(int node, std::vector<int> visited, std::vector<int> currentPath) {
    visited[node]++;
    currentPath.push_back(node);

    if (adjMatrixTotal.getOuts(node) == 0) {
        std::cout << "Caminho encontrado: ";
        int mmax = 0;
        for (int i = 0; i < currentPath.size(); ++i) {
            std::cout << currentPath[i];
            if (i < currentPath.size() - 1) {
                mmax += adjMatrixTotal.getCycle(currentPath[i], currentPath[i+1]);
                std::cout << " -> ";
            }
        }
        std::cout << " (max: " << mmax << ")";
        std::cout << std::endl;
    }
    
    for (int neighbor = 0; neighbor < adjMatrixTotal.size(); ++neighbor) {
        if (adjMatrixTotal.getCycle(node,neighbor) && visited[neighbor] < 5) {
            dfs_inner(neighbor, visited, currentPath);
        }
    }
    
    visited[node] = 0; // Desfaz a marcação ao retroceder na recursão
    currentPath.pop_back();
}

void CfgMatrix::dfs(int node) {
    std::vector<int> visited(adjMatrixTotal.size(), 0);
    std::vector<int> currentPath;
    dfs_inner(node, visited, currentPath);
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


    std::cout << "Ciclos encontrados:" << std::endl;
    for (const std::vector<int>& cycle : cycles) {
        for (int node : cycle) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }

    return cycles;
}
void CfgMatrix::print_all_cycles() {
    for (int start_node = 0; start_node < adjMatrixTotal.size(); ++start_node) {
        if(getObrigatoryPass(start_node))
            find_cycles_from_node(start_node);
    }
}