#include "main.h"

// const char *errmsgs[] = {"Ok", "Table overflow", "There is no such item", "Empty table"};
const char *errmsgs[] = {"Ok", "Key duplicate", "Table overflow", "There is no element with such a key"};

const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*dialog_options[])(Table *table) = {NULL, dialogAddItem, dialogFindItem, dialogDeleteItem, showTable};

// ---------------------------------

void initializeTable(Table *table) {
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        table->items[i].busy = 0;
    }
}

// ---------------------------------

int getInt(int *num) {
    int scanf_result;
    do {
        scanf_result = scanf("%d", num);

        if (scanf_result == -1) {
            return 0;
        }

        if (scanf_result == 0) {
            flushBuffer();
            printf("%s", "Try once again!: ");
        }
    } while (scanf_result == 0);
    
    flushBuffer();
    return 1;
}

char *getStr() {
    char *str = (char *) malloc(MAX_INFO_SIZE);
    char character;
    for (int i = 0; i < MAX_INFO_SIZE; i++) {
        character = getchar();

        if (character == EOF) {
            free(str);
            return NULL;
        } else if (character == '\n') {
            if (i > 0) {
                str[i] = '\0';
                break;
            } else {
                i--;
            }
        } else {
            str[i] = character;
        }
    }

    return str;
}

// ---------------------------------

void flushBuffer(void) {
    int c;
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
}

// ---------------------------------

int firstHashCode(int key) {
    return key % MAX_TABLE_SIZE;
}

// ---------------------------------

int secondHashCode(int key) {
    return key % (MAX_TABLE_SIZE - 1) + 1;
}

// ---------------------------------

int dialog(const char *msgs[], int NMsgs) {
    const char *err_msg = "";

    int option_num;
    int i, n;
    do {
        puts(err_msg);
        err_msg = "You are wrong. Repeate, please!";

        for (i = 0; i < NMsgs; i++)
            puts(msgs[i]);
        puts("Make your choice: --> ");

        n = getInt(&option_num);
        if (n == 0) // EOF -> quit from program
            option_num = 0;
    } while (option_num < 0 || option_num >= NMsgs);

    return option_num;
}

// ---------------------------------

int dialogAddItem(Table *table) {
    int key;
    int add_result, get_int_result;
    char *info = NULL;

    char *err_msg = "";
    do {
        puts(err_msg);
        err_msg = "Key must be non-negative!";

        puts("Enter key: -->");
        if ((get_int_result = getInt(&key)) == 0)
            return 0; // EOF -> игнорируем весь остальной ввод
    } while(key < 0);

    puts("Enter info:");
    info = getStr();
    if (info == NULL) {
        return 0; // EOF или ошибка при чтении
    }

    add_result = addItem(table, key, info);
    printf("\nResult of inserting element {key = %d, information = %s}: %s\n", key, info, errmsgs[add_result]);
    return 1;
}

int addItem(Table *table, int key, char *info) {
    int i = firstHashCode(key);
    int second_hash_code = secondHashCode(key);
    int n = 0; // Кол-во просмотренных элементов

    int passed_elements_counter = -1; // Кол-во пройденных элементов до первой встречи с удаленным элементом
    int deleted_elem_position = -1; // Позиция для вставки элемента в позицию удаленного элемента

    while (n < MAX_TABLE_SIZE && table->items[i].busy != 0) {
        if (table->items[i].busy == 1 && table->items[i].key == key) {
            return 1; // There is an element with such a key in the hash-table
        }
        
        if (passed_elements_counter == -1 && table->items[i].busy == -1) {
            deleted_elem_position = i;
            passed_elements_counter = n;
        }

        i = (i + second_hash_code) % MAX_TABLE_SIZE;
        n++;
    }

    if (n < MAX_TABLE_SIZE && deleted_elem_position == -1) {
        table->items[i] = (Item){.busy = 1, .key = key, .info = info};
    } else if (n < MAX_TABLE_SIZE && deleted_elem_position != -1) {
        int position_to_insert = (n < passed_elements_counter) ? i : deleted_elem_position;
        table->items[position_to_insert] = (Item){.busy = 1, .key = key, .info = info};
    } else if (deleted_elem_position != -1) {
        table->items[deleted_elem_position] = (Item){.busy = 1, .key = key, .info = info};
    } else {
        return 2; // Table overflow
    }

    return 0; // Ok
}

// ---------------------------------

int dialogFindItem(Table *table) {
    int key, get_int_key_result;
    char *err_msg = "";
    do {
        puts(err_msg);
        err_msg = "Key must be non-negative!";

        puts("Enter key: -->");
        if ((get_int_key_result = getInt(&key)) == 0) {
            return 0; // EOF -> игнорируем весь остальной ввод
        }
    } while(key < 0);

    int found_item_result = (findItem(table, key)->info != NULL) ? 0 : 3;
    printf("\nResult of searching element with the key = %d: %s\n", key, errmsgs[found_item_result]);
    return 1;
}

Item *findItem(Table *table, int key) {
    int i = firstHashCode(key);
    int second_hash_code = secondHashCode(key);
    int n = 0; // Кол-во просмотренных элементов

    while (n < MAX_TABLE_SIZE && table->items[i].busy != 0) {
        if (table->items[i].busy == 1 && table->items[i].key == key) {
            printf("Element: {key = %d, information = %s}\n", key, table->items[i].info);
            return &table->items[i]; // Ok
        }
        
        i = (i + second_hash_code) % MAX_TABLE_SIZE;
        n++;
    }
    return &((Item) {.info = NULL}); // There is no element with such a key
}

// ---------------------------------

int dialogDeleteItem(Table *table) {
    int key, get_int_key_result;
    char *err_msg = "";
    do {
        puts(err_msg);
        err_msg = "Key must be non-negative!";

        puts("Enter key: -->");
        if ((get_int_key_result = getInt(&key)) == 0)
            return 0; // EOF -> игнорируем весь остальной ввод
    } while(key < 0);

    int delete_result = deleteItem(table, key);
    printf("\nResult of deleting element with key = %d: %s\n", key, errmsgs[delete_result]);
    return 1;
}

int deleteItem(Table *table, int key) {
    Item *found_elem_ptr = findItem(table, key);
    if (found_elem_ptr->info != NULL) {
        found_elem_ptr->busy = -1;
        free(found_elem_ptr->info);
        return 0; // Ok
    }
    return 3; // There is no element with such a key in the table
}

// ---------------------------------

int showTable(Table *table) {
    int is_table_empty = 1;
    int found_elem_counter = 0;
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (table->items[i].busy == 1) {
            if (is_table_empty == 1) {
                puts("\nThe current content of the hash-table:");
                is_table_empty = 0;
            }
            printf("Element №%d: {key = %d, information = %s}\n", ++found_elem_counter, table->items[i].key, table->items[i].info);
        }
    }

    if (is_table_empty == 1) {
        puts("The hash-table is empty.");
    }

    return 1;
}

// ---------------------------------

void delTable(Table *table) {
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (table->items[i].busy == 1) {
            free(table->items[i].info);
        }
    }
}

// ---------------------------------
