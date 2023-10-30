#include "CfgMatrix.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstring>

#include "SparseMatrix.h"

/**
 * @brief Sets the number of cycles between two nodes in the conventional control flow graph.
 *
 * @param nodeA The starting node.
 * @param nodeB The ending node.
 * @param cycles The number of cycles between the two nodes.
 */
void CfgMatrix::setConv(int nodeA, int nodeB, int cycles) {
    adjMatrixTotal.setCycle(nodeA, nodeB, cycles);
}

/**
 * @brief Sets the number of cycles between two nodes in the loop control flow graph.
 *
 * @param nodeA The starting node.
 * @param nodeB The ending node.
 * @param cycles The number of cycles between the two nodes.
 */
void CfgMatrix::setLoop(int nodeA, int nodeB, int cycles) {
    adjMatrixLoop.setCycle(nodeA, nodeB, cycles);
}

void CfgMatrix::setPriority(int priority) {
    this->priority = priority;
}

/**
 * @brief Sets the number of iterations for a given node.
 *
 * @param node The node to set the number of iterations for.
 * @param iterations The number of iterations for the node.
 */
void CfgMatrix::setIteration(int node, int iterations) {
    this->iterations[node] = iterations;
}

/**
 * @brief Sets the name of a given node.
 *
 * @param node The node to set the name for.
 * @param name The name to set for the node.
 */
void CfgMatrix::setFuncCallName(int node, std::string name) {
    adjMatrixTotal.setName(node, name);
}

void CfgMatrix::setMyFunc(std::string name) {
    this->myFuckName = name;
}

/**
 * @brief Gets the number of cycles between two nodes in the conventional control flow graph.
 *
 * @param nodeA The starting node.
 * @param nodeB The ending node.
 * @return The number of cycles of block B.
 */
int CfgMatrix::getCycles(int nodeA, int nodeB) {
    return adjMatrixTotal.getCycle(nodeA, nodeB);
}

/**
 * @brief Gets the number of outgoing edges from a given node in the conventional control flow graph.
 *
 * @param node The node to get the number of outgoing edges from.
 * @return The number of outgoing edges from the node.
 */
int CfgMatrix::getOuts(int node) {
    return adjMatrixTotal.getOuts(node);
}

int CfgMatrix::getMyHashName() {
    std::hash<std::string> hasher;
    return -1 * std::abs(int(hasher(myFuckName)));
}

std::string CfgMatrix::getMyName() {
    return myFuckName;
}


/**
 * @brief Gets the size of the total adjacency matrix.
 *
 * @return The size of the total adjacency matrix.
 */
size_t CfgMatrix::getSize() {
    return adjMatrixTotal.size();
}


/**
 * @brief Gets the name of a given node.
 *
 * @param node The node to get the name for.
 * @return The name of the node.
 */
std::string CfgMatrix::getBlockName(int node) {
    return adjMatrixTotal.getName(node);
}

int CfgMatrix::getPriority() {
    return priority;
}

/**
 * @brief Gets the hash value of the name of a given node.
 *
 * @param node The node to get the hash value for.
 * @return The hash value of the name of the node.
 */
int CfgMatrix::getBlockNameHash(int node) {
    std::hash<std::string> hasher;
    return -1 * std::abs(int(hasher(getBlockName(node))));
}

/**
 * @brief Gets the number of iterations for a given node.
 *
 * @param node The node to get the number of iterations for.
 * @return The number of iterations for the node.
 */
int CfgMatrix::getIteration(int node) {
    auto it = iterations.find(node);
    if (it != iterations.end()) {
        return it->second;
    }
    return 0;
}

int CfgMatrix::isAloop(int node) {
    int loops = 0;
    for (int i =0; i< getSize(); i++){
        if (adjMatrixLoop.getCycle(i, node) != 0) {
            loops++;
            break;
        }
    }
    return loops;
}


/**
 * @brief Gets all cycles starting from a given node.
 *
 * @param start_node The node to start the search from.
 * @return A vector of vectors, where each inner vector represents a cycle found.
 */
std::vector<std::vector<int>> CfgMatrix::get_cycles_from_node(int start_node) {
    std::vector<std::vector<int>> cycles;
    std::vector<int> path;
    int num_nodes = adjMatrixTotal.size();
    
    dfs_cycles(start_node, start_node, path, cycles);

    return cycles;
}

/**
 * @brief Prints all cycles in the control flow graph.
 */
void CfgMatrix::printCycles() {
    std::cout << "---Conventional---" << std::endl;
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.printCycles();
    std::cout << "---Loop---" << std::endl;
    adjMatrixLoop.printCycles();
    std::cout << "---Full---" << std::endl;
    adjMatrixTotal.printCycles();
}

