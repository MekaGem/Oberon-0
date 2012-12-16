#pragma once

#include <map>

const int NO_TYPE = 0;
const int INT_TYPE = 1;
const int BOOL_TYPE = 2;
const int FLOAT_TYPE = 3;

const int LAST_NUMERIC_TYPE = FLOAT_TYPE;
const int MAX_TYPES_NUM = 4;
extern std::string typeName[];

class DataType
{
public:
    DataType()
    {
        type = NO_TYPE;
    }
    static const DataType& newInteger(int value)
    {
        DataType *res = new DataType();
        res->type = INT_TYPE;
        res->data.intValue = value;
        return *res;
    }
    static const DataType& newBoolean(bool value)
    {
        DataType *res = new DataType();
        res->type = BOOL_TYPE;
        res->data.boolValue = value;
        return *res;
    }
    static const DataType& newFloat(float value)
    {
        DataType *res = new DataType();
        res->type = FLOAT_TYPE;
        res->data.floatValue = value;
        return *res;
    }
    void print() const
    {
        if (type == NO_TYPE)
        {
            std::cerr << "No type to print." << std::endl;
            exit(-1);
        }

        if (type == INT_TYPE)
        {
            std::cout << data.intValue << std::endl;
        }
        else if (type == BOOL_TYPE)
        {
            if (data.boolValue)
            {
                std::cout << "true" << std::endl;
            }
            else    
            {
                std::cout << "false" << std::endl;
            }
        }
        else if (type == FLOAT_TYPE)
        {
            std::cout << data.floatValue << std::endl;
        }
    }
    const DataType& operator +(DataType d) const
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
    const DataType& operator -(DataType d) const
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
    const DataType& operator *(const DataType& d) const
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
    const DataType& operator /(const DataType& d) const
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
    const DataType& operator %(const DataType& d) const
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
    const DataType& operator &(const DataType& d) const
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
    const DataType& operator |(const DataType& d) const
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
    const DataType& operator ~() const
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
    const DataType& operator -() const
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

    const DataType& operator ==(const DataType& d) const
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
    const DataType& operator !=(const DataType& d) const
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
    const DataType& operator <(const DataType& d) const
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
    const DataType& operator <=(const DataType& d) const
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
    const DataType& operator >(const DataType& d) const
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
    const DataType& operator >=(const DataType& d) const
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
    ~DataType()
    {
    }
    union
    {
        int intValue;
        bool boolValue;
        float floatValue;
    } data;
    int type;

private:
    void wrongDataTypeError(int type1 = NO_TYPE, int type2 = NO_TYPE) const
    {
        std::cerr << "Wrong data type(" << typeName[type1] << ", " << typeName[type2] <<  ")." << std::endl;
        exit(-1);
    }
    void checkTypes(const DataType& d) const
    {
        if (type != d.type || type > LAST_NUMERIC_TYPE || type == NO_TYPE) 
        {
            wrongDataTypeError(type, d.type);
        }
    }
    void checkTypes() const 
    {
        if (type > LAST_NUMERIC_TYPE || type == NO_TYPE) 
        {
            wrongDataTypeError(type);
        }   
    }
};