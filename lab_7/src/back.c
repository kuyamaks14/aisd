/**
 * This file includes various functions for working with a graph data structure.
 * Supported operations: adding and deleting vertices, adding and deleting edges, finding the shortest paths, checking connectivity, building a minimum spanning tree (MST), and more.
 * It also implements Dijkstra's algorithm for finding the shortest path between two vertices in the graph.
 * The functions handle operations like initializing vertices, updating adjacency lists, and performing depth-first search (DFS) for various purposes.
 * Additionally, it provides functionality for entering graph data from a file and checking the achievability of exits in the graph.
 */

#include "back.h"

extern const char *vertex_types[];

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

char *get_str() {
    char *str = (char *) malloc(sizeof(char));
    *str = '\0';
    int str_size = sizeof(char);

    char character;
    for (int i = 0; ; i++) {
        character = getchar();

        if (character == EOF) {
            free(str);
            return NULL;
        } else if (character == '\n') {
            if (i > 0) {
                break;
            } else {
                i--;
            }
        } else {
            char *str2 = (char *) realloc(str, str_size + sizeof(char));
            str_size += sizeof(char);

            if (str == NULL) {
                free(str);
                return NULL;
            }
            str = str2;

            str[i] = character;
            str[i + 1] = '\0';
        }
    }

    return str;
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

// ---------------------------------

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

int check_achievability_exits(Graph *graph, int x, int y) {
    int entrance_idx = search_vertex(graph, x, y);
    if (entrance_idx == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    if (*(graph->adj_list[entrance_idx]->type) != 1) {
        return 9;  // This vertex is not an entrance;
    }

    int DFS_result = DFS(graph, entrance_idx, 0);
    return DFS_result;
}

// ---------------------------------

int check_graph_connectivity(Graph *graph) {
    int DFS_for_checking_connectivity_result = DFS(graph, 0, 1);
    return DFS_for_checking_connectivity_result;
}

// ---------------------------------

int DFS(Graph *graph, int entrance_idx, int flag_check_connectivity) {
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
    if (flag_check_connectivity == 0) {
        int found_exit_idx = DFS_visit(graph, entrance_idx, pred_idx_arr, color_arr, discovery_time_arr, finish_time_arr, &time, 0);
        if (found_exit_idx == -1) {
            return 10;  // No path from this entrance to any kind of exit
        }

        puts("\nOne of the possible paths:");
        DFS_print_path(graph, found_exit_idx, pred_idx_arr, 1);
    } else {
        int connected_components_count = 0;
        for (int i = 0; i < graph->num_vertices; i++) {
            if (color_arr[i] == 0) {
                if (connected_components_count == 1) {
                    return 15;  // The graph is not connected
                }

                connected_components_count++;
                DFS_visit(graph, entrance_idx, pred_idx_arr, color_arr, discovery_time_arr, finish_time_arr, &time, 1);
            }
        }
    }

    return 0;  // Ok
}

// ---------------------------------

int DFS_visit(Graph *graph, int vertex_idx, int pred_idx_arr[], int color_ptr[], int discovery_time_arr[], int finish_time_arr[], int *time_ptr, int flag_check_connectivity) {
    // DFS modification to find at least one exit from the specifiend entrance
    if (flag_check_connectivity == 0) {
        if (*(graph->adj_list[vertex_idx]->type) == 2) {
            return vertex_idx;  // This vertex is an exit
        }
    }

    color_ptr[vertex_idx] = 1;
    *time_ptr += 1;
    discovery_time_arr[vertex_idx] = *time_ptr;

    Vertex *adj_vertex_ptr = graph->adj_list[vertex_idx]->next;
    while (adj_vertex_ptr != NULL) {
        if (color_ptr[*(adj_vertex_ptr->id)] == 0) {
            pred_idx_arr[*(adj_vertex_ptr->id)] = vertex_idx;

            if (flag_check_connectivity == 0) {
                int found_exit_idx = DFS_visit(graph, *(adj_vertex_ptr->id), pred_idx_arr, color_ptr, discovery_time_arr, finish_time_arr, time_ptr, 0);
                if (found_exit_idx != -1) {
                    return found_exit_idx;  // An exit is found
                }
            } else {
                DFS_visit(graph, *(adj_vertex_ptr->id), pred_idx_arr, color_ptr, discovery_time_arr, finish_time_arr, time_ptr, 1);
            }
        }

        adj_vertex_ptr = adj_vertex_ptr->next;
    }

    color_ptr[vertex_idx] = 2;
    *time_ptr += 1;
    finish_time_arr[vertex_idx] = *time_ptr;

    return -1;  // It means this vertex is not an exit
}

// ---------------------------------

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

// ---------------------------------

void erase_binary_heap(PriorityQueue *queue_ptr, int queue_length) {
    for (int i = 0; i < queue_length; i++) {
        free(queue_ptr->elem_ptr_arr[i]);
    }

    free(queue_ptr->elem_ptr_arr);
    free(queue_ptr);
}

// ---------------------------------

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

// ---------------------------------

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

// ---------------------------------

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

// ---------------------------------

int get_elem_idx_by_vertex_idx(PriorityQueue *queue_ptr, int queue_length, int vertex_idx) {
    for (int i = 0; i < queue_length; i++) {
        if (*(queue_ptr->elem_ptr_arr[i]->vertex_ptr->id) == vertex_idx) {
            return i;
        }
    }
    return -1;
}

// ---------------------------------

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

// ---------------------------------

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

// ---------------------------------

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

int build_MST(Graph *graph, int x, int y) {
    if (!graph->num_vertices) {
        return 14;  // The graph is empty
    }

    int root_idx = search_vertex(graph, x, y);
    if (root_idx == -1) {
        return 2;  // Graph doesn't have such vertex(vertices)
    }

    int check_graph_connectivity_result = check_graph_connectivity(graph);
    if (check_graph_connectivity_result != 0) {
        return check_graph_connectivity_result;  // The graph is not connected
    }

    int entrance_count = 0;
    int exit_count = 0;
    int vertex_type = -1;
    for (int i = 0; i < graph->num_vertices; i++) {
        vertex_type = *(graph->adj_list[i]->type);
        switch (vertex_type) {
            case 1:
                entrance_count++;
                break;
            case 2:
                exit_count++;
                break;
        }
    }

    if (entrance_count == 0) {
        puts("\nWarning: the graph does not have an entrance.");
    }

    if (exit_count == 0) {
        puts("Warning: the graph does not have an exit.");
    }

    MST_prim(graph, root_idx);
    return 0;  // Ok
}

// ---------------------------------

void MST_prim(Graph *graph, int root_idx) {
    // Initialization
    int default_priority = INT_MAX - 1;
    PriorityQueue *queue_ptr = build_min_heap(graph, &default_priority);
    decrease_priority(queue_ptr, get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, root_idx), 0);

    // Processing
    int adj_elem_idx;
    while (queue_ptr->size) {
        PriorityQueueElem *queue_elem_ptr = extract_min(queue_ptr);
        Vertex *adj_vertex_ptr = graph->adj_list[*(queue_elem_ptr->vertex_ptr->id)]->next;
        while (adj_vertex_ptr != NULL) {
            adj_elem_idx = get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, *(adj_vertex_ptr->id));
            if (adj_elem_idx < queue_ptr->size && WEIGHT < queue_ptr->elem_ptr_arr[adj_elem_idx]->priority) {
                queue_ptr->elem_ptr_arr[adj_elem_idx]->pred_vertex_ptr = queue_elem_ptr->vertex_ptr;
                decrease_priority(queue_ptr, adj_elem_idx, WEIGHT);
            }
            adj_vertex_ptr = adj_vertex_ptr->next;
        }
    }

    int total_weight = WEIGHT * (graph->num_vertices - 1);
    printf("\nTotal weight of MST = %d\n", total_weight);

    Vertex *cur_vertex_ptr, *adj_vertex_ptr;
    int cur_elem_idx;
    for (int i = 0; i < graph->num_vertices; i++) {
        cur_elem_idx = get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, i);
        cur_vertex_ptr = graph->adj_list[i];
        adj_vertex_ptr = cur_vertex_ptr->next;
        while (adj_vertex_ptr != NULL) {
            adj_elem_idx = get_elem_idx_by_vertex_idx(queue_ptr, graph->num_vertices, *(adj_vertex_ptr->id));
            int pred_vertex_id_1 = (i == root_idx) ? -1 : *(queue_ptr->elem_ptr_arr[cur_elem_idx]->pred_vertex_ptr->id);
            int pred_vertex_id_2 = (*(adj_vertex_ptr->id) == root_idx) ? -1 : *(queue_ptr->elem_ptr_arr[adj_elem_idx]->pred_vertex_ptr->id);
            if (pred_vertex_id_1 != *(adj_vertex_ptr->id) && pred_vertex_id_2 != i) {
                delete_edge(graph, *(cur_vertex_ptr->x), *(cur_vertex_ptr->y), *(adj_vertex_ptr->x), *(adj_vertex_ptr->y));
            }
            adj_vertex_ptr = adj_vertex_ptr->next;
        }
    }

    erase_binary_heap(queue_ptr, graph->num_vertices);
}

