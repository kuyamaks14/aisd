#ifndef LAB1_TOOLS_H
#define LAB1_TOOLS_H

// Строка матрицы
typedef struct Line {
    int n; // Количество элементов в строке
    int *a; // массив элементов строки
} Line;

// Непрямоугольная матрица
typedef struct Matrix {
    int lines; // Количество строк в матрице
    Line *matr; // Массив строк матрицы
} Matrix;

int inputMatrix(Matrix *rm); // Ввод матрицы
void getRowsAmount(int *num); // Ввод числа строк
void getColumnsAmount(int *amount, int num); // Ввод числа колонок строки
void eraseMatrix(Matrix *rm, int filledRowsNum); // Освобождение памяти
void fillRow(Line *row); // Заполнение строки матрицы элементами
void flushBuffer(void); // Очистка потока ввода
void outputMatrix(Matrix *rm, const char *msg); // Вывод матрицы
int copyMatrix(Matrix *dest, Matrix *src); // Копирование матрицы
void sortMatrix(Matrix *rm); // Сортировка строк матрицы по возрастанию элементов первого столбца
int processMatrix(Matrix *newRM, Matrix *oldRM); // Копирование исходной матрицы и сортировка новой по возрастанию элементов первого столбца

#endif //LAB1_TOOLS_H
