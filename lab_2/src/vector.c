#include "vector.h"

int checkEOF(char str[]) {
    fgets(str, STACK_SIZE, stdin);
    if (str[0] == '\0') {
        return 1;
    }
    return 0;
}

void preventEndlessLoop(char str[]) {
    str[0] = '\0';
}

Stack stack = {.top = 0};
// Stack stack = {.top = 0};
void push(int elem) {
    if (stack.top < STACK_SIZE) {
        stack.stack[stack.top++] = elem;
    }
    puts("Denied: stack overflow.");
}

void pop(int *operand) {
    if (stack.top) {
        *operand = stack.stack[--stack.top];
    } else {
        printf("Denied: empty stack.");
    }
}