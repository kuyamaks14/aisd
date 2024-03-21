#include "list.h"

// --------------------------------------------------------------------------

int push(int elem) {
    Stack *newStackElemPtr = (Stack *) malloc(sizeof(Stack));
    newStackElemPtr->op = elem;

    if (stack.next == NULL) { // Стек пуст
        stack.next = newStackElemPtr;
        newStackElemPtr->next = NULL;
        newStackElemPtr->pos = 1;
    } else {
        newStackElemPtr->next = stack.next;
        newStackElemPtr->pos = stack.next->pos + 1;
        stack.next = newStackElemPtr;
    }

    return 0;
}

// --------------------------------------------------------------------------

int pop(int *operand) {
    if (stack.next == NULL) { // Стек пуст
        puts("Отказано: стек пуст.");
        return 1;
    }
    Stack *tempStackElemPtr = stack.next;
    stack.next = tempStackElemPtr->next;
    tempStackElemPtr->next = NULL;
    *operand = tempStackElemPtr->op;
    free(tempStackElemPtr);
    return 0;
}

// --------------------------------------------------------------------------

int getStackSize() {
    if (stack.next == NULL) { // Стек пуст
        return 0;
    }
    return stack.next->pos;
}

// --------------------------------------------------------------------------

void eraseStack() {
    Stack *curStackElemPtr = stack.next;
    Stack *nextStackElemtPtr = NULL;

    while (curStackElemPtr != NULL) {
        nextStackElemtPtr = curStackElemPtr->next;
        curStackElemPtr->next = NULL;
        free(curStackElemPtr);
        curStackElemPtr = nextStackElemtPtr;    
    }
    stack.next = NULL;
}

// --------------------------------------------------------------------------