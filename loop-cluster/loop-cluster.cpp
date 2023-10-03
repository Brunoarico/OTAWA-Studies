#include <iostream>
#include <string.h>

void printMatrix(int **matrix, int row, int collumn)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < collumn; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    
}
int main() {
    std::cout << "Start loop cluster" << std::endl;

    //Enter matrix
    // int matrix[5][5] = {{0, 70, 0, 0, 0}, 
    //                     {0, 0, 30, 25, 0}, 
    //                     {0, 0, 0, 0, 0}, 
    //                     {0, 0, 30, 0, 10}, 
    //                     {0, 0, 30, 0, 0}}; // No loops
    int matrix[9][9] = {{0, 55, 0, 0, 0, 0, 0, 0, 0}, 
                        {0, 0, 0, 0, 0, 0, 20, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 30, 0, 0, 0, 60},
                        {0, 0, 0, 0, 0, 20, 0, 20, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 20, 55, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0, 0}}; // Binary Search

    int matrixLoops[9][9] =    {{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
                                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 20, 0, 0},
                                {0, 0, 0, 0, 0, 0, 0, 0, 0},
                                {0, 0, 0, 0, 0, 0, 20, 0, 0},
                                {0, 0, 0, 0, 0, 0, 20, 0, 0}}; // Binary Search



    //Get loops
    //Get number of blocks
    int size = sizeof(matrix[0]) /  sizeof(int);
    int loopBacks = 0;
    printf("Number of blocks: %d\n", size);

    //Get number of loopbacks
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            // printf("%d ", matrix_loops[i][j]);
            if(matrixLoops[i][j])
            {
                loopBacks++;
            }
        }
        // printf("\n"); 
    }
    printf("There are %d loop-backs\n", loopBacks);

    //Get loopbacks and loops

    int loops = 0;
    int matrixLoopBacks[loopBacks][2];

    if (loopBacks != 0)
    {
            int loopBackCounter = 0;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if(matrixLoops[i][j])
                    {
                        matrixLoopBacks[loopBackCounter][0] = i;
                        matrixLoopBacks[loopBackCounter][1] = j;
                        int sameBackFlag = 0;
                        for (int k = 0; k < loopBackCounter; k++)
                        {
                            if(matrixLoopBacks[k][1] == matrixLoopBacks[loopBackCounter][1])
                            {
                                sameBackFlag = 1;
                            }
                        }
                        if (sameBackFlag == 0)
                        {
                            loops++;
                        }
                        
                        loopBackCounter++;
                    }
                }
            }

    }
    printf("There are %d loops\n", loops);
    
    int loopArrays[loops][size] = {};
    memset(loopArrays, 0, sizeof(loopArrays));
    
    int i, j, count = 0;
   
    for (i = 0; i < loopBacks; i++)
    {
        for (j = 0; j < i; j++)   
        {
            if (matrixLoopBacks[i][1] == matrixLoopBacks[j][1])
            {
                break;
            }
        }
        if (i == j)
        {
            loopArrays[count][matrixLoopBacks[i][1]] = 2;
        }
    }

    for (int i = 0; i < loops; i++)
    {
        int finishBlock = 0;
        for (int j = 0; j < size; j++)
        {
            if (loopArrays[i][j] == 2)
            {
                finishBlock = j;
                // printf("FinishBlock: %d\n", finishBlock);
            }       
        }
        for (int j = 0; j < loopBacks; j++)
        {
            if (matrixLoopBacks[j][1] == finishBlock)
            {
                loopArrays[i][matrixLoopBacks[j][0]] = 1;
            }   
        }
    }
    
    for (int k = 0; k < loops; k++)
    {
        int loopArraysAux[loops][size];
        memcpy(loopArraysAux, loopArrays, sizeof(loopArraysAux));

        int compare = 1;
        while (1)
        {
            memcpy(loopArrays, loopArraysAux, sizeof(loopArraysAux));
            for (int i = 0; i < size; i++)
            {
                if (loopArraysAux[k][i] != 0)
                {
                    for (int j = 0; j < size; j++)
                    {
                        if (matrix[i][j] != 0)
                        {
                            if (loopArraysAux[k][j] == 0)
                            {
                                loopArraysAux[k][j] = 3;
                            }
                            
                        }
                        
                    }
                }      
                
            }
            if (memcmp(loopArraysAux[k], loopArrays[k], size)  == 0)
                break;
        }
        memcpy(loopArrays, loopArraysAux, sizeof(loopArraysAux));

    }
    
    


    int solvedLoops = 0;
    for (int i = 0; i < loops; i++)
    {
        int loopSize = 0;
        //get the number of blocks in loop
        for (int j = 0; j < size; j++)
        {
            if (loopArrays[i][j] != 0)
            {
                loopSize++;
            }
        }
        int internalLoopMatrix[loopSize][loopSize];
        for (int j = 0; j < size; j++)
        {
            if (loopArrays[i][j] != 0)
            {
                loopSize++;
            }
        }
        

    }
    

    //While loops exists
    // while (solvedLoops != loops)
    // {
    //     for (int i = 0; i < loopBacks; i++)
    //     {

    //         //Check if there is a nested loop inside
    //         //Check if it was already solved
    //     }
        

    // //     //Check if there is a nested loop inside
    // }
    
        //Cluster loops

            //Identify loop matrix

            //Input loop matrix in bioinspired algorithm

            //Create a new block to represent the loop matrix

            //Generate new matrix clusterized

            
    return 0;
}