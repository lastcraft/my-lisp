#ifndef ATOMS_HEADER
#define ATOMS_HEADER

#include "type.h"

Type *number_type;
Type *identifier_type;
Type *string_type;

void declare_atoms(void);
Object *number(long);
int is_number(Object *);
Object *identifier(char *);
int is_identifier(Object *);
Object *quoted_string(char *);
int is_string(Object *);

#endif
