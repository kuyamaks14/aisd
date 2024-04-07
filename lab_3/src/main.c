#include "main.h"

extern const char *msgs[];
extern const int NMsgs;
extern int (*dialog_options[])(Table *table);

int main(void) {
    Table table = {.size = 0};
    int operation_code;

    while ((operation_code = dialog(msgs, NMsgs)))
        if (!dialog_options[operation_code](&table))
            break;

    puts("That's all.");
    delTable(&table);
    return 0;
}