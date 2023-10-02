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
    void dfs_cycles(int node, int start_node, std::vector<int>& path, std::vector<std::vector<int>>& cycles);

public:
    void addConv(int nodeA, int nodeB, int cycles, unsigned long addr); //seta uma aresta que conecta dois nos no sentido convencional
    void addLoop(int nodeA, int nodeB, int cycles, unsigned long addr); //seta uma aresta de loopback
    void addObrigatoryPass(int node, int iterations); // seta a quantidade de vezes que determinado bloco itera
    int getObrigatoryPass(int node); // devolve a quantidade de vezes que determinado bloco itera
    void exportCSVs();  // exporta os csvs relativos ao cfg
    void exportDots();  // exporta o dot file relativo ao cfg
    void printCycles(); // imprime os ciclos consumidos por cada bloco
    void printAddrs(); // imprime o endereco de cada bloco
    void printIterations(); 
    void printOuts(); // imprime a partir de um bloco para quai ele tem saida
    std::vector<std::vector<int>> find_cycles_from_node(int start_node); // encontra todos os ciclos que passam por um determinado no
    void print_all_cycles(); // imprime todos os ciclos do cfg
};  

#endif // CFGMATRIX_H
