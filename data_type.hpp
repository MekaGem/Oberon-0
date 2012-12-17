#pragma once

#include <map>
#include <cstdlib>
#include <string>
#include <iostream>

const int NO_TYPE = 0;
const int INT_TYPE = 1;
const int BOOL_TYPE = 2;
const int FLOAT_TYPE = 3;
const int STRING_TYPE = 4;

const int LAST_NUMERIC_TYPE = STRING_TYPE;
const int MAX_TYPES_NUM = 5;

extern std::string typeName[];

class DataType
{
public:
    DataType();
    static const DataType& newInteger(int value, bool isConst = false);
    static const DataType& newBoolean(bool value, bool isConst = false);
    static const DataType& newFloat(float value, bool isConst = false);
    //static const DataType& newString(const std::string &value, bool isConst = false);
    void print() const;
    const DataType& operator +(const DataType& d) const;
    const DataType& operator -(const DataType& d) const;
    const DataType& operator *(const DataType& d) const;
    const DataType& operator /(const DataType& d) const;
    const DataType& operator %(const DataType& d) const;
    const DataType& operator &(const DataType& d) const;
    const DataType& operator |(const DataType& d) const;
    const DataType& operator ~() const;
    const DataType& operator -() const;
    
    const DataType& operator ==(const DataType& d) const;
    const DataType& operator !=(const DataType& d) const;
    const DataType& operator <(const DataType& d) const;
    const DataType& operator <=(const DataType& d) const;
    const DataType& operator >(const DataType& d) const;
    const DataType& operator >=(const DataType& d) const;
    
    ~DataType();

    union Data
    {
        int intValue;
        bool boolValue;
        float floatValue;
    } data;
    std::vector<Data>* array;
    std::string* stringValue;

    int type;
    bool isConst;
    bool isArray;

private:
    void wrongDataTypeError(int type1 = NO_TYPE, int type2 = NO_TYPE) const;
    void checkTypes(DataType d) const;
    void checkTypes() const;
};