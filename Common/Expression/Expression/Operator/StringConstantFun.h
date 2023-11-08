#ifndef CSTRINGCONSTANTFUN_H
#define CSTRINGCONSTANTFUN_H
#include "./Core/FunctionBase.h"

class CStringConstantFun : public CFunctionBase
{
public:
    CStringConstantFun();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
    virtual bool inHerits(const QString& typeName);
};

#endif // CSTRINGCONSTANTFUN_H
