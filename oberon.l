%{
    #include <stdlib.h>
    #include "node.hpp"
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

"IF" {return _IF;}
"THEN" {return _THEN;}
"ELSEIF" {return _ELSEIF;}
"ELSE" {return _ELSE;}
"END" {return _END;}

"MODULE" {return _MODULE;}
"CONST" {return _CONST;}
"TYPE" {return _TYPE;}
"VAR" {return _VAR;}

"BEGIN" {return _BEGIN;}
"WHILE" {return _WHILE;}
"DO" {return _DO;}

"INTEGER" {return _INTEGER;}
"BOOLEAN" {return _BOOLEAN;}
"FLOAT" {return _FLOAT;}

{IDENT} { yylval.ident_node = new Ident(yytext); return IDENT; }

{FLOAT} { yylval.number_node = new Number(DataType::newFloat(atof(yytext))); return NUMBER; }
{INTEGER} { yylval.number_node = new Number(DataType::newInteger(atoi(yytext))); return NUMBER; }

[-\,\:+()~*&=\;\#\[\]\<\>] {return *yytext;}

[ \t\n] ;

. { yyerror((char*)"invalid character"); }

%%
