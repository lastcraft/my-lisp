#ifndef DICTIONARY_HEADER
#define DICTIONARY_HEADER

#include "type.h"

typedef struct Dictionary_ Dictionary;

Dictionary *create_dictionary(void);
void free_dictionary(Dictionary *);
void add(Dictionary *, char *, Object *);
Object *find(Dictionary *, char *);

#endif
