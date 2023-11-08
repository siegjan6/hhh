#ifndef CEXPVARIABLE_H
#define CEXPVARIABLE_H
#include <qvariant.h>

class CExpVariable
{
public:
    CExpVariable()
    {

    }

    template<typename T>
    CExpVariable(QString name,T val)
    {
        m_name = name;
        m_value.setValue(val);
    }
    QString m_name;
    QVariant m_value;
};

#endif // CEXPVARIABLE_H
