#ifndef GREATER_H
#define GREATER_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CGreater:public CFunctionBase,public IOperator
{
public:
    CGreater();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};
#endif // GREATER_H
