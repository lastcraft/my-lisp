#ifndef ATOMS_HEADER
#define ATOMS_HEADER

#include "type.h"
#include "list.h"

Type *number_type;
Type *string_type;

void create_atoms(void);
Object *number(long);
Object *string(char *);

#endif
