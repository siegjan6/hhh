#include "Add.h"

CAdd::CAdd()
{
    needParameterCount.m_count = 2;
    needParameterCount.option = Eq;
}

CFunctionBase *CAdd::create()
{
    return new CAdd();
}


QString Replace(QString str)
{
    if (!str.isEmpty())
    {
        int index = str.indexOf('\\');
        while (index > -1)
        {
            str = str.remove(index, 1);
            if (index >= str.length())
            {
                return str;
            }
            index = str.indexOf( '\\',index+1);
        }
    }
    return str;
}

IEmpty *CAdd::excute(bool* isok)
{
    int parameterCount = m_args.length();
    if(parameterCount != 2)
    {
        if(isok != NULL)
            *isok = false;
        ::onError();
        return NULL;
    }
    double a1 = 0;
    double a2 = 0;
    IEmpty* arg0 = m_args[0];
    IEmpty* arg1 = m_args[1];
    if( arg0 == NULL || arg1 == NULL)
    {
        if(isok != NULL)
            *isok = false;
        ::onError();
        return NULL;
    }
    //字符串处理
    bool isArg0ReturnByFun = false;
    bool isArg1ReturnByFun = false;
    bool isStrConcat = false;
    //arg0
    bool bArg0 = arg0->inHerits("CFunctionBase");
    if(bArg0)
    {
        isStrConcat = arg0->inHerits("CStringConstantFun");
       arg0 = ((CFunctionBase*)arg0)->excute(isok);
       if(isok!=NULL)
       {
           if(*isok == false)
               return NULL;
       }
       if( arg0 == NULL)
       {
           if(isok != NULL)
               *isok = false;
           ::onError();
           return NULL;
       }
        if (!isStrConcat)
        {
           isStrConcat = arg0->inHerits("QString");
        }
        isArg0ReturnByFun = true;
    }
    else
    {
        QString str = ((CVariant*)arg0)->toString();
        bool b1 = str.startsWith("\"")&& str.endsWith("\"");
        bool b2 = arg0->inHerits("QString");
        bool b3 = false;
        double result = str.toDouble(&b3);
        isStrConcat = b1 || b2 && (!b3);
    }

    //arg1
    bool bArg1 = arg1->inHerits("CFunctionBase");
    if(bArg1)
    {
        isStrConcat = arg1->inHerits("StringConstantFun");
        //CFunctionBase* base = arg1->value<CFunctionBase*>();
        //arg1 = base->Excute();
        arg1 = ((CFunctionBase*)arg1)->excute(isok);
        if(isok!=NULL)
        {
            if(*isok == false)
                return NULL;
        }

        if( arg1 == NULL)
        {
            if(isok != NULL)
                *isok = false;
            ::onError();
            return NULL;
        }
        if (!isStrConcat)
        {
           isStrConcat = arg1->inHerits("QString");
        }
        isArg1ReturnByFun = true;
    }
    else
    {
        QString str = arg1->toString();
        if(str == "true")str = "1";
        else if(str=="false")str = "0";
        bool b1 = str.startsWith("\"")&& str.endsWith("\"");
        bool b2 = arg1->inHerits("QString");
        bool b3 = false;
        double result = str.toDouble(&b3);
        isStrConcat = b1 || b2 && (!b3);
    }

    //字符串处理
    if (isStrConcat)  //arg0 || ag1 is QString
    {
        QString str1 = arg0->toString();
        QString str2 = arg1->toString();
        if(!isArg0ReturnByFun)
        {
            if(str1.endsWith("\""))
            {
             str1 = str1.remove(str1.length()-1);
            }
            if(str1.startsWith("\""))
            {
             str1 = str1.remove(0,1);
            }
        }
        if(!isArg1ReturnByFun)
        {
            if(str2.endsWith("\""))
            {
             str2 = str2.remove(str2.length()-1);
            }
            if(str2.startsWith("\""))
            {
             str2 = str2.remove(0,1);
            }
        }

        QString result = Replace(str1 + str2);
        if(result == "")
        {
            if(isok != NULL)
                *isok = false;
            ::onError();
        }
        CVariant* v= new CVariant();
        v->setValue(result);
        return v;
    }

    bool bOK = false;
    a1 = ((CVariant*)arg0)->toDouble(&bOK);
    if(bOK == false)
    {
        if(isok!=NULL)
            *isok = false;
        ::onError();
        return NULL;
    }
    a2 = ((CVariant*)arg1)->toDouble(&bOK);
    if(bOK == false)
    {
        if(isok!=NULL)
            *isok = false;
        ::onError();
        return NULL;
    }
    if(bArg0)
        delete arg0;
    if(bArg1)
        delete arg1;
    CVariant* _var = new CVariant();
    _var->setValue(a1+a2);
    return _var;
}

void CAdd::pushArg(IEmpty *arg,bool& isok)
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

QString CAdd::keyWord()
{
    return "+";
}

bool CAdd::inHerits(const QString &typeName)
{
    bool b = ("Add" == typeName);
    if(b == false)
        b = CFunctionBase::inHerits(typeName);
    if(b == false)
        b = IOperator::inHerits(typeName);
    return b;
}

float CAdd::level()
{
    return 3;
}
