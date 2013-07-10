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
    Binding *parent;
    Entry *first;
};

static Object *free_entry(Entry *);
static Entry **vacancy(Binding *);
static Entry **find_owner(Binding *, char *);
static int key_matches(char *, char *);

Binding *create_binding(Binding *parent) {
    Binding *binding = (Binding *)malloc(sizeof(Binding));
    binding->parent = parent;
    binding->first = NULL;
    return binding;
}

Binding *free_binding(Binding *binding) {
    Binding *parent = binding->parent;
    Entry *entry = binding->first;
    while (entry != NULL) {
        Entry *next = entry->next;
        destroy(free_entry(entry));
        entry = next;
    }
    free(binding);
    return parent;
}

void add(Binding *binding, char *key, Object *object) {
    Entry **slot = vacancy(binding);
    *slot = (Entry *)malloc(sizeof(Entry));
    (*slot)->next = NULL;
    (*slot)->key = strdup(key);
    (*slot)->object = object;
}

Object *extract(Binding *binding, char *key) {
    Entry **owner = find_owner(binding, key);
    if (owner == NULL) {
        return NULL;
    }
    Entry *entry = *owner;
    *owner = entry->next;
    return free_entry(entry);
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

static Object *free_entry(Entry *entry) {
    Object *object = entry->object;
    free(entry->key);
    free(entry);
    return object;
}

static Entry **vacancy(Binding *binding) {
    Entry **slot = &(binding->first);
    while (*slot != NULL) {
        slot = &((*slot)->next);
    }
    return slot;
}

static Entry **find_owner(Binding *binding, char *key) {
    Entry **owner = &(binding->first);
    while (*owner != NULL) {
        if (key_matches(key, (*owner)->key)) {
            return owner;
        }
        owner = &((*owner)->next);
    }
    return NULL;
}

static int key_matches(char *key1, char *key2) {
    return strcmp(key1, key2) == 0;
}
