#ifndef BACK_HEADER
#define BACK_HEADER


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


// Fixed distance between linked vertexs of a graph
#define WEIGHT (1)
#define SRC_FOLDER_NAME ("src/")


/**
 * @brief Definition of a struct representing a Vertex in a non-oriented graph.
 *
 * This struct contains pointers to integer values for id, x, y, and type of the vertex,
 * as well as a pointer to the next Vertex in a linked list structure.
 */
typedef struct Vertex {
    int *id;
    int *x;
    int *y;
    int *type;
    struct Vertex *next;
} Vertex;

/**
 * Defines a struct Graph representing a graph data structure.
 * Contains the number of vertices and an adjacency list of Vertex pointers.
 */
typedef struct Graph {
    int num_vertices;
    Vertex **adj_list;
} Graph;

/**
 * Struct representing an element in a non-decreasing priority queue.
 * Contains pointers to the current vertex, predecessor vertex, and the priority value.
 */
typedef struct PriorityQueueElem {
    Vertex *vertex_ptr;
    Vertex *pred_vertex_ptr;
    int priority;
} PriorityQueueElem;

/**
 * Defines a struct PriorityQueue representing non-decreasing prioirty queue.
 * Contains a size field and an array of PriorityQueueElem pointers.
 */
typedef struct PriorityQueue {
    int size;
    PriorityQueueElem **elem_ptr_arr;
} PriorityQueue;


/**
 * Reads an integer from the standard input and validates it based on the given conditions.
 *
 * @param num Pointer to store the integer read from input.
 * @param is_vertex_type Flag indicating if the integer represents a vertex type.
 * @return 1 if a valid integer is read, 0 otherwise.
 */
int get_int(int *num, int is_vertex_type);

/**
 * @brief Reads a string from the standard input.
 *
 * This function reads characters from the standard input until a newline character is encountered.
 * It dynamically allocates memory to store the string and reallocates memory as needed.
 * If the end of file (EOF) is reached, it frees the allocated memory and returns NULL.
 *
 * @return A pointer to the dynamically allocated string read from the input.
 */
char *get_str();

/**
 * Search for a vertex in the graph based on its coordinates (x, y).
 *
 * @param graph Pointer to the graph structure.
 * @param x The x-coordinate of the vertex to search.
 * @param y The y-coordinate of the vertex to search.
 * @return The index of the vertex in the adjacency list if found, -1 otherwise.
 */
int search_vertex(Graph *graph, int x, int y);

/**
 * Initializes a vertex with the given x, y, type, and id values.
 *
 * @param vertex_ptr Pointer to the vertex to be initialized
 * @param x The x coordinate of the vertex
 * @param y The y coordinate of the vertex
 * @param type The type of the vertex
 * @param id The id of the vertex
 */
void initialize_vertex(Vertex *vertex_ptr, int x, int y, int type, int id);

/**
 * Creates a new vertex in the graph with the given coordinates and type.
 *
 * @param graph Pointer to the graph structure
 * @param x X-coordinate of the vertex
 * @param y Y-coordinate of the vertex
 * @param type Type of the vertex
 * @return Pointer to the newly created vertex, or NULL if the vertex already exists
 */
Vertex *create_vertex(Graph *graph, int x, int y, int type);

/**
 * Check if two vertices are neighbors in the graph.
 *
 * @param graph Pointer to the graph structure
 * @param first_vertex_idx Index of the first vertex
 * @param second_vertex_idx Index of the second vertex
 * @return 1 if the vertices are neighbors, 0 otherwise
 */
int are_vertices_neighbors(Graph *graph, int first_vertex_idx, int second_vertex_idx);

/**
 * Check if two vertices are linked in the graph.
 *
 * @param graph Pointer to the graph structure.
 * @param first_vertex_idx Index of the first vertex.
 * @param second_vertex_idx Index of the second vertex.
 * @return 1 if the vertices are linked, 0 otherwise.
 */
int are_vertices_linked(Graph *graph, int first_vertex_idx, int second_vertex_idx);

/**
 * Updates the adjacency list of a graph to add an edge between two vertices.
 *
 * @param graph Pointer to the graph structure containing the adjacency list
 * @param first_vertex_idx Index of the first vertex in the adjacency list
 * @param second_vertex_idx Index of the second vertex in the adjacency list
 */
void update_adj_list_to_add_edge(Graph *graph, int first_vertex_idx, int second_vertex_idx);

/**
 * Add an edge between two vertices in the graph.
 *
 * @param graph Pointer to the graph structure
 * @param x1 X coordinate of the first vertex
 * @param y1 Y coordinate of the first vertex
 * @param x2 X coordinate of the second vertex
 * @param y2 Y coordinate of the second vertex
 * @return 0 if the edge was successfully added, 2 if any of the vertices do not exist,
 *         3 if the vertices are the same, 4 if the vertices are not neighbors,
 *         5 if the vertices are already linked
 */
int add_edge(Graph *graph, int x1, int y1, int x2, int y2);

