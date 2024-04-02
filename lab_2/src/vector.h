/*
    Хедер для реализации стека через вектор
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

#define STACK_SIZE (100)

typedef struct Stack {
    int stack[STACK_SIZE];
    int top;
} Stack;

#ifndef LAB_2_H
Stack stack = {.top = 0};
#endif

/*
    Функция записи в стек-вектор. Предотвращает переполнение, возвращая 1. В случае успешной записи возвращает 0.
        Time complexity: O(1)
        Memory complexity: O(1)
*/
int push(int elem);

/*
    Функция чтения из стека-вектора. Предотвращает чтение из пустого стека, возвращая 1. В случае успешного чтения возвращает 0.
        Time complexity: O(1)
        Memory complexity: O(1)
*/
int pop(int *operand);

/*
    Функция возвращает размер стека.
        Time complexity: O(1)
        Memory complexity: O(1)
*/
int getStackSize();

/*
    Функция опустошения стека.
        Time complexity: O(1)
        Memory complexity: O(1)
*/
void eraseStack();

#endif // LAB_2_FUNC_H