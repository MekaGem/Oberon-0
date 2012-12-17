%{
    #include "node.hpp"
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
    ActualParameters* actual_parameters_node;
    StatementSequence* statement_sequence_node;
    IFBody* ifbody_node;
    IFStatement* ifstatement_node;
    WhileStatement* while_statement_node;

    IdentList* ident_list_node;
    ConstDeclarations* const_declarations_node;
    Type* type_node;
    VarDeclarations* var_declarations_node;
    Declarations* declarations_node;
};

%token <number_node> NUMBER
%token <ident_node> IDENT
%token MOD
%token DIV
%token OR
%token LSEQ
%token GREQ
%token ASSIGN

%token _IF
%token _THEN
%token _ELSEIF
%token _ELSE
%token _BEGIN
%token _WHILE
%token _DO
%token _END
%token _MODULE
%token _CONST
%token _TYPE
%token _VAR

%token _INTEGER
%token _BOOLEAN
%token _FLOAT

%left ','
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

%type <node> MODULE

%type <declarations_node> DECLARATIONS
%type <ident_list_node> IDENT_LIST
%type <const_declarations_node> CONST_DECLARATIONS
%type <type_node> TYPE
%type <var_declarations_node> VAR_DECLARATIONS

%type <while_statement_node> WHILE_STATEMENT
%type <statement_sequence_node> STATEMENT_SEQUENCE
%type <ifstatement_node> IFSTATEMENT
%type <ifbody_node> IFBODY
%type <actual_parameters_node> ACTUAL_PARAMETERS
%type <statement_node> STATEMENT
%type <assign_node> ASSIGNMENT

%type <expression_node> EXPRESSION
%type <simple_expression_node> SIMPLE_EXPRESSION
%type <term_node> TERM
%type <factor_node> FACTOR
%type <selector_node> SELECTOR

%%

MODULE:
    _MODULE IDENT ';' DECLARATIONS _BEGIN STATEMENT_SEQUENCE _END 
    {ArgumentList *list = new ArgumentList; $4->run(list); $6->run(list); $$ = NULL;}
    | _MODULE IDENT ';' DECLARATIONS _END 
    {ArgumentList *list = new ArgumentList; $4->run(list); $$ = NULL;}
    ;

DECLARATIONS: 
    CONST_DECLARATIONS VAR_DECLARATIONS {$$ = new Declarations($1, $2);}
    | VAR_DECLARATIONS {$$ = new Declarations(NULL, $1);}
    | CONST_DECLARATIONS {$$ = new Declarations($1, NULL);}
    | {$$ = new Declarations(NULL, NULL);}
    ;

VAR_DECLARATIONS:
    VAR_DECLARATIONS IDENT_LIST ':' TYPE ';' {$$ = new VarDeclarations($2, $4, $1);};
    | _VAR {$$ = NULL;}
    ;

IDENT_LIST:
    IDENT_LIST ',' IDENT {$$ = new IdentList($3, $1);}
    | IDENT {$$ = new IdentList($1);}
    ;

TYPE:
    _INTEGER {$$ = new Type(INT_TYPE);}
    | _BOOLEAN {$$ = new Type(BOOL_TYPE);}
    | _FLOAT {$$ = new Type(FLOAT_TYPE);}
    ;

CONST_DECLARATIONS:
    CONST_DECLARATIONS IDENT '=' EXPRESSION ';' {$$ = new ConstDeclarations($2, $4, $1);}
    | _CONST {$$ = NULL;}
    ;

STATEMENT_SEQUENCE:
    STATEMENT {$$ = new StatementSequence($1); /*$1->run();*/}
    | STATEMENT_SEQUENCE ';' STATEMENT {$$ = new StatementSequence($3, $1); /*$3->run();*/}
    ;

STATEMENT:
    ASSIGNMENT {$$ = Statement::newAssignmentStatement($1);}
    | IFSTATEMENT {$$ = Statement::newIFStatement($1);}
    | WHILE_STATEMENT {$$ = Statement::newWhileStatement($1);}
    ;

WHILE_STATEMENT:
    _WHILE EXPRESSION _DO STATEMENT_SEQUENCE _END {$$ = new WhileStatement($2, $4);}
    ;

IFSTATEMENT:
    _IF EXPRESSION _THEN STATEMENT_SEQUENCE IFBODY _END {$$ = new IFStatement($2, $4, $5);}
    | _IF EXPRESSION _THEN STATEMENT_SEQUENCE IFBODY _ELSE STATEMENT_SEQUENCE _END {$$ = new IFStatement($2, $4, $5, $7);}
    ;

IFBODY:
    _ELSEIF EXPRESSION _THEN STATEMENT_SEQUENCE IFBODY {$$ = new IFBody($2, $4, $5);}
    | {$$ = NULL;}
    ;

ACTUAL_PARAMETERS:
    EXPRESSION {$$ = new ActualParameters($1);}
    | ACTUAL_PARAMETERS ',' EXPRESSION {$$ = new ActualParameters($3, $1);}
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
