#include "node.hpp"

std::map<std::string, Procedure*> procedures;

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
        std::cerr << "There is no any ident with name \"" << name << "\" in this scope" << std::endl;
        exit(-1);
    }
    else
    {
        /*
        std::cerr << arguments << "->at(" << name << ")" << std::endl;
        DataType res = arguments->at(name);
        std::cerr << "Data has been got: " << res.data.intValue;
        */
        //return res;
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
            return (factor.ident->run(arguments))[selector->run(arguments)];
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
            std::cout << " & ";
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
        return term->run(arguments) / factor->run(arguments);
    }
    else if (type == TERM_MOD)
    {
        return term->run(arguments) % factor->run(arguments);
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
    if (type == SIMPLE_EXPRESSION_TERM)
    {
        term->print();
    }
    else if (type == SIMPLE_EXPRESSION_LPLUS)
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
    if (type == SIMPLE_EXPRESSION_TERM)
    {
        return term->run(arguments);
    }
    else if (type == SIMPLE_EXPRESSION_LPLUS)
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
 : type(type), expr1(expr1), expr2(expr2), ident(NULL) {}

Expression::Expression(Ident* ident)
 : type(EXPRESSION_IDENT), expr1(NULL), expr2(NULL), ident(ident) {}

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
    else if (type == EXPRESSION_IDENT)
    {
        ident->print();
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
    else if (type == EXPRESSION_IDENT)
    {
        return ident->run(arguments);
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

    if (arguments == NULL || arguments->find(ident->name) == arguments->end())
    {
        std::cerr << "There is no any ident with name \"" << ident->name << "\" in this scope" << std::endl;
        exit(-1);
    }

    if (selector == NULL)
    {
        if ((*arguments)[ident->name].isArray)
        {
            std::cerr << "Ident with name \"" << ident->name 
                << "\" is an ARRAY and can't be assigned" << std::endl;
            exit(-1);
        }

        if ((*arguments)[ident->name].isConst)
        {
            std::cerr << "Ident with name \"" << ident->name 
                << "\" is a const and can't be assigned" << std::endl;
            exit(-1);
        }

        DataType data = expr->run(arguments);

        if ((*arguments)[ident->name].type != data.type)
        {
            std::cerr << typeName[(*arguments)[ident->name].type] << " can't be assigned with " 
                << typeName[data.type] << std::endl;
            exit(-1);
        }

        (*arguments)[ident->name] = data;

        #ifdef PRINT_ASSIGNMENT

        std::cerr << "Variable: " << ident->name << " is set with: ";
        data.print();
        std::cerr << std::endl;

        #endif  

        return DataType();
    }
    else
    {
        DataType index = selector->run(arguments);
        if (((*arguments)[ident->name])[index].isConst)
        {
            std::cerr << "Ident with name \"" << ident->name 
                << "\" is a const and can't be assigned" << std::endl;
            exit(-1);
        }

        DataType data = expr->run(arguments);

        if (((*arguments)[ident->name])[index].type != data.type)
        {
            std::cerr << typeName[((*arguments)[ident->name])[index].type] << " can't be assigned with " 
                << typeName[data.type] << std::endl;
            exit(-1);
        }

        ((*arguments)[ident->name])[index] = data;

        #ifdef PRINT_ASSIGNMENT

        std::cerr << "Variable: " << ident->name << "[" << index.data.intValue << "] is set with: ";
        data.print();
        std::cerr << std::endl;

        #endif  

        return DataType();
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

Statement* Statement::newProcedureCallStatement(ProcedureCall* procedureCall)
{   
    Statement* statement = new Statement();
    statement->type = STATEMENT_CALL;
    statement->statement.procedureCall = procedureCall;
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

Statement* Statement::newWriteStatement(Expression* expression)
{
    Statement* statement = new Statement();
    statement->type = STATEMENT_WRITE;
    statement->statement.writeExpression = expression;
    return statement;
}

Statement* Statement::newWriteLnStatement()
{
    Statement* statement = new Statement();
    statement->type = STATEMENT_WRITELN;
    return statement;
}

Statement* Statement::newReadStatement(Factor* factor)
{
    Statement* statement = new Statement();
    statement->type = STATEMENT_READ;
    statement->statement.readFactor = factor;
    return statement;
}

void Statement::print()
{
    if (type == STATEMENT_ASSIGN)
    {
        statement.assignment->print();
    }
    else if (type == STATEMENT_CALL)
    {
        statement.procedureCall->print();
    }
    else if (type == STATEMENT_IF)
    {
        statement.ifstatement->print();
    }
    else if (type == STATEMENT_WHILE)
    {
        statement.whileStatement->print();   
    }
    else if (type == STATEMENT_WRITE)
    {
        std::cout << "Write(";
        statement.writeExpression->print();
        std::cout << ")";
    }
    else if (type == STATEMENT_WRITELN)
    {
        std::cout << "WriteLn";
    }
    else if (type == STATEMENT_READ)
    {
        std::cout << "READ(";
        statement.readFactor->print();
        std::cout << ")";
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
    else if (type == STATEMENT_CALL)
    {
        return statement.procedureCall->run(arguments);
    }
    else if (type == STATEMENT_IF)
    {
        return statement.ifstatement->run(arguments);
    }
    else if (type == STATEMENT_WHILE)
    {
        statement.whileStatement->run(arguments);
    }
    else if (type == STATEMENT_WRITE)
    {
        statement.writeExpression->run(arguments).print();
        std::cout << " ";
    }
    else if (type == STATEMENT_WRITELN)
    {
        std::cout << std::endl;
    }
    else if (type == STATEMENT_READ)
    {
        if (statement.readFactor->type != Factor::FACTOR_IDENT)
        {
            std::cerr << "Bad factor in Read()" << std::endl;
            exit(-1);
        }

        std::string name = statement.readFactor->factor.ident->name;
        if (arguments->find(name) == arguments->end())
        {
            std::cerr << "No such ident in context for calling Read()" << std::endl;
            exit(-1);
        }

        int type = (*arguments)[name].type;
        DataType data;

        if (type == NO_TYPE)
        {
            std::cerr << "Can't read NO_TYPE" << std::endl;
            exit(-1);
        }
        else if (type == INT_TYPE)
        {
            int value;
            std::cin >> value;
            data = DataType::newInteger(value);
        }
        else if (type == BOOL_TYPE)
        {
            std::string s;
            std::cin >> s;
            bool value = false;

            if (s == "true" || s == "True" || s == "TRUE")
            {
                value = true;
            }
            else if (s == "false" || s == "False" || s == "FALSE")
            {
                value = false;
            }   
            else
            {
                std::cerr << "Wrong boolean value" << std::endl;
                exit(-1);
            }

            data = DataType::newBoolean(value);
        }
        else if (type == FLOAT_TYPE)
        {
            float value;
            std::cin >> value;
            data = DataType::newFloat(value);
        }
        else if (type == STRING_TYPE)
        {
            std::string value;
            std::cin >> value;
            data = DataType::newString(value);
        }

        if (statement.readFactor->selector != NULL)
        {
            DataType index;
            index = statement.readFactor->selector->run(arguments);

            ((*arguments)[name])[index] = data;
        }
        else
        {
            (*arguments)[name] = data;
        }
        return DataType();
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
    isIdent.clear();
    identName.clear();
    if (actualParameters != NULL)
    {
        actualParameters->run(arguments);
        params = actualParameters->params;
        isIdent = actualParameters->isIdent;
        identName = actualParameters->identName;
    }

    SimpleExpression* se;
    Term* t;
    Factor* f;

    bool isident = false;
    std::string iname = "";

    if (expression->type == Expression::EXPRESSION_SIMPLE)
    {
        se = expression->expr1;
        if (se->type == SimpleExpression::SIMPLE_EXPRESSION_TERM)
        {
            t = se->term;
            if (t->type == Term::TERM_FACTOR)
            {
                f = t->factor;
                if (f->type == Factor::FACTOR_IDENT)
                {
                    isident = true;
                    iname = f->factor.ident->name;
                }
            }
        }
    }
        
    isIdent.push_back(isident);
    identName.push_back(iname);
    params.push_back(expression->run(arguments));

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
    if (procedures.find(ident->name) == procedures.end())
    {
        std::cerr << "No such function called \"" << ident->name << "\"" << std::endl;
        exit(-1);
    }

    procedures[ident->name]->call(arguments, actualParameters);

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
        return DataType::newBoolean(true);
    }
    else
    {
        if (body == NULL)
        {
            return DataType::newBoolean(false);
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
        // std::cout << "IF statement : then" << std::endl;
        _then->run(arguments);
        return DataType::newBoolean(true);
    }

    if (body != NULL)
    {
        // std::cout << "IF statement : body" << std::endl;
        res = body->run(arguments);
        if (res.data.boolValue)
        {
            return res;
        }
    }

    if (_else != NULL)
    {
        // std::cout << "IF statement : else" << std::endl;
        return _else->run(arguments);
    }

    return DataType::newBoolean(false);
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
#ifdef PATH_LOGGING
    std::cerr << "WhileStatement::run" << std::endl;
#endif
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


/** IdentList **/

IdentList::IdentList(Ident* ident, IdentList* identList)
 : ident(ident), identList(identList) {}

void IdentList::print()
{
    if (identList != NULL)
    {
        identList->print();
    }
    std::cout << ", ";
    ident->print();
}

DataType IdentList::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "IdentList::run" << std::endl;
#endif
    name.clear();
    if (identList != NULL)
    {
        identList->run(arguments);
        name = identList->name;
    }
    name.push_back(ident->name);

    return DataType();
}


/** ConstDeclarations **/

ConstDeclarations::ConstDeclarations(Ident* ident, Expression* expression, ConstDeclarations* constDec)
 : ident(ident), expression(expression), constDec(constDec) {}

void ConstDeclarations::print()
{
    if (constDec != NULL)
    {
        constDec->print();
        std::cout << " ";
    }
    ident->print();
    std::cout << " = ";
    expression->print();
    std::cout << ";";
}

DataType ConstDeclarations::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "ConstDeclarations::run" << std::endl;
#endif
    name.clear();
    constValue.clear();
    if (constDec != NULL)
    {
        constDec->run(arguments);
        name = constDec->name;
        constValue = constDec->constValue;
    }

    name.push_back(ident->name);
    constValue.push_back(expression->run(arguments));
    constValue.back().isConst = true;

    return DataType();
}


/** Type **/

Type::Type(int type, Expression* expression)
 : type(type), expression(expression) {}

void Type::print()
{
    if (expression != NULL)
    {
        std::cout << "ARRAY ";
        expression->print();
        std::cout << " OF ";
    }
    std::cout << typeName[type];
}

DataType Type::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Type::run" << std::endl;
#endif
    if (expression == NULL)
    {
        return DataType();
    }
    else
    {
        return expression->run(arguments);
    }
}


/** VarDeclarations **/

VarDeclarations::VarDeclarations(IdentList* identList, Type* type, VarDeclarations* varDec)
 : identList(identList), type(type), varDec(varDec) {}

void VarDeclarations::print()
{
    if (varDec != NULL)
    {
        varDec->print();
        std::cout << " ";
    }
    identList->print();
    std::cout << " : ";
    type->print();
    std::cout << ";";
}

DataType VarDeclarations::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "VarDeclarations::run" << std::endl;
#endif
    name.clear();
    initValue.clear();
    if (varDec != NULL)
    {
        varDec->run(arguments);
        name = varDec->name;
        initValue = varDec->initValue;
    }

    identList->run(arguments);

    for (size_t index = 0; index < identList->name.size(); ++index)
    {
        name.push_back(identList->name[index]);
        if (type->expression == NULL) 
        {
            if (type->type == INT_TYPE)
            {
                initValue.push_back(DataType::newInteger(0));
            }
            else if (type->type == BOOL_TYPE)
            {
                initValue.push_back(DataType::newBoolean(0));   
            }
            else if (type->type == FLOAT_TYPE)
            {
                initValue.push_back(DataType::newFloat(0));   
            }
            else if (type->type == STRING_TYPE)
            {
                initValue.push_back(DataType::newString(""));      
            }
        }
        else
        {
            DataType size = type->expression->run(arguments);
            initValue.push_back(DataType::newArray(type->type, size));
        }
    }

    return DataType();
}


/** Declarations **/

Declarations::Declarations(ConstDeclarations* constDec, VarDeclarations* varDec)
 : constDec(constDec), varDec(varDec) {}

void Declarations::print()
{
    constDec->print();
    std::cout << std::endl;
    varDec->print();
}

DataType Declarations::run(ArgumentList* arguments)
{
#ifdef PATH_LOGGING
    std::cerr << "Declarations::run" << std::endl;
#endif
    if (constDec != NULL)
    {
        // std::cerr << "constDec" << std::endl;
        constDec->run(arguments);

        for (size_t index = 0; index < constDec->name.size(); ++index)
        {
            (*arguments)[constDec->name[index]] = constDec->constValue[index];
        }
    }

    if (varDec != NULL)
    {
        // std::cerr << "varDec" << std::endl;
        varDec->run(arguments);

        for (size_t index = 0; index < varDec->name.size(); ++index)
        {
            (*arguments)[varDec->name[index]] = varDec->initValue[index];
        }
    }

    if (arguments == NULL)
    {
        std::cerr << "ArgumentList is unavaliable, so initialization can't be finished" << std::endl;
        exit(-1);
    }

    std::vector<std::string> name;
    std::vector<DataType> value;

    if (constDec != NULL)
    {
        for (size_t index = 0; index < constDec->name.size(); ++index)
        {
            name.push_back(constDec->name[index]);
            value.push_back(constDec->constValue[index]);
        }
    }

    if (varDec != NULL)
    {
        for (size_t index = 0; index < varDec->name.size(); ++index)
        {
            name.push_back(varDec->name[index]);
            value.push_back(varDec->initValue[index]);
        }
    }

    for (size_t index1 = 0; index1 < name.size(); ++index1)
    {
        for (size_t index2 = index1 + 1; index2 < name.size(); ++index2)
        {
            if (name[index1] == name[index2])
            {
                std::cerr << "Ident \"" << name[index1] << "\" is declared twice" << std::endl;
                exit(-1);
            }
        }
    }

    return DataType();
}


/** FPSection **/

FPSection::FPSection(bool IsVar, IdentList* identList, Type* type, FPSection* section)
 : IsVar(IsVar), identList(identList), type(type), section(section) 
{
    params.clear();
    isVar.clear();
    types.clear();
    if (section != NULL)
    {
        params = section->params;
        isVar = section->isVar;
        types = section->types;
    }

    identList->run(NULL);
    for (size_t index = 0; index < identList->name.size(); ++index)
    {
        isVar.push_back(IsVar);
        params.push_back(identList->name[index]);
        types.push_back(type);
    }
}

void FPSection::print()
{
    if (section != NULL)
    {
        section->print();
        std::cout << "; ";
    }

    if (IsVar)
    {
        std::cout << "VAR ";
    }

    identList->print();
    std::cout << " : ";
    type->print();
}

DataType FPSection::run(ArgumentList* arguments)
{
    return DataType();
}


/** FormalParameters **/

FormalParameters::FormalParameters(FPSection* section)
 : section(section) {
    params.clear();
    isVar.clear();
    types.clear();
    if (section != NULL)
    {
        params = section->params;
        isVar = section->isVar;    
        types = section->types;
    }
}

void FormalParameters::print()
{
    std::cout << "(";
    if (section != NULL)
    {
        section->print();
    }
    std::cout << ")";
}

DataType FormalParameters::run(ArgumentList* arguments)
{
    return DataType();
}


/** ProcedureHead **/

ProcedureHead::ProcedureHead(Ident* name, FormalParameters* formalParams)
 : name(name), formalParams(formalParams)
{
    params.clear();
    isVar.clear();
    types.clear();
    if (formalParams != NULL)
    {
        params = formalParams->params;
        isVar = formalParams->isVar;    
        types = formalParams->types;
    }
}

void ProcedureHead::print()
{
    std::cout << "PROCEDURE ";
    name->print();

    if (formalParams != NULL)
    {
        std::cout << " ";
        formalParams->print();
    }
}

DataType ProcedureHead::run(ArgumentList* arguments)
{
    return DataType();
}


/** ProcedureBody **/

ProcedureBody::ProcedureBody(Declarations* dec, StatementSequence* statementSequence)
 : dec(dec), statementSequence(statementSequence) {}

void ProcedureBody::print()
{
    dec->print();
    if (statementSequence != NULL)
    {
        std::cout << " ";
        statementSequence->print();
    }
    std::cout << " END";
}

DataType ProcedureBody::run(ArgumentList* arguments)
{
    if (statementSequence == NULL)
    {
        return DataType();
    }
    else
    {
        return statementSequence->run(arguments);
    }
}


/** Procedure **/

Procedure::Procedure(ProcedureHead* head, ProcedureBody* body, Ident* name)
 : head(head), body(body), name(name)
{
    dec = body->dec;

    params.clear();
    isVar.clear();
    types.clear();
    if (head != NULL)
    {
        params = head->params;
        isVar = head->isVar;    
        types = head->types;
    }

    procedures[name->name] = this;
}

void Procedure::print()
{
    head->print();
    std::cout << " ; ";
    body->print();
    std::cout << " ";
    name->print();
}

DataType Procedure::run(ArgumentList* arguments)
{
    return DataType();
}

DataType Procedure::call(ArgumentList* arguments, ActualParameters* actualParameters)
{
    ArgumentList* localArguments = new ArgumentList;
    dec->run(localArguments);

    if (actualParameters == NULL)
    {
        if (params.size() == 0)
        {
            return body->run(localArguments);
        }
        else
        {
            std::cerr << "Not enough parameters" << std::endl;
            exit(-1);
        }
    }

    actualParameters->run(arguments);

    if (actualParameters->params.size() < params.size())
    {
        std::cerr << "Not enough parameters" << std::endl;
        exit(-1);
    }

    for (size_t index = 0; index < params.size(); ++index)
    {
        if (isVar[index] && !(actualParameters->isIdent[index]))
        {
            std::cerr << "An argument " << params[index] << " should be a variable" << std::endl;
            exit(-1);
        }

        /*
        std::cerr << "set an argument " << params[index] << " with ";
        actualParameters->params[index].print();
        std::cerr << std::endl; 
        */

        (*localArguments)[params[index]] = actualParameters->params[index];
    }

    body->run(localArguments);
    /*
    std::cerr << "Params are";
    for (size_t index = 0; index < params.size(); ++index)
    {
        std::cerr << " " << actualParameters->identName[index];//params[index];
    }
    std::cerr << std::endl;
    */
    for (size_t index = 0; index < params.size(); ++index)
    {
        //std::cerr << "return " << actualParameters->identName[index] << std::endl;
        if (isVar[index])
        {
            (*arguments)[actualParameters->identName[index]] = (*localArguments)[params[index]];
        }
    }

    return DataType();
}


/** ProcedureList **/

ProcedureList::ProcedureList(Procedure* procedure, ProcedureList* procedureList)
 : procedure(procedure), procedureList(procedureList) {}

void ProcedureList::print()
{
    if (procedureList != NULL)
    {
        procedureList->print();
        std::cout << " ; ";
    }
    procedure->print();
}

DataType ProcedureList::run(ArgumentList* arguments)
{
    return DataType();
}