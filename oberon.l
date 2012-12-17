%{
    #include <stdlib.h>
    #include "node.h"
    #include "oberon.tab.h"

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

"IF" {return IF;}
"THEN" {return THEN;}
"ELSEIF" {return ELSEIF;}
"ELSE" {return ELSE;}
"BEGIN" {return _BEGIN;}
"END" {return END;}

{IDENT} { yylval.ident_node = new Ident(yytext); return IDENT; }

{FLOAT} { yylval.number_node = new Number(DataType::newFloat(atof(yytext))); return NUMBER; }
{INTEGER} { yylval.number_node = new Number(DataType::newInteger(atoi(yytext))); return NUMBER; }

[-\,+()~*&=\;\#\[\]\<\>] {return *yytext;}

[ \t\n] ;

. { yyerror("invalid character"); }

%%
