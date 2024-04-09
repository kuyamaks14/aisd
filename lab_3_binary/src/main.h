#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MAX_TABLE_SIZE (10)
#define MAX_INFO_SIZE (100)

/*
    Элемент просматриваемой таблицы-вектора.
*/
typedef struct Item {
    int key;
    int release;
    int offset;
    int len;
} Item;

/*
    Просматриваемая таблица-вектор.
*/
typedef struct Table {
    int vector_size;
    int table_size;
    // int allocated_size;
    Item *first;
    FILE *fd;
} Table;

/*
    Описание:
        dialog реализует основной диалог с пользователем.
        Запрашивает номер операции, которую необходимо выполнить, валидирует и возвращает его.

    Возвращаемое значение:
        Номер выбранной опции (см. определение const char *msgs[]).
*/
int dialog(Table *table, const char *msgs[], int NMsgs);

/*
    Описание:
        getInt используется для ввода целого числа из stdin и присваивания его переменной по переданному указателю.

    Возвращаемое значение:
        0 - если был обнаружен EOF,
        1 - в остальных случаях.
*/
int getInt(int *num);

/*
    Описание:
        getStr используется для ввода строки из stdin и возвращения указателя на нее.

    Возвращаемое значение:
        NULL - если был обнаружен EOF,
        1 - в остальных случаях.
*/
char *getStr();

/*
    Описание:
        flushBuffer предназначен для очистки потока ввода.
*/
void flushBuffer(void);

/*
    Описание:
        dialogAddItem - диалоговая функция для вставки элемента в таблицу.
        Запрашивается ключ и информация, а затем делегируется вызов функции вставки - addItem.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogAddItem(Table *table);

int addItem(Table *table, int key, char *info);

/*
    Описание:
        dialogFindItem - диалоговая функция для поиска конкретного элемента в таблице.
        Запрашивается ключ и версия элемента, а затем делегируется вызов функции поиска - findItem.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogFindItem(Table *table);

/*
    Описание:
        findItem осуществляет поиск конкретного элемента в таблице.

    Возвращаемое значение:
        0 - элемент найден,
        2 - элемент не найден.
*/
int findItem(Table *table, int key, int release);

/*
    Описание:
        dialogFindAllItems - диалоговая функция для поиска всех элементов с одинаковым ключом в таблице.
        Запрашивается ключ элемента, а затем делегируется вызов функции поиска - findAllItems.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogFindAllItems(Table *table);

/*
    Описание:
        findAllItems осуществляет поиск всех элементов с одинаковым ключом в таблице.

    Возвращаемое значение:
        0 - элемент найден,
        2 - элемент не найден.
*/
int findAllItems(Table *table, int key);

char *getInfo(Table *table, int itemIndex);

/*
    Описание:
        dialogDeleteItem - диалоговая функция для удаления конкретного элемента из таблицы.
        Запрашивается ключ и версия элемента, а затем делегируется вызов функции удаления - deleteItem.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogDeleteItem(Table *table);

/*
    Описание:
        deleteItem осуществляет удаление конкретного элемента из таблицы.

    Возвращаемое значение:
        0 - элемент удален,
        2 - элемент с таким ключом не найден.
*/
int deleteItem(Table *table, int key, int release);

/*
    Описание:
        dialogDeleteAllItems - диалоговая функция для удаления всех элементов с одинаковым ключом в таблице.
        Запрашивается ключ элемента, а затем делегируется вызов функции поиска - deleteAllItems.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogDeleteAllItems(Table *table);

/*
    Описание:
        deleteAllItems осуществляет удаление всех элементов с одинаковым ключом в таблице.

    Возвращаемое значение:
        0 - элементы удалены,
        2 - ни один элемент с таким ключом не найден.
*/
int deleteAllItems(Table *table, int key);

/*
    Описание:
        showTable осуществляет вывод информации по всем элементам таблицы.

    Возвращаемое значение:
        1 - завершение вывода.
*/
int showTable(Table *table);

int saveTable(Table *table);

void delTable(Table *table);

int dialogLoadTable(Table *table);

int loadTable(Table *table, char *fname);

int dialogCreateTable(Table *table);

int createTable(Table *table, char *fname, int vector_size);

int checkIsFileOpened(Table *table);

#endif // MAIN_HEADER