#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <stdlib.h>

#define MAX_TABLE_SIZE (31)
#define MAX_INFO_SIZE (100)

/*
    Элемент хеш-таблицы.

    Поле busy:
        -1 - элемент удален,
        0 - элемент свободен,
        1 - элемент занят.
*/
typedef struct Item {
    int busy;
    int key;
    char *info;
} Item;

/*
    Хеш-таблица, организованная вектором.
    Коллизии разрешаются открытой адресацией, а именно перемешиванием сложением через двойное хеширование.
*/
typedef struct Table {
    int size;
    Item items[MAX_TABLE_SIZE];
} Table;

/*
    Описание:
        initializeTable для каждой ячейки хеш-таблицы присваивает полю busy значение 0, означающее, что данный элемент свободен.
        Функция необходима для корректной работы функций-обработчиков таблицы, чтобы не полагаться на стратегию инициализации компилятора.
*/
void initializeTable(Table *table);

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
        firstHashCode - первая вспомогательная хеш-функция для двойного хеширования
    Возвращаемое значение:
        Хеш от ключа в диапазоне от 0 до MAX_TABLE_SIZE - 1
*/
int firstHashCode(int key);

/*
    Описание:
        secondHashCode - вторая вспомогательная хеш-функция для двойного хеширования.
        Гарантируется, что MAX_TABLE_SIZE и secondHashCode - взаимно простые числа при условии, что MAX_TABLE_SIZE - простое число.
        Это обеспечивает, что последовательность проб, генерируемая основной хеш-функцией, содержит номер каждой ячейки хеш-таблицы ровно 1 раз.
    Возвращаемое значение:
        Хеш от ключа в диапазоне от 1 до MAX_TABLE_SIZE - 1
*/
int secondHashCode(int key);

/*
    Описание:
        dialog реализует основной диалог с пользователем.
        Запрашивает номер операции, которую необходимо выполнить, валидирует и возвращает его.

    Возвращаемое значение:
        Номер выбранной опции (см. определение const char *msgs[]).
*/
int dialog(const char *msgs[], int NMsgs);

/*
    Описание:
        dialogAddItem - диалоговая функция для вставки элемента в хеш-таблицу.
        Запрашивается ключ и информация, а затем делегируется вызов функции вставки - addItem.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogAddItem(Table *table);

/*
    Описание:
        addItem осуществляет вставку элемента в хеш-таблицу.

    Возвращаемое значение:
        0 - элемент вставлен,
        1 - таблица полная - вставка неуспешна.
*/
int addItem(Table *table, int key, char *info);

/*
    Описание:
        dialogFindItem - диалоговая функция для поиска конкретного элемента в хеш-таблице.
        Запрашивается ключ и версия элемента, а затем делегируется вызов функции поиска - findItem.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogFindItem(Table *table);

/*
    Описание:
        findItem осуществляет поиск элемента в хеш-таблице по заданному ключу.

    Возвращаемое значение:
        0 - элемент найден,
        2 - элемент не найден.
*/
Item *findItem(Table *table, int key);

/*
    Описание:
        dialogDeleteItem - диалоговая функция для удаления конкретного элемента из хеш-таблицы.
        Запрашивается ключ, а затем делегируется вызов функции удаления - deleteItem.

    Возвращаемое значение:
        0 - в случае, если был обнаружен EOF при вызове getInt,
        1 - во всех остальных случаях.
*/
int dialogDeleteItem(Table *table);

/*
    Описание:
        deleteItem осуществляет удаление элемента по ключу из хеш-таблицы.

    Возвращаемое значение:
        0 - элемент удален,
        2 - элемент с таким ключом не найден.
*/
int deleteItem(Table *table, int key);

/*
    Описание:
        showTable осуществляет вывод содержимого хеш-таблицы.
    
    Возвращаемое значение:
        1 - завершение вывода.
*/
int showTable(Table *table);

/*
    Описание:
        delTable осуществляет удаление из хеш-таблицы элемента по его ключу и освобождение выделенной под информацию памяти.
*/
void delTable(Table *table);

#endif // MAIN_HEADER