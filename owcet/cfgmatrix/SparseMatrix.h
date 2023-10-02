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
        int iter;
        unsigned long addr;

        SparseElement(int r, int c, int v) : row(r), col(c), value(v), iter(0), addr(0) {}
    };
    std::vector<std::vector<SparseElement>> matrix;
    bool exist(int row, int col);

public:
    void setCycle(int row, int col, int value);
    int getCycle(int row, int col);
    int getOuts(int node);
    void setAddr(int row, int col, unsigned long addr);
    int getAddr(int row, int col);
    int size();
    bool hasNegativeValues();
    SparseMatrix subtract(SparseMatrix& other);
    SparseMatrix sum(SparseMatrix& other);
    void generateGraphImage(const std::string& file) const;
    void printCycles();
    void printAddrs();
    void exportToCSV(const std::string& filename);
    void importFromCSV(const std::string& filename);
};  

#endif // SPARSEMATRIX_H
