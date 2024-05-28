/*
 * This file defines various functions for interacting with a graph data structure, 
 * including creating vertices, adding edges, deleting vertices, changing vertex information, 
 * checking achievability between entrances and exits, finding the shortest path, 
 * modifying the graph to MST, printing the graph, and reading a graph from a file. 
 * It also includes a dialog function for user interaction and an erase function to free memory. 
 * Each function performs specific operations on the graph and returns status messages based on the outcome.
 */

#include "front.h"

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
                         "No path between the specified entrance and exit",
                         "File is not found",
                         "The graph is empty",
                         "The graph is not connected"};

const char *msgs[] = {"0. Quit",
                      "1. Add vertex",
                      "2. Add edge",
                      "3. Delete vertex",
                      "4. Delete edge",
                      "5. Change vertex information",
                      "6. Check achievability from a specified entrance to at least one exit",
                      "7. Find the shortest path between an entrance and an exit specified",
                      "8. Modify the graph to MST",
                      "9. Show graph",
                      "10. Read a graph from the file"};

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*dialog_options[])(Graph *graph) = {NULL,
                                         dialog_create_vertex,
                                         dialog_add_edge,
                                         dialog_delete_vertex,
                                         dialog_delete_edge,
                                         dialog_change_vertex_information,
                                         dialog_check_achievability_exits,
                                         dialog_dijkstra,
                                         dialog_MST,
                                         print_graph,
                                         dialog_enter_graph_from_file};

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

// ---------------------------------

int dialog_MST(Graph *graph) {
    int x, y, get_int_result;

    puts("Enter X coordinate of a root vertex to build MST: -->");
    if ((get_int_result = get_int(&x, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter Y coordinate of a root vertex to build MST: -->");
    if ((get_int_result = get_int(&y, 0)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int build_MST_result = build_MST(graph, x, y);
    printf("\nResult of modifying the graph to MST: %s\n", errmsgs[build_MST_result]);
    return 1;
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

int dialog_enter_graph_from_file(Graph *graph) {
    char *fname;

    puts("Enter file name (e.g.: some_file.txt):");
    fname = get_str();
    if (fname == NULL) {
        return 0; // EOF или ошибка при чтении
    }

    int enter_graph_from_file_result = enter_graph_from_file(graph, fname);
    printf("\nResult of reading a graph from the file %s: %s\n", fname,errmsgs[enter_graph_from_file_result]);
    return 1;  // Ok
}

// ---------------------------------

