%{

#include "type.h"
#include "atoms.h"
#include "pair.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

Stack *current_values = NULL;

void destroy_object_stack(void *);

int main(int argc, char **argv) {
    declare_atoms();
    declare_pair();
    current_values = create_stack();
    push(current_values, (void *)create_stack());
    yyparse();
    if (! is_empty(current_values)) {
        write_object((Object *)peek((Stack *)peek(current_values)), (Printf)printf);
    }
    printf("\n");
    destroy_stack(current_values, destroy_object_stack);
    free_dictionary();
    return 0;
}

void destroy_object_stack(void *object_stack) {
    destroy_stack((Stack *)object_stack, (StackDestructor)destroy);    
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

statement: element;

pair: pair_head '.' pair_tail;

pair_head: '(' element {
        Stack *nesting = create_stack();
        push(nesting, pop((Stack *)peek(current_values)));
        push(current_values, nesting);
    };
    
pair_tail: element ')' {
        Object *second = pop((Stack *)peek(current_values));
        Object *first = pop((Stack *)peek(current_values));
        destroy_object_stack(pop(current_values));
        push((Stack *)peek(current_values), (void *)pair(first, second));
    };
    
element: atom
    | pair;
    
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
