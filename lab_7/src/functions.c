#include "main.h"

const char *vertex_types[3] = {"default", "entrance", "exit"};
// const char *errmsgs[] = {"Ok", "Key duplicate", "No node with such a key", "Empty B-tree", "No successor or such node in B-tree"};
const char *errmsgs[] = {"Ok",
                         "Such vertex already exists",
                         "Graph doesn't have such vertex(vertices)",
                         "These vertices are the same",
                         "These vertices are not neighbors",
                         "These vertices are already linked",
                         "These vertices are not linked"};

// const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Find next min", "4. Delete", "5. Show"};
// const char *msgs[] = {"0. Quit", "1. Add vertex", "2. Add edge", "3. Delete vertex", "4. Delete edge", "5. Change vertex information", "6. Show graph"};
const char *msgs[] = {"0. Quit", "1. Add vertex", "2. Add edge", "3. Delete vertex", "4. Delete edge", "5. Show graph"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

// int (*dialog_options[])(Node **root) = {NULL, dialog_b_tree_insert, dialog_b_tree_search, dialog_b_tree_find_successor, dilog_b_tree_delete, b_tree_print};
int (*dialog_options[])(Graph *graph) = {NULL, dialog_create_vertex, dialog_add_edge, dialog_delete_vertex, dialog_delete_edge, print_graph};

// ---------------------------------

int get_int(int *num, int is_vertex_type) {
    int scanf_result;
    do {
        scanf_result = scanf("%d", num);

        if (scanf_result == -1) {
            return 0;
        }

        if (scanf_result == 0 || (is_vertex_type == 1 && (*num < 0 || *num > 2) || *num < 0)) {
            scanf("%*[^\n]");
            printf("%s", "Try once again!: ");
            scanf_result = 0;
        }
    } while (scanf_result == 0);
    
    scanf("%*[^\n]");
    return 1;
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

        n = get_int(&option_num, 0);

        if (n == 0) // EOF -> quit from program
            option_num = 0;
    } while (option_num < 0 || option_num >= NMsgs);
    return option_num;
}

// ---------------------------------

int search_vertex(Graph *graph, int x, int y) {
    for (int i = 0; i < graph->num_vertices; i++) {
        if (*(graph->adj_list[i]->x) == x && *(graph->adj_list[i]->y) == y) {
            return i;
        }
    }
    return -1;
}

// ---------------------------------

void initialize_vertex(Vertex *vertex_ptr, int x, int y, int type, int id) {
    int *x_ptr = (int *) malloc(sizeof(int));
    int *y_ptr = (int *)malloc(sizeof(int));
    int *type_ptr = (int *)malloc(sizeof(int));
    int *id_ptr = (int *)malloc(sizeof(int));

    *x_ptr = x;
    *y_ptr = y;
    *type_ptr = type;
    *id_ptr = id;

    vertex_ptr->id = id_ptr;
    vertex_ptr->x = x_ptr;
    vertex_ptr->y = y_ptr;
    vertex_ptr->type = type_ptr;
    vertex_ptr->next = NULL;
}

// ---------------------------------

