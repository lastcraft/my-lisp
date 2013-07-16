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

static Entry *create_entry(Entry *, char *, Object *);
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
    *slot = create_entry(NULL, key, object);
}

void overwrite(Binding *binding, char *key, Object *object) {
    Object *previous = extract(binding, key);
    if (previous != NULL) {
        destroy(previous);
    }
    add(binding, key, object);
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
    return (binding->parent == NULL ? NULL : find(binding->parent, key));
}

static Entry *create_entry(Entry *next, char *key, Object *object) {
    Entry *entry = (Entry *)malloc(sizeof(Entry));
    entry->next = NULL;
    entry->key = strdup(key);
    entry->object = object;
    return entry;
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
