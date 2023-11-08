#ifndef CFUNCTIONBASE_H
#define CFUNCTIONBASE_H
#include <qlist.h>
#include "./Interface/IEmpty.h"
#include "./Interface/IFrequency.h"
#include "./Interface/ParameterCount.h"
#include "./Core/Variant.h"
class CFunctionBase : public IEmpty,public IFrequency
{
public:
    virtual ~CFunctionBase();
    virtual CFunctionBase* create() = 0;
    virtual IEmpty* excute(bool* isok = NULL) = 0;
    virtual void pushArg(IEmpty* arg,bool& isok) = 0;
    virtual bool needBrackets(){return true;}
    virtual QString keyWord() = 0;
    virtual void Release();
    virtual bool inHerits(const QString& typeName)
    {
        return typeName == "CFunctionBase";
    }

    template<typename T>
    bool pushValue(const T& arg,bool& isok)
    {
        CVariant* base = new CVariant();
        base->setValue(arg);
        pushArg(base,isok);
        return isok;
    }
    virtual IEmpty* popArg();

    QList<IEmpty*> m_args;
public:
    CParameterCount needParameterCount;
    bool operator == (CFunctionBase* other);
    bool operator != (CFunctionBase* other);
    virtual CFunctionBase* Clone();
};

#endif // CFUNCTIONBASE_H
