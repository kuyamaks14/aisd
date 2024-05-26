#ifndef MAIN_HEADER
#define MAIN_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

typedef struct Vertex {
    int id;
    int x;
    int y;
    int type;
    struct Vertex *next;
} Vertex;

typedef struct Graph {
    int num_vertices;
    Vertex **adj_list;
} Graph;

/*
    Описание:
        get_int используется для ввода целого числа из stdin и присваивания его переменной по переданному указателю.

    Возвращаемое значение:
        0 - если был обнаружен EOF,
        1 - в остальных случаях.
*/
int get_int(int *num, int is_vertex_type);

/*
    Описание:
        dialog реализует основной диалог с пользователем.
        Запрашивает номер операции, которую необходимо выполнить, валидирует и возвращает его.

    Возвращаемое значение:
        Номер выбранной опции (см. определение const char *msgs[]).
*/
int dialog(const char *msgs[], int NMsgs);

/*
 */
int search_vertex(Graph *graph, int x, int y);

/*
*/
void initialize_vertex(Vertex *vertex_ptr, int x, int y, int type, int id);

/*
*/
int dialog_create_vertex(Graph *graph);

/*
*/
Vertex *create_vertex(Graph *graph, int x, int y, int type);

/*
*/
int are_vertices_neighbors(Graph *graph, int first_vertex_idx, int second_vertex_idx);

/*
*/
int are_vertices_linked(Graph *graph, int first_vertex_idx, int second_vertex_idx);

/*
*/
void update_adj_list_to_add_edge(Graph *graph, int first_vertex_idx, int second_vertex_idx);

/*
*/
int dialog_add_edge(Graph *graph);

/*
*/
int add_edge(Graph *graph, int x1, int y1, int x2, int y2);

/*
*/
int dialog_delete_vertex(Graph *graph);

/*
*/
int delete_vertex(Graph *graph, int x, int y);


/*
*/
int print_graph(Graph *graph);

#endif // MAIN_HEADER