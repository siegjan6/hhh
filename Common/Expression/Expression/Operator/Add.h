#ifndef ADD_H
#define ADD_H
#include "./Core/FunctionBase.h"
#include "./Interface/IOperator.h"
class CAdd : public CFunctionBase,public IOperator
{
public:
    CAdd();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
    virtual float level();

};

#endif // ADD_H
