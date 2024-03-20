/*
    Хедер для реализации стека через вектор
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>

#define STACK_SIZE (100)

int checkEOF(char str[]);
void preventEndlessLoop(char str[]);

typedef struct Stack
{
    int stack[STACK_SIZE];
    int top;
} Stack;
// Stack stack = {.top = 0};

void push(int elem);
void pop(int *operand);

#endif // LAB_2_FUNC_H