int dialog_create_vertex(Graph *graph) {
    int x, y, type, get_int_result;

    puts("Enter X coordinate: -->");
    if ((get_int_result = get_int(&x, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter Y coordinate: -->");
    if ((get_int_result = get_int(&y, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter vertex type (0 - default, 1 - entrance, 2 - exit): -->");
    if ((get_int_result = get_int(&type, 1)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    Vertex *created_vertex = create_vertex(graph, x, y, type);
    int create_vertex_result = (created_vertex == NULL) ? 1 : 0;
    printf("\nResult of creating vertex {x = %d, y = %d, type = %s}: %s\n", x, y, vertex_types[type], errmsgs[create_vertex_result]);
    return 1;
}

Vertex *create_vertex(Graph *graph, int x, int y, int type) {
    int search_vertex_result = search_vertex(graph, x, y);
    if (search_vertex_result >= 0) {
        return NULL;
    }

    Vertex *new_vertex_ptr = (Vertex *) malloc(sizeof(Vertex));
    initialize_vertex(new_vertex_ptr, x, y, type, graph->num_vertices);

    graph->adj_list = (Vertex **) realloc(graph->adj_list, sizeof(Vertex *) * (graph->num_vertices + 1));
    graph->adj_list[graph->num_vertices] = new_vertex_ptr;
    graph->num_vertices++;

    return new_vertex_ptr;
}

// ---------------------------------

int are_vertices_neighbors(Graph *graph, int first_vertex_idx, int second_vertex_idx) {
    Vertex *vertex1 = graph->adj_list[first_vertex_idx];
    Vertex *vertex2 = graph->adj_list[second_vertex_idx];
    if (abs(*(vertex1->x) - *(vertex2->x)) == 1 && *(vertex1->y) == *(vertex2->y) || abs(*(vertex1->y) - *(vertex2->y)) == 1 && *(vertex1->x) == *(vertex2->x)) {
        return 1;  // Neighbors
    }
    return 0; 
}

// ---------------------------------

int are_vertices_linked(Graph *graph, int first_vertex_idx, int second_vertex_idx) {
    Vertex *cur_vertex_ptr = graph->adj_list[first_vertex_idx];
    while (cur_vertex_ptr->next != NULL) {
        cur_vertex_ptr = cur_vertex_ptr->next;
        if (*(cur_vertex_ptr->id) == second_vertex_idx) {
            return 1;  // These vertices are linked
        }
    }
    return 0;
}

// ---------------------------------

void update_adj_list_to_add_edge(Graph *graph, int first_vertex_idx, int second_vertex_idx) {
    Vertex *vertices_ptr_pair[2] = {graph->adj_list[first_vertex_idx], graph->adj_list[second_vertex_idx]};

    for (int i = 0; i < 2; i++) {
        Vertex *cur_vertex_ptr = vertices_ptr_pair[i];

        Vertex *copy_vertex_ptr = (Vertex *) malloc(sizeof(Vertex));
        *copy_vertex_ptr = *vertices_ptr_pair[i ^ 1];
        copy_vertex_ptr->next = NULL;

        while (cur_vertex_ptr->next != NULL) {
            cur_vertex_ptr = cur_vertex_ptr->next;
        }
        cur_vertex_ptr->next = copy_vertex_ptr;
    }
}

// ---------------------------------

int dialog_add_edge(Graph *graph) {
    int x1, y1;
    int x2, y2;

    const char *msgs[4] = {"Enter X coordinate of the first vertex: -->",
                           "Enter Y coordinate of the first vertex: -->",
                           "Enter X coordinate of the second vertex: -->",
                           "Enter Y coordinate of the second vertex: -->"};
    
    int *coordinate_ptrs_arr[4] = {&x1, &y1, &x2, &y2};

    for (int i = 0; i < 4; i++) {
        puts(msgs[i]);
        if (get_int(coordinate_ptrs_arr[i], 0) == 0) {
            return 0;  // EOF -> игнорируем весь остальной ввод
        }
    }

    int add_edge_result = add_edge(graph, x1, y1, x2, y2);
    printf("\nResult of adding edge ((%d, %d), (%d, %d)): %s\n", x1, y1, x2, y2, errmsgs[add_edge_result]);
    return 1;
}

int add_edge(Graph *graph, int x1, int y1, int x2, int y2) {
    int first_vertex_idx = search_vertex(graph, x1, y1);
    int second_vertex_idx = search_vertex(graph, x2, y2);

    if (first_vertex_idx == -1 || second_vertex_idx == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    if (first_vertex_idx == second_vertex_idx) {
        return 3;  // These vertices are the same
    }

    if (!are_vertices_neighbors(graph, first_vertex_idx, second_vertex_idx)) {
        return 4;  // These vertices are not neighbors
    }

    if (are_vertices_linked(graph, first_vertex_idx, second_vertex_idx)) {
        return 5;  // These vertices are already linked
    }

    update_adj_list_to_add_edge(graph, first_vertex_idx, second_vertex_idx);
    return 0;
}

// ---------------------------------

int dialog_delete_vertex(Graph *graph) {
    int x, y, get_int_result;

    puts("Enter X coordinate: -->");
    if ((get_int_result = get_int(&x, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter Y coordinate: -->");
    if ((get_int_result = get_int(&y, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int delete_vertex_result = delete_vertex(graph, x, y);
    printf("\nResult of deleting vertex (%d, %d): %s\n", x, y, errmsgs[delete_vertex_result]);
    return 1;
}

// ---------------------------------

int delete_vertex(Graph *graph, int x, int y) {
    int vertex_id = search_vertex(graph, x, y);
    if (vertex_id == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    Vertex *cur_adj_vertex_ptr = graph->adj_list[vertex_id]->next;
    while (cur_adj_vertex_ptr != NULL) {
        int cur_adj_vertex_id = *(cur_adj_vertex_ptr->id);
        Vertex *cur_vertex_ptr = graph->adj_list[cur_adj_vertex_id];
        while (*(cur_vertex_ptr->next->id) != vertex_id) {
            cur_vertex_ptr = cur_vertex_ptr->next;
        }
        Vertex *vertex_to_del_ptr = cur_vertex_ptr->next;
        cur_vertex_ptr->next = vertex_to_del_ptr->next;
        vertex_to_del_ptr->next = NULL;
        free(vertex_to_del_ptr);
        vertex_to_del_ptr = NULL;

        graph->adj_list[vertex_id]->next = cur_adj_vertex_ptr->next;
        cur_adj_vertex_ptr->next = NULL;
        free(cur_adj_vertex_ptr);
        cur_adj_vertex_ptr = graph->adj_list[vertex_id]->next;
    }

    free(graph->adj_list[vertex_id]->id);
    free(graph->adj_list[vertex_id]->x);
    free(graph->adj_list[vertex_id]->y);
    free(graph->adj_list[vertex_id]->type);
    free(graph->adj_list[vertex_id]);

    for (int i = vertex_id; i < graph->num_vertices - 1; i++) {
        graph->adj_list[i] = graph->adj_list[i + 1];
        *(graph->adj_list[i + 1]->id) -= 1;
    }
    graph->adj_list = (Vertex **) realloc(graph->adj_list, sizeof(Vertex *) * (graph->num_vertices - 1));
    graph->num_vertices--;

    return 0;  // Ok
}

// ---------------------------------

int dialog_delete_edge(Graph *graph) {
    int x1, y1;
    int x2, y2;

    const char *msgs[4] = {"Enter X coordinate of the first vertex: -->",
                           "Enter Y coordinate of the first vertex: -->",
                           "Enter X coordinate of the second vertex: -->",
                           "Enter Y coordinate of the second vertex: -->"};
    
    int *coordinate_ptrs_arr[4] = {&x1, &y1, &x2, &y2};

    for (int i = 0; i < 4; i++) {
        puts(msgs[i]);
        if (get_int(coordinate_ptrs_arr[i], 0) == 0) {
            return 0;  // EOF -> игнорируем весь остальной ввод
        }
    }
    
    int delete_edge_result = delete_edge(graph, x1, y1, x2, y2);
    printf("\nResult of deleting edge ((%d, %d), (%d, %d)): %s\n", x1, y1, x2, y2, errmsgs[delete_edge_result]);
    return 1;
}

void update_adj_list_to_delete_edge(Graph *graph, int first_vertex_idx, int second_vertex_idx) {
    Vertex *vertices_ptr_pair[2] = {graph->adj_list[first_vertex_idx], graph->adj_list[second_vertex_idx]};

    for (int i = 0; i < 2; i++) {
        Vertex *cur_vertex_ptr = vertices_ptr_pair[i];
        int another_vertex_id = *(vertices_ptr_pair[i ^ 1]->id);

        while (*(cur_vertex_ptr->next->id) != another_vertex_id) {
            cur_vertex_ptr = cur_vertex_ptr->next;
        }
        Vertex *another_vertex_ptr = cur_vertex_ptr->next;
        cur_vertex_ptr->next = another_vertex_ptr->next;

        another_vertex_ptr->id = NULL;
        another_vertex_ptr->x = NULL;
        another_vertex_ptr->y = NULL;
        another_vertex_ptr->type = NULL;
        another_vertex_ptr->next = NULL;

        free(another_vertex_ptr);
    }
}

int delete_edge(Graph *graph, int x1, int y1, int x2, int y2) {
    int first_vertex_idx = search_vertex(graph, x1, y1);
    int second_vertex_idx = search_vertex(graph, x2, y2);

    if (first_vertex_idx == -1 || second_vertex_idx == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    if (first_vertex_idx == second_vertex_idx) {
        return 3;  // These vertices are the same
    }

    if (!are_vertices_linked(graph, first_vertex_idx, second_vertex_idx)) {
        return 6;  // These vertices are not linked
    }

    update_adj_list_to_delete_edge(graph, first_vertex_idx, second_vertex_idx);

    return 0;  // Ok
}

// ---------------------------------

int print_graph(Graph *graph) {
    puts("");

    if (graph->num_vertices == 0) {
        puts("Empty graph");
        return 1;
    }

    for (int i = 0; i < graph->num_vertices; i++) {
        Vertex *cur_vertex_ptr = graph->adj_list[i];
        printf("%d {(%d, %d), %s}: ", *(cur_vertex_ptr->id), *(cur_vertex_ptr->x), *(cur_vertex_ptr->y), vertex_types[*(cur_vertex_ptr->type)]);

        int adjacent_vertices_num = 0;
        while (cur_vertex_ptr->next != NULL) {
            if (adjacent_vertices_num) {
                printf("%s", " -> ");
            }
            cur_vertex_ptr = cur_vertex_ptr->next;
            printf("%d {(%d, %d), %s}", *(cur_vertex_ptr->id), *(cur_vertex_ptr->x), *(cur_vertex_ptr->y), vertex_types[*(cur_vertex_ptr->type)]);
            adjacent_vertices_num++;
        }
        puts("");
    }
    return 1;
}

// ---------------------------------
