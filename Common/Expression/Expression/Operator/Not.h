#ifndef CNOT_H
#define CNOT_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CNot:public CFunctionBase,public IOperator
{
public:
    CNot();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return false;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // CNOT_H
