#ifndef CFGMATRIX_H
#define CFGMATRIX_H

#include <map>
#include "SparseMatrix.h"
#include <string>

class CfgMatrix {
private:
    SparseMatrix adjMatrixTotal;
    SparseMatrix adjMatrixLoop;
    std::map<unsigned int, int> iterations;
    void dfs_inner(int node, std::vector<int> visited, std::vector<int> currentPath);
    void dfs_cycles(int node, int start_node, std::vector<int>& path, std::vector<std::vector<int>>& cycles);

public:
    void addConv(int nodeA, int nodeB, int cycles, unsigned long addr);
    void addLoop(int nodeA, int nodeB, int cycles, unsigned long addr);
    void addObrigatoryPass(int node, int iterations);
    int getObrigatoryPass(int node);
    void exportCSVs();
    void exportDots();
    void printCycles();
    void printAddrs();
    void printIterations();
    void printOuts();
    void dfs(int node);
    std::vector<std::vector<int>> find_cycles_from_node(int start_node);
    //void exportJSON(const std::string& filename);
    void print_all_cycles();
};  

#endif // CFGMATRIX_H
