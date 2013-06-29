#ifndef DICTONARY_HEADER
#define DICTIONARY_HEADER

#include "type.h"

typedef struct Dictionary_ Dictionary;

Dictionary *create_dictionary(void);
void destroy_dictionary(Dictionary *);
Object *find(Dictionary *, char *);
void add(Dictionary *, char *, Object *);

#endif
