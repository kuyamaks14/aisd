#include "main.h"

extern const char *msgs[];
extern const int NMsgs;
extern int (*dialog_options[])(Node **root);

int main(void) {
    Node *root = NULL;
    int operation_code;

    while ((operation_code = dialog(msgs, NMsgs)))
        if (!dialog_options[operation_code](&root))
            break;

    puts("\nThat's all.");
    del_tree_map(&root);
    return 0;
}