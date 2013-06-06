#include "list.h"
#include <stdlib.h>

List delete_head(List);
List clone(List);

struct Pair_ {
    void *item;
    Pair *next;
};

List empty_list(void) {
    return NULL;
}

int list_is_empty(List list) {
    return list == NULL;
}

List cons(void *item, List list) {
    Pair *pair = (Pair *)malloc(sizeof(Pair));
    pair->item = item;
    pair->next = list;
    return pair;
}

void delete_list(List list) {
    while (list != NULL) {
        list = delete_head(list);
    }
}

void *car(List list) {
    return list->item;
}

List cdr(List list) {
    if (list_is_empty(list)) {
        return NULL;
    }
    clone(list->next);
}

List delete_head(List list) {
    List tail = list->next;
    free(list);
    return tail;
}

List clone(List list) {
    cons(list->item, clone(list->next));
}
