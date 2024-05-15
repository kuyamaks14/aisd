#include "main.h"

extern const char *msgs[];
extern const int NMsgs;
extern int (*dialog_options[])(Node **root);

int main(void) {
    Node *root = b_tree_create();
    int operation_code;

    while ((operation_code = dialog(msgs, NMsgs)))
        if (!dialog_options[operation_code](&root))
            break;

    puts("\nThat's all.");
    b_tree_erase(root);
    return 0;
}