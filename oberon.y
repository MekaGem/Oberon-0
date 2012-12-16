%{
    #include "node.h"
    #include <iostream>
    #include <map>

    void yyerror(char *);
    int yylex(void);
%}

%union
{
    Node* node;
    Ident* ident_node;
    Number* number_node;
    Selector* selector_node;
    Factor* factor_node;
    Term* term_node;
    SimpleExpression* simple_expression_node;
    Expression* expression_node;
    Assignment* assign_node;
    Statement* statement_node;
    StatementSequence* statement_sequence_node;
};

%token <number_node> NUMBER
%token <ident_node> IDENT
%token MOD
%token DIV
%token OR
%token LSEQ
%token GREQ
%token ASSIGN

%left '+'
%left '-'
%left '*'
%left DIV
%left '&'
%left OR
%left '~'
%left MOD
%left ';'
%left '<'
%left '>'
%left LSEQ
%left GREQ
%left '#'
%left '='
%left ASSIGN

%type <statement_sequence_node> STATEMENT_SEQUENCE
%type <statement_node> STATEMENT
%type <assign_node> ASSIGNMENT
%type <expression_node> EXPRESSION
%type <simple_expression_node> SIMPLE_EXPRESSION
%type <term_node> TERM
%type <factor_node> FACTOR
%type <selector_node> SELECTOR

%%

STATEMENT_SEQUENCE:
    STATEMENT {$$ = new StatementSequence($1);}
    | STATEMENT_SEQUENCE ';' STATEMENT {$$ = new StatementSequence($3, $1);}
    ;

STATEMENT:
    ASSIGNMENT {$$ = new Statement($1);}
    ;

ASSIGNMENT:
    IDENT SELECTOR ASSIGN EXPRESSION {$$ = new Assignment($1, $2, $4);}
    ;

EXPRESSION:
    SIMPLE_EXPRESSION '=' SIMPLE_EXPRESSION {$$ = new Expression(Expression::EXPRESSION_EQ, $1, $3);}
    | SIMPLE_EXPRESSION '#' SIMPLE_EXPRESSION {$$ = new Expression(Expression::EXPRESSION_NOTEQ, $1, $3);}
    | SIMPLE_EXPRESSION LSEQ SIMPLE_EXPRESSION {$$ = new Expression(Expression::EXPRESSION_LSEQ, $1, $3);}
    | SIMPLE_EXPRESSION '<' SIMPLE_EXPRESSION {$$ = new Expression(Expression::EXPRESSION_LS, $1, $3);}
    | SIMPLE_EXPRESSION GREQ SIMPLE_EXPRESSION {$$ = new Expression(Expression::EXPRESSION_GREQ, $1, $3);}
    | SIMPLE_EXPRESSION '>' SIMPLE_EXPRESSION {$$ = new Expression(Expression::EXPRESSION_GR, $1, $3);}
    | SIMPLE_EXPRESSION {$$ = new Expression(Expression::EXPRESSION_SIMPLE, $1);}
    ;

SIMPLE_EXPRESSION:
    TERM {$$ = new SimpleExpression(SimpleExpression::SIMPLE_EXPRESSION_LPLUS, $1);}
    | '+' TERM {$$ = new SimpleExpression(SimpleExpression::SIMPLE_EXPRESSION_LPLUS, $2);}
    | '-' TERM {$$ = new SimpleExpression(SimpleExpression::SIMPLE_EXPRESSION_LMINUS, $2);}
    | SIMPLE_EXPRESSION '+' TERM {$$ = new SimpleExpression(SimpleExpression::SIMPLE_EXPRESSION_PLUS, $3, $1);}
    | SIMPLE_EXPRESSION '-' TERM {$$ = new SimpleExpression(SimpleExpression::SIMPLE_EXPRESSION_MINUS, $3, $1);}
    | SIMPLE_EXPRESSION OR TERM {$$ = new SimpleExpression(SimpleExpression::SIMPLE_EXPRESSION_OR, $3, $1);}
    ;

TERM: 
    FACTOR {$$ = new Term(Term::TERM_FACTOR, $1);}
    | TERM '*' FACTOR {$$ = new Term(Term::TERM_MULT, $3, $1);}
    | TERM DIV FACTOR {$$ = new Term(Term::TERM_DIV, $3, $1);}
    | TERM MOD FACTOR {$$ = new Term(Term::TERM_MOD, $3, $1);}
    | TERM '&' FACTOR {$$ = new Term(Term::TERM_AND, $3, $1);}
    ;

FACTOR:
    IDENT SELECTOR {$$ = new Factor(Factor::FACTOR_IDENT, $1, $2);}
    | NUMBER {$$ = new Factor(Factor::FACTOR_NUMBER, $1);}
    | '(' EXPRESSION ')' {$$ = new Factor(Factor::FACTOR_EXPR, $2);}
    | '~' FACTOR {$$ = new Factor(Factor::FACTOR_NOT, $2);}
    ;

SELECTOR:
    '[' EXPRESSION ']' {$$ = new Selector($2)}
    | {$$ = NULL;}
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    freopen("input.txt", "r", stdin);
    // freopen("log.txt", "w", stderr);
    yyparse();
    return 0;
}
