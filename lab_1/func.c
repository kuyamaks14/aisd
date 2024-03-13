#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

int inputMatrix(Matrix *rm) {
    getRowsAmount(&rm->lines); // Ввод числа строк матрицы
    rm->matr = (Line *) calloc(rm->lines, sizeof(Line)); // Выделение памяти под массив строк
    if (rm->matr == NULL) {
        return 1;
    }

    for (int i = 0; i < rm->lines; i++) {
        getColumnsAmount(&(rm->matr[i].n), i); // Ввод числа колонок i-ой строки матрицы

        rm->matr[i].a = (int *) calloc(rm->matr[i].n, sizeof(int)); // Выделение памяти под элементы i-ой строки
        if (rm->matr[i].a == NULL) { // Неудачное выдеделение памяти под i-ую строку матрицы
            eraseMatrix(rm, i); // Освобождение выделенной к текущему моменту памяти
            return 1;
        }

        fillRow(rm->matr + i); // Заполненние элементами i-ой строки матрицы
    }

    puts("The matrix is formed!\n");

    return 0;
}

void getRowsAmount(int *amount) {
    const char *msg = "";
    do {
        printf("%s\n", msg);
        printf("%s", "Enter amount of rows: ");
        scanf("%d", amount);
        flushBuffer(); // Очистка буфера ввода
        msg = "You are wrong, repeat please!";
    } while(*amount < 1);
}

void getColumnsAmount(int *amount, int num) {
    const char *msg = "";
    do {
        printf("%s\n", msg);
        printf("%s%d%s", "Enter amount of columns for row №", num + 1, ": ");
        scanf("%d", amount);
        flushBuffer(); // Очистка буфера ввода
        msg = "You are wrong, repeat please!";
    } while(*amount < 1);
}

void fillRow(Line *row) {
    printf("\n%s\n", "Filling the row...");
    for (int i = 0; i < row->n; i++) {
        scanf("%d", row->a + i);
    }
    flushBuffer(); // Очистка буфера ввода
    puts("");
}

void eraseMatrix(Matrix *rm, int filledRowsNum) {
    for (int i = 0; i < filledRowsNum; i++) {
        free(rm->matr[i].a);
    }
    free(rm->matr);
}

void flushBuffer(void) {
    int c;
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
}

void outputMatrix(Matrix *rm, const char *msg) {
    printf("%s\n", msg);

    for (int i = 0; i < rm->lines; i++) {
        for (int j = 0; j < rm->matr[i].n; j++) {
            printf("%d ", rm->matr[i].a[j]);
        }
        puts("");
    }
    puts("");
}

int copyMatrix(Matrix *dest, Matrix *src) {
    dest->lines = src->lines; // Копирование количества строк

    dest->matr = (Line *) calloc(dest->lines, sizeof(Line)); // Выделение памяти под массив строк
    if (dest->matr == NULL) {
        return 1;
    }

    for (int i = 0; i < dest->lines; i++) {
        dest->matr[i].n = src->matr[i].n; // Копирование количества элементов i-ой строки
        dest->matr[i].a = (int *) calloc(dest->matr[i].n, sizeof(int)); // Выделение памяти под элементы i-ой строки
        if (dest->matr[i].a == NULL) { // Неудачное выдеделение памяти под i-ую строку матрицы
            eraseMatrix(dest, i); // Освобождение выделенной к текущему моменту памяти
            return 1;
        }

        for (int j = 0; j < dest->matr[i].n; j++) { // Копирование элементов i-ой строки
            dest->matr[i].a[j] = src->matr[i].a[j];
        }
    }

    return 0;
}

void sortMatrix(Matrix *rm) {
    int j;
    int currentNum;
    Line currentPtr;

    for (int i = 1; i < rm->lines; i++) {
        j = i - 1;
        currentNum = rm->matr[i].a[0];
        currentPtr = rm->matr[i];

        while (j >= 0 && currentNum < rm->matr[j].a[0]) {
            rm->matr[j + 1] = rm->matr[j];
            j--;
        }

        rm->matr[j + 1] = currentPtr;
    }
}

int processMatrix(Matrix *newRM, Matrix *oldRM) {
    if (copyMatrix(newRM, oldRM) == 1) { // Ошибка выделения памяти
        return 1;
    }

    sortMatrix(newRM); // Сортировка скопированной матрицы

    return 0;
}