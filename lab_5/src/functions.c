#include "main.h"

const char *errmsgs[] = {"Ok", "Key duplicate", "No node with such a key", "Empty TreeMap", "Memory allocation error", "No node in TreeMap"};

const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Find next min", "4. Delete", "5. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*dialog_options[])(Node **root) = {NULL, dialog_add_node, dialog_find_node, dialog_find_next_min_node, dialog_del_node, show_tree_map};

// ---------------------------------

int get_int(int *num) {
    int scanf_result;
    do {
        scanf_result = scanf("%d", num);

        if (scanf_result == -1) {
            return 0;
        }

        if (scanf_result == 0) {
            scanf("%*[^\n]");
            printf("%s", "Try once again!: ");
        }
    } while (scanf_result == 0);
    
    scanf("%*[^\n]");
    return 1;
}

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

void flushBuffer(void) {
    int c;
    do {
        c = getchar();
    } while(c != '\n' && c != EOF);
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

        n = get_int(&option_num);
        if (n == 0) // EOF -> quit from program
            option_num = 0;
    } while (option_num < 0 || option_num >= NMsgs);

    return option_num;
}

// ---------------------------------

void initialize_node(Node *node_ptr, int key, char *info) {
    node_ptr->key = key;
    node_ptr->info = info;
    node_ptr->left = node_ptr->right = NULL;
    node_ptr->next = NULL;
}

// ---------------------------------

Node *find_parent(Node *root, int key) {
    Node *par_ptr = NULL;
    Node *child_ptr = root;

    while (child_ptr->key != key) {
        par_ptr = child_ptr;
        if (key < child_ptr->key) {
            child_ptr = child_ptr->left;
        } else {
            child_ptr = child_ptr->right;
        }
    }
    return par_ptr;
}

void copy_next_node(Node *root, Node **node_ptr, Node **par_ptr) {
    if (*node_ptr == NULL) {
        *node_ptr = *par_ptr;
        *par_ptr = find_parent(root, (*node_ptr)->key);
    } else {
        while ((*node_ptr)->left != NULL || (*node_ptr)->right != NULL) {
            *par_ptr = *node_ptr;
            if ((*node_ptr)->left != NULL) {
                *node_ptr = (*node_ptr)->left;
            } else {
                *node_ptr = (*node_ptr)->right;
            }
        }
    }
}

void sew_tree(Node *root) {
    Node *node_ptr = root;
    Node *par_ptr = NULL;
    Node *cur_thread_ptr, *next_thread_ptr;
    cur_thread_ptr = next_thread_ptr = NULL;
    
    int iters = 0;
    while (cur_thread_ptr != root) {
        if (!iters) {
            copy_next_node(root, &node_ptr, &par_ptr);
            if ((cur_thread_ptr = node_ptr) == root) {
                break;
            }
        }
        
        if (par_ptr->left == cur_thread_ptr) {
            node_ptr = par_ptr->right;
            copy_next_node(root, &node_ptr, &par_ptr);
            next_thread_ptr = node_ptr;
        } else {
            next_thread_ptr = par_ptr;
            node_ptr = par_ptr;
            par_ptr = find_parent(root, node_ptr->key);
        }

        cur_thread_ptr->next = next_thread_ptr;
        cur_thread_ptr = next_thread_ptr;
        
        iters++;
    }
}

// ---------------------------------

