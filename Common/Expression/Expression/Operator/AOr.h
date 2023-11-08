#ifndef CAOR_H
#define CAOR_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CAOr:public CFunctionBase,public IOperator
{
public:
    CAOr();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // CAOR_H
