#include "main_header.h"

extern Stack stack;

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
    int op1, op2, res;
    int i = 0;
    while (str[i] != '\n') {
        if (str[i] >= 48 && str[i] <= 57) { // str[i] - цифра? (48 и 57 - ASCII коды для '0' и '9')
            push(str[i] - 48); // ASCII код минус 48, чтобы получить цифру в 10 с.с.
            i++;
            // puts("")
            // for (int i = 0; i < stack.top; i++)
            //     printf("stack[%d] = %d", i, stack.stack[i]);
            continue;
        } else if (str[i] == 42) { // ASCII код знака '*'
            pop(&op1);
            pop(&op2);
            res = op1 * op2;
            push(res);
        } else if (str[i] == 43) { // ASCII код знака '+'
            pop(&op1);
            pop(&op2);
            res = op1 + op2;
            push(res);
        } else if (str[i] == 45) { // ASCII код знака '-'
            pop(&op2); // В постфиксной форме операнды записываются в исходном порядке, но стек работает по правилу LIFO и операция '-' - некоммутативная
            pop(&op1); // Поэтому из стека читаем сначала op2, а потом op1
            res = op1 - op2;
            push(res);
        } else if (str[i] == 47) { // ASCII код знака '/'
            pop(&op2); // Порядок чтения из стека аналогичен для операции '-'
            pop(&op1);
            res = op1 / op2;
            push(res);
        } else {
            printf("Строка содержит запрещенный символ: %c. Исправьте и введите выражение заново.\n", str[i]);
            eraseStack();
            return 1;
        }
        i++;
        // puts("");
        // for (int i = 0; i < stack.top; i++)
        //     printf(" stack[%d] = %d", i, stack.stack[i]);
    }

    pop(&res);
    printf("Результат вычисления постфиксного выражения: %d\n", res);

    eraseStack();
    
    return 0;
}

// --------------------------------------------------------------------------

int processPrefixExpression(char str[STRING_SIZE]) {
    int op1, op2, res;

    int i = 0;
    for (; str[i] != '\n'; i++); // Найдем индекс строки, на котором заканчивается префиксное выражение, чтобы далее сканировать его справа налево
    i--;
    while (i >= 0) {
        if (str[i] >= 48 && str[i] <= 57) { // str[i] - цифра? (48 и 57 - ASCII коды для '0' и '9')
            push(str[i] - 48); // ASCII код минус 48, чтобы получить цифру в 10 с.с.
            i--;
            // puts("")
            // for (int i = 0; i < stack.top; i++)
            //     printf("stack[%d] = %d", i, stack.stack[i]);
            continue;
        } else if (str[i] == 42) { // ASCII код знака '*'
            pop(&op1);
            pop(&op2);
            res = op1 * op2;
            push(res);
        } else if (str[i] == 43) { // ASCII код знака '+'
            pop(&op1);
            pop(&op2);
            res = op1 + op2;
            push(res);
        } else if (str[i] == 45) { // ASCII код знака '-'
            pop(&op1);
            pop(&op2);
            res = op1 - op2;
            push(res);
        } else if (str[i] == 47) { // ASCII код знака '/'
            pop(&op1); 
            pop(&op2);
            res = op1 / op2;
            push(res);
        } else {
            printf("Строка содержит запрещенный символ: %c. Исправьте и введите выражение заново.\n", str[i]);
            eraseStack();
            return 1;
        }
        
        i--;
        // puts("");
        // for (int i = 0; i < stack.top; i++)
        //     printf(" stack[%d] = %d", i, stack.stack[i]);
    }

    pop(&res);
    printf("Результат вычисления префиксного выражения: %d\n", res);

    eraseStack();

    return 0;
}

// --------------------------------------------------------------------------