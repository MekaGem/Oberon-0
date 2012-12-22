#pragma once

#include <map>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include "data_type.hpp"

// extern std::map<std::string, DataType> id;
typedef std::map<std::string, DataType> ArgumentList;

// #define PATH_LOGGING
// #define PRINT_ASSIGNMENT

class Node 
{
public:
    virtual void print() = 0;
    virtual DataType run(ArgumentList* arguments = NULL) = 0;
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
class IdentList;
class ConstDeclarations;
class Declarations;
class FPSection;
class FormalParameters;
class ProcedureHead;
class ProcedureBody;
class Procedure;
class ProcedureList;

extern std::map<std::string, Procedure*> procedures;

class Number : public Node
{
public:
    Number(DataType data);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    DataType data;
};

class Ident : public Node 
{
public:
    Ident(const char *name);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    std::string name;
};

class Selector : public Node
{
public:
    Selector(Expression* exp = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Expression* exp;
};

class Factor : public Node
{
public:
    Factor(int type, Node *node, Selector *selector = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

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
    virtual DataType run(ArgumentList* arguments = NULL);

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
    virtual DataType run(ArgumentList* arguments = NULL);

    int type;
    Term* term;
    SimpleExpression* simpleExpr;
    static const int SIMPLE_EXPRESSION_TERM = 1;
    static const int SIMPLE_EXPRESSION_LPLUS = 2;
    static const int SIMPLE_EXPRESSION_LMINUS = 3;
    static const int SIMPLE_EXPRESSION_PLUS = 4;
    static const int SIMPLE_EXPRESSION_MINUS = 5;
    static const int SIMPLE_EXPRESSION_OR = 6;
};

class Expression : public Node
{
public:
    Expression(int type, SimpleExpression* expr1, SimpleExpression* expr2 = NULL);
    Expression(Ident* ident);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    int type;
    SimpleExpression* expr1;
    SimpleExpression* expr2;
    Ident* ident; // does not work
    static const int EXPRESSION_SIMPLE = 1;
    static const int EXPRESSION_EQ = 2;
    static const int EXPRESSION_NOTEQ = 3;
    static const int EXPRESSION_LS = 4;
    static const int EXPRESSION_LSEQ = 5;
    static const int EXPRESSION_GR = 6;
    static const int EXPRESSION_GREQ = 7;
    static const int EXPRESSION_IDENT = 8; // does not work
};

class Assignment : public Node
{
public:
    Assignment(Ident* ident, Selector* selector, Expression* expr);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Ident* ident;
    Selector* selector;
    Expression* expr;
};

class Statement : public Node
{
public:
    Statement();
    static Statement* newAssignmentStatement(Assignment* assignment);
    static Statement* newProcedureCallStatement(ProcedureCall* procedureCall);
    static Statement* newIFStatement(IFStatement* ifstatement);
    static Statement* newWhileStatement(WhileStatement* whileStatement);
    static Statement* newWriteStatement(Expression* expression);
    static Statement* newWriteLnStatement();
    static Statement* newReadStatement(Factor* factor);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    int type;
    union
    {
        Assignment* assignment;
        IFStatement* ifstatement;
        WhileStatement* whileStatement;

