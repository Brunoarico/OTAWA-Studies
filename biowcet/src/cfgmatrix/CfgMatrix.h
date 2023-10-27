#ifndef CFGMATRIX_H
#define CFGMATRIX_H

#include <map>
#include <string>

#include "SparseMatrix.h"

class CfgMatrix {
   private:
    SparseMatrix adjMatrixTotal;
    SparseMatrix adjMatrixLoop;
    std::map<unsigned int, int> iterations;

    void dfs_cycles(int node, int start_node, std::vector<int>& path, std::vector<std::vector<int>>& cycles);

   public:
    void setConv(int nodeA, int nodeB, int cycles);  // seta uma aresta que conecta dois nos no sentido convencional
    void setLoop(int nodeA, int nodeB, int cycles);  // seta uma aresta de loopback
    void setIteration(int node, int iterations);     // seta a quantidade de vezes que determinado bloco itera
    void setFuncName(int node, std::string name);    // seta o nome de um bloco

    int getIteration(int node);           // devolve a quantidade de vezes que determinado bloco itera
    std::string getBlockName(int node);   // devolve o nome de um bloco caso seja funcao
    int getCycles(int nodeA, int nodeB);  // devolve a quantidade de ciclos entre dois blocos
    int getOuts(int node);                // devolve a quantidade de saidas de um bloco
    int getBlockNameHash(int node);       // devolve o hash do nome de um bloco
    size_t getSize();                     // devolve o tamanho da matriz (quantidade de blocos)

    void exportCSVs(std::string funcname);  // exporta os csvs relativos ao cfg
    void exportDots(std::string funcname);  // exporta o dot file relativo ao cfg
    void printCycles();                     // imprime os ciclos consumidos por cada bloco
    void printFunctions();                  // imprime o nome de cada bloco
    void printIterations();
    void printOuts();                                                    // imprime a partir de um bloco para quai ele tem saida
    std::vector<std::vector<int>> get_cycles_from_node(int start_node);  // encontra todos os ciclos que passam por um determinado no
    void print_all_cycles();                                             // imprime todos os ciclos do cfg
};

#endif  // CFGMATRIX_H
