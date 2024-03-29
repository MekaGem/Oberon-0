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
"PROCEDURE" {return _PROCEDURE;}
"CONST" {return _CONST;}
"TYPE" {return _TYPE;}
"VAR" {return _VAR;}

"ARRAY" {return _ARRAY;}
"OF" {return _OF;}

"BEGIN" {return _BEGIN;}
"WHILE" {return _WHILE;}
"DO" {return _DO;}

"Write" {return _WRITE;}
"WriteLn" {return _WRITELN;}
"Read" {return _READ;}

"INTEGER" {return _INTEGER;}
"BOOLEAN" {return _BOOLEAN;}
"FLOAT" {return _FLOAT;}
"STRING" {return _STRING;}

"true"|"True"|"TRUE" { yylval.number_node = new Number(DataType::newBoolean(true)); return NUMBER; }
"false"|"False"|"FALSE" { yylval.number_node = new Number(DataType::newBoolean(false)); return NUMBER; }

"\""([^\"])*"\"" { std::string s(yytext); s = s.substr(1, s.size() - 2); yylval.number_node = new Number(DataType::newString(s)); return NUMBER; }

{IDENT} { yylval.ident_node = new Ident(yytext); return IDENT; }

{FLOAT} { yylval.number_node = new Number(DataType::newFloat(atof(yytext))); return NUMBER; }
{INTEGER} { yylval.number_node = new Number(DataType::newInteger(atoi(yytext))); return NUMBER; }

[-\,\:+()~*&=\;\#\[\]\<\>] {return *yytext;}

[ \t\n] ;

. { yyerror((char*)"invalid character"); }

%%
