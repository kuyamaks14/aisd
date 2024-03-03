#include <stdio.h>
#include "tools.h"

int main(void) {
    int m = getAmountOfRows();

    int n[m];
    getAmountOfColumns(m, n);

    int **rowsPtrArr = fillMatrix(m, n);

    printMatrix(m, n, rowsPtrArr);

    int nCopy[m];
    copyArrayOfLengths(m, nCop, int n[m])

    freeMatrix(m, n, rowsPtrArr);
    return 0;
}


