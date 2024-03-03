#ifndef LAB1_TOOLS_H
#define LAB1_TOOLS_H

int getAmountOfRows(void);

void getAmountOfColumns(int m, int n[m]);

int **fillMatrix(int m, int n[m]);

void printMatrix(int m, int n[m], int *rowsPtrArr[m]);

void copyArrayOfLengths(int m, int nCopy[m], int n[m]);

void freeMatrix(int m, int n[m], int *rowsPtrArr[m]);

#endif //LAB1_TOOLS_H
