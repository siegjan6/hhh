#ifndef IOPERATOR_H
#define IOPERATOR_H
#include "./Interface/IEmpty.h"
class IOperator:public IEmpty
{
public:
    virtual float level() = 0;
    virtual bool inHerits(const QString& typeName)
    {
        return typeName == "IOperator";
    }
};

#endif // IOPERATOR_H
