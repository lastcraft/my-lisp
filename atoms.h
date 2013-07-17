#ifndef ATOMS_HEADER
#define ATOMS_HEADER

#include <stdbool.h>
#include "type.h"

Type *boolean_type;
Type *number_type;
Type *identifier_type;
Type *string_type;

void declare_atoms(void);
Object *boolean(bool);
int is_boolean(Object *);
bool is_true(Object *);
bool is_false(Object *);
Object *number(long);
int is_number(Object *);
Object *identifier(char *);
int is_identifier(Object *);
Object *quoted_string(char *);
int is_string(Object *);

#endif
