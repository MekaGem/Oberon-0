%{
    #include <stdlib.h>
    #include "node.h"
    #include "sample.tab.c"

    void yyerror(char *);
%}

%option noyywrap

DIGIT [0-9]
LETTER [a-zA-Z]
IDENT {LETTER}({LETTER}|{DIGIT})*
INTEGER {DIGIT}+
FLOAT {DIGIT}+"."{DIGIT}+
NUMBER {FLOAT}|{INTEGER}

%%

"DIV" {return DIV;}
"MOD" {return MOD;}
"OR" {return OR;}
"<=" {return LSEQ;}
">=" {return GREQ;}
":=" {return ASSIGN;}

{IDENT} { yylval.ident_node = new Ident(yytext); return IDENT; }

{FLOAT} { yylval.number_node = new Number(DataType::newFloat(atof(yytext))); return NUMBER; }
{INTEGER} { yylval.number_node = new Number(DataType::newInteger(atoi(yytext))); return NUMBER; }

[-+()~*&=\;\#\[\]\<\>\n] {return *yytext;}

[ \t] ;

. { yyerror("invalid character"); }

%%
