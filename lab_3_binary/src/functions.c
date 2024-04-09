#include "main.h"

const char *errmsgs[] = {"Ok", "Table overflow", "There is no such item", "Empty table", "Memory allocation error", "File Read Error", "Memory reallocation error", "Table Save Error", "Create File Error", "Cannot open the file"};

const char *msgs[] = {"0. Quit", "1. Add", "2. Load", "3. Create", "4. Find", "5. FindAll", "6. Delete", "7. DeleteAll", "8. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*dialog_options[])(Table *table) = {NULL, dialogAddItem, dialogLoadTable, dialogCreateTable, dialogFindItem, dialogFindAllItems, dialogDeleteItem, dialogDeleteAllItems, showTable};

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
    char *str = (char *) malloc(sizeof(char) * MAX_INFO_SIZE);
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
    } while (c != '\n' && c != EOF);
}

// ---------------------------------

int dialog(Table *table, const char *msgs[], int NMsgs) {
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

    if ((option_num == 1 || (option_num >= 4 && option_num <= 8)) && !checkIsFileOpened(table)) {
        if (!checkIsFileOpened(table)) {
            puts("You haven't created/loaded a file to execute this operation!");
            return 0; // File is not opened
        }
    }

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

    free(info);
    return 1;
}

int addItem(Table *table, int key, char *info) {
    int release = 0;
    
    if (table->table_size < table->vector_size) {
        for (int i = 0; i < table->table_size; i++) {
            if (table->first[i].key == key) {
                release = (release <= table->first[i].release) ? table->first[i].release + 1 : release;
            }
        }

        Item *first_new = (Item *) realloc(table->first, sizeof(Item) * (table->table_size + 1));
        if (first_new == NULL) {
            return 4; // Memory allocation error
        }
        table->first = first_new;

        table->first[table->table_size].key = key;
        table->first[table->table_size].release = release;
        table->first[table->table_size].len = strlen(info) + 1; // + 1, т.к. учитываем нулевой байт

        fseek(table->fd, 0, SEEK_END);
        table->first[table->table_size].offset = ftell(table->fd);
        fwrite(info, sizeof(char), table->first[table->table_size].len, table->fd);

        ++(table->table_size);

        return 0; // Ok
    }
    return 1; // Table overflow
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
    return 1; // Ok
}

int findItem(Table *table, int key, int release) {
    int fread_res;
    for (int i = 0; i < table->table_size; i++) {
        if (table->first[i].key == key && table->first[i].release == release) {
            char *info = getInfo(table, i);
            if (info == NULL) {
                return 5; // File Read Error;
            }
            printf("Item: key = %d, release = %d, information = %s\n", key, release, info);
            free(info);
            return 0; // Ok
        }
    }
    return 2; // There is no such element
}

// ---------------------------------

char *getInfo(Table *table, int itemIndex) {
    int fread_res;
    char *info = (char *) malloc(sizeof(char) * table->first[itemIndex].len);
    fseek(table->fd, table->first[itemIndex].offset, SEEK_SET);
    if ((fread_res = fread(info, sizeof(char), table->first[itemIndex].len, table->fd)) == 0) {
        free(info);
        return NULL; // File Read Error
    }
    return info; // Ok
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
    return 1; // Ok
}

int findAllItems(Table *table, int key) {
    int fread_res;
    int isFound = 0;
    for (int i = 0; i < table->table_size; i++) {
        if (table->first[i].key == key) {
            char *info = getInfo(table, i);
            if (info == NULL) {
                return 5; // File Read Error;
            }
            printf("Item: key = %d, release = %d, information = %s\n", key, table->first[i].release, info);
            isFound = 1;
            free(info);
        }
    }
    
    if (isFound) {
        return 0; // Ok
    }
    return 2; // There is no such element
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
    for (int i = 0; i < table->table_size; i++) {
        if (table->first[i].key == key && table->first[i].release == release) {
            Item last_element = table->first[table->table_size - 1];

            Item *first_new = realloc(table->first, sizeof(Item) * (table->table_size - 1));
            if (first_new == NULL) {
                return 6; // Memory reallocation error
            }
            table->first = first_new;

            // Если удаляемый элемент - последний, то усекаем область памяти, выделенную под элементы
            // Иначе необходимо ещё и на место удаляемого элемента записать последний
            if (i < table->table_size - 1) {
                table->first[i] = last_element;
            }
            
            --(table->table_size);
            if (table->table_size == 0) {
                table->first = NULL;
            }
            return 0; // Ok
        }
    }

    return 2; // There is no such element
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
    int isFound = 0;
    for (int i = 0; i < table->table_size; i++) {
        if (table->first[i].key == key) {
            Item last_element = table->first[table->table_size - 1];

            Item *first_new = realloc(table->first, sizeof(Item) * (table->table_size - 1));
            if (first_new == NULL) {
                return 6; // Memory reallocation error
            }
            table->first = first_new;

            // Если удаляемый элемент - последний, то усекаем область памяти, выделенную под элементы
            // Иначе необходимо ещё и на место удаляемого элемента записать последний
            if (i < table->table_size - 1) {
                table->first[i] = last_element;
            }
            
            --i;
            --(table->table_size);
            isFound = 1;
        }
    }

    if (isFound){
        return 0; // Ok
    }
    return 2; // There is no such element
}

