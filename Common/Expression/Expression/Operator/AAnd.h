#ifndef CAAND_H
#define CAAND_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CAAnd:public CFunctionBase,public IOperator
{
public:
    CAAnd();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // CAAND_H
