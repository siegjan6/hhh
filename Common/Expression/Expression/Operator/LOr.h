#ifndef LOR_H
#define LOR_H

#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CLOr:public CFunctionBase,public IOperator
{
public:
    CLOr();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // LOR_H
