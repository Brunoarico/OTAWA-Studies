#include "SparseMatrix.h"
#include <iostream>
#include <vector>

int main() {
    SparseMatrix sparse;
    std::cout<< "Teste de matriz esparsa" << std::endl;
    sparse.importFromCSV("./conv.csv");
    sparse.print();
    return 0;
}
