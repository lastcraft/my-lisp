%{

#include "type.h"
#include "atoms.h"
#include <stdlib.h>
#include <stdio.h>

Object *current_object = NULL;

int main(int argc, char **argv) {
    create_atoms();
    yyparse();
    write_object(current_object, (Printf)printf);
    printf("\n");
    destroy(current_object);
    free_dictionary();
    return 0;
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

list: list_start elements list_end;

list_start: '(';
list_end: ')';

elements: element
    | element elements;
    
element: atom
    | list;
    
atom: NUMBER { current_object = number($1); }
    | IDENTIFIER { current_object = identifier($1); }
    | QUOTED_STRING { current_object = quoted_string($1); }
    | NIL { current_object = nil(); };