/**
 * @brief Prints the name of each node in the control flow graph.
 */
void CfgMatrix::printFunctions() {
    for (int i = 0; i < adjMatrixTotal.size(); ++i) {
        std::cout << "Node(" << i << ") = " << adjMatrixTotal.getName(i) << std::endl;
    }
}

/**
 * @brief Prints the number of iterations for each node in the control flow graph.
 */
void CfgMatrix::printIterations() {
    for (auto it = iterations.begin(); it != iterations.end(); ++it) {
        std::cout << "Iterations of node " << it->first << ": " << it->second << std::endl;
    }
}

/**
 * @brief Prints the number of outgoing edges from each node in the control flow graph.
 */
void CfgMatrix::printOuts() {
    for (int i = 0; i < adjMatrixTotal.size(); ++i) {
        std::cout << "Outs(" << i << ") = " << adjMatrixTotal.getOuts(i) << std::endl;
    }
}

/**
 * @brief Prints all cycles starting from nodes with non-zero number of iterations.
 */
void CfgMatrix::printAllLoops() {
    std::vector<std::vector<int>> cycles;
    for (int start_node = 0; start_node < adjMatrixTotal.size(); ++start_node) {
        if (isAloop(start_node)) {
            cycles = get_cycles_from_node(start_node);
            std::cout << "Cycles Found:" << std::endl;
            for (const std::vector<int>& cycle : cycles) {
                
                for (int node : cycle) {
                    std::cout << node << " " ;
                }
                int deps = checkNeastedLoops(cycle);
                if(deps == -1) std::cout << " (invalid)"<< std::endl;
                else std::cout << " deps: " << deps << std::endl;
            }
        }
    }
}

/**
 * @brief Finds the head of the loop that contains the given start node.
 *
 * @param start_node The node to start the search from.
 * @return The head of the loop that contains the given start node, or -1 if the node is not part of a loop.
 */
int CfgMatrix::findLoopHead(int start_node) {
    std::vector<std::vector<int>> cycles;
    if (isAloop(start_node)) {
        cycles = get_cycles_from_node(start_node);
        for (const std::vector<int>& cycle : cycles) {
            int deps = checkNeastedLoops(cycle);
            if(deps > -1) return cycle[1];
        }
        return -1;
    }
    else return -1;
}


int CfgMatrix::loopHead(int node) {
    if(getIteration(node) < 0) return -1;
    else return findLoopHead(node);
}

/**
 * @brief Exports the conventional and loop control flow graphs to CSV files.
 *
 * @param funcname The name of the function to use in the CSV file names.
 */
void CfgMatrix::exportCSVs(std::string funcname) {
    adjMatrixLoop.exportToCSV(funcname + "_loop.csv");
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.exportToCSV(funcname + "_conv.csv");
}

/**
 * @brief Exports the conventional, loop, and full control flow graphs to DOT files.
 *
 * @param funcname The name of the function to use in the DOT file names.
 */
void CfgMatrix::exportDots(std::string funcname) {
    adjMatrixLoop.generateGraphImage(funcname + "_loop.dot");
    SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
    adjMatrixConv.generateGraphImage(funcname + "_conv.dot");
    adjMatrixTotal.generateGraphImage(funcname + "_full.dot");
}

/**
 * @brief Finds all cycles starting from a given node using depth-first search.
 *
 * @param node The current node being visited.
 * @param start_node The starting node of the search.
 * @param path The current path being taken.
 * @param cycles A vector of vectors, where each inner vector represents a cycle found.
 */
void CfgMatrix::dfs_cycles(int node, int start_node, std::vector<int>& path, std::vector<std::vector<int>>& cycles) {
    path.push_back(node);

    for (int neighbor = 0; neighbor < adjMatrixTotal.size(); ++neighbor) {
        if (adjMatrixTotal.getCycle(node, neighbor) > 0) {
            if (neighbor == start_node && path.size() > 1) {
                // We find a directed cycle
                cycles.push_back(path);
            } else if (find(path.begin(), path.end(), neighbor) == path.end()) {
                dfs_cycles(neighbor, start_node, path, cycles);
            }
        }
    }
    path.pop_back();
}

int CfgMatrix::checkNeastedLoops(std::vector<int> loop) {
    int dependency = 0;
    for (int i = 1; i < loop.size(); i++) {
        if(isAloop(loop[i])) {
            if(loop[i] <= loop[0]) //invalid dependency
                return -1; 
            if (loop[i]) 
                dependency++;
        }
    }
    return dependency;
}



