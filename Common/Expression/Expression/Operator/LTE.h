#ifndef LTE_H
#define LTE_H

#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CLTE:public CFunctionBase,public IOperator
{
public:
    CLTE();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // LTE_H
