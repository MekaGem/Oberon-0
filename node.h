#pragma once

#include <map>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include "data_type.h"

extern std::map<std::string, DataType> id;

// #define PATH_LOGGING

class Node 
{
public:
    virtual void print() = 0;
    virtual DataType run() = 0;
};

class Number;
class Ident;
class Selector;
class Factor;
class Term;
class SimpleExpression;
class Expression;
class Assignment;
class Statement;
class StatementSequence;
class ActualParameters;
class ProcedureCall;
class IFBody;
class IFStatement;
class WhileStatement;

class Number : public Node
{
public:
    Number(DataType data);
    virtual void print();
    virtual DataType run();

    DataType data;
};

class Ident : public Node 
{
public:
    Ident(const char *name);
    virtual void print();
    virtual DataType run();

    std::string name;
};

class Selector : public Node
{
public:
    Selector(Expression* exp = NULL);
    virtual void print();
    virtual DataType run();

    Expression* exp;
};

class Factor : public Node
{
public:
    Factor(int type, Node *node, Selector *selector = NULL);
    virtual void print();
    virtual DataType run();

    union
    {
        Ident* ident;
        Number* number;
        Expression* expr;
        Factor* factor;
    } factor;
    Selector* selector;
    int type;
    static const int FACTOR_IDENT = 1;
    static const int FACTOR_NUMBER = 2;
    static const int FACTOR_EXPR = 3;
    static const int FACTOR_NOT = 4;
};

class Term : public Node
{
public:
    Term(int type, Factor* factor, Term* term = NULL);
    virtual void print();
    virtual DataType run();

    int type;
    Factor* factor;
    Term* term;
    static const int TERM_FACTOR = 1;
    static const int TERM_MULT = 2;
    static const int TERM_DIV = 3;
    static const int TERM_MOD = 4;
    static const int TERM_AND = 5;
};

class SimpleExpression : public Node
{
public:
    SimpleExpression(int type, Term* term, SimpleExpression* simpleExpr = NULL);
    virtual void print();
    virtual DataType run();

    int type;
    Term* term;
    SimpleExpression* simpleExpr;
    static const int SIMPLE_EXPRESSION_LPLUS = 1;
    static const int SIMPLE_EXPRESSION_LMINUS = 2;
    static const int SIMPLE_EXPRESSION_PLUS = 3;
    static const int SIMPLE_EXPRESSION_MINUS = 4;
    static const int SIMPLE_EXPRESSION_OR = 5;
};

class Expression : public Node
{
public:
    Expression(int type, SimpleExpression* expr1, SimpleExpression* expr2 = NULL);
    virtual void print();
    virtual DataType run();

    int type;
    SimpleExpression* expr1;
    SimpleExpression* expr2;
    static const int EXPRESSION_SIMPLE = 1;
    static const int EXPRESSION_EQ = 2;
    static const int EXPRESSION_NOTEQ = 3;
    static const int EXPRESSION_LS = 4;
    static const int EXPRESSION_LSEQ = 5;
    static const int EXPRESSION_GR = 6;
    static const int EXPRESSION_GREQ = 7;
};

class Assignment : public Node
{
public:
    Assignment(Ident* ident, Selector* selector, Expression* expr);
    virtual void print();
    virtual DataType run();

    Ident* ident;
    Selector* selector;
    Expression* expr;
};

class Statement : public Node
{
public:
    Statement();
    static Statement* newAssignmentStatement(Assignment* assignment);
    static Statement* newIFStatement(IFStatement* ifstatement);
    static Statement* newWhileStatement(WhileStatement* whileStatement);
    virtual void print();
    virtual DataType run();

    int type;
    union
    {
        Assignment* assignment;
        IFStatement* ifstatement;
        WhileStatement* whileStatement;
    } statement;
    static const int STATEMENT_ASSIGN = 1;
    static const int STATEMENT_CALL = 2;
    static const int STATEMENT_IF = 3;
    static const int STATEMENT_WHILE = 4;
};

class StatementSequence : public Node
{
public:
    StatementSequence(Statement* statement, StatementSequence* statementSequence = NULL);
    virtual void print();
    virtual DataType run();

    Statement* statement;
    StatementSequence* statementSequence;
};

class ActualParameters : public Node
{
public:
    ActualParameters(Expression* expression, ActualParameters* actualParameters = NULL);
    virtual void print();
    virtual DataType run();

    Expression* expression;
    ActualParameters* actualParameters;
    std::vector<DataType> params;
};

class ProcedureCall : public Node
{
public:
    ProcedureCall(Ident* ident, ActualParameters* actualParameters = NULL);
    virtual void print();
    virtual DataType run();

    Ident* ident;
    ActualParameters* actualParameters;
};

class IFBody : public Node
{
public:
    IFBody(Expression* _ELSEIF, StatementSequence* _THEN, IFBody* _BODY = NULL);
    virtual void print();
    virtual DataType run();

    Expression* _ELSEIF;
    StatementSequence* _THEN;
    IFBody* _BODY;
};

class IFStatement : public Node
{
public:
    IFStatement(Expression* _IF, StatementSequence* _THEN, IFBody* _BODY, StatementSequence* _ELSE = NULL);
    virtual void print();
    virtual DataType run();

    Expression* _IF;
    StatementSequence* _THEN;
    IFBody* _BODY;
    StatementSequence* _ELSE;
};

class WhileStatement : public Node
{
public:
    WhileStatement(Expression* _while, StatementSequence* _do);
    virtual void print();
    virtual DataType run();

    Expression* _while;
    StatementSequence* _do;
};