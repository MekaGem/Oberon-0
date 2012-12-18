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

const DataType& DataType::newString(const std::string& value, bool isConst)
{
    DataType *res = new DataType();
    res->type = STRING_TYPE;
    res->stringValue = new std::string(value);
    res->isConst = isConst;
    return *res;
}

const DataType& DataType::newArray(int type, const DataType& size)
{
    DataType *res = new DataType();
    res->type = type;
    res->isArray = true;

    if (size.type != INT_TYPE || size.data.intValue <= 0)
    {
        std::cerr << "Bad ARRAY size" << std::endl;
        exit(-1);
    }

    if (type == NO_TYPE || type > LAST_TRIVIAL_TYPE)
    {
        std::cerr << "Bad ARRAY type" << std::endl;
        exit(-1);   
    }

    if (type == INT_TYPE)
    {
        res->array = new std::vector<DataType>(size.data.intValue, newInteger(0));
    }
    else if (type == BOOL_TYPE)
    {
        res->array = new std::vector<DataType>(size.data.intValue, newBoolean(false));
    }
    else if (type == FLOAT_TYPE)
    {
        res->array = new std::vector<DataType>(size.data.intValue, newFloat(0.0f));
    }
    else if (type == STRING_TYPE)
    {
        res->array = new std::vector<DataType>(size.data.intValue, newString(""));
    }
    return *res;
}

DataType& DataType::operator [](const DataType& index) const
{
    if (!isArray)
    {
        std::cerr << "Not an ARRAY, you can't use []" << std::endl;
        exit(-1);
    }

    if (index.type != INT_TYPE)
    {
        std::cerr << "Bad ARRAY index type" << std::endl;
        exit(-1);
    }

    if (index.data.intValue < 0 || index.data.intValue >= array->size())
    {
        std::cerr << "ARRAY index is out of bound" << std::endl;
        exit(-1);
    }

    return array->at(index.data.intValue);
}

void DataType::print() const
{
    if (isArray)
    {
        std::cout << "{";
        for (size_t index = 0; index < array->size(); ++index)
        {
            if (index > 0)
            {
                std::cout << ", ";
            }
            array->at(index).print();
        }
        std::cout << "}";
    }
    else
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
        else if (type == STRING_TYPE)
        {
            std::cout << (*stringValue);
        }
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
    else if (type == STRING_TYPE)
    {
        return newString((*stringValue) + (*d.stringValue));
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String - is unavaliable" << std::endl;
        exit(-1);
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String * is unavaliable" << std::endl;
        exit(-1);
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String DIV is unavaliable" << std::endl;
        exit(-1);
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String MOD is unavaliable" << std::endl;
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String & is unavaliable" << std::endl;
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String OR is unavaliable" << std::endl;
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String NOT is unavaliable" << std::endl;
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
    else if (type == STRING_TYPE)
    {
        std::cerr << "String left- is unavaliable" << std::endl;
        exit(-1);
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
    else if (type == STRING_TYPE)
    {
        return newBoolean((*stringValue) == (*d.stringValue));   
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
    else if (type == STRING_TYPE)
    {
        return newBoolean((*stringValue) != (*d.stringValue));   
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
    else if (type == STRING_TYPE)
    {
        return newBoolean((*stringValue) < (*d.stringValue));   
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
    else if (type == STRING_TYPE)
    {
        return newBoolean((*stringValue) <= (*d.stringValue));   
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
    else if (type == STRING_TYPE)
    {
        return newBoolean((*stringValue) > (*d.stringValue));   
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
    else if (type == STRING_TYPE)
    {
        return newBoolean((*stringValue) >= (*d.stringValue));   
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
    if (isArray || d.isArray)
    {
        std::cerr << "Trivial operations with arrays can not be made" << std::endl;
        exit(-1);
    }

    if (type != d.type || type > LAST_TRIVIAL_TYPE || type == NO_TYPE) 
    {
        wrongDataTypeError(type, d.type);
    }
}
void DataType::checkTypes() const 
{
    if (isArray)
    {
        std::cerr << "Trivial operations with arrays can not be made" << std::endl;
        exit(-1);
    }

    if (type > LAST_TRIVIAL_TYPE || type == NO_TYPE) 
    {
        wrongDataTypeError(type);
    }   
}