// ---------------------------------

int enter_graph_from_file(Graph *graph, char *fname) {
    char *adjusted_fname = (char *) malloc(sizeof(char) * (strlen(fname) + strlen(SRC_FOLDER_NAME) + 1));

    for (int i = 0; i < strlen(SRC_FOLDER_NAME); i++) {
        adjusted_fname[i] = SRC_FOLDER_NAME[i];
    }

    strcpy(adjusted_fname + strlen(SRC_FOLDER_NAME), fname);

    FILE *fp = fopen(adjusted_fname, "r");
    if (fp == NULL) {
        free(fname);
        free(adjusted_fname);
        return 13;  // File is not found
    }

    int x, y, type;
    int fscanf_res;
    while (1) {
        fscanf_res = fscanf(fp, "%d %d %d", &x, &y, &type);
        if (fscanf_res == 0) {
            fscanf(fp, "%*[^\n]");
            continue;
        }

        if (x == -1) {
            break;
        }

        create_vertex(graph, x, y, type);
    }

    int x1, y1, x2, y2;
    while (1) {
        fscanf_res = fscanf(fp, "%d %d %d %d", &x1, &y1, &x2, &y2);
        if (fscanf_res == 0) {
            fscanf(fp, "%*[^\n]");
            continue;
        }

        if (fscanf_res == -1) {
            break;
        }

        add_edge(graph, x1, y1, x2, y2);
    }

    free(fname);
    free(adjusted_fname);
    fclose(fp);
    return 0;  // Ok
}

// ---------------------------------
