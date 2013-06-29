#include "dictionary.h"
#include "type.h"
#include <stdlib.h>
#include <string.h>

typedef struct Entry_ {
    Entry *next;
    char *key;
    Object *object;
} Entry;

struct Dictionary_ {
    Entry *first;
};

static key_matches(char *, char *);

Dictionary *create_dictionary(void) {
    Dictionary *dictionary = (Dictionary *)malloc(sizeof(Dictionary));
    dictionary->first = NULL;
    return dictionary;
}

void destroy_dictionary(Dictionary *dictionary) {
    
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

void add(Dictionary *dictionary, char *key, Object *object) {
    
}

static key_matches(char *key1, char *key2) {
    return strcmp(key1, key2) == 0;
}
