#include "Abs.h"

CAbs::CAbs()
{
    this->needParameterCount.m_count = 1;
    this->needParameterCount.option = Eq;
}

CFunctionBase *CAbs::create()
{
    return new CAbs();
}

IEmpty *CAbs::excute(bool *isok)
{
    if (m_args.length() > 1)
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

    double a1 = 0;
    IEmpty* arg0 = m_args[0];
    if (NULL == arg0)
    {
        if(isok != NULL)
        {
            *isok = false;
            ::onError();
        }
        return NULL;
    }
    QString FunctionBaseStr = "CFunctionBase";
    bool bArg0 = arg0->inHerits(FunctionBaseStr);
    if(bArg0)
    {
        arg0 = ((CFunctionBase*)arg0)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
    }

    CVariant* varg0 = dynamic_cast<CVariant*>(arg0);
    if(varg0 == NULL)
    {
        if(isok != NULL)
        {
            *isok = false;
            ::onError();
        }
        return NULL;
    }
    bool bOK = false;
    a1 = varg0->toDouble(&bOK);
    if(bOK == false)
    {
        if(isok != NULL)
        {
            *isok = false;
            ::onError();
        }
        return NULL;
    }
    if(bArg0) delete arg0;
    CVariant* v = new CVariant();
    v->setValue(qAbs(a1));
    return v;
}

void CAbs::pushArg(IEmpty *arg, bool &isok)
{
    if (NULL == arg)
    {
        isok = false;
        ::onError();
        return;
    }

    if (m_args.length() == 1)
    {
         isok = false;
         ::onError();
         return;
    }
    m_args.append(arg);
}

QString CAbs::keyWord()
{
    return "Abs";
}
