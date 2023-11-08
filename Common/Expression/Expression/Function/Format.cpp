#include "Format.h"

CFormat::CFormat()
{
    this->needParameterCount.m_count = 1;
    this->needParameterCount.option = MoreThan;
}

CFunctionBase *CFormat::create()
{
    return new CFormat();
}

QString myPrintf(QString strFmt,QList<QVariant*> args);

IEmpty *CFormat::excute(bool *isok)
{
    if (m_args.length() < 2)
    {
        if(isok != NULL)
        {
            *isok = false;
            ::onError();
        }
        return NULL;
    }

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
        arg0 = ((CFunctionBase*)arg0)->excute(isok);
        if(isok!=NULL)
        {
            if(*isok == false)
                return NULL;
        }
    }

    QList<IEmpty*> arr ;
    QList<QVariant*> arr2;
    for (int i = 1; i <= m_args.length() - 1; i++)
    {
       IEmpty* ot = m_args[i];

       if ( ot->inHerits(FunctionBaseStr))
       {
            ot = ((CFunctionBase*)ot)->excute(isok);
       }
       arr.append(ot);
       QVariant* varg = dynamic_cast<QVariant*>(ot);
       arr2.append(varg);
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
    QString result = myPrintf(varg0->toString(),arr2);
    if(arg0) delete arg0;
    for(int i = 0 ;i < arr.length();i++)
    {
        if(arr[i]!= NULL)
            delete arr[i];
    }
    CVariant* v = new CVariant();

    v->setValue(result);
    return v;
}

void CFormat::pushArg(IEmpty *arg, bool &isok)
{
    if (NULL == arg)
    {
        isok = false;
        ::onError();
        return;
    }
    m_args.append(arg);
}

QString CFormat::keyWord()
{
    return "Format";
}

QStringList GetSubList(QString src,QStringList childs)
{
   for(int i = 0 ;i < childs.length();i++)
   {
        src = src.replace(childs[i],"|");
   }
   QStringList result = src.split("|",QString::SkipEmptyParts);
   return result;
}

bool myReplace(QString& data,QString src,QString desc)
{
    int index = data.indexOf(src);
    if(index == -1)
        return false;
    data.replace(index,src.length(),desc);
    return true;
}

QString myPrintf(QString strFmt,QList<QVariant*> args)
{
    QString::SplitBehavior sp = QString::SkipEmptyParts;
    QStringList strList = strFmt.split(QRegExp("%[0-9.]*[dfs]"),sp);
    QStringList strOpers = GetSubList(strFmt,strList);
    for(int i = 0;i < strOpers.length();i++)
    {
        QString& oper = strOpers[i];
        if(oper.at(oper.length()-1) == 'd')
        {
            bool isOK = false;
            int val = args.at(i)->toInt(&isOK);
            if(!isOK)return "";
            QString later;
            later= later.sprintf(oper.toLatin1().data(),val);
            if(!myReplace(strFmt,oper,later))return "";

        }
        else if(oper.at(oper.length()-1) == 'f')
        {
            bool isOK = false;
            double val = args.at(i)->toDouble(&isOK);
            if(!isOK)return "";
            QString later;
            later = later.sprintf(oper.toLatin1().data(),val);
            if(!myReplace(strFmt,oper,later))return "";
        }
        else
        {
            QString later;
            later = later.sprintf(oper.toLatin1().data(),args.at(i)->toByteArray().data());
            if(!myReplace(strFmt,oper,later))
                return "";
        }
    }
    return strFmt;
}
