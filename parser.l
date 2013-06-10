%{
#include "y.tab.h"
#include "list.h"
#include "type.h"
%}

%%
[0-9]+ {
    yylval.l = atoi(yytext);
    return NUMBER;
}

[a-zA-Z_-] {
    yylval.s = yytext;
    return SYMBOL;
}

[\t ];

\n {
    printf("ok\n");
    return 0;
}

\( { return '('; }
\) { return ')'; }

. ;
%%
