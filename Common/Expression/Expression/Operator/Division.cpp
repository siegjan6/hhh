#include "Division.h"

CDivision::CDivision()
{
    needParameterCount.m_count = 2;
    needParameterCount.option = Eq;
}
CFunctionBase *CDivision::create()
{
    return new CDivision();
}

IEmpty *CDivision::excute(bool *isok)
{
    if (m_args.length() >2)
   {
        if(isok != NULL)
        {
            *isok = false;
        }
        ::onError();
        return NULL;
   }

   if (m_args.length() < 1)
   {
       if(isok != NULL)
       {
           *isok = false;
       }
       ::onError();
       return NULL;
   }

   double a1 = 0;
   double a2 = 0;

   IEmpty* arg0 = 0;
   IEmpty* arg1 = 0;
   if(m_args.length() == 1)
    {
        // 作为负号使用
        arg1 = m_args.at(0);
        arg0 = 0;
    }
    else
    {
        arg0 = m_args.at(0);
        arg1 = m_args.at(1);
    }
   const QString FunctionBaseStr = "CFunctionBase";
   bool bArg0 = false;
   if(arg0 != NULL)
        bArg0 = arg0->inHerits(FunctionBaseStr);
   if(bArg0)
   {
       arg0 = ((CFunctionBase*)arg0)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
   }
   bool bArg1 = false;
   if(arg1 != NULL)
       bArg1 = arg1->inHerits(FunctionBaseStr);
   if(bArg1)
   {
       arg1 = ((CFunctionBase*)arg1)->excute(isok);
       if(isok!=NULL){
           if(*isok == false)
               return NULL;
       }
   }

    if (NULL == arg0 )//|| (arg0 is DBNull)
    {
       a1 = 0;
    }
    else
    {
       bool bOK = false;
       a1 = arg0->toFloat(&bOK);
       if(!bOK)
       {
           if(isok != NULL)
           {
               *isok = false;
           }
           ::onError();
           return NULL;
       }
    }

    if (NULL == arg1 )//|| (arg1 is DBNull)
    {
       a2 = 0;
    }
    else
    {
       bool bOK = false;
       a2 = arg1->toFloat(&bOK);
       if(!bOK)
       {
           if(isok != NULL)
           {
               *isok = false;
           }
           ::onError();
           return NULL;
       }
    }
    if(bArg0)
        delete arg0;
    if(bArg1)
        delete arg1;
    CVariant* v = new CVariant();
    v->setValue(a1/a2);
    return v;
}

void CDivision::pushArg(IEmpty *arg,bool& isok)
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

QString CDivision::keyWord()
{
    return "/";
}

bool CDivision::inHerits(const QString &typeName)
{
    bool b = ("CDivision" == typeName);
    if(b == false)
        b = CFunctionBase::inHerits(typeName);
    if(b == false)
        b = IOperator::inHerits(typeName);
    return b;
}

float CDivision::level()
{
    return 4;
}
