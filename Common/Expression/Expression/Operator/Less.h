#ifndef LESS_H
#define LESS_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CLess:public CFunctionBase,public IOperator
{
public:
    CLess();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // LESS_H
