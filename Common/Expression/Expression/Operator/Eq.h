#ifndef EQ_H
#define EQ_H

#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CEq:public CFunctionBase,public IOperator
{
public:
    CEq();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // EQ_H
