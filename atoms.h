#ifndef ATOMS_HEADER
#define ATOMS_HEADER

#include "type.h"

typedef struct Pair_ Pair;

Type *nil_type;
Type *number_type;
Type *identifier_type;
Type *string_type;
Type *pair_type;

void create_atoms(void);
Object *nil(void);
Object *number(long);
Object *identifier(char *);
Object *quoted_string(char *);
Object *pair(Object *, Object *);

#endif