/**
 * Deletes a vertex from the graph.
 *
 * @param graph Pointer to the graph structure
 * @param x X-coordinate of the vertex to be deleted
 * @param y Y-coordinate of the vertex to be deleted
 * @return 2 if the vertex doesn't exist in the graph, 0 if deletion is successful
 */
int delete_vertex(Graph *graph, int x, int y);

/**
 * Updates the adjacency list in a graph to delete an edge between two vertices.
 *
 * @param graph Pointer to the graph structure
 * @param first_vertex_idx Index of the first vertex in the adjacency list
 * @param second_vertex_idx Index of the second vertex in the adjacency list
 */
void update_adj_list_to_delete_edge(Graph *graph, int first_vertex_idx, int second_vertex_idx);

/**
 * Deletes an edge between two vertices in the graph.
 *
 * @param graph Pointer to the graph structure
 * @param x1 X coordinate of the first vertex
 * @param y1 Y coordinate of the first vertex
 * @param x2 X coordinate of the second vertex
 * @param y2 Y coordinate of the second vertex
 * @return 2 if one of the vertices doesn't exist in the graph,
 *         3 if both vertices are the same,
 *         6 if the vertices are not linked,
 *         0 if the edge is successfully deleted
 */
int delete_edge(Graph *graph, int x1, int y1, int x2, int y2);

/**
 * Change the information of a vertex in the graph.
 *
 * @param graph Pointer to the graph structure
 * @param x X-coordinate of the vertex to be changed
 * @param y Y-coordinate of the vertex to be changed
 * @param new_x_ptr Pointer to the new X-coordinate value (NULL if not changing)
 * @param new_y_ptr Pointer to the new Y-coordinate value (NULL if not changing)
 * @param new_type_ptr Pointer to the new type value (NULL if not changing)
 * @return 0 if the information was successfully changed,
 *         1 if the new vertex coordinates already exist,
 *         2 if the vertex to be changed doesn't exist in the graph,
 *         7 if all edges linked with the vertex need to be removed to change its coordinates,
 *         8 if the vertex already has the specified type
 */
int change_vertex_information(Graph *graph, int x, int y, int *new_x_ptr, int *new_y_ptr, int *new_type_ptr);

/**
 * Perform Depth First Search (DFS) traversal on the given graph starting from a specified entrance vertex.
 *
 * @param graph Pointer to the graph structure
 * @param entrance_idx Index of the entrance vertex to start the DFS traversal from
 * @param flag_check_connectivity Flag to indicate whether to check graph connectivity or find a path to an exit
 * @return 0 if successful, 10 if no path from entrance to exit, 15 if graph is not connected
 */
int DFS(Graph *graph, int entrance_idx, int flag_check_connectivity);

/**
 * Perform Depth First Search (DFS) visit on a graph starting from a given vertex index.
 *
 * @param graph Pointer to the graph structure
 * @param vertex_idx Index of the current vertex being visited
 * @param pred_idx_arr Array to store the predecessor index of each vertex
 * @param color_ptr Array to store the color of each vertex (0 - white, 1 - gray, 2 - black)
 * @param discovery_time_arr Array to store the discovery time of each vertex
 * @param finish_time_arr Array to store the finish time of each vertex
 * @param time_ptr Pointer to the current time value
 * @param flag_check_connectivity Flag to indicate if checking connectivity or finding an exit
 * @return Index of the exit vertex if found, -1 otherwise
 */
int DFS_visit(Graph *graph, int vertex_idx, int pred_idx_arr[], int color_arr[], int discovery_time_arr[], int finish_time_arr[], int *time_ptr, int flag_check_connectivity);

/**
 * Recursively prints the path of vertices starting from a given vertex index in a graph after DFS traversal.
 * If the predecessor index array is provided, it prints the path from the starting vertex to the current vertex.
 *
 * @param graph Pointer to the graph structure
 * @param vertex_idx Index of the current vertex in the adjacency list
 * @param pred_idx_arr Array containing predecessor indices for each vertex
 * @param flag_first_call Flag indicating if it is the first call to the function
 */
void DFS_print_path(Graph *graph, int vertex_idx, int pred_idx_arr[], int flag_first_call);

/**
 * Check if a specific vertex is an entrance in the graph and determine its achievability.
 *
 * @param graph Pointer to the graph structure.
 * @param x The x-coordinate of the vertex to check.
 * @param y The y-coordinate of the vertex to check.
 * @return 2 if the vertex doesn't exist in the graph, 9 if the vertex is not an entrance,
 *         or the result of the Depth First Search (DFS) from the entrance vertex.
 */
int check_achievability_exits(Graph *graph, int x, int y);

/**
 * Check the connectivity of the graph using Depth First Search algorithm.
 *
 * @param graph Pointer to the graph structure
 * @return Integer indicating the result of the DFS algorithm for checking connectivity
 */
int check_graph_connectivity(Graph *graph);

/**
 * Build a min heap priority queue based on the given graph and default priority value.
 *
 * @param graph Pointer to the graph structure containing vertices
 * @param default_priority_ptr Pointer to the default priority value to assign to each element in the queue (can be NULL)
 * @return Pointer to the built priority queue
 */
