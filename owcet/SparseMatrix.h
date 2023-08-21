#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <vector>

class SparseMatrix {
private:
    struct SparseElement {
        int row;
        int col;
        int value;

        SparseElement(int r, int c, int v) : row(r), col(c), value(v) {}
    };

    std::vector<std::vector<SparseElement>> matrix;

public:
    void set(int row, int col, int value);
    int get(int row, int col);
    void print();
};

#endif // SPARSEMATRIX_H
