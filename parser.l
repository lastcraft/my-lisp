%{

#include "type.h"
#include "list.h"
#include "y.tab.h"

%}

%%

[0-9]+ {
    yylval.l = atoi(yytext);
    return NUMBER;
}

[a-zA-Z_+-][a-zA-Z0-9_+-]* {
    yylval.s = yytext;
    return SYMBOL;
}

[\t ];

\n {
    return 0;
}

\( { return '('; }
\) { return ')'; }

. ;

%%
