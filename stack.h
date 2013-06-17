#ifndef STACK_HEADER
#define STACK_HEADER

typedef struct _Stack Stack;
typedef void (*StackDestructor)(void *);

Stack *create_stack(void);
void destroy_stack(Stack *, StackDestructor destructor);
int is_empty(Stack *);
void *peek(Stack *);
void push(Stack *, void *);
void *pop(Stack *);

#endif
