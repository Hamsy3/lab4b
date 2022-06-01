#include "tree.h"
Node *new_node (int border_x_max, int border_x_min, int border_y_max, int border_y_min, int max_len) {
    Node *node = calloc(1, sizeof(Node));
    node->border_x_max = border_x_max;
    node->border_x_min = border_x_min;
    node->border_y_max = border_y_max;
    node->border_y_min = border_y_min;
    node->max_len = max_len;
    node->len = 0;
    node->up_right = NULL;
    node->up_left = NULL;
    node->down_left = NULL;
    node->down_right = NULL;
    node->elem = NULL;
    return node;
}
Elem *new_elem (int key1_x, int key2_y, char *info) {
    Elem *elem = calloc(1, sizeof(Elem));
    elem->info = info;
    elem->key1_x = key1_x;
    elem->key2_y = key2_y;
    elem->next = NULL;
    return elem;
}
void print_elems (Elem *elem) {
    Elem *ptr = elem;
    while (ptr) {
        printf("%d %d(%s) ", ptr->key1_x, ptr->key2_y, ptr->info);
        ptr = ptr->next;
    }
}
void reversed_print_elems (Elem *elem) {
    Elem *ptr = elem;
    Elem *prev = elem;
    Elem *tmp = elem;
    while (ptr->next) {
        ptr = ptr->next;
    }
    prev = ptr;
    printf("%d %d(%s) ", ptr->key1_x, ptr->key2_y, ptr->info);
    while (prev != elem) {
        prev = elem;
        while (prev->next != ptr) {
            prev = prev->next;
            if (prev->next == ptr) {
                tmp = prev;
            }
        }
        ptr = tmp;
        printf("%d %d(%s) ", prev->key1_x, prev->key2_y, prev->info);
    }
}
void in_order (Node *root) {
    if (root) {
        in_order(root->down_left);
        in_order(root->up_left);
        in_order (root->down_right);
        in_order(root->up_right);
        if (root) {
            if(root->elem){
                print_elems(root->elem);
            }
        }
    }
}
void special_reverse_in_order (Node *root, int dec1, int dec2) {
    if (root) {
        special_reverse_in_order(root->up_right, dec1, dec2);
        special_reverse_in_order (root->down_right, dec1, dec2);
        special_reverse_in_order(root->up_left, dec1, dec2);
        special_reverse_in_order(root->down_left, dec1, dec2);
        if (root) {
            if((root->elem) && ((abs(root->elem->key1_x)/10%10) == dec1) && ((abs(root->elem->key2_y)/10%10) == dec2)){
                reversed_print_elems (root->elem);
            }
        }
    }
}
void insert_in_list (Elem *elem, Elem **head) {
    Elem *ptr = NULL;
    if ((*head == NULL) || ((elem->key1_x < (*head)->key1_x) || ((elem->key1_x  == (*head)->key1_x) && (elem->key2_y  < (*head)->key2_y)))) {
        elem->next = *head;
        *head = elem;
    } else {
        ptr = *head;
        while ((ptr->next != NULL) && ((elem->key1_x > ptr->key1_x) || ((elem->key1_x  == ptr->key1_x) && (elem->key2_y  > ptr->key2_y)))) {
            ptr = ptr->next;
        }
        elem->next = ptr->next;
        ptr->next = elem;
    }
}
void insert_in_list_for_del (Elem **elem, Elem **head) {
    Elem *ptr = NULL;
    if ((*head == NULL) || (((*elem)->key1_x < (*head)->key1_x) || (((*elem)->key1_x  == (*head)->key1_x) && ((*elem)->key2_y  < (*head)->key2_y)))) {
        (*elem)->next = *head;
        *head = (*elem);
    } else {
        ptr = *head;
        while ((ptr->next != NULL) && (((*elem)->key1_x > ptr->key1_x) || (((*elem)->key1_x  == ptr->key1_x) && ((*elem)->key2_y  > ptr->key2_y)))) {
            ptr = ptr->next;
        }
        (*elem)->next = ptr->next;
        ptr->next = (*elem);
    }
}
void subdivide (Node **node) {
    float border_x_middle = ((*node)->border_x_max + (*node)->border_x_min)/2;
    float border_y_middle = ((*node)->border_y_max + (*node)->border_x_min)/2;
    (*node)->up_right = new_node((*node)->border_x_max, border_x_middle, (*node)->border_y_max, border_y_middle, (*node)->max_len);
    (*node)->up_left = new_node(border_x_middle, (*node)->border_x_min, (*node)->border_y_max, border_y_middle, (*node)->max_len);
    (*node)->down_left = new_node(border_x_middle, (*node)->border_x_min, border_y_middle, (*node)->border_y_min, (*node)->max_len);
    (*node)->down_right = new_node((*node)->border_x_max, border_x_middle, border_y_middle, (*node)->border_y_min, (*node)->max_len);
}
int boundary_check (int key1_x, int key2_y, Node *root) {
    return ((key1_x <= root->border_x_max) && (key1_x >= root->border_x_min)) && ((key2_y <= root->border_y_max) && (key2_y >= root->border_y_min));
}
int insert (int key1, int key2, char* info, Node **root) {
    if (*root == NULL) {
        *root = (Node*)calloc(1, sizeof(Node));
        printf("Please, input max_len\n");
        (*root)->len = 0;
        input_int(&((*root)->max_len));
        printf("Please, input border (x_max)\n");
        scanf("%f", &((*root)->border_x_max));
        printf("Please, input border (x_min)\n");
        scanf("%f", &((*root)->border_x_min));
        printf("Please, input border (y_max)\n");
        scanf("%f",  &((*root)->border_y_max));
        printf("Please, input border (y_min)\n");
        scanf("%f", &((*root)->border_y_min));
        if (((key1 >= (*root)->border_x_max) || (key1 <= (*root)->border_x_min)) || ((key2 >= (*root)->border_y_max) || (key2 <= (*root)->border_y_min))) {
            free((*root));
            (*root) = NULL;
            free(info);
            return 1;
        }
        (*root)->up_right = NULL;
        (*root)->up_left = NULL;
        (*root)->down_left = NULL;
        (*root)->down_right = NULL;
    }
    Elem *elem = NULL;
    if (boundary_check(key1, key2, *root) == 0) {
        return 1;
    }
    if (((*root)->len < (*root)->max_len) && ((*root)->down_left == NULL)) {
        elem = new_elem(key1, key2, info);
        insert_in_list(elem, (&(*root)->elem));
        ++((*root)->len);
        return 0;
    }
    if ((*root)->down_left == NULL) {
        subdivide(root);
        Elem *ptr = (*root)->elem;
        while (ptr) {
            insert(ptr->key1_x, ptr->key2_y, ptr->info, root);
            ptr = ptr->next;
        }
        ptr = (*root)->elem;
        while ((*root)->elem != NULL) {
            ptr = ptr->next;
            if ((*root)->elem) {
                free((*root)->elem);
            }
            (*root)->elem = ptr;
        }
        (*root)->len = -1;
    }
    if (insert(key1, key2, info, &((*root)->up_right)) == 0) {
        return 0;
    }
    if (insert(key1, key2, info, &((*root)->down_right)) == 0) {
        return 0;
    }
    if (insert(key1, key2, info, &((*root)->up_left)) == 0) {
        return 0;
    }
    if (insert(key1, key2, info, &((*root)->down_left)) == 0) {
        return 0;
    }
    return 1;
}
int read_from_txt_file (char *name, Node **root) {
    int key1 = 0;
    int key2 = 0;
    int len = 0;
    char *info = NULL;
    FILE *fd = fopen(name, "r");
    if (fd == NULL) {
        return 1;
    } else {
    while ((fscanf(fd, "%d", &key1) == 1) && (fscanf(fd, "%d", &key2) == 1) && (fscanf(fd, "%d", &len)==1)) {
        info = calloc(len, sizeof(char));
        fgets(info, len, fd);
        insert(key1, key2, info, root);
    }
    fclose(fd);
    free(name);
    return 0;
}
}
void free_tree (Node *root) {
    if (root) {
        free_tree(root->up_right);
        free_tree (root->down_right);
        free_tree(root->up_left);
        free_tree(root->down_left);
        if (root) {
            if(root->elem){
                Elem *ptr = root->elem;
                while (root->elem != NULL) {
                    ptr = ptr->next;
                    free(root->elem->info);
                    free(root->elem);
                    root->elem = ptr;
                }
            }
        }
        free(root);
    }
}
Elem *search (Node *root, int key1, int key2) {
    Node *ptr_node = root;
    while (ptr_node->up_left) {
        if (boundary_check(key1, key2, ptr_node->up_right) == 1) {
            ptr_node = ptr_node->up_right;
        } else if (boundary_check(key1, key2, ptr_node->up_left) == 1) {
            ptr_node = ptr_node->up_left;
        } else if (boundary_check(key1, key2, ptr_node->down_left) == 1) {
            ptr_node = ptr_node->down_left;
        } else if (boundary_check(key1, key2, ptr_node->down_right) == 1) {
            ptr_node = ptr_node->down_right;
        }
    }
    Elem *ptr_list = ptr_node->elem;
    while (ptr_list) {
        if ((ptr_list->key1_x == key1) && (ptr_list->key2_y == key2)) {
            return ptr_list;
        }
        ptr_list = ptr_list->next;
    }
    return NULL;
}
void delete (Node **root, int key1, int key2) {
    Node *ptr_node = *root;
    Node *prev_node = *root;
    while (ptr_node->up_left) {
        if (boundary_check(key1, key2, ptr_node->up_right) == 1) {
            prev_node = ptr_node;
            ptr_node = ptr_node->up_right;
        } else if (boundary_check(key1, key2, ptr_node->up_left) == 1) {
            prev_node = ptr_node;
            ptr_node = ptr_node->up_left;
        } else if (boundary_check(key1, key2, ptr_node->down_left) == 1) {
            prev_node = ptr_node;
            ptr_node = ptr_node->down_left;
        } else if (boundary_check(key1, key2, ptr_node->down_right) == 1) {
            prev_node = ptr_node;
            ptr_node = ptr_node->down_right;
        }
    }
    Elem *ptr_list = ptr_node->elem;
    Elem *prev_list = ptr_node->elem;
    while (ptr_list) {
        if ((ptr_list->key1_x == key1) && (ptr_list->key2_y == key2)) {
            break;
        }
        prev_list = ptr_list;
        ptr_list = ptr_list->next;
    }
    if (ptr_list == ptr_node->elem) {
        ptr_node->elem = ptr_list->next;
        free(ptr_list->info);
        free(ptr_list);
        ptr_list = NULL;
        (ptr_node->len)--;
    } else {
        prev_list->next = ptr_list->next;
        free(ptr_list->info);
        free(ptr_list);
        ptr_list = NULL;
        (ptr_node->len)--;
    }
    if (prev_node->down_left) {
        if (prev_node->max_len == (prev_node->down_right->len + prev_node->down_left->len + prev_node->up_left->len + prev_node->up_right->len)) {
            prev_node->len = prev_node->down_right->len + prev_node->down_left->len + prev_node->up_left->len + prev_node->up_right->len;
            prev_node->elem = calloc(1, sizeof(Elem));
            Elem *ptr_prev_list = prev_node->elem;
            for (int i = 1; i < prev_node->len; ++i) {
                ptr_prev_list->next = calloc(1, sizeof(Elem));
                ptr_prev_list = ptr_prev_list->next;
                ptr_prev_list->next = NULL;
            }
            ptr_prev_list = prev_node->elem;
            ptr_list = prev_node->down_left->elem;
            while (ptr_list) {
                ptr_prev_list->key1_x = ptr_list->key1_x;
                ptr_prev_list->key2_y = ptr_list->key2_y;
                ptr_prev_list->info = calloc(strlen(ptr_list->info) + 1, sizeof(char));
                strcpy (ptr_prev_list->info, ptr_list->info);
                ptr_prev_list = ptr_prev_list->next;
                ptr_list = ptr_list->next;
            }
            ptr_list = prev_node->up_left->elem;
            while (ptr_list) {
                ptr_prev_list->key1_x = ptr_list->key1_x;
                ptr_prev_list->key2_y = ptr_list->key2_y;
                ptr_prev_list->info = calloc(strlen(ptr_list->info) + 1, sizeof(char));
                strcpy (ptr_prev_list->info, ptr_list->info);
                ptr_prev_list = ptr_prev_list->next;
                ptr_list = ptr_list->next;
            }
            ptr_list = prev_node->down_right->elem;
            while (ptr_list) {
                ptr_prev_list->key1_x = ptr_list->key1_x;
                ptr_prev_list->key2_y = ptr_list->key2_y;
                ptr_prev_list->info = calloc(strlen(ptr_list->info) + 1, sizeof(char));
                strcpy (ptr_prev_list->info, ptr_list->info);
                ptr_prev_list = ptr_prev_list->next;
                ptr_list = ptr_list->next;
            }
            ptr_list = prev_node->up_right->elem;
            while (ptr_list) {
                ptr_prev_list->key1_x = ptr_list->key1_x;
                ptr_prev_list->key2_y = ptr_list->key2_y;
                ptr_prev_list->info = calloc(strlen(ptr_list->info) + 1, sizeof(char));
                strcpy (ptr_prev_list->info, ptr_list->info);
                ptr_prev_list = ptr_prev_list->next;
                ptr_list = ptr_list->next;
            }
            free_tree(prev_node->up_left);
            prev_node->up_left = NULL;
            free_tree(prev_node->down_left);
            prev_node->down_left = NULL;
            free_tree(prev_node->up_right);
            prev_node->up_right = NULL;
            free_tree(prev_node->down_right);
            prev_node->down_right = NULL;
        }
    }
 
}
void special_find (Node *root, int isfirst, Elem **max_point) {
    static float max;
    if (isfirst == 1) {
        max = -1;
        isfirst = 0;
    }
    if (root->up_left) {
        special_find(root->up_left, isfirst, max_point);
      
    }
    if (root->down_left) {
        special_find(root->down_left, isfirst, max_point);
    }
    if (root->up_right) {
        special_find(root->up_right, isfirst, max_point);
    }
    if (root->down_right) {
        special_find(root->down_right, isfirst, max_point);
    }
    if (root->down_left == NULL) {
        Elem *ptr_list = root->elem;
        while (ptr_list) {
                if (sqrt(ptr_list->key1_x * ptr_list->key1_x + ptr_list->key2_y * ptr_list->key2_y) > max) {
                    max = sqrt(ptr_list->key1_x * ptr_list->key1_x + ptr_list->key2_y * ptr_list->key2_y);
                    *max_point = ptr_list;
                }
                ptr_list = ptr_list->next;
            }
    }
}

