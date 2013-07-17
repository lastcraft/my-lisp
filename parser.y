%{

#include "type.h"
#include "nil.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Stack *input = NULL;

extern int yyparse(void);
extern int yylex(void);
extern void yyerror(char *);

void create_reader(void);
void free_reader(void);
Object *read(void);
static void destroy_object_stack(void *);
static Object *pour_stack_into_list(Object *, Stack *);

void create_reader(void) {
    input = create_stack();
    push(input, (void *)create_stack());
}

void free_reader(void) {
    destroy_stack(input, destroy_object_stack);
}

Object *read(void) {
    yyparse();
    if (is_empty(input)) {
        return nil();
    } else {
        return (Object *)pop((Stack *)peek(input));
    }
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

%}

%token NIL
%token <b> BOOLEAN
%token <l> NUMBER
%token <s> IDENTIFIER
%token <s> QUOTED_STRING

%union {
    bool b;
    long l;
    char *s;
}

%%

statement: elements;

list: list_head elements '.' pair_tail
    | list_head elements list_tail
    | list_head empty_list;

list_head: '(' {
        push(input, create_stack());
    };
    
empty_list: ')' {
        destroy_object_stack((Stack *)pop(input));
        push((Stack *)peek(input), (void *)nil());
    };

pair_tail: element ')' {
        Stack *stack = pop(input);
        Object *second = pop(stack);
        Object *first = pop(stack);
        Object *list = pair(first, second);
        list = pour_stack_into_list(list, stack);
        destroy_object_stack(stack);
        push((Stack *)peek(input), (void *)list);
    };
    
list_tail: ')' {
        Stack *stack = pop(input);
        Object *last = pop(stack);
        Object *list = pair(last, nil());
        list = pour_stack_into_list(list, stack);
        destroy_object_stack(stack);
        push((Stack *)peek(input), (void *)list);
    };
    
elements: element | elements element;
    
element: atom
    | list;
    
atom: BOOLEAN {
        push((Stack *)peek(input), (void *)boolean($1));
    }
    | NUMBER {
        push((Stack *)peek(input), (void *)number($1));
    }
    | IDENTIFIER {
        push((Stack *)peek(input), (void *)identifier($1));
    }
    | QUOTED_STRING {
        push((Stack *)peek(input), (void *)quoted_string($1));
    }
    | NIL {
        push((Stack *)peek(input), (void *)nil());
    };
