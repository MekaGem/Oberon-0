#include "node.h"

std::string typeName[4] = {"NO_TYPE", "INTEGER", "BOOLEAN", "FLOAT"};
std::map<std::string, DataType> id;


/** Number **/

Number::Number(DataType data) : data(data) { }

void Number::print() 
{
    data.print();
}

DataType Number::run(ArgumentList* arguments)
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

DataType Ident::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Ident::run" << std::endl;
#endif
    if (arguments == NULL || arguments->find(name) == arguments->end())
    {
        if (id.find(name) == id.end())
        {
            std::cerr << "There is no any ident with name \"" << name << "\" in this scope" << std::endl;
            exit(-1);
        }
        else
        {
            return id[name];
        }
    }
    else
    {
        return arguments->at(name);
    }
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

DataType Selector::run(ArgumentList* arguments)
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
        return exp->run(arguments);
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

DataType Factor::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Factor::run" << std::endl;
#endif
    if (type == FACTOR_IDENT)
    {
        if (selector == NULL)
        {
            return factor.ident->run(arguments);
        }   
        else
        {
            std::cerr << "Arrays are not ready now!" << std::endl;
            exit(-1);
        }
    }
    else if (type == FACTOR_NUMBER)
    {
        return factor.number->run(arguments);
    }
    else if (type == FACTOR_EXPR)
    {
        return factor.expr->run(arguments);
    }
    else if (type == FACTOR_NOT)
    {
        return ~(factor.factor->run(arguments));
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

DataType Term::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Turn::run" << std::endl;
#endif
    if (type == TERM_FACTOR)
    {
        return factor->run(arguments);
    }
    else if (type == TERM_MULT)
    {
        return term->run(arguments) * factor->run(arguments);
    }
    else if (type == TERM_DIV)
    {
        return term->run(arguments) * factor->run(arguments);
    }
    else if (type == TERM_MOD)
    {
        return term->run(arguments) / factor->run(arguments);
    }
    else if (type == TERM_AND)
    {
        return term->run(arguments) & factor->run(arguments);
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

DataType SimpleExpression::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "SimpleExpression::run" << std::endl;
#endif
    if (type == SIMPLE_EXPRESSION_LPLUS)
    {
        return term->run(arguments);
    }
    else if (type == SIMPLE_EXPRESSION_LMINUS)
    {
        return -(term->run(arguments));
    }
    else if (type == SIMPLE_EXPRESSION_PLUS)
    {
        return simpleExpr->run(arguments) + term->run(arguments);
    }
    else if (type == SIMPLE_EXPRESSION_MINUS)
    {
        return simpleExpr->run(arguments) - term->run(arguments);
    }
    else if (type == SIMPLE_EXPRESSION_OR)
    {
        return simpleExpr->run(arguments) | term->run(arguments);
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

DataType Expression::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Expression::run" << std::endl;
#endif
    if (type == EXPRESSION_SIMPLE)
    {
        return expr1->run(arguments);
    }
    else if (type == EXPRESSION_EQ)
    {
        return expr1->run(arguments) == expr2->run(arguments);
    }
    else if (type == EXPRESSION_NOTEQ)
    {
        return expr1->run(arguments) != expr2->run(arguments);
    }
    else if (type == EXPRESSION_LS)
    {
        return expr1->run(arguments) < expr2->run(arguments);
    }
    else if (type == EXPRESSION_LSEQ)
    {
        return expr1->run(arguments) <= expr2->run(arguments);
    }
    else if (type == EXPRESSION_GR)
    {
        return expr1->run(arguments) > expr2->run(arguments);
    }
    else if (type == EXPRESSION_GREQ)
    {
        return expr1->run(arguments) >= expr2->run(arguments);
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

DataType Assignment::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Assignment::run" << std::endl;
#endif
    if (selector == NULL)
    {
        if (arguments == NULL || arguments->find(ident->name) == arguments->end())
        {
            if (id.find(ident->name) == id.end())
            {
                std::cerr << "There is no any ident with name \"" << ident->name << "\" in this scope" << std::endl;
                exit(-1);
            }
            else
            {
                #ifdef PRINT_ASSIGNMENT

                id[ident->name] = expr->run(arguments);
                std::cerr << "Global variable: " << ident->name << " is set with: ";
                id[ident->name].print();
                std::cerr << std::endl;

                #endif
                return id[ident->name];
            }
        }
        else
        {
            #ifdef PRINT_ASSIGNMENT

            (*arguments)[ident->name] = expr->run(arguments);
            std::cerr << "Local variable: " << ident->name << " is set with: ";
            arguments->at(ident->name).print();
            std::cerr << std::endl;

            #endif  
            return arguments->at(ident->name);
        }
    }
    else
    {
        std::cerr << "Arrays are not ready now!" << std::endl;
        exit(-1);
    }
}


/** Statement **/

Statement::Statement() : type(0) {}

Statement* Statement::newAssignmentStatement(Assignment* assignment)
{   
    Statement* statement = new Statement();
    statement->type = STATEMENT_ASSIGN;
    statement->statement.assignment = assignment;
    return statement;
}

Statement* Statement::newIFStatement(IFStatement* ifstatement)
{
    Statement* statement = new Statement();
    statement->type = STATEMENT_IF;
    statement->statement.ifstatement = ifstatement;
    return statement;
}

Statement* Statement::newWhileStatement(WhileStatement* whileStatement)
{
    Statement* statement = new Statement();
    statement->type = STATEMENT_WHILE;
    statement->statement.whileStatement = whileStatement;
    return statement;   
}

void Statement::print()
{
    if (type == STATEMENT_ASSIGN)
    {
        statement.assignment->print();
    }
    else if (type == STATEMENT_IF)
    {
        statement.ifstatement->print();
    }
    else if (type == STATEMENT_WHILE)
    {
        statement.whileStatement->print();   
    }
}

DataType Statement::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Statement::run" << std::endl;
#endif
    if (type == STATEMENT_ASSIGN)
    {
        return statement.assignment->run(arguments);
    }
    else if (type == STATEMENT_IF)
    {
        return statement.ifstatement->run(arguments);
    }
    else if (type == STATEMENT_WHILE)
    {
        statement.whileStatement->run(arguments);
    }
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

DataType StatementSequence::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "StatementSequence::run" << std::endl;
#endif
    if (statementSequence == NULL)
    {
        return statement->run(arguments);
    }
    else
    {
        statementSequence->run(arguments);
        return statement->run(arguments);
   }
}


/** ActualParameters **/

ActualParameters::ActualParameters(Expression* expression, ActualParameters* actualParameters)
 : expression(expression), actualParameters(actualParameters) {}

void ActualParameters::print()
{
    if (actualParameters == NULL)
    {
        expression->print();
    }
    else
    {
        actualParameters->print();
        std::cout << ", ";
        expression->print();
    }
}

DataType ActualParameters::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "ActualParameters::run" << std::endl;
#endif
    params.clear();
    if (actualParameters == NULL)
    {
        params.push_back(expression->run(arguments));
    }
    else
    {
        actualParameters->run(arguments);
        params = actualParameters->params;
        params.push_back(expression->run(arguments));
    }

    return DataType();
}


/** ProcedureCall **/

ProcedureCall::ProcedureCall(Ident* ident, ActualParameters* actualParameters)
 : ident(ident), actualParameters(actualParameters) {}

void ProcedureCall::print()
{
    ident->print();
    if (actualParameters != NULL)
    {
        std::cout << "(";
        actualParameters->print();
        std::cout << ")";
    }
}

DataType ProcedureCall::run(ArgumentList* arguments)
{   
#ifdef PATH_LOGGING
    std::cerr << "ProcedureCall::run" << std::endl;
#endif
   /* if (ident->name == "Write")
    {
        if (actualParameters == NULL)
        {
            std::cerr << "Write procedure needs at least one parameter" << std::endl;
            exit(-1);
        }

        actualParameters->run(arguments);
        std::vector<DataType> params = actualParameters->params;

        for (size_t index = 0; index < params.size(); ++index)
        {
            params[i].print();
            std::cout << " ";
        }
    }
    else if (ident->name == "Ololo")
    {
        if (actualParameters == NULL)
        {
            std::cerr << "Write procedure needs at least one parameter" << std::endl;
            exit(-1);
        }

        actualParameters->run(arguments);
        std::vector<DataType> params = actualParameters->params;

        for (size_t index = 0; index < params.size(); ++index)
        {
            params[i].print();
            std::cout << " ";
        }
    }*/

    return DataType();
}


/** IFBody **/

IFBody::IFBody(Expression* _elseif, StatementSequence* _then, IFBody* body)
 : _elseif(_elseif), _then(_then), body(body) {}

void IFBody::print()
{
    std::cout << "ELSEIF ";
    _elseif->print();
    std::cout << " THEN ";
    _then->print();
    if (body != NULL)
    {
        std::cout << " ";
        body->print();
    }
}

DataType IFBody::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "IFBody::run" << std::endl;
#endif
    DataType res = _elseif->run(arguments);
    if (res.type != BOOL_TYPE)
    {
        std::cout << "ELSEIF statement is not a boolean" << std::endl;
        exit(-1);
    }

    if (res.data.boolValue)
    {
        _then->run(arguments);
        return res;
    }
    else
    {
        if (body == NULL)
        {
            return ~res;
        }
        else
        {
            return body->run(arguments);
        }
    }
}


