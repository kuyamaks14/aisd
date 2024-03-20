#include "main_header.h"

int main(void) {
    char str[STACK_SIZE];
    while (!checkEOF(str)) {
        printf("&&&: %s\n", str);
        preventEndlessLoop(str);
    }
    printf("%s", "hello");
    return 0;
}