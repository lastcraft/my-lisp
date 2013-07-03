%{

#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *current_values = NULL;

extern int yyparse(void);
extern int yylex(void);
extern void yyerror(char *);

static Stack *create_context(void);
static void free_context(Stack *);
static void destroy_object_stack(void *);
static Object *pour_stack_into_list(Object *, Stack *);
static Object *read(Stack *);
static void print(Object *value);
static Object *eval(Object *);
static Object *apply(char *, Object *);

int main(int argc, char **argv) {
    declare_nil();
    declare_atoms();
    declare_pair();
    current_values = create_context();
    print(eval(read(current_values)));
    free_context(current_values);
    free_declarations();
    return 0;
}

static Stack *create_context(void) {
    Stack *context = create_stack();
    push(context, (void *)create_stack());
    return context;
}

static void free_context(Stack *context) {
    destroy_stack(context, destroy_object_stack);
}

static void destroy_object_stack(void *object_stack) {
    destroy_stack((Stack *)object_stack, (StackDestructor)destroy);    
}

static Object *pour_stack_into_list(Object *list, Stack *stack) {
    while (! is_empty(stack)) {
        list = pair((Object *)pop(stack), list);
    }
    return list;
}

static Object *read(Stack *input) {
    yyparse();
    if (! is_empty(input)) {
        return (Object *)peek((Stack *)peek(input));
    } else {
        return NULL;
    }
}

static void print(Object *value) {
    if (value != NULL) {
        write_object(value, (Printf)printf);
        printf("\n");
    }
}

static Object *eval(Object *object) {
    if (object == NULL) {
        return NULL;
    }
    if (is_pair(object)) {
        if (is_identifier(car(object))) {
            return apply((char *)value(car(object)), cdr(object));
        } else {
            printf("Identifier expected\n");
            return NULL;
        }
    } else {
        return object;
    }
}

static Object *apply(char *symbol, Object *values) {
    printf("Applying %s to ", symbol);
    write_object(values, (Printf)printf);
    printf("\n");
    return nil();
}

%}

%token NIL
%token <l> NUMBER
%token <s> IDENTIFIER
%token <s> QUOTED_STRING

%union {
    long l;
    char *s;
}

%%

statement: elements;

list: list_head elements '.' pair_tail
    | list_head elements list_tail
    | list_head empty_list;

list_head: '(' {
        push(current_values, create_stack());
    };
    
empty_list: ')' {
        destroy_object_stack((Stack *)pop(current_values));
        push((Stack *)peek(current_values), (void *)nil());
    };

pair_tail: element ')' {
        Stack *stack = pop(current_values);
        Object *second = pop(stack);
        Object *first = pop(stack);
        Object *list = pair(first, second);
        list = pour_stack_into_list(list, stack);
        destroy_object_stack(stack);
        push((Stack *)peek(current_values), (void *)list);
    };
    
list_tail: ')' {
        Stack *stack = pop(current_values);
        Object *last = pop(stack);
        Object *list = pair(last, nil());
        list = pour_stack_into_list(list, stack);
        destroy_object_stack(stack);
        push((Stack *)peek(current_values), (void *)list);
    };
    
elements: element | elements element;
    
element: atom
    | list;
    
atom: NUMBER {
        push((Stack *)peek(current_values), (void *)number($1));
    }
    | IDENTIFIER {
        push((Stack *)peek(current_values), (void *)identifier($1));
    }
    | QUOTED_STRING {
        push((Stack *)peek(current_values), (void *)quoted_string($1));
    }
    | NIL {
        push((Stack *)peek(current_values), (void *)nil());
    };
