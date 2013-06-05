#ifndef LIST_HEADER

typedef struct Pair_ Pair;
typedef Pair* List;

List empty_list(void);
List cons(void *, List);
void *car(List);
List cdr(List);
int list_is_empty(List);

#endif
