#include <stdio.h>
#include "tools.h"

int main(void) {
    Matrix originalMatrix = {0, NULL}; // Инициализация исходной матрицы
    Matrix resultMatrix = {0, NULL}; // Инициализация результирующей матрицы

    if (inputMatrix(&originalMatrix) == 1) { // Ввод созданной матрицы
        puts("Memory allocation error");
        return 1;
    }

    if (processMatrix(&resultMatrix, &originalMatrix) == 1) { // Формирование аналогичной матрицы и ее сортировка
        puts("Memory allocation error");
    };

    outputMatrix(&originalMatrix, "Original matrix"); // Вывод исходной матрицы
    outputMatrix(&resultMatrix, "Result matrix"); // Вывод результирующей матрицы

    eraseMatrix(&originalMatrix, originalMatrix.lines); // Освобождение памяти под исходную матрицу
    eraseMatrix(&resultMatrix, resultMatrix.lines); // Освобождение памяти под результирующую матрицу
    return 0;
}


