#ifndef LIST_HEADER

typedef struct Pair_ Pair;
typedef Pair* List;

List empty_list(void);
int list_is_empty(List);
List cons(void *, List);
void delete_list(List);
void *car(List);
List cdr(List);

#endif
