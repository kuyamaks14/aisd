#include "main.h"

const char *errmsgs[] = {"Ok", "Key duplicate", "No node with such a key", "Empty B-tree", "No successor or such node in B-tree"};

const char *msgs[] = {"0. Quit", "1. Add", "2. Find", "3. Find next min", "4. Delete", "5. Show"};
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

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
            printf("\nItem{key: %d, info: %s, pos: %d, n: %d, leaf}", (*ptr)->key[i], (*ptr)->info[i], i, (*ptr)->n);
        }
    } else {
        for (int i = (*ptr)->n - 1; i >= 0; i--) {
            b_tree_print(&((*ptr)->next_ptr[i + 1]));
            printf("\nItem{key: %d, info: %s, pos: %d, n: %d}", (*ptr)->key[i], (*ptr)->info[i], i, (*ptr)->n);
        }
        b_tree_print(&((*ptr)->next_ptr[0]));
    }

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
            puts("sit 1");
            if (target->n == T - 1 && ((i < ptr->n && ptr->next_ptr[i + 1]->n >= T) || (i > 0 && ptr->next_ptr[i - 1]->n >= T))) {
                puts("sit 1a");
                if (i < ptr->n && ptr->next_ptr[i + 1]->n >= T) {  // Right child contains more than T child nodes.
                    puts("sit 1aa");
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
                    puts("sit 1ab");
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
                puts("sit 1b");
                if (i < ptr->n) {
                    puts("sit 1ba");
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
                } else {
                    puts("sit 1bb");
                    Node *left = ptr->next_ptr[i - 1];

                    for (int j = 0; j < T - 1; j++) {
                        target->key[T + j] = target->key[j];
                        target->info[T + j] = target->info[j];
                    }
                    for (int j = 0; j < T; j++) {
                        target->next_ptr[T + j] = target->next_ptr[j];
                    }

                    target->key[T - 1] = ptr->key[i - 1];
                    target->info[T - 1] = ptr->info[i - 1];

                    for (int j = 0; j < T - 1; j++) {
                        target->key[j] = left->key[j];
                        target->info[j] = left->info[j];
                    }
                    for (int j = 0; j < T; j++) {
                        target->next_ptr[j] = left->next_ptr[j];
                    }

                    for (int j = i - 1; j < ptr->n - 1; j++) {
                        ptr->key[j] = ptr->key[j + 1];
                        ptr->info[j] = ptr->info[j + 1];
                    }
                    for (int j = i - 1; j < ptr->n; j++) {
                        ptr->next_ptr[j] = ptr->next_ptr[j + 1];
                    }

                    ptr->n--;
                    target->n += T;
                    free(left);
                }
            }

            ptr = target;
            continue;
        } else {  // situation 2
            puts("sit 2");
            Node *target = ptr->next_ptr[i];
            Node *right = ptr->next_ptr[i + 1];

            if (target->n >= T) {
                puts("sit 2a");
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
                puts("sit 2b");
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
                puts("sit 2c");
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
        puts("free");
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
    int result_code = (successor != NULL) ? 0 : 4;
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

int b_tree_erase(Node *ptr) {
    if (ptr->leaf) {
        for (int i = ptr->n - 1; i >= 0; i--) {
            free(ptr->info[i]);
        }

        free(ptr);
    } else {
        for (int i = ptr->n - 1; i >= 0; i--) {
            b_tree_erase(ptr->next_ptr[i + 1]);
            free(ptr->info[i]);
        }

        b_tree_erase(ptr->next_ptr[0]);
        free(ptr);
    }

    return 1;  // Ok
}

// ---------------------------------