#ifndef GTE_H
#define GTE_H

#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CGTE:public CFunctionBase,public IOperator
{
public:
    CGTE();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // GTE_H
