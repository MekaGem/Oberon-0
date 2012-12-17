#include "node.h"

std::string typeName[4] = {"NO_TYPE", "INTEGER", "BOOLEAN", "FLOAT"};
std::map<std::string, DataType> id;


/** Number **/

Number::Number(DataType data) : data(data) { }

void Number::print() 
{
    data.print();
}

DataType Number::run()
{
#ifdef PATH_LOGGING
    std::cerr << "Number::run" << std::endl;
#endif
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
#ifdef PATH_LOGGING
    std::cerr << "Ident::run" << std::endl;
#endif
    return id[name];
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
#ifdef PATH_LOGGING
    std::cerr << "Selector::run" << std::endl;
#endif
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
#ifdef PATH_LOGGING
    std::cerr << "Factor::run" << std::endl;
#endif
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
        std::cout << "(";
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
        std::cout << ")";
    }
}

DataType Term::run()
{
#ifdef PATH_LOGGING
    std::cerr << "Turn::run" << std::endl;
#endif
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
        std::cout << "(";
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
        std::cout << ")";
    }
}

DataType SimpleExpression::run()
{
#ifdef PATH_LOGGING
    std::cerr << "SimpleExpression::run" << std::endl;
#endif
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
        std::cout << "(";
        expr1->print();
        std::cout << " = ";
        expr2->print();
        std::cout << ")";
    }
    else if (type == EXPRESSION_NOTEQ)
    {
        std::cout << "(";
        expr1->print();
        std::cout << " # ";
        expr2->print();
        std::cout << ")";
    }
    else if (type == EXPRESSION_LS)
    {
        std::cout << "(";
        expr1->print();
        std::cout << " < ";
        expr2->print();
        std::cout << ")";
    }
    else if (type == EXPRESSION_LSEQ)
    {
        std::cout << "(";
        expr1->print();
        std::cout << " <= ";
        expr2->print();
        std::cout << ")";
    }
    else if (type == EXPRESSION_GR)
    {
        std::cout << "(";
        expr1->print();
        std::cout << " > ";
        expr2->print();
        std::cout << ")";
    }
    else if (type == EXPRESSION_GREQ)
    {
        std::cout << "(";
        expr1->print();
        std::cout << " < ";
        expr2->print();
        std::cout << ")";
    }
}

DataType Expression::run()
{
#ifdef PATH_LOGGING
    std::cerr << "Expression::run" << std::endl;
#endif
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

Assignment::Assignment(Ident* ident, Selector* selector, Expression* expr)
 : ident(ident), selector(selector), expr(expr) {}

void Assignment::print()
{
    ident->print();
    if (selector != NULL)
    {
       selector->print();
    }
    std::cout << " := ";
    expr->print();
}

DataType Assignment::run()
{
#ifdef PATH_LOGGING
    std::cerr << "Assignment::run" << std::endl;
#endif
    if (selector == NULL)
    {
        std::cerr << "Variable: " << ident->name << " is set with: ";
        (expr->run()).print();
        std::cerr << std::endl;

        id[ident->name] = expr->run();
        return id[ident->name];
    }
    else
    {
        std::cerr << "Arrays are not ready now!" << std::endl;
        exit(-1);
    }
}


/** Statement **/

Statement::Statement(Assignment* assignment) : assignment(assignment) {}

void Statement::print()
{
    assignment->print();
}

DataType Statement::run()
{
#ifdef PATH_LOGGING
    std::cerr << "Statement::run" << std::endl;
#endif
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
        std::cout << ";" << std::endl;
        statement->print();
    }
}

DataType StatementSequence::run()
{
#ifdef PATH_LOGGING
    std::cerr << "StatementSequence::run" << std::endl;
#endif
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