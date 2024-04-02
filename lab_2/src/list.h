/*
    Хедер для реализации стека через односвязный линейный список
*/

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
    int pos;
    int op;
    struct Stack *next;
} Stack;

#ifndef LAB_2_H
Stack stack = {.pos = -1, .next = NULL}; // Головной элемент списка
#endif

/*
    Функция записи в стек-список. Возвращает 0 для унификации интерфейса программы.
        Time complexity: O(1)
        Memory complexity: O(1)
*/
int push(int elem);

/*
    Функция чтения из стека-списка. Предотвращает чтение из пустого стека, возвращая 1. В случае успешного чтения возвращает 0.
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
    Функция опустошения стека. Освобождает динамически выделенную память обратно в кучу.
        Time complexity: O(n)
        Memory complexity: O(1)
*/
void eraseStack();

#endif // LAB_2_FUNC_H