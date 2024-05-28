/**
 * Functions for interactive dialog with the user to manipulate a graph structure.
 * Includes options to create vertices, add edges, delete vertices and edges, change vertex information,
 * check achievability of exits, find shortest path, build Minimum Spanning Tree (MST),
 * print graph information, erase graph, read graph from file.
 */


#ifndef FRONT_HEADER
#define FRONT_HEADER


#include <stdio.h>
#include <stdlib.h>
#include "back.h"


/**
 * Prompts the user with a list of messages and waits for the user to choose an option.
 * If the user enters an invalid option, it prompts for a valid choice.
 * Returns the chosen option number.
 */
int dialog(const char *msgs[], int NMsgs);

/**
 * Creates a new vertex in the graph with the specified coordinates and type.
 *
 * @param graph Pointer to the graph structure
 * @return 1 if the vertex was successfully created, 0 if EOF was encountered during input
 */
int dialog_create_vertex(Graph *graph);

/**
 * Function to prompt the user to enter coordinates for two vertices and add an edge between them in the graph.
 *
 * @param graph Pointer to the graph structure
 * @return 1 if the edge was successfully added, 0 if there was an error or EOF
 */
int dialog_add_edge(Graph *graph);

/**
 * Deletes a vertex from the graph based on the provided X and Y coordinates.
 *
 * @param graph Pointer to the graph structure
 * @return 1 if the vertex was successfully deleted, 0 otherwise
 */
int dialog_delete_vertex(Graph *graph);

/**
 * Deletes an edge between two vertices in the graph.
 *
 * Prompts the user to enter the coordinates of the two vertices.
 * If successful, deletes the edge between the specified vertices.
 *
 * @param graph Pointer to the graph structure
 * @return 1 if the edge deletion was successful, 0 otherwise
 */
int dialog_delete_edge(Graph *graph);

/**
 * Function to interactively change the information of a vertex in the graph.
 * Prompts the user to enter the X and Y coordinates of the vertex, and then choose
 * what information to change (X coordinate, Y coordinate, or vertex type).
 * Handles user input errors and EOF conditions gracefully.
 *
 * @param graph Pointer to the graph structure
 * @return 1 if the operation was successful, 0 if EOF condition was encountered
 */
int dialog_change_vertex_information(Graph *graph);

/**
 * Function to prompt the user to enter the X and Y coordinates of an entrance vertex,
 * check the achievability to any kind of exit from this entrance, and print the result.
 *
 * @param graph Pointer to the graph structure
 * @return 1 if the function executed successfully, 0 if an error occurred
 */
int dialog_check_achievability_exits(Graph *graph);

/**
 * Prompts the user to enter the coordinates of an entrance and an exit,
 * then calls a function to find the shortest path between them in the graph.
 *
 * @param graph A pointer to the graph structure
 * @return 1 if the operation was successful, 0 if there was an error or EOF
 */
int dialog_dijkstra(Graph *graph);

/**
 * Prompts the user to enter the X and Y coordinates of a root vertex to build a Minimum Spanning Tree (MST) in the graph.
 * Calls the build_MST function with the provided coordinates and prints the result.
 *
 * @param graph A pointer to the graph structure where the MST will be built.
 * @return 1 if the operation was successful, 0 if the input was interrupted (EOF).
 */
int dialog_MST(Graph *graph);

/**
 * Print the graph information including vertices and their adjacent vertices.
 *
 * @param graph Pointer to the graph structure containing vertices and edges
 * @return 1 if the graph is printed successfully, 0 otherwise
 */
int print_graph(Graph *graph);

/**
 * Function to erase all vertices and edges in the graph and free the memory.
 *
 * @param graph Pointer to the graph structure to be erased.
 */
void erase_graph(Graph *graph);

/**
 * Reads a graph from a file and updates the graph accordingly.
 *
 * @param graph Pointer to the graph structure
 * @return 1 if the operation was successful, 0 if there was an error or EOF
 */
int dialog_enter_graph_from_file(Graph *graph);

#endif // MAIN_HEADER