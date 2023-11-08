#ifndef ABS_H
#define ABS_H

#include "./Core/FunctionBase.h"
class CAbs : public CFunctionBase
{
public:
    CAbs();
    virtual CFunctionBase* create();
    virtual IEmpty* excute(bool* isok = NULL);
    virtual void pushArg(IEmpty* arg,bool& isok);
    virtual bool needBrackets(){return true;}
    virtual QString keyWord();
};
#endif // ABS_H
