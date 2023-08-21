#include "SparseMatrix.h"
#include <iostream>
#include <vector>

int main() {
    SparseMatrix sparse;

    sparse.set(0, 1, 5);
    sparse.set(1, 2, 8);
    sparse.set(2, 0, 3); 
    sparse.print();
    return 0;
}
