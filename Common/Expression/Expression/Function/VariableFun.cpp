#include "VariableFun.h"

CVariableFun::CVariableFun()
{
    needParameterCount.m_count = 1;
}

CFunctionBase *CVariableFun::create()
{
    return new CVariableFun();
}

QMap<QString,QString> varList;
IEmpty *CVariableFun::excute(bool *isok)
{
    CVariant* v = new CVariant();
    v->setValue(_varValue);
    return v;
}

void CVariableFun::pushArg(IEmpty *arg,bool& isok)
{
    if (NULL == arg)
    {
        isok = false;
        ::onError();
        return;
    }
    if (m_args.length() > 1)
    {
        isok = false;
        ::onError();
        return;
    }

    m_args.append(arg);
}

QString CVariableFun::keyWord()
{
    return "VariableFun";
}

bool CVariableFun::inHerits(const QString &typeName)
{
    bool b = ("CVariableFun" == typeName);
    if(b == false)
        b = CFunctionBase::inHerits(typeName);
    return b;
}

QString CVariableFun::getVarName()
{
    if (m_args.length() > 1)
    {
       throw ("argument is more");
    }
    IEmpty* arg0 = m_args.at(0);
    bool bArg0 = arg0->inHerits("CFunctionBase");
    if(bArg0)
    {
        bool isok = true;
       arg0 = ((CFunctionBase*)arg0)->excute(&isok);
        if(!isok)
        {
            delete arg0;
            return "";
        }
    }
    else
    {
       if (NULL == arg0)
           return "";
    }
    QString varName =  arg0->toString();
    if(bArg0)delete arg0;
    return varName;
}

void CVariableFun::setVarValue(const QVariant &varValue)
{
    _varValue = varValue;
}