// ---------------------------------

int showTable(Table *table) {
    int result_status = 3; // Empty table by default

    if (table->table_size > 0) {
        result_status = 0;
        for (int i = 0; i < table->table_size; i++) {
            Item cur_item = table->first[i];
            char *info = getInfo(table, i);
            if (info == NULL) {
                result_status = 5; // File Read Error;
                break;
            }
            printf("Item №%d: key = %d, release = %d, information = %s\n", i + 1, cur_item.key, cur_item.release, info);
            free(info);
        }
    }
    printf("\nShow table: %s\n", errmsgs[result_status]);
    return 1;
}

// ---------------------------------

void delTable(Table *table) {
    if (table->first != NULL) {
        free(table->first);
        table->table_size = table->vector_size = 0;
        table->first = NULL;
    }
}

// ---------------------------------

int saveTable(Table *table) {
    if (!checkIsFileOpened(table)) {
        return 1; // File is not opened to save
    }

    fseek(table->fd, 0, SEEK_SET);
    fwrite(&table->vector_size, sizeof(int), 1, table->fd);
    fwrite(&table->table_size, sizeof(int), 1, table->fd);
    if (table->table_size != 0) {
        fwrite(table->first, sizeof(Item), table->table_size, table->fd);
    }

    int result_status = 0;
    char *info_arr[table->table_size];
    for (int i = 0; i < table->table_size; i++) {
        if ((info_arr[i] = getInfo(table, i)) == NULL) {
            result_status = 7;
            for (int j = 0; j < i; j++) {
                free(info_arr[j]);
            }
            printf("\nSave table: %s\n", errmsgs[result_status]);
            return result_status;
        }
    }

    int cumulative_offset = sizeof(int) * 2 + sizeof(Item) * table->vector_size;
    int fwrite_res;
    for (int i = 0; i < table->table_size; i++) {
        fseek(table->fd, cumulative_offset, SEEK_SET);
        if ((fwrite_res = fwrite(info_arr[i], sizeof(char), table->first[i].len, table->fd)) == 0) {
            result_status = 7;
            for (int j = 0; j < table->table_size; j++) {
                free(info_arr[j]);
            }
            printf("\nSave table: %s\n", errmsgs[result_status]);
            return result_status;
        }
        cumulative_offset += table->first[i].len;
    }

    fclose(table->fd);
    
    printf("\nSave table: %s\n", errmsgs[result_status]);
    return result_status; // Ok
}

// ---------------------------------

int dialogCreateTable(Table *table) {
    int vector_size, get_vector_size_result;
    puts("Enter vector size: -->");
    if ((get_vector_size_result = getInt(&vector_size)) == 0) {
        return 0; // EOF
    }

    puts("Enter file name: -->");
    char *fname = getStr();
    if (fname == NULL) {
        return 0; // EOF или ошибка при чтении
    }

    int create_result = createTable(table, fname, vector_size);
    printf("\nCreate table with name = %s and vector size = %d: %s\n", fname, vector_size, errmsgs[create_result]);

    free(fname);
    return 1; // Ok
}

// ---------------------------------

int createTable(Table *table, char *fname, int vector_size) {
    table->table_size = 0;
    table->vector_size = vector_size;
    if ((table->fd = fopen(fname, "w+b")) == NULL) {
        return 8; // CreateFileError
    }

    fwrite(&table->vector_size, sizeof(int), 1, table->fd);
    fwrite(&table->table_size, sizeof(int), 1, table->fd);

    char s[sizeof(Item) * table->vector_size];
    fwrite(s, sizeof(char), sizeof(Item) * table->vector_size, table->fd); // Чтобы сдвинуть SEEK_END для обеспечения нужного формата бинарного файла

    return 0; // Ok
}

// ---------------------------------

int dialogLoadTable(Table *table) {
    printf("Enter a file name to load the table: -->\n");
    char *fname = getStr();

    int load_table_result = loadTable(table, fname);

    printf("\nLoad table from file with the name = %s: %s\n", fname, errmsgs[load_table_result]);
    free(fname);

    if (load_table_result) {
        exit(1); // Loading error
    }
    return 1; // Ok
}

// ---------------------------------

int loadTable(Table *table, char *fname) {
    if ((table->fd = fopen(fname, "r+b")) != NULL) {
        if (fread(&table->vector_size, sizeof(int), 1, table->fd) == 0) {
            puts("br1");
            return 5; // File read error
        }

        if (fread(&table->table_size, sizeof(int), 1, table->fd) == 0) {
            return 5; // File read error
        }

        if ((table->first = (Item *) calloc(table->table_size, sizeof(Item))) == NULL) {
            return 4; // Memory allocation error
        }

        
        if (table->table_size > 0 && fread(table->first, sizeof(Item), table->table_size, table->fd) == 0) {
            free(table->first);
            return 5; // File read error
        }

        return 0;
    }
    return 9; // Cannot open the file
}

// ---------------------------------

int checkIsFileOpened(Table *table) {
    return table->fd != NULL;
}

// ---------------------------------