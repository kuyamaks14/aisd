#include "main.h"

const char *errmsgs[] = {"Ok", "Key duplicate", "No node with such a key", "Empty TreeMap", "Memory allocation error", "No node in TreeMap"};

const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Find next min", "4. Delete", "5. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

// int (*dialog_options[])(Node **root) = {NULL, dialog_add_node, dialog_find_node, dialog_find_next_min_node, dialog_del_node, show_tree_map};
// int (*dialog_options[])(Node **root) = {NULL, dialog_b_tree_insert, dialog_b_tree_search, b_tree_print};
int (*dialog_options[])(Node **root) = {NULL, dialog_b_tree_insert, dialog_b_tree_search, dialog_b_tree_find_successor, dilog_b_tree_delete, b_tree_print};


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

Node *b_tree_create(void) {
    Node *new_node_ptr = (Node *) malloc(sizeof(Node));
    new_node_ptr->n = 0;
    new_node_ptr->leaf = 1;
    return new_node_ptr;
}

// ---------------------------------

char *copy_information(char *src) {
    char *new_str = (char *) malloc(sizeof(char) * strlen(src) + 1);
    strcpy(new_str, src);
    return new_str;
}

// ---------------------------------

int dialog_b_tree_search(Node **root) {
    int key, get_int_key_result;
    puts("Enter key: -->");
    if ((get_int_key_result = get_int(&key)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int found_node_pos;
    int found_item_result = (b_tree_search(*root, key, &found_node_pos) != NULL) ? 0 : 2;
    printf("\nResult of searching node with the key = %d: %s\n", key, errmsgs[found_item_result]);
    if (found_item_result == 0) {
        printf("Index of found node: %d\n", found_node_pos);
    }
    return 1;
}

Node *b_tree_search(Node *ptr, int key, int *pos) {
    int i = 0;
    while (i < ptr->n && key > ptr->key[i]) {
        i++;
    }
    
    if (i < ptr->n && key == ptr->key[i]) {
        if (pos != NULL) {
            *pos = i;
        }
        return ptr;  // Ok
    } else if (ptr->leaf) {
        if (pos != NULL) {
            *pos = -1;
        }
        return NULL;  // No node with such a key
    } else {
        return b_tree_search(ptr->next_ptr[i], key, pos);
    }
}

// ---------------------------------

void b_tree_split_child(Node *ptr, int i) {
    Node *new_child_ptr = (Node *) malloc(sizeof(Node));
    Node *child_ptr = ptr->next_ptr[i];
    
    new_child_ptr->leaf = child_ptr->leaf;
    new_child_ptr->n = T - 1;

    int j;
    for (j = 0; j < T - 1; j++) {
        new_child_ptr->key[j] = child_ptr->key[j + T];
        new_child_ptr->info[j] = child_ptr->info[j + T];
        child_ptr->info[j + T] = NULL;
    }

    if (!child_ptr->leaf) {
        for (j = 0; j < T; j++) {
            new_child_ptr->next_ptr[j] = child_ptr->next_ptr[j + T];
            child_ptr->next_ptr[j + T] = NULL;
        }
    }

    child_ptr->n = T - 1;

    for (j = ptr->n; j >= i + 1; j--) {
        ptr->next_ptr[j + 1] = ptr->next_ptr[j];
    }
    ptr->next_ptr[i + 1] = new_child_ptr;

    for (j = ptr->n; j >= i + 1; j--) {
        ptr->key[j] = ptr->key[j - 1];
        ptr->info[j] = ptr->info[j - 1];
    }
    ptr->key[i] = child_ptr->key[T - 1];
    ptr->info[i] = child_ptr->info[T - 1];
    child_ptr->info[T - 1] = NULL;
    ptr->n++;
}

// ---------------------------------

void b_tree_insert_nonfull(Node *ptr, int key, char *info) {
    int i = ptr->n;
    if (ptr->leaf) {
        while (i >= 1 && key < ptr->key[i - 1]) {
            ptr->key[i] = ptr->key[i - 1];
            ptr->info[i] = ptr->info[i - 1];
            i--;
        }
        ptr->key[i] = key;
        ptr->info[i] = info;
        ptr->n++;
    } else {
        while (i >= 1 && key < ptr->key[i - 1]) {
            i--;
        }

        if (ptr->next_ptr[i]->n == 2 * T - 1) {
            b_tree_split_child(ptr, i);
            if (key > ptr->key[i]) {
                i++;
            }
        }

        b_tree_insert_nonfull(ptr->next_ptr[i], key, info);
    }
}

// ---------------------------------

int dialog_b_tree_insert(Node **root) {
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

    int add_result = b_tree_insert(root, key, info);
    printf("\nResult of inserting node {key = %d, information = %s}: %s\n", key, info, errmsgs[add_result]);
    return 1;
}

int b_tree_insert(Node **root, int key, char *info) {
    Node *fount_node = b_tree_search(*root, key, NULL);
    if (fount_node != NULL) {
        return 1;  // Key duplicate
    }

    Node *root_copy = *root;
    if (root_copy->n == 2 * T - 1) {
        Node *new_root = (Node *) malloc(sizeof(Node));
        *root = new_root;
        new_root->leaf = 0;
        new_root->n = 0;
        new_root->next_ptr[0] = root_copy;

        b_tree_split_child(new_root, 0);
        b_tree_insert_nonfull(new_root, key, info);
    } else {
        b_tree_insert_nonfull(root_copy, key, info);
    }

    return 0;  // Ok
}

// ---------------------------------

int b_tree_print(Node **ptr) {
    if ((*ptr)->n == 0) {
        puts("\nEmpty B-tree.");
        return 3;
    }

    if ((*ptr)->leaf == 1) {
        for (int i = (*ptr)->n - 1; i >= 0; i--) {
            printf("\nNode{key: %d, info: %s, pos: %d, n: %d, leaf}", (*ptr)->key[i], (*ptr)->info[i], i, (*ptr)->n);
        }
    } else {
        for (int i = (*ptr)->n - 1; i >= 0; i--) {
            b_tree_print(&((*ptr)->next_ptr[i + 1]));
            printf("\nNode{key: %d, info: %s, pos: %d, n: %d}", (*ptr)->key[i], (*ptr)->info[i], i, (*ptr)->n);
        }
        b_tree_print(&((*ptr)->next_ptr[0]));
    }

    puts("");
    return 1;  // Ok
}

// ---------------------------------

int dilog_b_tree_delete(Node **root) {
    int key, get_int_result;

    puts("Enter key: -->");
    if ((get_int_result = get_int(&key)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int delete_result = b_tree_delete(root, key);
    printf("\nDelete node with key = %d: %s\n", key, errmsgs[delete_result]);
    return 1;
}

int b_tree_delete(Node **root, int key) {
    b_tree_process_root(root);
    Node *ptr = *root;
    
    while (!ptr->leaf) {
        int i = 0;  // Key position.
        while (i < ptr->n && key > ptr->key[i]) {
            i++;
        }

        if (i == ptr->n || key != ptr->key[i]) {  // situation 1   
            Node *target = ptr->next_ptr[i];
            if (target->n == T - 1 && (ptr->next_ptr[i + 1]->n >= T || (i > 0 && ptr->next_ptr[i - 1]->n >= T))) {
                if (ptr->next_ptr[i + 1]->n >= T) {  // Right child contains more than T child nodes.
                    Node *right = ptr->next_ptr[i + 1];

                    target->key[T - 1] = ptr->key[i];
                    target->info[T - 1] = ptr->info[i];

                    ptr->key[i] = right->key[0];
                    ptr->info[i] = right->info[0];

                    target->next_ptr[T] = right->next_ptr[0];

                    for (int j = 0; j < right->n - 1; j++) {
                        right->key[j] = right->key[j + 1];
                        right->info[j] = right->info[j + 1];
                    }
                    for (int j = 0; j < right->n; j++) {
                        right->next_ptr[j] = right->next_ptr[j + 1];
                    }

                    target->n++;
                    right->n--;
                } else {  // Left child contains more than T child nodes.
                    Node *left = ptr->next_ptr[i - 1];

                    for (int j = T - 1; j >= 1; j--) {
                        target->key[j] = target->key[j - 1];
                        target->info[j] = target->info[j - 1];
                    }
                    for (int j = T; j >= 1; j--) {
                        target->next_ptr[j] = target->next_ptr[j - 1];
                    }
                    target->key[0] = ptr->key[i - 1];
                    target->info[0] = ptr->info[i - 1];

                    ptr->key[i - 1] = left->key[left->n - 1];
                    ptr->info[i - 1] = left->info[left->n - 1];

                    target->next_ptr[0] = left->next_ptr[left->n];

                    target->n++;
                    left->n--;
                }
            } else if (target->n == T - 1 && ((i == ptr->n || ptr->next_ptr[i + 1]->n == T - 1) && (i == 0 || ptr->next_ptr[i - 1]->n == T - 1))) {
                Node *right = ptr->next_ptr[i + 1];

                target->key[T - 1] = ptr->key[i];
                target->info[T - 1] = ptr->info[i];

                for (int j = 0; j < T - 1; j++) {
                    target->key[T + j] = right->key[j];
                    target->info[T + j] = right->info[j];
                }
                for (int j = 0; j < T; j++) {
                    target->next_ptr[T + j] = right->next_ptr[j];
                }

                for (int j = i; j < ptr->n - 1; j++) {
                    ptr->key[j] = ptr->key[j + 1];
                    ptr->info[j] = ptr->info[j + 1];
                }
                for (int j = i + 1; j < ptr->n; j++) {
                    ptr->next_ptr[j] = ptr->next_ptr[j + 1];
                }

                ptr->n--;
                target->n += T;
                free(right);
            }

            ptr = target;
            continue;
        } else {  // situation 2
            Node *target = ptr->next_ptr[i];
            Node *right = ptr->next_ptr[i + 1];

            if (target->n >= T) {
                int predecessor_key;
                char *predecessor_info;
                b_tree_find_predecessor_key(root, key, &predecessor_key, &predecessor_info);

                ptr->key[i] = predecessor_key;
                free(ptr->info[i]);
                ptr->info[i] = copy_information(predecessor_info);
                
                ptr = target;
                key = predecessor_key;
                continue;
                // b_tree_delete(&target, predecessor_key);
            } else if (right->n >= T) {
                int successor_key;
                char *successor_info;
                b_tree_find_successor_key(root, key, &successor_key, &successor_info);

                ptr->key[i] = successor_key;
                free(ptr->info[i]);
                ptr->info[i] = copy_information(successor_info);

                ptr = right;
                key = successor_key;
                continue;
                // b_tree_delete(&right, successor_key);
            } else {
                target->key[T - 1] = ptr->key[i];
                target->info[T - 1] = ptr->info[i];

                for (int j = i; j < ptr->n - 1; j++) {
                    ptr->key[j] = ptr->key[j + 1];
                    ptr->info[j] = ptr->info[j + 1];
                }
                for (int j = i + 1; j < ptr->n; j++) {
                    ptr->next_ptr[j] = ptr->next_ptr[j + 1];
                }
                ptr->n--;

                for (int j = 0; j < T - 1; j++) {
                    target->key[T + j] = right->key[j];
                    target->info[T + j] = right->info[j];
                }
                for (int j = 0; j < T; j++) {
                    target->next_ptr[T + j] = right->next_ptr[j];
                }
                target->n += T;

                free(right);

                ptr = target;
                continue;
            }
        }
    }

    // ptr - leaf
    int i = 0;
    while (i < ptr->n && key > ptr->key[i]) {
        i++;
    }

    if (i < ptr->n && key == ptr->key[i]) {
        free(ptr->info[i]);

        for (int j = i; j < ptr->n - 1; j++) {
            ptr->key[j] = ptr->key[j + 1];
            ptr->info[j] = ptr->info[j + 1];
        }
        ptr->n--;

        return 0;  // Ok
    }

    return 2;  // No node with such a key
}

void b_tree_process_root(Node **root) {
    Node *x = *root;
    if (!x->leaf && x->n == 1 && x->next_ptr[0]->n == T - 1 && x->next_ptr[1]->n == T - 1) {
        Node *left = x->next_ptr[0];
        Node *right = x->next_ptr[1];
        left->key[T - 1] = x->key[0];
        left->info[T - 1] = x->info[0];
        // (*x)->info[0] = NULL;
        for (int i = 0; i < T - 1; i++) {
            left->key[T + i] = right->key[i];
            left->info[T + i] = right->info[i];
            // right->info[i] = NULL;
        }
        for (int i = 0; i < T; i++) {
            left->next_ptr[T + i] = right->next_ptr[i];
            // right->next_ptr[i] = NULL;
        }
        left->n += T;
        *root = left;
        free(x);
        free(right);
    }
}

// ---------------------------------

Node *b_tree_find_parent(Node **root, int key) {
    // Node *child = b_tree_search(*root, key, NULL);
    // if (child == NULL) {
    //     return NULL;  // No such node -> no parent.
    // }

    Node *parent = NULL;
    Node *cur = *root;
    while (1) {
        int i = 0;
        while (i < cur->n && key > cur->key[i]) {
            i++;
        }

        if (i < cur->n && key == cur->key[i]) {
            return parent;
        }

        if (cur->leaf) {
            return NULL;  // No such node -> no parent.
        }

        parent = cur;
        cur = cur->next_ptr[i];
    }
}

// ---------------------------------

int dialog_b_tree_find_successor(Node **root) {
    int key, get_int_result;

    puts("Enter key: -->");
    if ((get_int_result = get_int(&key)) == 0) {
        return 0;  // EOF -> игнорируем весь остальной ввод
    }

    int successor_key;
    char *successor_info;
    Node *successor = b_tree_find_successor_key(root, key, &successor_key, &successor_info);
    int result_code = (successor != NULL) ? 0 : 5;
    printf("\nFind succeccor of node with key = %d: %s\n", key, errmsgs[result_code]);
    if (successor != NULL) {
        printf("Successor: key = %d, info = %s\n", successor_key, successor_info);
    }
    return 1;
}

Node *b_tree_find_successor_key(Node **root, int key, int *successor_key, char **successor_info) {
    int pos;
    Node *ptr = b_tree_search(*root, key, &pos);
    if (ptr == NULL) {
        return NULL;  // No such node -> no successor.
    }

    Node *successor = NULL;
    if (!ptr->leaf) {
        successor = ptr->next_ptr[pos + 1];
        while (!successor->leaf) {
            successor = successor->next_ptr[0];
        }
        *successor_key = successor->key[0];
        *successor_info = successor->info[0];
        return successor;
    } else if (ptr->n > pos + 1) {
        successor = ptr;
        *successor_key = successor->key[pos + 1];
        *successor_info = successor->info[pos + 1];
        return successor;
    } else {
        Node *parent = b_tree_find_parent(root, key);
        while (parent != NULL) {
            int i = 0;
            while (i < parent->n && key > parent->key[i]) {
                i++;
            }
            if (i == parent->n) {
                parent = b_tree_find_parent(root, parent->key[0]);
            } else {
                successor = parent;
                *successor_key = successor->key[i];
                *successor_info = successor->info[i];
                return successor;
            }
        }
        return NULL;
    }
}

// ---------------------------------

Node *b_tree_find_predecessor_key(Node **root, int key, int *predecessor_key, char **predecessor_info) {
    int pos;
    Node *ptr = b_tree_search(*root, key, &pos);
    if (ptr == NULL) {
        return NULL;  // No such node -> no successor.
    }

    Node *predecessor = NULL;
    if (!ptr->leaf) {
        predecessor = ptr->next_ptr[pos];
        while (!predecessor->leaf) {
            predecessor = predecessor->next_ptr[predecessor->n];
        }
        *predecessor_key = predecessor->key[predecessor->n - 1];
        *predecessor_info = predecessor->info[predecessor->n - 1];
        return predecessor;
    } else if (pos > 0) {
        predecessor = ptr;
        *predecessor_key = predecessor->key[pos - 1];
        *predecessor_info = predecessor->info[pos - 1];
        return predecessor;
    } else {
        Node *parent = b_tree_find_parent(root, key);
        while (parent != NULL) {
            int i = 0;
            while (i < parent->n && key > parent->key[i]) {
                i++;
            }
            if (i == 0) {
                parent = b_tree_find_parent(root, parent->key[0]);
            } else {
                predecessor = parent;
                *predecessor_key = predecessor->key[i - 1];
                *predecessor_info = predecessor->info[i - 1];
                return predecessor;
            }
        }
        return NULL;
    }
}

// ---------------------------------

// void initialize_node(Node *node_ptr, int key, char *info) {
//     node_ptr->key = key;
//     node_ptr->info = info;
//     node_ptr->left = node_ptr->right = NULL;
//     node_ptr->next = NULL;
// }

// // ---------------------------------

// Node *find_parent(Node *root, int key) {
//     Node *par_ptr = NULL;
//     Node *child_ptr = root;

//     while (child_ptr->key != key) {
//         par_ptr = child_ptr;
//         if (key < child_ptr->key) {
//             child_ptr = child_ptr->left;
//         } else {
//             child_ptr = child_ptr->right;
//         }
//     }
//     return par_ptr;
// }

// void copy_next_node(Node *root, Node **node_ptr, Node **par_ptr) {
//     if (*node_ptr == NULL) {
//         *node_ptr = *par_ptr;
//         *par_ptr = find_parent(root, (*node_ptr)->key);
//     } else {
//         while ((*node_ptr)->left != NULL || (*node_ptr)->right != NULL) {
//             *par_ptr = *node_ptr;
//             if ((*node_ptr)->left != NULL) {
//                 *node_ptr = (*node_ptr)->left;
//             } else {
//                 *node_ptr = (*node_ptr)->right;
//             }
//         }
//     }
// }

// void sew_tree(Node *root) {
//     Node *node_ptr = root;
//     Node *par_ptr = NULL;
//     Node *cur_thread_ptr, *next_thread_ptr;
//     cur_thread_ptr = next_thread_ptr = NULL;
    
//     int iters = 0;
//     while (cur_thread_ptr != root) {
//         if (!iters) {
//             copy_next_node(root, &node_ptr, &par_ptr);
//             if ((cur_thread_ptr = node_ptr) == root) {
//                 break;
//             }
//         }
        
//         if (par_ptr->left == cur_thread_ptr) {
//             node_ptr = par_ptr->right;
//             copy_next_node(root, &node_ptr, &par_ptr);
//             next_thread_ptr = node_ptr;
//         } else {
//             next_thread_ptr = par_ptr;
//             node_ptr = par_ptr;
//             par_ptr = find_parent(root, node_ptr->key);
//         }

//         cur_thread_ptr->next = next_thread_ptr;
//         cur_thread_ptr = next_thread_ptr;
        
//         iters++;
//     }
// }

// // ---------------------------------

// int dialog_add_node(Node **root) {
//     int key, get_int_result;
//     char *info = NULL;

//     puts("Enter key: -->");
//     if ((get_int_result = get_int(&key)) == 0) {
//         return 0;  // EOF -> игнорируем весь остальной ввод
//     }

//     puts("Enter info:");
//     info = get_str();
//     if (info == NULL) {
//         return 0;  // EOF или ошибка при чтении
//     }

//     int add_result = add_node(root, key, info);
//     printf("\nResult of inserting node {key = %d, information = %s}: %s\n", key, info, errmsgs[add_result]);
//     return 1;
// }

// int add_node(Node **root, int key, char *info) {
//     Node *found_node_ptr = find_node(*root, key);
//     if (found_node_ptr != NULL) {
//         return 1;  // Key duplicate
//     }
    
//     Node *new_node_ptr = (Node *) malloc(sizeof(Node));
//     if (new_node_ptr == NULL) {
//         return 4;  // Memory allocation error.
//     }
//     initialize_node(new_node_ptr, key, info);
    
//     Node *cur_node_ptr = NULL;  // Parent of root.
//     Node *next_node_ptr = *root;

//     while (next_node_ptr != NULL) {
//         cur_node_ptr = next_node_ptr;
//         if (key < cur_node_ptr->key) {
//             next_node_ptr = cur_node_ptr->left;
//         } else {
//             next_node_ptr = cur_node_ptr->right;
//         }
//     }

//     if (cur_node_ptr == NULL) {  // Empty tree.
//         *root = new_node_ptr;
//     } else {
//         if (key < cur_node_ptr->key) {
//             cur_node_ptr->left = new_node_ptr;
//         } else {
//             cur_node_ptr->right = new_node_ptr;
//         }
//     }

//     sew_tree(*root);

//     return 0;  // Ok
// }

// // ---------------------------------

// int dialog_find_node(Node **root) {
//     int key, get_int_key_result;
//     puts("Enter key: -->");
//     if ((get_int_key_result = get_int(&key)) == 0) {
//         return 0;  // EOF -> игнорируем весь остальной ввод
//     }

//     int found_item_result = (find_node(*root, key) != NULL) ? 0 : 2;
//     printf("\nResult of searching node with the key = %d: %s\n", key, errmsgs[found_item_result]);
//     return 1;
// }

// Node *find_node(Node *root, int key) {
//     Node *cur_node_ptr = NULL;  // Parent of root node.
//     Node *next_node_ptr = root;

//     while (next_node_ptr != NULL) {
//         cur_node_ptr = next_node_ptr;
//         if (key == cur_node_ptr->key) {
//             return cur_node_ptr; // Ok
//         }

//         if (key < cur_node_ptr->key) {
//             next_node_ptr = cur_node_ptr->left;
//         } else {
//             next_node_ptr = cur_node_ptr->right;
//         }
//     }

//     return NULL;  // No node with such a key
// }

// // ---------------------------------

// int dialog_find_next_min_node(Node **root) {
//     int key, get_int_key_result;
//     puts("Enter key: -->");
//     if ((get_int_key_result = get_int(&key)) == 0) {
//         return 0;  // EOF -> игнорируем весь остальной ввод
//     }

//     Node *found_node = find_next_min_node(*root, key);
//     int found_next_min_node_result = (found_node != NULL) ? 0 : 5;
//     printf("\nResult of finding node with the least key, but more than %d: %s\n", key, errmsgs[found_next_min_node_result]);
//     if (found_node != NULL) {
//         printf("Node {key: %d, info: %s}\n", found_node->key, found_node->info);
//     }
//     return 1;
// }

// Node *find_next_min_node(Node *root, int key) {
//     Node *found_node_ptr = find_node(root, key);
//     if (found_node_ptr == NULL) {
//         return NULL;  // No node with such a key.
//     }

//     Node *descendant_ptr = found_node_ptr->right;
//     if (descendant_ptr != NULL) {
//         Node *next_left_descendant_ptr = descendant_ptr;
//         while (next_left_descendant_ptr != NULL) {
//             descendant_ptr = next_left_descendant_ptr;
//             next_left_descendant_ptr = descendant_ptr->left;
//         }
//         return descendant_ptr;  // Ok
//     } else {
//         Node *cur_node_ptr = found_node_ptr;
//         Node *par_ptr = find_parent(root, cur_node_ptr->key);
//         while (par_ptr != NULL && par_ptr->right == cur_node_ptr) {
//             cur_node_ptr = par_ptr;
//             par_ptr = find_parent(root, cur_node_ptr->key);
//         }
//         return par_ptr;  // {NULL: not ok, !NULL: ok}
//     }
// }

// // ---------------------------------

// int dialog_del_node(Node **root) {
//     int key, get_int_result;

//     puts("Enter key: -->");
//     if ((get_int_result = get_int(&key)) == 0) {
//         return 0;  // EOF -> игнорируем весь остальной ввод
//     }

//     int add_result = del_node(root, key);
//     printf("\nDelete node with key = %d: %s\n", key, errmsgs[add_result]);
//     return 1;
// }

// int del_node(Node **root, int key) {
//     // Firstly, it is necessary to find target node.
//     Node *found_node_ptr = find_node(*root, key);
//     if (found_node_ptr == NULL) {
//         return 2; // No node with such a key
//     }

//     Node *node_to_del_ptr, *descendant_ptr;
//     if (found_node_ptr->left == NULL || found_node_ptr->right == NULL) {  // Found node has at least one leaf as it descendant
//         node_to_del_ptr = found_node_ptr;
//         descendant_ptr = (node_to_del_ptr->left != NULL) ? node_to_del_ptr->left : node_to_del_ptr->right;
        
//         Node *par_ptr = find_parent(*root, node_to_del_ptr->key);

//         if (par_ptr == NULL) {  // Delete the root
//             *root = descendant_ptr;
//             if (*root != NULL) {
//                 (*root)->next = NULL;
//             }
//         } else {
//             if (par_ptr->left == node_to_del_ptr) {
//                 par_ptr->left = descendant_ptr;
//             } else {
//                 par_ptr->right = descendant_ptr;
//             }
//         }

//         // Deleting the node
//         if (node_to_del_ptr->left == descendant_ptr) {
//             node_to_del_ptr->left = NULL;
//         } else {
//             node_to_del_ptr->right = NULL;
//         }

//         node_to_del_ptr->next = NULL;
//         free(node_to_del_ptr->info);
//         free(node_to_del_ptr);
//     } else {  // Found node has two descendants
//         Node *node_to_del_ptr = find_next_min_node(*root, key);
//         Node *par_ptr = find_parent(*root, node_to_del_ptr->key);
//         Node *descendant_par_ptr = node_to_del_ptr->right;  // Right just because node_to_del_ptr can not have left descendant.

//         if (par_ptr->left == node_to_del_ptr) {
//             par_ptr->left = descendant_par_ptr;
//         } else {
//             par_ptr->right = descendant_par_ptr;
//         }

//         node_to_del_ptr->right = node_to_del_ptr->next = NULL;
//         free(found_node_ptr->info);
//         found_node_ptr->key = node_to_del_ptr->key;
//         found_node_ptr->info = node_to_del_ptr->info;
//         free(node_to_del_ptr);
//     }

//     sew_tree(*root);

//     return 0;  // Ok
// }

// // ---------------------------------

// int show_tree_map(Node **root) {
//     if (*root == NULL) {
//         puts("Tree map is empty!");
//         return 1;
//     }

//     Node *first_node_ptr = *root;
//     while (first_node_ptr->left != NULL || first_node_ptr->right != NULL) {
//         if (first_node_ptr->left != NULL) {
//             first_node_ptr = first_node_ptr->left;
//         } else {
//             first_node_ptr = first_node_ptr->right;
//         }
//     }

//     puts("Content of tree map:");
//     Node *cur_node_ptr = first_node_ptr;
//     while (cur_node_ptr != NULL) {
//         printf("Node {key: %d, info: %s}\n", cur_node_ptr->key, cur_node_ptr->info);
//         cur_node_ptr = cur_node_ptr->next;
//     }

//     return 1;
// }

// // ---------------------------------

// int del_tree_map(Node **root){
//     if (*root == NULL) {
//         return 0;  // Nothing to delete
//     }

//     Node *first_node_ptr = *root;
//     while (first_node_ptr->left != NULL || first_node_ptr->right != NULL) {
//         if (first_node_ptr->left != NULL) {
//             first_node_ptr = first_node_ptr->left;
//         } else {
//             first_node_ptr = first_node_ptr->right;
//         }
//     }

//     Node *cur_node_ptr = first_node_ptr;
//     Node *prev_node_ptr = NULL;
//     while (cur_node_ptr != NULL) {
//         prev_node_ptr = cur_node_ptr;
//         cur_node_ptr = prev_node_ptr->next;

//         prev_node_ptr->left = prev_node_ptr->right = prev_node_ptr->next = NULL;
//         printf("Free memory for node {key: %d, info: %s}\n", prev_node_ptr->key, prev_node_ptr->info);
//         free(prev_node_ptr->info);
//         free(prev_node_ptr);
//     }

//     *root = NULL;

//     return 1;  // Ok
// }

// // ---------------------------------
