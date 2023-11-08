#include "StringConstantFun.h"

CStringConstantFun::CStringConstantFun()
{
    needParameterCount.m_count = 1;
}

CFunctionBase *CStringConstantFun::create()
{
    return new CStringConstantFun();
}
QString Replace(QString str);

IEmpty* CStringConstantFun::excute(bool *isok)
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
    IEmpty* arg0 = m_args.at(0);
    bool isStrConcat = false;
    const QString FunctionBaseStr = "CFunctionBase";
    bool bArg0 = arg0->inHerits(FunctionBaseStr);
    if(bArg0)
    {
       isStrConcat = arg0->inHerits("StringConstantFun");
       arg0 = ((CFunctionBase*)arg0)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
       if (!isStrConcat)
       {
           isStrConcat = arg0->inHerits("String");
       }
    }
    else
    {
       if (NULL == arg0)
       {
           if(isok != NULL)
           {
               *isok = false;
               ::onError();
           }
           return NULL;
       }
       QString str = arg0->toString();
       isStrConcat = str.startsWith("\"") && str.endsWith("\"");
    }

    if (isStrConcat)//|| arg0 is string)
    {
       QString str1 = arg0->toString();
       if (str1.endsWith("\""))
       {
           str1 = str1.remove(str1.length()-1, 1);
       }
       if (str1.startsWith("\""))
       {
           str1 = str1.remove(0, 1);
       }
       str1 = Replace(str1);

       if(bArg0)delete arg0;
       CVariant* v = new CVariant();
       v->setValue(str1);
       return v;
    }
    CVariant* v = new CVariant();
    v->setValue(m_args.at(0)->toString());

    if(bArg0)delete arg0;
    return v;
}

void CStringConstantFun::pushArg(IEmpty *arg,bool& isok)
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

QString CStringConstantFun::keyWord()
{
    return "StringConstant";
}

bool CStringConstantFun::inHerits(const QString &typeName)
{
    bool b = ("CStringConstantFun" == typeName);
    if(b == false)
        b = CFunctionBase::inHerits(typeName);
    return b;
}
