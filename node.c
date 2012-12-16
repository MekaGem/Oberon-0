#include "node.h"

std::string typeName[4] = {"NO_TYPE", "INTEGER", "BOOLEAN", "FLOAT"};
std::map<std::string, DataType*> id;


/** Number **/

Number::Number(DataType data) : data(data) { }

void Number::print() 
{
    data.print();
}

DataType Number::run()
{
    return data;
}


/** Ident **/

Ident::Ident(const char *name) : name(name) { }

void Ident::print() 
{
    std::cout << name;
}

DataType Ident::run()
{
    return *id[name];
}


/** Selector **/

Selector::Selector(Expression* exp) : exp(exp) {}

void Selector::print()
{
    if (exp != NULL)
    {
        std::cout << "[";
        exp->print();
        std::cout << "]";
    }
}

DataType Selector::run()
{
    if (exp == NULL)
    {
        return DataType();
    }
    else
    {
        return exp->run();
    }
}


/** Factor **/

Factor::Factor(int type, Node *node, Selector *selector) : type(type), selector(selector)
{
    if (type == FACTOR_IDENT)
    {
        factor.ident = dynamic_cast<Ident*>(node);
    }
    else if (type == FACTOR_NUMBER)
    {
        factor.number = dynamic_cast<Number*>(node);
    }
    else if (type == FACTOR_EXPR)
    {
        factor.expr = dynamic_cast<Expression*>(node);
    }
    else if (type == FACTOR_NOT)
    {
        factor.factor = dynamic_cast<Factor*>(node);
    }
}

void Factor::print()
{
    if (type == FACTOR_IDENT)
    {
        std::cout << "ident: ";
        factor.ident->print();
        std::cout << "selector: ";
        if (selector == NULL)
        {
            std::cout << "NULL";
        }
        else
        {
            selector->print();
        }
    }
    else if (type == FACTOR_NUMBER)
    {
        factor.number->print();
    }
    else if (type == FACTOR_EXPR)
    {
        factor.expr->print();
    }
    else if (type == FACTOR_NOT)
    {
        std::cout << "~";
        factor.factor->print();
    }
}

DataType Factor::run()
{
    if (type == FACTOR_IDENT)
    {
        if (selector == NULL)
        {
            return factor.ident->run();
        }   
        else
        {
            std::cerr << "Arrays are not ready now!" << std::endl;
            exit(-1);
        }
    }
    else if (type == FACTOR_NUMBER)
    {
        return factor.number->run();
    }
    else if (type == FACTOR_EXPR)
    {
        return factor.expr->run();
    }
    else if (type == FACTOR_NOT)
    {
        return ~(factor.factor->run());
    }
}


/** Term **/

Term::Term(int type, Factor* factor, Term* term) : type(type), factor(factor), term(term) {}

void Term::print()
{
    if (type == TERM_FACTOR)
    {
        factor->print();
    }
    else
    {
        term->print();
        if (type == TERM_MULT)
        {
            std::cout << " * ";
        }
        else if (type == TERM_DIV)
        {
            std::cout << " DIV ";
        }
        else if (type == TERM_MOD)
        {
            std::cout << " MOD ";
        }
        else if (type == TERM_AND)
        {
            std::cout << " AND ";
        }
        factor->print();
    }
}

DataType Term::run()
{
    if (type == TERM_FACTOR)
    {
        return factor->run();
    }
    else if (type == TERM_MULT)
    {
        return term->run() * factor->run();
    }
    else if (type == TERM_DIV)
    {
        return term->run() * factor->run();
    }
    else if (type == TERM_MOD)
    {
        return term->run() / factor->run();
    }
    else if (type == TERM_AND)
    {
        return term->run() & factor->run();
    }
}


/** SimpleExpression **/

SimpleExpression::SimpleExpression(int type, Term* term, SimpleExpression* simpleExpr)
 : type(type), term(term), simpleExpr(simpleExpr) {}

