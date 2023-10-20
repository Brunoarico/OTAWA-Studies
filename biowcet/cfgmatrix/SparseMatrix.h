#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <vector>
#include <string>
#include <map>

class SparseMatrix {
private:
    struct SparseElement {
        int row;
        int col;
        int value;
        int iter;
        std::string name;

        SparseElement(int r, int c, int v) : row(r), col(c), value(v), iter(0), name("") {}
    };
    
    std::vector<std::vector<SparseElement>> matrix;
    mutable std::map<unsigned int, std::string> func;


    bool exist(int row, int col);

public:
    void setCycle(int row, int col, int value);
    int getCycle(int row, int col);
    int getOuts(int node);
    void setName(int node, std::string name);
    std::string getName(int node);
    int size();
    bool hasNegativeValues();
    SparseMatrix subtract(SparseMatrix& other);
    SparseMatrix sum(SparseMatrix& other);
    void generateGraphImage(const std::string& file) const;
    void printCycles();
    void exportToCSV(const std::string& filename);
    void importFromCSV(const std::string& filename);
};  

#endif // SPARSEMATRIX_H
