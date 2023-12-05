#include "CfgMatrix.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "../utils/utilities.h"
#include "SparseMatrix.h"

/**
 * [HRG04]
 * [HRG05]
 * @brief Sets the number of cycles between two nodes in the conventional control flow graph.
 *
 * @param nodeA The starting node.
 * @param nodeB The ending node.
 * @param cycles The number of cycles between the two nodes.
 */
void CfgMatrix::setConv(int nodeA, int nodeB, int cycles) {
    if (cycles > maxCycle) maxCycle = cycles;
    adjMatrixTotal.setCycle(nodeA, nodeB, cycles);
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Sets the number of cycles between two nodes in the loop control flow graph.
 *
 * @param nodeA The starting node.
 * @param nodeB The ending node.
 * @param cycles The number of cycles between the two nodes.
 */
void CfgMatrix::setLoop(int nodeA, int nodeB, int cycles) {
    if (cycles > maxCycle) maxCycle = cycles;
    adjMatrixLoop.setCycle(nodeA, nodeB, cycles);
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Sets the priority
*/
void CfgMatrix::setPriority(int priority) {
    this->priority = priority;
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Sets the number of iterations for a given node.
 *
 * @param node The node to set the number of iterations for.
 * @param iterations The number of iterations for the node.
 */
void CfgMatrix::setIteration(int node, int iterations) {
    this->iterations[node] = iterations;
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Sets the name of a given node.
 *
 * @param node The node to set the name for.
 * @param name The name to set for the node.
 */
void CfgMatrix::setFuncCallName(int node, std::string name) {
    adjMatrixTotal.setName(node, name);
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Sets the name of a function.
 */
void CfgMatrix::setMyFunc(std::string name) {
    this->myFuckName = name;
}

/**
 * [HRG04]
 * [HRG05]
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
 * [HRG04]
 * [HRG05]
 * @brief Gets the number of outgoing edges from a given node in the conventional control flow graph.
 *
 * @param node The node to get the number of outgoing edges from.
 * @return The number of outgoing edges from the node.
 */
int CfgMatrix::getOuts(int node) {
    return adjMatrixTotal.getOuts(node);
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Gets the hash of a function.
 */
int CfgMatrix::getMyHashName() {
    std::hash<std::string> hasher;
    return -1 * std::abs(int(hasher(myFuckName)));
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Gets the name of a function.
 */
std::string CfgMatrix::getMyName() {
    return myFuckName;
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Gets the size of the total adjacency matrix.
 *
 * @return The size of the total adjacency matrix.
 */
size_t CfgMatrix::getSize() {
    return adjMatrixTotal.size();
}

/**
 * [HRG04]
 * [HRG05]
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

int CfgMatrix::getMaxCycle() {
    return maxCycle;
}

/**
 * [HRG04]
 * [HRG05]
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
 * [HRG04]
 * [HRG05]
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

/**
 * [HRG04]
 * [HRG05]
 * @brief Gets the number of loops.
 */
int CfgMatrix::isAloop(int node) {
    int loops = 0;
    for (int i = 0; i < getSize(); i++) {
        if (adjMatrixLoop.getCycle(i, node) != 0) {
            loops++;
            break;
        }
    }
    return loops;
}

/**
 * [HRG04]
 * [HRG05]
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
    if (getVerbose()) {
        printf("---Conventional---\n");
        SparseMatrix adjMatrixConv = adjMatrixTotal.subtract(adjMatrixLoop);
        adjMatrixConv.printCycles();

        printf("---Loop---\n");
        adjMatrixLoop.printCycles();

        printf("---Full---\n");
        adjMatrixTotal.printCycles();
    }
}

/**
 * @brief Prints the name of each node in the control flow graph.
 */
void CfgMatrix::printFunctions() {
    for (int i = 0; i < adjMatrixTotal.size(); ++i) {
        if (getVerbose()) printf("Node(%d) = %s\n", i, adjMatrixTotal.getName(i).c_str());
    }
}

/**
 * @brief Prints the number of iterations for each node in the control flow graph.
 */
void CfgMatrix::printIterations() {
    for (auto it = iterations.begin(); it != iterations.end(); ++it) {
        if (getVerbose()) printf("Iterations of node %d: %d\n", it->first, it->second);
    }
}

/**
 * @brief Prints the number of outgoing edges from each node in the control flow graph.
 */
void CfgMatrix::printOuts() {
    for (int i = 0; i < adjMatrixTotal.size(); ++i) {
        if (getVerbose()) printf("Outs(%d) = %d\n", i, adjMatrixTotal.getOuts(i));
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
            if (getVerbose()) printf("Cycles Found:\n");
            for (const std::vector<int>& cycle : cycles) {
                for (int node : cycle)
                    if (getVerbose()) printf("%d ", node);
                int deps = checkNeastedLoops(cycle);
                if (deps == -1)
                    if (getVerbose())
                        printf(" (invalid)\n");
                    else if (getVerbose())
                        printf(" (deps: %d)\n", deps);
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
            if (deps > -1) return cycle[1];
        }
        return -1;
    } else
        return -1;
}

/**
 * [HRG04]
 * [HRG05]
 * @brief Finds the head of the loop that contains the given node, if it exists and has a positive number of iterations.
 *
 * @param node The node to start the search from.
 * @return The head of the loop that contains the given node, or -1 if the node is not part of a loop or has a non-positive number of iterations.
 */
int CfgMatrix::loopHead(int node) {
    if (getIteration(node) < 0)
        return -1;
    else
        return findLoopHead(node);
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
 * [HRG04]
 * [HRG05]
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

/**
 * [HRG04]
 * [HRG05]
 * @brief Checks the number of nested loops in a given loop.
 *
 * @param loop A vector of integers representing the nodes in the loop.
 * @return The number of nested loops in the given loop, or -1 if there is an invalid dependency.
 */
int CfgMatrix::checkNeastedLoops(std::vector<int> loop) {
    int dependency = 0;
    for (int i = 1; i < loop.size(); i++) {
        if (isAloop(loop[i])) {
            if (loop[i] <= loop[0])  // invalid dependency
                return -1;
            if (loop[i])
                dependency++;
        }
    }
    return dependency;
}