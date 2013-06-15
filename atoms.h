#ifndef ATOMS_HEADER
#define ATOMS_HEADER

#include "type.h"
#include "list.h"

Type *nil_type;
Type *number_type;
Type *identifier_type;
Type *string_type;

void create_atoms(void);
Object *nil(void);
Object *number(long);
Object *identifier(char *);
Object *quoted_string(char *);

#endif