/** IFStatement **/

IFStatement::IFStatement(Expression* _if, StatementSequence* _then, IFBody* body, StatementSequence* _else)
 : _if(_if), _then(_then), body(body), _else(_else) {}

void IFStatement::print()
{
    std::cout << "IF ";
    _if->print();
    std::cout << " THEN ";
    _then->print();

    if (body != NULL)
    {
        std::cout << " ";
        body->print();
    }

    if (_else != NULL)
    {
        std::cout << " ";
        _else->print();
    }
}

DataType IFStatement::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "IFStatement::run" << std::endl;
#endif
    DataType res = _if->run(arguments);
    if (res.type != BOOL_TYPE)
    {
        std::cout << "IF statement is not a boolean" << std::endl;
        exit(-1);
    }

    if (res.data.boolValue)
    {
        _then->run(arguments);
        return res;
    }

    if (body != NULL)
    {
        res = body->run(arguments);
        if (res.data.boolValue)
        {
            return res;
        }
    }

    if (_else != NULL)
    {
        _else->run(arguments);
    }

    return DataType();
}


/** WhileStatement **/

WhileStatement::WhileStatement(Expression* _while, StatementSequence* _do)
 : _while(_while), _do(_do) {}

void WhileStatement::print()
{
    std::cout << "WHILE ";
    _while->print();
    std::cout << " DO ";
    _do->print();
    std::cout << " END";
}

DataType WhileStatement::run(ArgumentList* arguments)
{
    DataType res = _while->run(arguments);
    if (res.type != BOOL_TYPE)
    {
        std::cout << "WHILE statement is not a boolean" << std::endl;
        exit(-1);
    }

    while (res.data.boolValue)
    {
        _do->run(arguments);
        res = _while->run(arguments);
    }

    return DataType();
}