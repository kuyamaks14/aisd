#include "main_header.h"

extern struct Stack stack;

int main(void) {
    char str[STRING_SIZE]; // В записи операнды представлены только одной цифрой, поэтому можно можно вводить строку
    int statusCode;

    while (!checkEOF(str)) { // Завершаем чтение из stdin в случае EOF
        processAsPrefixOrPostfix(str);
        preventEndlessLoop(str);
    }
    return 0;
}