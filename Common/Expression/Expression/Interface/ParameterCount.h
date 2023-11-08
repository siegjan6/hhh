#ifndef CPARAMETERCOUNT_H
#define CPARAMETERCOUNT_H
#include <qvariant.h>
enum ParameterType
{
    pt_Invalid = QVariant::Type::Invalid,
    pt_String = QVariant::Type::String,
    pt_Double = QVariant::Type::Double,
    pt_DateTime = QVariant::Type::DateTime,
    pt_Bool = QVariant::Type::Bool,
    pt_Variable
};

enum ParameterOption
{
    Eq,
    LessThan,
    MoreThan,
};

class CParameterCount
{

public:
    CParameterCount(){m_count = 0;}
    CParameterCount(int c,ParameterOption p = Eq)
    {
        m_count = c;
        option = p;
    }
    int m_count;
    ParameterOption option;
};

#endif // CPARAMETERCOUNT_H
