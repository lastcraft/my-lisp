#include "binding.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>

typedef struct Entry_ Entry;
struct Entry_ {
    Entry *next;
    char *key;
    Object *object;
};

struct Binding_ {
    Entry *first;
};

Entry **vacancy(Binding *);
static int key_matches(char *, char *);

Binding *create_binding(Binding *parent) {
    Binding *binding = (Binding *)malloc(sizeof(Binding));
    binding->first = NULL;
    return binding;
}

Binding *free_binding(Binding *binding) {
    Entry *entry = binding->first;
    while (entry != NULL) {
        Entry *next = entry->next;
        free(entry->key);
        destroy(entry->object);
        free(entry);
        entry = next;
    }
    free(binding);
    return NULL;
}

void add(Binding *binding, char *key, Object *object) {
    Entry **slot = vacancy(binding);
    *slot = (Entry *)malloc(sizeof(Entry));
    (*slot)->next = NULL;
    (*slot)->key = strdup(key);
    (*slot)->object = object;
}

Object *find(Binding *binding, char *key) {
    Entry *entry = binding->first;
    while (entry != NULL) {
        if (key_matches(key, entry->key)) {
            return entry->object;
        }
        entry = entry->next;
    }
    return NULL;
}

Entry **vacancy(Binding *binding) {
    Entry **slot = &(binding->first);
    while (*slot != NULL) {
        slot = &((*slot)->next);
    }
    return slot;
}

static int key_matches(char *key1, char *key2) {
    return strcmp(key1, key2) == 0;
}
