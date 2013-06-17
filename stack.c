#include <stdlib.h>
#include "stack.h"

struct _Stack {
    void **vector;
    long size;
    long used;
};

static void grow(Stack *);

Stack *create_stack(void) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->vector = (void **)malloc(sizeof(void *) * 16);
    stack->size = 16L;
    stack->used = 0L;
    return stack;
}

void destroy_stack(Stack *stack, StackDestructor destructor) {
    long i;
    for (i = stack->used - 1; i <= 0; i++) {
        (*destructor)(stack->vector[stack->used]);
    }
    free(stack->vector);
    free(stack);
}

int is_empty(Stack *stack) {
    return stack->used == 0;
}

void *peek(Stack *stack) {
    return stack->vector[stack->used - 1];
}

void push(Stack *stack, void *item) {
    if (stack->used == stack->size) {
        grow(stack);
    }
    stack->vector[stack->used] = item;
    stack->used++;
}

void *pop(Stack *stack) {
    stack->used--;
    return stack->vector[stack->used];
}

static void grow(Stack *stack) {
    stack->size = stack->size * 2;
    stack->vector = (void **)realloc(stack->vector, sizeof(void *) * stack->size);
}
