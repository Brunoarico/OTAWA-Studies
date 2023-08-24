#include "SparseMatrix.h"
#include <iostream>
#include <fstream>
#include <string>

void SparseMatrix::set(int row, int col, int value) {
    if (row < 0 || col < 0) {
        return; // Invalid input
    }

    if (row >= matrix.size()) {
        matrix.resize(row + 1);
    }

    if (value != 0) {
        if (col >= matrix[row].size()) {
            matrix[row].resize(col + 1, SparseElement(row, col, 0));
        }
        matrix[row][col] = SparseElement(row, col, value);
    }
}

int SparseMatrix::get(int row, int col) {
    if (row < 0 || col < 0 || row >= matrix.size()) {
        return 0; // Invalid input or default value
    }

    if (col >= matrix[row].size()) {
        return 0; // Default value for non-existing elements
    }

    return matrix[row][col].value;
}

void SparseMatrix::print() {
    int numRows = matrix.size();
    int numCols = 0;
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            numCols = std::max(numCols, element.col + 1);
        }
    }

    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int value = get(row, col);
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

void SparseMatrix::exportToCSV(const std::string& filename) {
    std::ofstream csvFile(filename);

    if (!csvFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo CSV para escrita." << std::endl;
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
        set(value1, value2, value3);
    }

    csvFile.close();
}