        Expression* writeExpression;
        Factor* readFactor;
        ProcedureCall* procedureCall;
    } statement;
    static const int STATEMENT_ASSIGN = 1;
    static const int STATEMENT_CALL = 2;
    static const int STATEMENT_IF = 3;
    static const int STATEMENT_WHILE = 4;
    static const int STATEMENT_WRITE = 5;
    static const int STATEMENT_WRITELN = 6;
    static const int STATEMENT_READ = 7;
};

class StatementSequence : public Node
{
public:
    StatementSequence(Statement* statement, StatementSequence* statementSequence = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Statement* statement;
    StatementSequence* statementSequence;
};

class ActualParameters : public Node
{
public:
    ActualParameters(Expression* expression, ActualParameters* actualParameters = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Expression* expression;
    ActualParameters* actualParameters;
    std::vector<DataType> params;
    std::vector<std::string> identName;
    std::vector<bool> isIdent;
};

class ProcedureCall : public Node
{
public:
    ProcedureCall(Ident* ident, ActualParameters* actualParameters = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Ident* ident;
    ActualParameters* actualParameters;
};

class IFBody : public Node
{
public:
    IFBody(Expression* _elseif, StatementSequence* _then, IFBody* body = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Expression* _elseif;
    StatementSequence* _then;
    IFBody* body;
};

class IFStatement : public Node
{
public:
    IFStatement(Expression* _if, StatementSequence* _then, IFBody* body, StatementSequence* _else = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Expression* _if;
    StatementSequence* _then;
    IFBody* body;
    StatementSequence* _else;
};

class WhileStatement : public Node
{
public:
    WhileStatement(Expression* _while, StatementSequence* _do);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Expression* _while;
    StatementSequence* _do;
};

class IdentList : public Node
{
public:
    IdentList(Ident* ident, IdentList* identList = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Ident* ident;
    IdentList* identList;

    std::vector<std::string> name;
};

class ConstDeclarations : public Node
{
public:
    ConstDeclarations(Ident* ident, Expression* expression, ConstDeclarations* constDec = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Ident* ident;
    Expression* expression;
    ConstDeclarations* constDec;

    std::vector<std::string> name;
    std::vector<DataType> constValue;
};

class Type : public Node
{
public:
    Type(int type, Expression* expression = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    int type;
    Expression* expression;
};

class VarDeclarations : public Node
{
public:
    VarDeclarations(IdentList* identList, Type* type, VarDeclarations* varDec = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    IdentList* identList;
    Type* type;
    VarDeclarations* varDec;

    std::vector<std::string> name;
    std::vector<DataType> initValue;
};

class Declarations : public Node
{
public:
    Declarations(ConstDeclarations* constDec, VarDeclarations* varDec);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    ConstDeclarations* constDec;
    VarDeclarations* varDec;

    std::vector<std::string> name;
    std::vector<DataType> initValue;
};

class FPSection : public Node
{
public:
    FPSection(bool IsVar, IdentList* identList, Type* type, FPSection* section = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    bool IsVar;
    IdentList* identList;
    Type* type;
    FPSection* section;

    std::vector<std::string> params;
    std::vector<Type*> types;
    std::vector<bool> isVar;
};

class FormalParameters : public Node
{
public:
    FormalParameters(FPSection* section = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    FPSection* section;

    std::vector<std::string> params;
    std::vector<Type*> types;
    std::vector<bool> isVar;
};

class ProcedureHead : public Node
{
public:
    ProcedureHead(Ident* name, FormalParameters* formalParams = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Ident* name;
    FormalParameters* formalParams;

    std::vector<std::string> params;
    std::vector<Type*> types;
    std::vector<bool> isVar;
};

class ProcedureBody : public Node
{
public:
    ProcedureBody(Declarations* dec, StatementSequence* statementSequence = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Declarations* dec;
    StatementSequence* statementSequence;
};

class Procedure : public Node
{
public:
    Procedure(ProcedureHead* head, ProcedureBody* body, Ident* name);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);
    DataType call(ArgumentList* arguments, ActualParameters* actualParameters);

    ProcedureHead* head;
    ProcedureBody* body;
    Ident* name;

    Declarations* dec; // declarations from body

    std::vector<std::string> params;
    std::vector<Type*> types;
    std::vector<bool> isVar;
};

class ProcedureList : public Node
{
public:
    ProcedureList(Procedure* procedure, ProcedureList* procedureList = NULL);
    virtual void print();
    virtual DataType run(ArgumentList* arguments = NULL);

    Procedure* procedure;
    ProcedureList* procedureList;
};