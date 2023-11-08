#ifndef LAND_H
#define LAND_H

#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CLAnd:public CFunctionBase,public IOperator
{
public:
    CLAnd();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};


#endif // LAND_H