void SimpleExpression::print()
{
    if (type == SIMPLE_EXPRESSION_LPLUS)
    {
        std::cout << "+";
        term->print();
    }
    else if (type == SIMPLE_EXPRESSION_LMINUS)
    {
        std::cout << "-";
        term->print();
    }
    else
    {
        simpleExpr->print();
        if (type == SIMPLE_EXPRESSION_PLUS)
        {
            std::cout << " + ";
        }
        else if (type == SIMPLE_EXPRESSION_MINUS)
        {
            std::cout << " - ";
        }
        else if (type == SIMPLE_EXPRESSION_OR)
        {
            std::cout << " OR ";
        }
        term->print();
    }
}

DataType SimpleExpression::run()
{
    if (type == SIMPLE_EXPRESSION_LPLUS)
    {
        return term->run();
    }
    else if (type == SIMPLE_EXPRESSION_LMINUS)
    {
        return -(term->run());
    }
    else if (type == SIMPLE_EXPRESSION_PLUS)
    {
        return simpleExpr->run() + term->run();
    }
    else if (type == SIMPLE_EXPRESSION_MINUS)
    {
        return simpleExpr->run() - term->run();
    }
    else if (type == SIMPLE_EXPRESSION_OR)
    {
        return simpleExpr->run() | term->run();
    }
}


/** Expression **/

Expression::Expression(int type, SimpleExpression* expr1, SimpleExpression* expr2)
 : type(type), expr1(expr1), expr2(expr2) {}

void Expression::print()
{
    if (type == EXPRESSION_SIMPLE)
    {
        expr1->print();
    }
    else if (type == EXPRESSION_EQ)
    {
        expr1->print();
        std::cout << " = ";
        expr2->print();
    }
    else if (type == EXPRESSION_NOTEQ)
    {
        expr1->print();
        std::cout << " # ";
        expr2->print();
    }
    else if (type == EXPRESSION_LS)
    {
        expr1->print();
        std::cout << " < ";
        expr2->print();
    }
    else if (type == EXPRESSION_LSEQ)
    {
        expr1->print();
        std::cout << " <= ";
        expr2->print();
    }
    else if (type == EXPRESSION_GR)
    {
        expr1->print();
        std::cout << " > ";
        expr2->print();
    }
    else if (type == EXPRESSION_GREQ)
    {
        expr1->print();
        std::cout << " < ";
        expr2->print();
    }
}

DataType Expression::run()
{
    if (type == EXPRESSION_SIMPLE)
    {
        return expr1->run();
    }
    else if (type == EXPRESSION_EQ)
    {
        return expr1->run() == expr2->run();
    }
    else if (type == EXPRESSION_NOTEQ)
    {
        return expr1->run() != expr2->run();
    }
    else if (type == EXPRESSION_LS)
    {
        return expr1->run() < expr2->run();
    }
    else if (type == EXPRESSION_LSEQ)
    {
        return expr1->run() <= expr2->run();
    }
    else if (type == EXPRESSION_GR)
    {
        return expr1->run() > expr2->run();
    }
    else if (type == EXPRESSION_GREQ)
    {
        return expr1->run() >= expr2->run();
    }
}


/** Assignment **/

Assignment::Assignment(Ident* ident, Selector* selector, Expression* expr) {}

void Assignment::print()
{
    ident->print();
    selector->print();
    std::cout << " := ";
    expr->print();
}

DataType Assignment::run()
{
    if (selector == NULL)
    {
        *(id[ident->name]) = expr->run();
        return *(id[ident->name]);
    }
    else
    {
        std::cerr << "Arrays are not ready now!" << std::endl;
        exit(-1);
    }
}


/** Statement **/

Statement::Statement(Assignment* assignment) {}

void Statement::print()
{
    assignment->print();
}

DataType Statement::run()
{
    assignment->run();
}


/** StatementSequence **/

StatementSequence::StatementSequence(Statement* statement, StatementSequence* statementSequence)
 : statement(statement), statementSequence(statementSequence) {}

void StatementSequence::print()
{
    if (statementSequence == NULL)
    {
        statement->print();
    }
    else
    {
        statementSequence->print();
        std::cout << " ; ";
        statement->print();
    }
}

DataType StatementSequence::run()
{
    if (statementSequence == NULL)
    {
        return statement->run();
    }
    else
    {
        statementSequence->run();
        return statement->run();
   }
}