#include "main.h"

extern const char *msgs[];
extern const int NMsgs;
extern int (*dialog_options[])(Graph *graph);

int main(void) {
    Graph *graph = &(Graph) {.num_vertices = 0, .adj_list = NULL};
    int operation_code;

    while ((operation_code = dialog(msgs, NMsgs)))
        if (!dialog_options[operation_code](graph))
            break;

    puts("\nThat's all.");
    erase_graph(graph);
    return 0;
}