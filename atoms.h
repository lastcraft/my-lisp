#ifndef ATOMS_HEADER
#define ATOMS_HEADER

#include "type.h"

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

Object *car(Object *);
Object *cdr(Object *);

#endif