PriorityQueue *build_min_heap(Graph *graph, int *default_priority_ptr);

/**
 * @brief Frees the memory allocated for a binary heap.
 *
 * @param queue_ptr Pointer to the priority queue structure.
 * @param queue_length The length of the priority queue.
 */
void erase_binary_heap(PriorityQueue *queue_ptr, int queue_length);

/**
 * Perform min heapify operation on a priority queue starting from a given index.
 *
 * @param queue_ptr Pointer to the priority queue structure
 * @param idx Index to start min heapify operation from
 */
void min_heapify(PriorityQueue *queue_ptr, int idx);

/**
 * Extracts the minimum element from the priority queue and maintains the min-heap property.
 *
 * @param queue_ptr Pointer to the priority queue
 * @return Pointer to the minimum element extracted from the priority queue, or NULL if the queue is empty
 */
PriorityQueueElem *extract_min(PriorityQueue *queue_ptr);

/**
 * Decreases the priority of an element in the priority queue.
 *
 * @param queue_ptr Pointer to the priority queue structure
 * @param idx Index of the element to decrease the priority
 * @param new_priority New priority value to set
 * @return 1 if the request is denied, 0 if the operation is successful
 */
int decrease_priority(PriorityQueue *queue_ptr, int idx, int new_priority);

/**
 * Find the index of an element in a PriorityQueue based on the given vertex index.
 *
 * @param queue_ptr Pointer to the PriorityQueue structure
 * @param queue_length Length of the PriorityQueue
 * @param vertex_idx Index of the vertex to search for
 * @return Index of the element if found, -1 otherwise
 */
int get_elem_idx_by_vertex_idx(PriorityQueue *queue_ptr, int queue_length, int vertex_idx);

/**
 * Find the shortest path between an entrance and an exit vertex in the graph using Dijkstra's algorithm.
 *
 * @param graph Pointer to the graph structure
 * @param x1 X-coordinate of the entrance vertex
 * @param y1 Y-coordinate of the entrance vertex
 * @param x2 X-coordinate of the exit vertex
 * @param y2 Y-coordinate of the exit vertex
 * @return 2 if the entrance or exit vertex is not found in the graph, 9 if the entrance vertex is not an entrance,
 *         11 if the exit vertex is not an exit, or the result of Dijkstra's algorithm
 */
int find_shortest_path_between_entrance_exit(Graph *graph, int x1, int y1, int x2, int y2);

/**
 * Recursively prints the shortest path from the source vertex to the given vertex index in a graph using Dijkstra's algorithm.
 *
 * @param graph Pointer to the graph structure containing vertices and edges
 * @param queue_ptr Pointer to the priority queue structure used in Dijkstra's algorithm
 * @param vertex_idx Index of the current vertex in the adjacency list
 * @param flag_first_call Flag indicating if it is the first call to the function
 */
void print_path_dijkstra(Graph *graph, PriorityQueue *queue_ptr, int vertex_idx, int flag_first_call);

/**
 * Perform Dijkstra's algorithm to find the shortest path between two vertices in a graph.
 *
 * @param graph Pointer to the graph structure containing vertices and edges
 * @param origin_vertex_idx Index of the origin vertex for the shortest path
 * @param exit_vertex_idx Index of the exit vertex for the shortest path
 * @return 0 if the shortest path is found successfully, 12 if no path exists between the specified vertices
 */
int dijkstra(Graph *graph, int origin_vertex_idx, int exit_vertex_idx);

/**
 * @brief Builds a Minimum Spanning Tree (MST) starting from a specified vertex in the graph
 *
 * This function checks if the graph is empty, searches for the specified root vertex,
 * verifies the connectivity of the graph, counts the number of entrance and exit vertices,
 * and then constructs the MST using Prim's algorithm.
 *
 * @param graph Pointer to the graph structure
 * @param x X-coordinate of the root vertex
 * @param y Y-coordinate of the root vertex
 * @return 0 if the MST is successfully built, 2 if the specified vertex doesn't exist,
 * 14 if the graph is empty, or the result of check_graph_connectivity function if the graph is not connected
 */
int build_MST(Graph *graph, int x, int y);

/**
 * Perform Prim's algorithm to find the Minimum Spanning Tree (MST) in a graph.
 *
 * @param graph Pointer to the graph structure
 * @param root_idx Index of the root vertex for the MST
 */
void MST_prim(Graph *graph, int root_idx);

/**
 * Reads vertex and edge information from a file and populates the graph accordingly.
 * Example of file structure:
 *  -- create vertex (x, y, type)
 *  x1 y1 type1
 *  x2 y2 typ2    
 *  -- breakpoint
 *  -1 -1 -1
 *  -- add edge (x1, y1, x2, y2)
 *  x1 y1 x2 y2
 * @param graph Pointer to the graph structure to be populated.
 * @param fname Name of the file containing vertex and edge information.
 * @return 0 if the operation is successful, 13 if the file is not found.
 */
int enter_graph_from_file(Graph *graph, char *fname);

#endif  // BACK_HEADER