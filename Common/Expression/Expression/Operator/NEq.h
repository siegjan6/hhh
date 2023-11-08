#ifndef NEQ_H
#define NEQ_H

#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CNEq:public CFunctionBase,public IOperator
{
public:
    CNEq();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // NEQ_H
