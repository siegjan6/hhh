#include "AAnd.h"

CAAnd::CAAnd()
{
    needParameterCount.m_count = 2;
    needParameterCount.option = Eq;
}

CFunctionBase *CAAnd::create()
{
    return new CAAnd();
}

IEmpty *CAAnd::excute(bool* isok)
{
    if (m_args.length() >2)
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

   long a1 = 0;
   long a2 = 0;

   IEmpty* arg0 = m_args.at(0);
   IEmpty* arg1 = m_args.at(1);
   const QString FunctionBaseStr = "CFunctionBase";
   bool bArg0 = arg0->inHerits(FunctionBaseStr);
   if(bArg0)
   {
       arg0 = ((CFunctionBase*)arg0)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
   }
   double d1 = 0;
   bool bOK = false;
   d1 = ((CVariant*)arg0)->toDouble(&bOK);
   if(bOK)
   {
       a1 = d1;
   }
   else
   {
       if(isok != NULL)
       {
           *isok = false;
           ::onError();
       }
       return NULL;
   }
   bool bArg1= arg1->inHerits(FunctionBaseStr);
   if(bArg1)
   {
       arg1 = ((CFunctionBase*)arg1)->excute(isok); if(isok!=NULL){if(*isok == false)return NULL;}
   }
   double d2 = 0;
   bOK = false;
   d2 = ((CVariant*)arg1)->toDouble(&bOK);
   if(bOK)
   {
       a2 = d2;
   }
   else
   {
       if(isok != NULL)
       {
           *isok = false;
           ::onError();
       }
       return NULL;
   }
   if(bArg0)
       delete arg0;
   if(bArg1)
       delete arg1;
    CVariant* v = new CVariant();
    v->setValue(a1&a2);
   return v;
}

void CAAnd::pushArg(IEmpty *arg,bool& isok)
{
    if (NULL == arg)
    {
        //"Multiplication";
        isok = false;
        ::onError();
        return;
    }


    if (m_args.length() >= 2)
    {
        // ("- argument is more");
        isok = false;
        ::onError();
        return;
    }
    m_args.append(arg);
}

QString CAAnd::keyWord()
{
    return "&";
}

bool CAAnd::inHerits(const QString &typeName)
{
    bool b = ("CAAnd" == typeName);
    if(b == false)
        b = CFunctionBase::inHerits(typeName);
    if(b == false)
        b = IOperator::inHerits(typeName);
    return b;
}

float CAAnd::level()
{
    return -1;
}
