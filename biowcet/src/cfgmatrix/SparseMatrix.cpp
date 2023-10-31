#include "SparseMatrix.h"

#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

/**
 * @brief Directory where CSV files will be stored.
 */
std::string csvDir = "./csv/";

/**
 * @brief Directory where DOT files will be stored.
 */
std::string dotDir = "./dot/";

/**
 * @brief Directory where PNG files will be stored.
 */
std::string pngDir = "./png/";

/**
 * @brief Command to create directories for DOT and PNG files.
 */
std::string mkImgDir = "mkdir -p " + dotDir + " " + pngDir;

/**
 * @brief Command to create directory for CSV files.
 */
std::string mkCsvDir = "mkdir -p " + csvDir;

/**
 * @brief Sets the value of a cycle in the sparse matrix.
 *
 * @param row Row index of the cycle.
 * @param col Column index of the cycle.
 * @param value Value to be set.
 */
void SparseMatrix::setCycle(int row, int col, int value) {
    if (row < 0 || col < 0) return;
    if (row >= matrix.size()) matrix.resize(row + 1);
    if (value != 0) {
        if (col >= matrix[row].size()) matrix[row].resize(col + 1, SparseElement(row, col, 0));
        matrix[row][col] = SparseElement(row, col, value);
    }
}

/**
 * @brief Gets the value of a cycle in the sparse matrix.
 *
 * @param row Row index of the cycle.
 * @param col Column index of the cycle.
 * @return int Value of the cycle.
 */
int SparseMatrix::getCycle(int row, int col) {
    if (row < 0 || col < 0 || row >= matrix.size()) return 0;  // Invalid input or default value
    if (col >= matrix[row].size()) return 0;                   // Default value for non-existing elements
    return matrix[row][col].value;
}

/**
 * @brief Checks if a cycle exists in the sparse matrix.
 *
 * @param row Row index of the cycle.
 * @param col Column index of the cycle.
 * @return true If the cycle exists.
 * @return false If the cycle does not exist.
 */
bool SparseMatrix::exist(int row, int col) {
    if (row < 0 || col < 0 || row >= matrix.size() || col >= matrix[row].size())
        return false;  // Invalid input
    else if (matrix[row][col].value == 0)
        return false;  // Default value for non-existing elements
    else
        return true;
}

/**
 * @brief Sets the name of a node in the sparse matrix.
 *
 * @param node Index of the node.
 * @param name Name to be set.
 */
void SparseMatrix::setName(int node, std::string name) {
    func[node] = name;
}

/**
 * @brief Gets the name of a node in the sparse matrix.
 *
 * @param node Index of the node.
 * @return std::string Name of the node.
 */
std::string SparseMatrix::getName(int node) {
    if (func.find(node) == func.end())
        return "-";
    else
        return func[node];
}

/**
 * @brief Gets the number of rows in the sparse matrix.
 *
 * @return int Number of rows.
 */
int SparseMatrix::size() {
    return matrix.size();
}

/**
 * @brief Subtracts another sparse matrix from this one.
 *
 * @param other Sparse matrix to be subtracted.
 * @return SparseMatrix Result of the subtraction.
 */
SparseMatrix SparseMatrix::subtract(SparseMatrix& other) {
    SparseMatrix result;

    int numRows = std::max(matrix.size(), other.matrix.size());
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numRows; ++col) {
            int value = getCycle(row, col) - other.getCycle(row, col);
            result.setCycle(row, col, value);
            result.setName(row, func[row]);
        }
    }

    return result;
}

/**
 * @brief Sums another sparse matrix to this one.
 *
 * @param other Sparse matrix to be summed.
 * @return SparseMatrix Result of the sum.
 */
SparseMatrix SparseMatrix::sum(SparseMatrix& other) {
    SparseMatrix result;

    int numRows = std::max(matrix.size(), other.matrix.size());
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numRows; ++col) {
            int value = getCycle(row, col) + other.getCycle(row, col);
            result.setCycle(row, col, value);
            result.setName(row, func[row]);
        }
    }

    return result;
}

/**
 * @brief Checks if the sparse matrix has negative values.
 *
 * @return true If the sparse matrix has negative values.
 * @return false If the sparse matrix does not have negative values.
 */
bool SparseMatrix::hasNegativeValues() {
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            if (element.value < 0) return true;
        }
    }
    return false;
}

/**
 * @brief Generates a PNG image of the sparse matrix.
 *
 * @param file Name of the PNG file to be generated.
 */
void SparseMatrix::generateGraphImage(const std::string& file) const {
    system(mkImgDir.c_str());

    std::ofstream dotFile(dotDir + file);

    if (!dotFile.is_open()) {
        printError("Error opening dot file.\n");
        return;
    }

    dotFile << "digraph G {" << std::endl;
    for (int row = 0; row < matrix.size(); ++row) {
        for (int col = 0; col < matrix[row].size(); ++col) {
            if (matrix[row][col].value != 0) {
                dotFile << "   \"" + std::to_string(row) + ":" + func[row] + "\" -> "
                        << "\"" + std::to_string(col) + ":" + func[col] + "\""
                        << " [label=" << matrix[row][col].value << "];" << std::endl;
            }
        }
    }
    dotFile << "}" << std::endl;
    dotFile.close();

    // Generate the PNG image using Graphviz
    std::string command = "dot -Tpng " + dotDir + file + " -o " + pngDir + file.substr(0, file.size() - 4) + ".png";
    system(command.c_str());
}

/**
 * @brief Prints the cycles of the sparse matrix.
 */
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

/**
 * @brief Gets the number of outgoing edges of a node in the sparse matrix.
 *
 * @param node Index of the node.
 * @return int Number of outgoing edges.
 */
int SparseMatrix::getOuts(int node) {
    int out = 0;
    for (int col = 0; col < matrix[node].size(); ++col) {
        if (matrix[node][col].value != 0) out++;
    }
    return out;
}

/**
 * @brief Exports the sparse matrix to a CSV file.
 *
 * @param filename Name of the CSV file to be exported.
 */
void SparseMatrix::exportToCSV(const std::string& filename) {
    system(mkCsvDir.c_str());
    std::ofstream csvFile(csvDir + filename);

    if (!csvFile.is_open()) {
        printError("Error to open CSV file to write.\n");
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

/**
 * @brief Imports a sparse matrix from a CSV file.
 *
 * @param filename Name of the CSV file to be imported.
 */
void SparseMatrix::importFromCSV(const std::string& filename) {
    std::ifstream csvFile(filename);

    if (!csvFile.is_open()) {
        printError("Error in opening CSV file.\n");
        return;
    } else {
        //std::cout << "CSV file open with success." << std::endl;
    }

    int value1, value2, value3;
    char comma;

    while (csvFile >> value1 >> comma >> value2 >> comma >> value3) {
        setCycle(value1, value2, value3);
    }

    csvFile.close();
}
