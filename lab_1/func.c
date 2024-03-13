#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

int getAmountOfRows(void) {
    int amount;
    printf("%s", "Enter amount of rows: ");
    scanf("%d", &amount);

    while (amount <= 0) {
        printf("%s", "Amount of rows must be more than 0! Enter amount of rows: ");
        scanf("%d", &amount);
    }

    return amount;
}

void getAmountOfColumns(int m, int n[m]) {
    for (int i = 0; i < m; i++) {
        int amount;
        printf("%s%d: ", "Enter amount of columns in a row №", i + 1);
        scanf("%d", &amount);

        while (amount <= 0) {
            printf("%s%d: ", "Amount of columns must be more than 0! Enter amount of columns in a row №", i + 1);
            scanf("%d", &amount);
        }

        *(n + i) = amount;
    }

    puts("");
}

int **fillMatrix(int m, int n[m]) {
    puts("Filling the matrix...\n");

    int **rowsPtrArr = (int **) malloc(sizeof(int *) * m);

    for (int i = 0; i < m; i++) {
        *(rowsPtrArr + i) = (int *) malloc(sizeof(int) * n[i]);

        printf("%s%d\n", "Filling the row №", i + 1);
        for (int j = 0; j < n[i]; j++) {
            printf("%s%d%s%d%s", "Matrix[", i, "][", j, "] = ");
            scanf("%d", *(rowsPtrArr + i) + j);
        }

        puts("");
    }

    return rowsPtrArr;
}

void printMatrix(int m, int n[m], int *rowsPtrArr[m]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n[i]; j++) {
            printf("%d ", *(*(rowsPtrArr + i) + j));
        }

        puts("");
    }
}

void copyArrayOfLengths(int m, int nCopy[m], int n[m]) {
    for (int i = 0; i < m; i++) {
        *(nCopy + i) = *(n + i);
    }
}

void copyArrayOfPtrs(int m, int *rowsPtrArrCopy[m], int *rowsPtrArr[m])  {
    for (int i = 0; i < m; i++) {
        *(rowsPtrArrCopy + i) = *(rowsPtrArr + i);
    }
}

void sortMatrixRows(int m, int nCopy[m], int *rowsPtrArrCopy[m]) {
    int j;
    int currentNum;
    int *currentPtr;
    int currentLength;

    for (int i = 1; i < m; i++) {
        j = i - 1;
        currentNum = **(rowsPtrArrCopy + i);
        currentPtr = *(rowsPtrArrCopy + i);
        currentLength = *(nCopy + i);

        while (j >= 0 && currentNum < **(rowsPtrArrCopy + j)) {
            *(rowsPtrArrCopy + j + 1) = *(rowsPtrArrCopy + j);
            *(nCopy + j + 1) = *(nCopy + j);
            j--;
        }

        *(rowsPtrArrCopy + j + 1) = currentPtr;
        *(nCopy + j + 1) = currentLength;
    }
}

void freeMatrix(int m, int n[m], int *rowsPtrArr[m]) {
    for (int i = 0; i < m; i++) {
        free(*(rowsPtrArr + i));
    }

    free(rowsPtrArr);
}
