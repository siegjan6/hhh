#ifndef CMULTIPLICATION_H
#define CMULTIPLICATION_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CMultiplication:public CFunctionBase,public IOperator
{
public:
    CMultiplication();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // CMULTIPLICATION_H
