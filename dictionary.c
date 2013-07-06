#include "dictionary.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>

typedef struct Entry_ Entry;
struct Entry_ {
    Entry *next;
    char *key;
    Object *object;
};

struct Dictionary_ {
    Entry *first;
};

Entry **vacancy(Dictionary *);
static int key_matches(char *, char *);

Dictionary *create_dictionary(void) {
    Dictionary *dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    dictionary->first = NULL;
    return dictionary;
}

void free_dictionary(Dictionary *dictionary) {
    Entry *entry = dictionary->first;
    while (entry != NULL) {
        Entry *next = entry->next;
        free(entry->key);
        destroy(entry->object);
        free(entry);
        entry = next;
    }
    free(dictionary);
}

void add(Dictionary *dictionary, char *key, Object *object) {
    Entry **slot = vacancy(dictionary);
    *slot = (Entry *)malloc(sizeof(Entry));
    (*slot)->next = NULL;
    (*slot)->key = strdup(key);
    (*slot)->object = object;
}

Object *find(Dictionary *dictionary, char *key) {
    Entry *entry = dictionary->first;
    while (entry != NULL) {
        if (key_matches(key, entry->key)) {
            return entry->object;
        }
    }
    return NULL;
}

Entry **vacancy(Dictionary *dictionary) {
    Entry **slot = &(dictionary->first);
    while (*slot != NULL) {
        slot = &((*slot)->next);
    }
    return slot;
}

static int key_matches(char *key1, char *key2) {
    return strcmp(key1, key2) == 0;
}
