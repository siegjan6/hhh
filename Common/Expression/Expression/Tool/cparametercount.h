#ifndef CPARAMETERCOUNT_H
#define CPARAMETERCOUNT_H

enum ParameterType
{
    Invalid,
    String = 3,
    Double,
    DateTime,
    Boolean,
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
