#include "Not.h"

CNot::CNot()
{
    needParameterCount.m_count = 1;
    needParameterCount.option = Eq;
}

CFunctionBase *CNot::create()
{
    return new CNot();
}

IEmpty *CNot::excute(bool *isok)
{
    if (m_args.length() >1)
    {
        if(isok != NULL)
        {
            *isok = false;
            ::onError();
        }
        return NULL;
    }
       if (m_args.length() < 1)
       {
           if(isok != NULL)
           {
               *isok = false;
               ::onError();
           }
           return NULL;
       }

       IEmpty* arg0 = m_args.at(0);
       const QString FunctionBaseStr = "CFunctionBase";
       bool bArg0 = arg0->inHerits(FunctionBaseStr);
       if(bArg0)
       {
           arg0 = ((CFunctionBase*)arg0)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
       }
       bool b = ((CVariant*)arg0)->toBool();

       if(bArg0)delete arg0;
        CVariant* v = new CVariant();
        v->setValue(!b);
       return v;
}

void CNot::pushArg(IEmpty *arg,bool& isok)
{
    if (NULL == arg)
    {
        isok = false;
        ::onError();
        return;
    }

    if (m_args.length() >= 2)
    {
        isok = false;
        ::onError();
        return;
    }
    m_args.append(arg);
}

QString CNot::keyWord()
{
    return "!";
}

bool CNot::inHerits(const QString &typeName)
{
    bool b = ("CMultiplication" == typeName);
    if(b == false)
        b = CFunctionBase::inHerits(typeName);
    if(b == false)
        b = IOperator::inHerits(typeName);
    return b;
}

float CNot::level()
{
    return 5;
}
