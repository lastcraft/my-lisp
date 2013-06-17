#ifndef STACK_HEADER
#define STACK_HEADER

typedef struct _Stack Stack;

Stack *create_stack();
void destroy_stack(Stack *, void (*destructor)(void *));
void push(Stack *, void *);
void *pop(Stack *);

#endif
