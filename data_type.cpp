#include "data_type.hpp"

std::string typeName[5] = {"NO_TYPE", "INTEGER", "BOOLEAN", "FLOAT", "STRING"};

DataType::DataType()
{
    isConst = true;
    isArray = false;
    type = NO_TYPE;
}
const DataType& DataType::DataType::newInteger(int value, bool isConst)
{
    DataType *res = new DataType();
    res->type = INT_TYPE;
    res->data.intValue = value;
    res->isConst = isConst;
    return *res;
}
const DataType& DataType::newBoolean(bool value, bool isConst)
{
    DataType *res = new DataType();
    res->type = BOOL_TYPE;
    res->data.boolValue = value;
    res->isConst = isConst;
    return *res;
}
const DataType& DataType::newFloat(float value, bool isConst)
{
    DataType *res = new DataType();
    res->type = FLOAT_TYPE;
    res->data.floatValue = value;
    res->isConst = isConst;
    return *res;
}
/*
DataType DataType::newString(const std::string &value, bool isConst)
{
    DataType *res = new DataType();
    res->type = STRING_TYPE;
    //strcpy(res->stringValue, value.c_str());
    res->isConst = isConst;
    return *res;
}
*/
void DataType::print() const
{
    if (type == NO_TYPE)
    {
        std::cerr << "No type to print." << std::endl;
        exit(-1);
    }

    if (type == INT_TYPE)
    {
        std::cout << data.intValue;
    }
    else if (type == BOOL_TYPE)
    {
        if (data.boolValue)
        {
            std::cout << "true";
        }
        else    
        {
            std::cout << "false";
        }
    }
    else if (type == FLOAT_TYPE)
    {
        std::cout << data.floatValue;
    }
}
const DataType& DataType::operator +(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newInteger(data.intValue + d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        std::cerr << "Boolean + is unavaliable" << std::endl;
        exit(-1);
    }
    else if (type == FLOAT_TYPE)
    {
        return newFloat(data.floatValue + d.data.floatValue);
    }
}
const DataType& DataType::operator -(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newInteger(data.intValue - d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        std::cerr << "Boolean - is unavaliable" << std::endl;
        exit(-1);
    }
    else if (type == FLOAT_TYPE)
    {
        return newFloat(data.floatValue - d.data.floatValue);
    }
}
const DataType& DataType::operator *(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newInteger(data.intValue * d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        std::cerr << "Boolean * is unavaliable" << std::endl;
        exit(-1);
    }
    else if (type == FLOAT_TYPE)
    {
        return newFloat(data.floatValue * d.data.floatValue);
    }
}
const DataType& DataType::operator /(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newInteger(data.intValue / d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        std::cerr << "Boolean DIV is unavaliable" << std::endl;
        exit(-1);
    }
    else if (type == FLOAT_TYPE)
    {
        return newFloat(data.floatValue / d.data.floatValue);
    }
}
const DataType& DataType::operator %(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newInteger(data.intValue % d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        std::cerr << "Boolean MOD is unavaliable" << std::endl;
        exit(-1);
    }
    else if (type == FLOAT_TYPE)
    {
        std::cerr << "Float MOD is unavaliable." << std::endl;
        exit(-1);
    }
}
const DataType& DataType::operator &(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newInteger(data.intValue & d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue && d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        std::cerr << "Float & is unavaliable." << std::endl;
        exit(-1);
    }
}
const DataType& DataType::operator |(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newInteger(data.intValue | d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue || d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        std::cerr << "Float OR is unavaliable." << std::endl;
        exit(-1);
    }
}
const DataType& DataType::operator ~() const
{
    checkTypes();
    if (type == INT_TYPE)
    {
        return newInteger(~(data.intValue));
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(!(data.boolValue));
    }
    else if (type == FLOAT_TYPE)
    {
        std::cerr << "Float NOT is unavaliable." << std::endl;
        exit(-1);
    }
}
const DataType& DataType::operator -() const
{
    checkTypes();
    if (type == INT_TYPE)
    {
        return newInteger(-(data.intValue));
    }
    else if (type == BOOL_TYPE)
    {
        std::cerr << "Boolean left- is unavaliable." << std::endl;
        exit(-1);   
    }
    else if (type == FLOAT_TYPE)
    {   
        return newFloat(-(data.floatValue));   
    }
}

const DataType& DataType::operator ==(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newBoolean(data.intValue == d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue == d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        return newBoolean(data.floatValue == d.data.floatValue);   
    }
}
const DataType& DataType::operator !=(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newBoolean(data.intValue != d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue != d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        return newBoolean(data.floatValue != d.data.floatValue);   
    }
}
const DataType& DataType::operator <(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newBoolean(data.intValue < d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue < d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        return newBoolean(data.floatValue < d.data.floatValue);   
    }
}
const DataType& DataType::operator <=(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newBoolean(data.intValue <= d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue <= d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        return newBoolean(data.floatValue <= d.data.floatValue);   
    }
}
const DataType& DataType::operator >(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newBoolean(data.intValue > d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue > d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        return newBoolean(data.floatValue > d.data.floatValue);   
    }
}
const DataType& DataType::operator >=(const DataType& d) const
{
    checkTypes(d);
    if (type == INT_TYPE)
    {
        return newBoolean(data.intValue >= d.data.intValue);
    }
    else if (type == BOOL_TYPE)
    {
        return newBoolean(data.boolValue >= d.data.boolValue);
    }
    else if (type == FLOAT_TYPE)
    {
        return newBoolean(data.floatValue >= d.data.floatValue);   
    }
}
DataType::~DataType()
{
}

void DataType::wrongDataTypeError(int type1, int type2) const
{
    std::cerr << "Wrong data type(" << typeName[type1] << ", " << typeName[type2] <<  ")." << std::endl;
    exit(-1);
}
void DataType::checkTypes(DataType d) const
{
    if (type != d.type || type > LAST_NUMERIC_TYPE || type == NO_TYPE) 
    {
        wrongDataTypeError(type, d.type);
    }
}
void DataType::checkTypes() const 
{
    if (type > LAST_NUMERIC_TYPE || type == NO_TYPE) 
    {
        wrongDataTypeError(type);
    }   
}