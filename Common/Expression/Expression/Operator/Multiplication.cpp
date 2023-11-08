#include "Multiplication.h"

CMultiplication::CMultiplication()
{
    needParameterCount.m_count = 2;
    needParameterCount.option = Eq;
}

CFunctionBase *CMultiplication::create()
{
    return new CMultiplication();
}

IEmpty *CMultiplication::excute(bool *isok)
{
    if (m_args.length()> 2)
    {
        if(isok != NULL)
        {
            *isok = false;
            ::onError();
        }
        return NULL;
    }

    if (m_args.length() < 2)
    {
        if(isok != NULL)
        {
            *isok = false;
            ::onError();
        }
        return NULL;
    }

    double a1 = 0;
    double a2 = 0;

    IEmpty* arg0 = m_args[0];
    IEmpty* arg1 = m_args[1];
    const QString FunctionBaseStr = "CFunctionBase";
    bool bArg0= false;
    if(arg0 != NULL)
    {
        bArg0 = arg0->inHerits(FunctionBaseStr);
        if(bArg0)
        {
          arg0 = ((CFunctionBase*)arg0)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
        }
    }
    bool bArg1 = false;
    if(arg1 != NULL)
    {
        bArg1 = arg1->inHerits(FunctionBaseStr);
        if(bArg1)
        {
          arg1 = ((CFunctionBase*)arg1)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
        }
    }

    if (NULL == arg0 )
    {
      a1 = 0;
    }
    else
    {
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
    }

    if (NULL == arg1 )
    {
      a2 = 0;
    }
    else
    {
        CVariant* varg1 = dynamic_cast<CVariant*>(arg1);
        if(varg1 == NULL)
        {
            if(isok != NULL)
            {
                *isok = false;
                ::onError();
            }
            return NULL;
        }
        bool bOK = false;
        a2 = varg1->toDouble(&bOK);
        if(bOK == false)
        {
            if(isok != NULL)
            {
                *isok = false;
                ::onError();
            }
            return NULL;
        }
    }
    if(bArg0)delete arg0;
    if(bArg1)delete arg1;
    CVariant* v = new CVariant();
    v->setValue(a1*a2);
    return v;
}

void CMultiplication::pushArg(IEmpty *arg,bool& isok)
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

QString CMultiplication::keyWord()
{
    return "*";
}

bool CMultiplication::inHerits(const QString &typeName)
{
    bool b = ("CMultiplication" == typeName);
    if(b == false)
        b = CFunctionBase::inHerits(typeName);
    if(b == false)
        b = IOperator::inHerits(typeName);
    return b;
}

float CMultiplication::level()
{
    return 4;
}
