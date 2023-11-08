#ifndef CSUBTRACT_H
#define CSUBTRACT_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CSubtract:public CFunctionBase,public IOperator
{
public:
    CSubtract();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();
};

#endif // CSUBTRACT_H
