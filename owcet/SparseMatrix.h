#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <vector>
#include <string>

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
    bool hasNegativeValues();
    SparseMatrix subtract(SparseMatrix& other);
    void generateGraphImage(const std::string& file) const;
    void print();
    void exportToCSV(const std::string& filename);
    void importFromCSV(const std::string& filename);
};  

#endif // SPARSEMATRIX_H
