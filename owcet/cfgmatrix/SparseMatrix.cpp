#include "SparseMatrix.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>

std::string csvDir = "./csv/";
std::string dotDir = "./dot/";
std::string pngDir = "./png/";
std::string mkImgDir = "mkdir -p "+ dotDir + " " + pngDir;
std::string mkCsvDir = "mkdir -p "+ csvDir;

void SparseMatrix::setCycle(int row, int col, int value) {
    if (row < 0 || col < 0) return;
    if (row >= matrix.size()) matrix.resize(row + 1);
    if (value != 0) {
        if (col >= matrix[row].size()) matrix[row].resize(col + 1, SparseElement(row, col, 0));
        matrix[row][col] = SparseElement(row, col, value);
    }
}

int SparseMatrix::getCycle(int row, int col) {
    if (row < 0 || col < 0 || row >= matrix.size()) return 0; // Invalid input or default value
    if (col >= matrix[row].size()) return 0; // Default value for non-existing elements
    return matrix[row][col].value;
}

bool SparseMatrix::exist(int row, int col) {
    if (row < 0 || col < 0 || row >= matrix.size() || col >= matrix[row].size()) return false; // Invalid input
    else if (matrix[row][col].value == 0) return false; // Default value for non-existing elements
    else return true;
}

void SparseMatrix::setName(int node, std::string name) {
    func[node] = name;
}

std::string SparseMatrix::getName(int node) {
    if(func.find(node) == func.end()) return "-";
    else return func[node];
}

int SparseMatrix::size() {
    return matrix.size();
}


SparseMatrix SparseMatrix::subtract(SparseMatrix& other){
    SparseMatrix result;

    int numRows = std::max(matrix.size(), other.matrix.size());
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numRows; ++col) {
            int value = getCycle(row, col) - other.getCycle(row, col);
            result.setCycle(row, col, value);
        }
    }

    return result;
}

SparseMatrix SparseMatrix::sum(SparseMatrix& other){
    SparseMatrix result;

    int numRows = std::max(matrix.size(), other.matrix.size());
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numRows; ++col) {
            int value = getCycle(row, col) + other.getCycle(row, col);
            result.setCycle(row, col, value);
        }
    }

    return result;
}

bool SparseMatrix::hasNegativeValues(){
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            if (element.value < 0) return true;
        }
    }
    return false;
}

void SparseMatrix::generateGraphImage(const std::string& file) const{

    system(mkImgDir.c_str());

    std::ofstream dotFile(dotDir + file);
    
    if (!dotFile.is_open()) {
        std::cout << "Error opening dot file." << std::endl;
        return;
    }

    dotFile << "digraph G {" << std::endl;
    for (int row = 0; row < matrix.size(); ++row) {
        for (int col = 0; col < matrix[row].size(); ++col) {
            if (matrix[row][col].value != 0) {
                dotFile << "   \""+std::to_string(row)+":"+func[row]+"\" -> " << "\""+std::to_string(col)+":"+ func[col]+"\"" << " [label=" << matrix[row][col].value << "];" << std::endl;
                //std::cout << row <<" " << col << " " << matrix[row][col].value << std::endl;
            }
        }
    }
 
    dotFile << "}" << std::endl;
    dotFile.close();

    // Generate the PNG image using Graphviz
    std::string command = "dot -Tpng " + dotDir + file + " -o " + pngDir + file.substr(0, file.size() - 4) +".png";
    std::cout << command << std::endl;
    system(command.c_str());

    std::cout << "Graph image generated." << std::endl;
}

void SparseMatrix::printCycles() {
    int numRows = matrix.size();
    int numCols = 0;
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            numCols = std::max(numCols, element.col + 1);
        }
    }

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int value = getCycle(row, col);
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}


int SparseMatrix::getOuts(int node) {
    int out = 0;
    for (int col = 0; col < matrix[node].size(); ++col) {
        if (matrix[node][col].value != 0) out++;
    }
    return out;
}

void SparseMatrix::exportToCSV(const std::string& filename) {
    system(mkCsvDir.c_str());
    std::ofstream csvFile(csvDir + filename);

    if (!csvFile.is_open()) {
        std::cerr << "Error to open CSV file to write." << std::endl;
        return;
    }

    for (size_t row = 0; row < matrix.size(); ++row) {
        for (size_t col = 0; col < matrix[row].size(); ++col) {
            int value = matrix[row][col].value;
            if (value != 0) {
                csvFile << row << "," << col << "," << value << "\n";
            }
        }
    }

    csvFile.close();
}


void SparseMatrix::importFromCSV(const std::string& filename) {
    std::ifstream csvFile(filename);

    if (!csvFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo CSV para leitura." << std::endl;
        return;
    }
    else {
        std::cout << "Arquivo CSV aberto com sucesso." << std::endl;
    }

    int value1, value2, value3;
    char comma; // Para consumir as vírgulas

    while (csvFile >> value1 >> comma >> value2 >> comma >> value3) {
        setCycle(value1, value2, value3);
    }

    csvFile.close();
}
