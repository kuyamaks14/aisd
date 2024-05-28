#include "main.h"

const char *vertex_types[3] = {"default", "entrance", "exit"};

const char *errmsgs[] = {"Ok",
                         "Such vertex already exists",
                         "Graph doesn't have such vertex(vertices)",
                         "These vertices are the same",
                         "These vertices are not neighbors",
                         "These vertices are already linked",
                         "These vertices are not linked",
                         "Remove all edges linked with this vertex to change it coordinate",
                         "This vertex already has this type",
                         "This vertex is not entrance",
                         "No path from this entrance to any kind of exit",
                         "This vertex is not exit",
                         "No path between the specified entrance and exit"};

const char *msgs[] = {"0. Quit",
                      "1. Add vertex",
                      "2. Add edge",
                      "3. Delete vertex",
                      "4. Delete edge",
                      "5. Change vertex information",
                      "6. Check achievability from a specified entrance to at least one exit",
                      "7. Find the shortest path between an entrance and an exit specified",
                      "8. Show graph"};

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*dialog_options[])(Graph *graph) = {NULL,
                                         dialog_create_vertex,
                                         dialog_add_edge,
                                         dialog_delete_vertex,
                                         dialog_delete_edge,
                                         dialog_change_vertex_information,
                                         dialog_check_achievability_exits,
                                         dialog_dijkstra,
                                         print_graph};

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
    if (abs(*(vertex1->x) - *(vertex2->x)) == WEIGHT && *(vertex1->y) == *(vertex2->y) || abs(*(vertex1->y) - *(vertex2->y)) == WEIGHT && *(vertex1->x) == *(vertex2->x)) {
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

int dialog_change_vertex_information(Graph *graph) {
    int x, y, get_int_result;

    puts("Enter X coordinate: -->");
    if ((get_int_result = get_int(&x, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter Y coordinate: -->");
    if ((get_int_result = get_int(&y, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("\nWhat do you want to change?");
    const char *err_msg = "";
    const char *options[] = {"0. X coordinate", "1. Y coordinate", "2. Vertex type"};
    int options_num = (int)(sizeof(options) / sizeof(options[0]));
    int chosen_option;
    do { 
        puts(err_msg);
        err_msg = "You are wrong, repeat please!";

        for (int i = 0; i < options_num; i++) {
            puts(options[i]);
        }
        puts("Make your choice: --> ");

        if (get_int(&chosen_option, 0) == 0) {
            return 0;  // EOF -> quit from program
        }
    } while (chosen_option < 0 || chosen_option >= options_num);
        
    if (chosen_option == 0) {
        int new_x;
        puts("Enter new X coordinate: -->");
        if ((get_int_result = get_int(&new_x, 0)) == 0) {
            return 0;  // EOF -> игнорируем весь остальной ввод
        }
        int change_vertex_information_result = change_vertex_information(graph, x, y, &new_x, NULL, NULL);
        printf("\nResult of changing X coordinate to %d: %s\n", new_x, errmsgs[change_vertex_information_result]);
    } else if (chosen_option == 1) {
        int new_y;
        puts("Enter new Y coordinate: -->");
        if ((get_int_result = get_int(&new_y, 0)) == 0) {
            return 0;  // EOF -> игнорируем весь остальной ввод
        }
        int change_vertex_information_result = change_vertex_information(graph, x, y, NULL, &new_y, NULL);
        printf("\nResult of changing Y coordinate to %d: %s\n", new_y, errmsgs[change_vertex_information_result]);
    } else {
        int new_type;
        puts("Enter new vertex type (0 - default, 1 - entrance, 2 - exit): -->");
        if ((get_int_result = get_int(&new_type, 1)) == 0) {
            return 0;  // EOF -> игнорируем весь остальной ввод
        }
        int change_vertex_information_result = change_vertex_information(graph, x, y, NULL, NULL, &new_type);
        printf("\nResult of changing vertex type to %s: %s\n", vertex_types[new_type], errmsgs[change_vertex_information_result]);
    }

    return 1;
}

int change_vertex_information(Graph *graph, int x, int y, int *new_x_ptr, int *new_y_ptr, int *new_type_ptr) {
    int vertex_idx = search_vertex(graph, x, y);
    if (vertex_idx == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    if (new_x_ptr != NULL || new_y_ptr != NULL) {
        if (graph->adj_list[vertex_idx]->next != NULL) {
            return 7;  // Remove all edges linked with this vertex to change it coordinate
        }

        if (new_x_ptr != NULL) {
            if (search_vertex(graph, *new_x_ptr, *(graph->adj_list[vertex_idx]->y)) != -1) {
                return 1;  // Such vertex already exists
            }
            *(graph->adj_list[vertex_idx]->x) = *new_x_ptr;
        } else {
            if (search_vertex(graph, *(graph->adj_list[vertex_idx]->x), *new_y_ptr) != -1) {
                return 1;  // Such vertex already exists
            }
            *(graph->adj_list[vertex_idx]->y) = *new_y_ptr;
        }
    } else {
        if (*(graph->adj_list[vertex_idx]->type) == *new_type_ptr) {
            return 8;  // This vertex already has this type
        }
        *(graph->adj_list[vertex_idx]->type) = *new_type_ptr;
    }

    return 0;  // Ok
}

// ---------------------------------

int dialog_check_achievability_exits(Graph *graph) {
    int x, y, get_int_result;

    puts("Enter X coordinate of an entrance: -->");
    if ((get_int_result = get_int(&x, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter Y coordinate of an entrance: -->");
    if ((get_int_result = get_int(&y, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int check_achievability_exits_result = check_achievability_exits(graph, x, y);
    printf("\nResult of finding path from this entrance to any kind of exit: %s\n", errmsgs[check_achievability_exits_result]);
    return 1;
}

int check_achievability_exits(Graph *graph, int x, int y) {
    int entrance_idx = search_vertex(graph, x, y);
    if (entrance_idx == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    if (*(graph->adj_list[entrance_idx]->type) != 1) {
        return 9;  // This vertex is not an entrance;
    }

    int DFS_result = DFS(graph, entrance_idx);
    return DFS_result;
}

int DFS(Graph *graph, int entrance_idx) {
    int pred_idx_arr[graph->num_vertices];
    int color_arr[graph->num_vertices];  // 0 - white, 1 - gray, 2 - black
    int discovery_time_arr[graph->num_vertices];
    int finish_time_arr[graph->num_vertices];

    // DFS Initialization
    for (int i = 0; i < graph->num_vertices; i++) {
        pred_idx_arr[i] = -1;
        color_arr[i] = 0;
    }
    int time = 0;

    // DFS Processing for the specified entrance
    int found_exit_idx = DFS_visit(graph, entrance_idx, pred_idx_arr, color_arr, discovery_time_arr, finish_time_arr, &time);
    if (found_exit_idx == -1) {
        return 10;  // No path from this entrance to any kind of exit
    }

    puts("\nOne of the possible paths:");
    DFS_print_path(graph, found_exit_idx, pred_idx_arr, 1);
    return 0;  // Ok
}

int DFS_visit(Graph *graph, int vertex_idx, int pred_idx_arr[], int color_ptr[], int discovery_time_arr[], int finish_time_arr[], int *time_ptr) {
    // DFS modification to find at least one exit from the specifiend entrance
    if (*(graph->adj_list[vertex_idx]->type) == 2) {
        return vertex_idx;  // This vertex is an exit
    }

    color_ptr[vertex_idx] = 1;
    *time_ptr += 1;
    discovery_time_arr[vertex_idx] = *time_ptr;

    Vertex *adj_vertex_ptr = graph->adj_list[vertex_idx]->next;
    while (adj_vertex_ptr != NULL) {
        if (color_ptr[*(adj_vertex_ptr->id)] == 0) {
            pred_idx_arr[*(adj_vertex_ptr->id)] = vertex_idx;

            int found_exit_idx = DFS_visit(graph, *(adj_vertex_ptr->id), pred_idx_arr, color_ptr, discovery_time_arr, finish_time_arr, time_ptr);
            if (found_exit_idx != -1) {
                return found_exit_idx;  // An exit is found
            }
        }

        adj_vertex_ptr = adj_vertex_ptr->next;
    }

    color_ptr[vertex_idx] = 2;
    *time_ptr += 1;
    finish_time_arr[vertex_idx] = *time_ptr;

    return -1;  // It means this vertex is not an exit
}

void DFS_print_path(Graph *graph, int vertex_idx, int pred_idx_arr[], int flag_first_call) {
    int id = *(graph->adj_list[vertex_idx]->id);
    int x = *(graph->adj_list[vertex_idx]->x);
    int y = *(graph->adj_list[vertex_idx]->y);
    int type = *(graph->adj_list[vertex_idx]->type);

    if (pred_idx_arr[vertex_idx] == -1) {
        printf("{№%d, (%d, %d), %s} -> ", id, x, y, vertex_types[type]);
    } else {
        DFS_print_path(graph, pred_idx_arr[vertex_idx], pred_idx_arr, 0);
        if (flag_first_call) {
            printf("{№%d, (%d, %d), %s}\n", id, x, y, vertex_types[type]);
        } else {
            printf("{№%d, (%d, %d), %s} -> ", id, x, y, vertex_types[type]);
        }
    }
}

// ---------------------------------

// time: O(nlog(n))
PriorityQueue *build_min_heap(Graph *graph, int *default_priority_ptr) {
    // Initialization of a queue with priorities
    PriorityQueue *queue_ptr = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    queue_ptr->elem_ptr_arr = (PriorityQueueElem **) malloc(sizeof(PriorityQueueElem *) * graph->num_vertices);

    queue_ptr->size = graph->num_vertices;
    for (int i = 0; i < graph->num_vertices; i++) {
        PriorityQueueElem *queue_elem_ptr = (PriorityQueueElem *) malloc(sizeof(PriorityQueueElem));
        queue_elem_ptr->vertex_ptr = graph->adj_list[i];
        queue_elem_ptr->pred_vertex_ptr = NULL;
        if (default_priority_ptr != NULL) {
            queue_elem_ptr->priority = *default_priority_ptr;   
        }

        queue_ptr->elem_ptr_arr[i] = queue_elem_ptr;
    }

    int heap_size = queue_ptr->size;
    int idx = (heap_size - 1) / 2;
    while (idx >= 0) {
        min_heapify(queue_ptr, idx);
        idx--;
    }

    return queue_ptr;
}

void erase_binary_heap(PriorityQueue *queue_ptr, int queue_length) {
    for (int i = 0; i < queue_length; i++) {
        free(queue_ptr->elem_ptr_arr[i]);
    }

    free(queue_ptr->elem_ptr_arr);
    free(queue_ptr);
}

// time: O(log(n))
void min_heapify(PriorityQueue *queue_ptr, int idx) {
    int left_idx = 2*idx + 1;
    int right_idx = 2*idx + 2;

    if (left_idx < queue_ptr->size) {
        int smallest_idx;

        int cur_elem_priority = queue_ptr->elem_ptr_arr[idx]->priority;
        int left_elem_priority = queue_ptr->elem_ptr_arr[left_idx]->priority;

        if (right_idx < queue_ptr->size) {
            int right_elem_priority = queue_ptr->elem_ptr_arr[right_idx]->priority;

            if (cur_elem_priority <= left_elem_priority && cur_elem_priority <= right_elem_priority) {
                smallest_idx = idx;
            } else if (left_elem_priority <= cur_elem_priority && left_elem_priority <= right_elem_priority) {
                smallest_idx = left_idx;
            } else {
                smallest_idx = right_idx;
            }
        } else {
            if (cur_elem_priority <= left_elem_priority) {
                smallest_idx = idx;
            } else {
                smallest_idx = left_idx;
            }
        }

        if (smallest_idx != idx) {
            PriorityQueueElem *cur_elem_ptr = queue_ptr->elem_ptr_arr[idx];
            queue_ptr->elem_ptr_arr[idx] = queue_ptr->elem_ptr_arr[smallest_idx];
            queue_ptr->elem_ptr_arr[smallest_idx] = cur_elem_ptr;
            min_heapify(queue_ptr, smallest_idx);
        }
    }
}

// time: O(log(n))
PriorityQueueElem *extract_min(PriorityQueue *queue_ptr) {
    if (queue_ptr->size == 0) {
        return NULL;  // Queue is empty
    }

    PriorityQueueElem *min_elem_ptr = queue_ptr->elem_ptr_arr[0];
    queue_ptr->elem_ptr_arr[0] = queue_ptr->elem_ptr_arr[queue_ptr->size - 1];
    queue_ptr->elem_ptr_arr[queue_ptr->size - 1] = min_elem_ptr; // To avoid losing the pointer and correctly free memory later
    queue_ptr->size -= 1;

    min_heapify(queue_ptr, 0);

    return min_elem_ptr;
}

int decrease_priority(PriorityQueue *queue_ptr, int idx, int new_priority) {
    if (new_priority >= queue_ptr->elem_ptr_arr[idx]->priority) {
        return 1;  // Request denied
    }

    queue_ptr->elem_ptr_arr[idx]->priority = new_priority;

    PriorityQueueElem *cur_queue_elem_ptr;
    while (idx > 0 && queue_ptr->elem_ptr_arr[(idx - 1) / 2]->priority > queue_ptr->elem_ptr_arr[idx]->priority) {
        cur_queue_elem_ptr = queue_ptr->elem_ptr_arr[idx];
        queue_ptr->elem_ptr_arr[idx] = queue_ptr->elem_ptr_arr[(idx - 1) / 2];
        queue_ptr->elem_ptr_arr[(idx - 1) / 2] = cur_queue_elem_ptr;

        idx = (idx - 1) / 2;
    }

    return 0;  // Ok
}

int get_elem_idx_by_vertex_idx(PriorityQueue *queue_ptr, int queue_length, int vertex_idx) {
    for (int i = 0; i < queue_length; i++) {
        if (*(queue_ptr->elem_ptr_arr[i]->vertex_ptr->id) == vertex_idx) {
            return i;
        }
    }
    return -1;
}



// ---------------------------------

int dialog_dijkstra(Graph *graph) {
    int x1, y1;
    int x2, y2;

    const char *msgs[4] = {"Enter X coordinate of the entrance: -->",
                           "Enter Y coordinate of the entrance: -->",
                           "Enter X coordinate of the exit: -->",
                           "Enter Y coordinate of the exit: -->"};
    
    int *coordinate_ptrs_arr[4] = {&x1, &y1, &x2, &y2};

    for (int i = 0; i < 4; i++) {
        puts(msgs[i]);
        if (get_int(coordinate_ptrs_arr[i], 0) == 0) {
            return 0;  // EOF -> игнорируем весь остальной ввод
        }
    }

    int find_shortest_path_between_entrance_exit_result = find_shortest_path_between_entrance_exit(graph, x1, y1, x2, y2);
    printf("\nResult of finding the shortest path between the specified entrance (%d, %d) and exit (%d, %d): %s\n", x1, y1, x2, y2, errmsgs[find_shortest_path_between_entrance_exit_result]);
    return 1;
}

int find_shortest_path_between_entrance_exit(Graph *graph, int x1, int y1, int x2, int y2) {
    int entrance_idx = search_vertex(graph, x1, y1);
    int exit_idx = search_vertex(graph, x2, y2);
    if (entrance_idx == -1 || exit_idx == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    if (*(graph->adj_list[entrance_idx]->type) != 1) {
        return 9;  // This vertex is not entrance
    }

    if (*(graph->adj_list[exit_idx]->type) != 2) {
        return 11;  // This vertex is not exit
    }

    int dijkstra_result = dijkstra(graph, entrance_idx, exit_idx);
    return dijkstra_result;
}

void print_path_dijkstra(Graph *graph, PriorityQueue *queue_ptr, int vertex_idx, int flag_first_call) {
    Vertex *cur_vertex_ptr = graph->adj_list[vertex_idx];
    int queue_elem_idx = get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, *(graph->adj_list[vertex_idx]->id));

    if (flag_first_call) {
        printf("The length of this shortest path = %d\n", queue_ptr->elem_ptr_arr[queue_elem_idx]->priority);
    }

    if (queue_ptr->elem_ptr_arr[queue_elem_idx]->pred_vertex_ptr == NULL) {
        printf("{№%d (%d, %d), %s} -> ", *(cur_vertex_ptr->id), *(cur_vertex_ptr->x), *(cur_vertex_ptr->y), vertex_types[*(cur_vertex_ptr->type)]);
    } else {
        print_path_dijkstra(graph, queue_ptr, *(queue_ptr->elem_ptr_arr[queue_elem_idx]->pred_vertex_ptr->id), 0);
        printf("{№%d (%d, %d), %s}", *(cur_vertex_ptr->id), *(cur_vertex_ptr->x), *(cur_vertex_ptr->y), vertex_types[*(cur_vertex_ptr->type)]);
        if (!flag_first_call) {
            printf(" -> ");
        }
    }
}

int dijkstra(Graph *graph, int origin_vertex_idx, int exit_vertex_idx) {
    // Initialization
    int default_priority = INT_MAX - 1;
    PriorityQueue *queue_ptr = build_min_heap(graph, &default_priority);
    
    decrease_priority(queue_ptr, get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, origin_vertex_idx), 0);
    
    // Processing
    int adj_elem_idx;
    while (queue_ptr->size) {
        PriorityQueueElem *queue_elem_ptr = extract_min(queue_ptr);
        Vertex *adj_vertex_ptr = graph->adj_list[*(queue_elem_ptr->vertex_ptr->id)]->next;
        while (adj_vertex_ptr != NULL) {
            adj_elem_idx = get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, *(adj_vertex_ptr->id));
            if (queue_ptr->elem_ptr_arr[adj_elem_idx]->priority > queue_elem_ptr->priority + WEIGHT) {
                decrease_priority(queue_ptr, adj_elem_idx, queue_elem_ptr->priority + WEIGHT);
                adj_elem_idx = get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, *(adj_vertex_ptr->id));
                queue_ptr->elem_ptr_arr[adj_elem_idx]->pred_vertex_ptr = queue_elem_ptr->vertex_ptr;
            }
            adj_vertex_ptr = adj_vertex_ptr->next;
        }
    }
    
    // Printing the shortest path
    int exit_queue_elem_idx = get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, *(graph->adj_list[exit_vertex_idx]->id));
    if (queue_ptr->elem_ptr_arr[exit_queue_elem_idx]->pred_vertex_ptr == NULL) {
        return 12;  // No path between the specified entrance and exit
    }

    print_path_dijkstra(graph, queue_ptr, exit_vertex_idx, 1);

    erase_binary_heap(queue_ptr, graph->num_vertices);

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

void erase_graph(Graph *graph) {
    Vertex *cur_vertex_ptr = NULL;
    Vertex *vertex_to_del_ptr = NULL;

    for (int i = 0; i < graph->num_vertices; i++) {
        cur_vertex_ptr = graph->adj_list[i];
        while (cur_vertex_ptr->next != NULL) {
            vertex_to_del_ptr = cur_vertex_ptr->next;
            cur_vertex_ptr->next = vertex_to_del_ptr->next;

            vertex_to_del_ptr->id = NULL;
            vertex_to_del_ptr->x = NULL;
            vertex_to_del_ptr->y = NULL;
            vertex_to_del_ptr->type = NULL;
            vertex_to_del_ptr->next = NULL;

            free(vertex_to_del_ptr);
        }

        free(cur_vertex_ptr->id);
        free(cur_vertex_ptr->x);
        free(cur_vertex_ptr->y);
        free(cur_vertex_ptr->type);
        
        cur_vertex_ptr->id = NULL;
        cur_vertex_ptr->x = NULL;
        cur_vertex_ptr->y = NULL;
        cur_vertex_ptr->type = NULL;

        free(cur_vertex_ptr);
    }

    free(graph->adj_list);
    graph->adj_list = NULL;
    graph->num_vertices = 0;
}

// ---------------------------------
