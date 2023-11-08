#ifndef CDIVISION_H
#define CDIVISION_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CDivision:public CFunctionBase,public IOperator
{
public:
    CDivision();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // CDIVISION_H
