#ifndef FORMAT_H
#define FORMAT_H

#include "./Core/FunctionBase.h"
class CFormat : public CFunctionBase
{
public:
    CFormat();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
};
#endif // FORMAT_H
