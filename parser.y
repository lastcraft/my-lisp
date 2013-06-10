%{

#include "type.h"
#include "atoms.h"

int main(int argc, char **argv) {
    yyparse();
    return 0;
}

%}

%token NUMBER SYMBOL
%union {
    long l;
    char *s;
    Object *object;
}

%%

statement: element;
list: '(' elements ')';
elements: element
    | element elements;
element: atom
    | list;
atom: NUMBER
    | SYMBOL;
