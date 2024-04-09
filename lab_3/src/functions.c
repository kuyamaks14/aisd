#include "main.h"

const char *errmsgs[] = {"Ok", "Table overflow", "There is no such item", "Empty table"};

const char *msgs[] = {"0. Quit", "1. Add", "2. Load", "3. Find", "4. FindAll", "5. Delete", "6. DeleteAll", "7. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*dialog_options[])(Table *table) = {NULL, dialogAddItem, dialogLoadTable, dialogFindItem, dialogFindAllItems, dialogDeleteItem, dialogDeleteAllItems, showTable};

// ---------------------------------

void initializeTable(Table *table) {
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        table->items[i].busy = 0;
    }
}

// ---------------------------------

int getInt(int *num) {
    // puts("Input a number: ");
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
    // char *fgets_result = NULL;

    // scanf("%*s");
    // puts("Input a string: -->");

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

    // fgets_result = fgets(str, MAX_INFO_SIZE, stdin);

    // if (fgets_result == NULL) {
    //     free(str);
    // }

    // flushBuffer();
    // return fgets_result;
}

// ---------------------------------

void flushBuffer(void) {
    int c;
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
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

    puts("Enter key: -->");
    get_int_result = getInt(&key);
    if (get_int_result == 0) {
        return 0; // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter info:");
    info = getStr();
    if (info == NULL) {
        return 0; // EOF или ошибка при чтении
    }

    add_result = addItem(table, key, info);
    printf("\nAdd item with key = %d and information = %s: %s\n", key, info, errmsgs[add_result]);
    return 1;
}

int addItem(Table *table, int key, char *info) {
    int release = 0;
    
    int free_position_index = -1;
    int is_free_position_found = 0;

    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (table->items[i].busy == 1) {
            if (table->items[i].key == key) {
                release = (release <= table->items[i].release) ? table->items[i].release + 1 : release;
            }
        } else if (is_free_position_found == 0) {
            free_position_index = i;
            is_free_position_found = 1;
        }
    }

    if (is_free_position_found) {
        table->items[free_position_index] = (Item) {.key = key, .info = info, .release = release, .busy = 1};
        table->size++;
        return 0; // Ok
    }
    return 1; // Table overflow
}

// ---------------------------------

int dialogFindAllItems(Table *table) {
    int key;
    int get_int_result;
    printf("Enter key: -->\n");
    get_int_result = getInt(&key);
    if (get_int_result == 0) {
        return 0; // EOF
    }

    int find_all_result = findAllItems(table, key);

    printf("\nFind all items with key = %d: %s\n", key, errmsgs[find_all_result]);
    return 1;
}

int findAllItems(Table *table, int key) {
    int busy_items_counter = 0;
    int isFound = 0;
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (busy_items_counter == table->size) {
            break;
        }

        if (table->items[i].busy == 1) {
            busy_items_counter++;
            if (table->items[i].key == key) {
                printf("Item: key = %d, release = %d, information = %s\n", key, table->items[i].release, table->items[i].info);
                isFound = 1;
            }
        }
    }
    
    if (isFound) {
        return 0;
    }
    return 2;
}

// ---------------------------------

int dialogFindItem(Table *table) {
    int key, release;

    int get_int_key_result, get_int_release_result;
    printf("Enter key: -->\n");
    get_int_key_result = getInt(&key);
    printf("Enter release: -->\n");
    get_int_release_result = getInt(&release);

    if (get_int_key_result == 0 || get_int_release_result == 0) {
        return 0; // EOF
    }

    int find_result = findItem(table, key, release);
    printf("\nFind all items with key = %d and release = %d: %s\n", key, release, errmsgs[find_result]);
    return 1;
}

int findItem(Table *table, int key, int release) {
    int busy_items_counter = 0;

    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (busy_items_counter == table->size) {
            break;
        }

        if (table->items[i].busy == 1) {
            busy_items_counter++;
            if (table->items[i].key == key && table->items[i].release == release) {
                printf("Item: key = %d, release = %d, information = %s\n", key, release, table->items[i].info);
                return 0;
            }
        }
    }

    return 2;
}

// ---------------------------------

int dialogDeleteItem(Table *table) {
    int key, release;

    int get_int_key_result, get_int_release_result;
    printf("Enter key: -->\n");
    get_int_key_result = getInt(&key);
    printf("Enter release: -->\n");
    get_int_release_result = getInt(&release);

    if (get_int_key_result == 0 || get_int_release_result == 0) {
        return 0; // EOF
    }

    int delete_result = deleteItem(table, key, release);
    printf("\nDelete item with key = %d and release = %d: %s\n", key, release, errmsgs[delete_result]);
    return 1;
}

int deleteItem(Table *table, int key, int release) {
    int primary_table_size = table->size;
    int busy_items_counter = 0;

    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (busy_items_counter == primary_table_size) {
            break;
        }

        if (table->items[i].busy == 1) {
            busy_items_counter++;
            if (table->items[i].key == key && table->items[i].release == release) {
                table->items[i].busy = 0;
                table->size--;
                free(table->items[i].info);
                return 0;
            }
        }
    }

    return 2;
}

// ---------------------------------

int dialogDeleteAllItems(Table *table) {
    int key;
    int get_int_result;
    printf("Enter key: -->\n");
    get_int_result = getInt(&key);
    if (get_int_result == 0) {
        return 0; // EOF
    }

    int delete_all_result = deleteAllItems(table, key);

    printf("\nDelete all items with key = %d: %s\n", key, errmsgs[delete_all_result]);
    return 1;
}

int deleteAllItems(Table *table, int key) {
    int primary_table_size = table->size;
    int busy_items_counter = 0;
    int isFound = 0;
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (busy_items_counter == primary_table_size) {
            break;
        }

        if (table->items[i].busy == 1) {
            busy_items_counter++;
            if (table->items[i].key == key) {
                printf("Deleting item: key = %d, release = %d, information = %s\n", key, table->items[i].release, table->items[i].info);
                isFound = 1;
                table->items[i].busy = 0;
                table->size--;
                free(table->items[i].info);
            }
        }
    }

    if (isFound){
        return 0;
    }
    return 2;
}

// ---------------------------------

int showTable(Table *table) {
    int result_status = 3; // Empty table by default

    if (table->size > 0) {
        int busy_items_counter = 0;
        for (int i = 0; i < MAX_TABLE_SIZE; i++) {
            if (busy_items_counter == table->size) {
                break;
            }

            if (table->items[i].busy == 1) {
                busy_items_counter++;
                result_status = 0;
                Item cur_item = table->items[i];
                printf("Item №%d: key = %d, release = %d, information = %s\n", busy_items_counter, cur_item.key, cur_item.release, cur_item.info);
            }
        }
    }
    printf("\nShow table: %s\n", errmsgs[result_status]);
    return 1; // Ok
}

// ---------------------------------

void delTable(Table *table) {
    int primary_table_size = table->size;
    int busy_items_counter = 0;
    for (int i = 0; i < MAX_TABLE_SIZE; i++) {
        if (busy_items_counter == primary_table_size) {
            break;
        }

        if (table->items[i].busy == 1) {
            busy_items_counter++;
            table->size--;

            table->items[i].busy = 0;
            free(table->items[i].info);
        }
    }
}

// ---------------------------------

int dialogLoadTable(Table *table) {
    // временная заглушка
    return 0;
}

// ---------------------------------
