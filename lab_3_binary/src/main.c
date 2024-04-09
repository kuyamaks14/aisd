#include "main.h"

extern const char *msgs[];
extern const int NMsgs;
extern int (*dialog_options[])(Table *table);

int main(void) {
    Table table = {.vector_size = 0, .table_size = 0, .first = NULL, .fd = NULL};
    int operation_code;

    while ((operation_code = dialog(&table, msgs, NMsgs)))
        if (!dialog_options[operation_code](&table))
            break;

    saveTable(&table);

    puts("That's all.");
    delTable(&table);
    return 0;
}