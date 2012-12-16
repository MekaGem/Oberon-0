#pragma once

#include <cstdio>
#include <string>
#include <iostream>
#include "data_type.h"

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

class Statement : public Node
{
public:

};

class StatementSequence : public Node
{
public:

};