int dialog_add_node(Node **root) {
    int key, get_int_result;
    char *info = NULL;

    puts("Enter key: -->");
    if ((get_int_result = get_int(&key)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    puts("Enter info:");
    info = get_str();
    if (info == NULL) {
        return 0;  // EOF или ошибка при чтении
    }

    int add_result = add_node(root, key, info);
    printf("\nResult of inserting node {key = %d, information = %s}: %s\n", key, info, errmsgs[add_result]);
    return 1;
}

int add_node(Node **root, int key, char *info) {
    Node *found_node_ptr = find_node(*root, key);
    if (found_node_ptr != NULL) {
        return 1;  // Key duplicate
    }
    
    Node *new_node_ptr = (Node *) malloc(sizeof(Node));
    if (new_node_ptr == NULL) {
        return 4;  // Memory allocation error.
    }
    initialize_node(new_node_ptr, key, info);
    
    Node *cur_node_ptr = NULL;  // Parent of root.
    Node *next_node_ptr = *root;

    while (next_node_ptr != NULL) {
        cur_node_ptr = next_node_ptr;
        if (key < cur_node_ptr->key) {
            next_node_ptr = cur_node_ptr->left;
        } else {
            next_node_ptr = cur_node_ptr->right;
        }
    }

    if (cur_node_ptr == NULL) {  // Empty tree.
        *root = new_node_ptr;
    } else {
        if (key < cur_node_ptr->key) {
            cur_node_ptr->left = new_node_ptr;
        } else {
            cur_node_ptr->right = new_node_ptr;
        }
    }

    sew_tree(*root);

    return 0;  // Ok
}

// ---------------------------------

int dialog_find_node(Node **root) {
    int key, get_int_key_result;
    puts("Enter key: -->");
    if ((get_int_key_result = get_int(&key)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int found_item_result = (find_node(*root, key) != NULL) ? 0 : 2;
    printf("\nResult of searching node with the key = %d: %s\n", key, errmsgs[found_item_result]);
    return 1;
}

Node *find_node(Node *root, int key) {
    Node *cur_node_ptr = NULL;  // Parent of root node.
    Node *next_node_ptr = root;

    while (next_node_ptr != NULL) {
        cur_node_ptr = next_node_ptr;
        if (key == cur_node_ptr->key) {
            return cur_node_ptr; // Ok
        }

        if (key < cur_node_ptr->key) {
            next_node_ptr = cur_node_ptr->left;
        } else {
            next_node_ptr = cur_node_ptr->right;
        }
    }

    return NULL;  // No node with such a key
}

// ---------------------------------

int dialog_find_next_min_node(Node **root) {
    int key, get_int_key_result;
    puts("Enter key: -->");
    if ((get_int_key_result = get_int(&key)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    Node *found_node = find_next_min_node(*root, key);
    int found_next_min_node_result = (found_node != NULL) ? 0 : 5;
    printf("\nResult of finding node with the least key, but more than %d: %s\n", key, errmsgs[found_next_min_node_result]);
    if (found_node != NULL) {
        printf("Node {key: %d, info: %s}\n", found_node->key, found_node->info);
    }
    return 1;
}

Node *find_next_min_node(Node *root, int key) {
    Node *found_node_ptr = find_node(root, key);
    if (found_node_ptr == NULL) {
        return NULL;  // No node with such a key.
    }

    Node *descendant_ptr = found_node_ptr->right;
    if (descendant_ptr != NULL) {
        Node *next_left_descendant_ptr = descendant_ptr;
        while (next_left_descendant_ptr != NULL) {
            descendant_ptr = next_left_descendant_ptr;
            next_left_descendant_ptr = descendant_ptr->left;
        }
        return descendant_ptr;  // Ok
    } else {
        Node *cur_node_ptr = found_node_ptr;
        Node *par_ptr = find_parent(root, cur_node_ptr->key);
        while (par_ptr != NULL && par_ptr->right == cur_node_ptr) {
            cur_node_ptr = par_ptr;
            par_ptr = find_parent(root, cur_node_ptr->key);
        }
        return par_ptr;  // {NULL: not ok, !NULL: ok}
    }
}

// ---------------------------------

int dialog_del_node(Node **root) {
    int key, get_int_result;

    puts("Enter key: -->");
    if ((get_int_result = get_int(&key)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int add_result = del_node(root, key);
    printf("\nDelete node with key = %d: %s\n", key, errmsgs[add_result]);
    return 1;
}

int del_node(Node **root, int key) {
    // Firstly, it is necessary to find target node.
    Node *found_node_ptr = find_node(*root, key);
    if (found_node_ptr == NULL) {
        return 2; // No node with such a key
    }

    Node *node_to_del_ptr, *descendant_ptr;
    if (found_node_ptr->left == NULL || found_node_ptr->right == NULL) {  // Found node has at least one leaf as it descendant
        node_to_del_ptr = found_node_ptr;
        descendant_ptr = (node_to_del_ptr->left != NULL) ? node_to_del_ptr->left : node_to_del_ptr->right;
        
        Node *par_ptr = find_parent(*root, node_to_del_ptr->key);

        if (par_ptr == NULL) {  // Delete the root
            *root = descendant_ptr;
            if (*root != NULL) {
                (*root)->next = NULL;
            }
        } else {
            if (par_ptr->left == node_to_del_ptr) {
                par_ptr->left = descendant_ptr;
            } else {
                par_ptr->right = descendant_ptr;
            }
        }

        // Deleting the node
        if (node_to_del_ptr->left == descendant_ptr) {
            node_to_del_ptr->left = NULL;
        } else {
            node_to_del_ptr->right = NULL;
        }

        node_to_del_ptr->next = NULL;
        free(node_to_del_ptr->info);
        free(node_to_del_ptr);
    } else {  // Found node has two descendants
        Node *node_to_del_ptr = find_next_min_node(*root, key);
        Node *par_ptr = find_parent(*root, node_to_del_ptr->key);
        Node *descendant_par_ptr = node_to_del_ptr->right;  // Right just because node_to_del_ptr can not have left descendant.

        if (par_ptr->left == node_to_del_ptr) {
            par_ptr->left = descendant_par_ptr;
        } else {
            par_ptr->right = descendant_par_ptr;
        }

        node_to_del_ptr->right = node_to_del_ptr->next = NULL;
        free(found_node_ptr->info);
        found_node_ptr->key = node_to_del_ptr->key;
        found_node_ptr->info = node_to_del_ptr->info;
        free(node_to_del_ptr);
    }

    sew_tree(*root);

    return 0;  // Ok
}

// ---------------------------------

int show_tree_map(Node **root) {
    if (*root == NULL) {
        puts("Tree map is empty!");
        return 1;
    }

    Node *first_node_ptr = *root;
    while (first_node_ptr->left != NULL || first_node_ptr->right != NULL) {
        if (first_node_ptr->left != NULL) {
            first_node_ptr = first_node_ptr->left;
        } else {
            first_node_ptr = first_node_ptr->right;
        }
    }

    puts("Content of tree map:");
    Node *cur_node_ptr = first_node_ptr;
    while (cur_node_ptr != NULL) {
        printf("Node {key: %d, info: %s}\n", cur_node_ptr->key, cur_node_ptr->info);
        cur_node_ptr = cur_node_ptr->next;
    }

    return 1;
}

// ---------------------------------

int del_tree_map(Node **root){
    if (*root == NULL) {
        return 0;  // Nothing to delete
    }

    Node *first_node_ptr = *root;
    while (first_node_ptr->left != NULL || first_node_ptr->right != NULL) {
        if (first_node_ptr->left != NULL) {
            first_node_ptr = first_node_ptr->left;
        } else {
            first_node_ptr = first_node_ptr->right;
        }
    }

    Node *cur_node_ptr = first_node_ptr;
    Node *prev_node_ptr = NULL;
    while (cur_node_ptr != NULL) {
        prev_node_ptr = cur_node_ptr;
        cur_node_ptr = prev_node_ptr->next;

        prev_node_ptr->left = prev_node_ptr->right = prev_node_ptr->next = NULL;
        printf("Free memory for node {key: %d, info: %s}\n", prev_node_ptr->key, prev_node_ptr->info);
        free(prev_node_ptr->info);
        free(prev_node_ptr);
    }

    *root = NULL;

    return 1;  // Ok
}

// ---------------------------------
