#include "main_header.h"

extern struct Stack stack;

// --------------------------------------------------------------------------

int checkEOF(char str[]) {
    fgets(str, STACK_SIZE, stdin);
    if (str[0] == '\0') {
        return 1;
    }
    return 0;
}

// --------------------------------------------------------------------------

void preventEndlessLoop(char str[]) {
    str[0] = '\0';
}

// --------------------------------------------------------------------------

int processAsPrefixOrPostfix(char str[STRING_SIZE]) {
    if (str[0] == 42 || str[0] == 43 || str[0] == 45 || str[0] == 47) { // str[0] - один из знаков ('+', '-', '*', '/') ?
        processPrefixExpression(str);
        return 0;
    } else if (str[0] >= 48 && str[0] <= 57) {
        processPostfixExpression(str);
        return 0;
    } else {
        printf("Строка содержит запрещенный символ: %c. Исправьте и введите выражение снова.\n", str[0]);
        return 1;
    }
}

// --------------------------------------------------------------------------

int processPostfixExpression(char str[STRING_SIZE]) {
    int statusCode = 0;
    int op1, op2, res;

    int i = 0;
    while (str[i] != '\n') {
        statusCode = 0;
        if (str[i] >= 48 && str[i] <= 57) { // str[i] - цифра? (48 и 57 - ASCII коды для '0' и '9')
            statusCode += push(str[i] - 48); // ASCII код минус 48, чтобы получить цифру в 10 с.с.
            i++;
            // puts("")
            // for (int i = 0; i < stack.top; i++)
            //     printf("stack[%d] = %d", i, stack.stack[i]);
            continue;
        } else if (str[i] == 42) { // ASCII код знака '*'
            statusCode += pop(&op1);
            statusCode += pop(&op2);
            res = op1 * op2;
            statusCode += push(res);
        } else if (str[i] == 43) { // ASCII код знака '+'
            statusCode += pop(&op1);
            statusCode += pop(&op2);
            res = op1 + op2;
            statusCode += push(res);
        } else if (str[i] == 45) { // ASCII код знака '-'
            statusCode += pop(&op2); // В постфиксной форме операнды записываются в исходном порядке, но стек работает по правилу LIFO и операция '-' - некоммутативная
            statusCode += pop(&op1); // Поэтому из стека читаем сначала op2, а потом op1
            res = op1 - op2;
            statusCode += push(res);
        } else if (str[i] == 47) { // ASCII код знака '/'
            statusCode += pop(&op2); // Порядок чтения из стека аналогичен для операции '-'
            statusCode += pop(&op1);
            res = op1 / op2;
            statusCode += push(res);
        } else {
            printf("Строка содержит запрещенный символ: %c. Исправьте и введите выражение заново.\n", str[i]);
            eraseStack();
            return 1;
        }

        if (statusCode > 0) {
            puts("Арифметическое выражение некорректное. Проверьте и введите заново.");
            eraseStack();
            return 1;
        }

        i++;
    }

    if (getStackSize() == 1) { // Если выражение корректное, то после всех вычислений стек будет состоять лишь из одного элемента - результата
        pop(&res);
    } else {
        puts("Арифметическое выражение некорректное (нет конечного результата вычислений). Проверьте и введите заново.");
        eraseStack();
        return 1;
    }

    printf("Результат вычисления постфиксного выражения: %d\n", res);

    eraseStack();
    
    return 0;
}

// --------------------------------------------------------------------------

int processPrefixExpression(char str[STRING_SIZE]) {
    int statusCode = 0;
    int op1, op2, res;

    int i = 0;
    for (; str[i] != '\n'; i++); // Найдем индекс строки, на котором заканчивается префиксное выражение, чтобы далее сканировать его справа налево
    i--;
    while (i >= 0) {
        statusCode = 0;
        if (str[i] >= 48 && str[i] <= 57) { // str[i] - цифра? (48 и 57 - ASCII коды для '0' и '9')
            statusCode += push(str[i] - 48); // ASCII код минус 48, чтобы получить цифру в 10 с.с.
            i--;
            // puts("")
            // for (int i = 0; i < stack.top; i++)
            //     printf("stack[%d] = %d", i, stack.stack[i]);
            continue;
        } else if (str[i] == 42) { // ASCII код знака '*'
            statusCode += pop(&op1);
            statusCode += pop(&op2);
            res = op1 * op2;
            statusCode += push(res);
        } else if (str[i] == 43) { // ASCII код знака '+'
            statusCode += pop(&op1);
            statusCode += pop(&op2);
            res = op1 + op2;
            statusCode += push(res);
        } else if (str[i] == 45) { // ASCII код знака '-'
            statusCode += pop(&op1);
            statusCode += pop(&op2);
            res = op1 - op2;
            statusCode += push(res);
        } else if (str[i] == 47) { // ASCII код знака '/'
            statusCode += pop(&op1);
            statusCode += pop(&op2);
            res = op1 / op2;
            statusCode += push(res);
        } else {
            printf("Строка содержит запрещенный символ: %c. Исправьте и введите выражение заново.\n", str[i]);
            eraseStack();
            return 1;
        }
        
        if (statusCode > 0) {
            puts("Арифметическое выражение некорректное. Проверьте и введите заново.");
            eraseStack();
            return 1;
        }

        i--;
    }

    if (getStackSize() == 1) { // Если выражение корректное, то после всех вычислений стек будет состоять лишь из одного элемента - результата
        pop(&res);
    } else {
        puts("Арифметическое выражение некорректное (нет конечного результата вычислений). Проверьте и введите заново.");
        eraseStack();
        return 1;
    }

    printf("Результат вычисления префиксного выражения: %d\n", res);

    eraseStack();

    return 0;
}

// --------------------------------------------------------------------------