#include "./ExpCompile.h"

#include"../Interface/IOperator.h"
#include <qvariant.h>
#include "../Operator/StringConstantFun.h"
#include"../Operator/Add.h"
#include"../Operator/AAnd.h"
#include"../Operator/AOr.h"
#include"../Operator/Division.h"
#include"../Operator/Multiplication.h"
#include"../Operator/Not.h"
#include"../Operator/Subtract.h"
#include"../Operator/LAnd.h"
#include "../Operator/LOr.h"
#include "../Operator/Eq.h"
#include "../Operator/NEq.h"
#include "../Operator/Greater.h"
#include "../Operator/GTE.h"
#include "../Operator/Less.h"
#include "../Operator/LTE.h"

#include"../Function/VariableFun.h"
#include "../Function/Abs.h"
#include "../Function/Format.h"
CExpCompile::CExpCompile()
{
}

CExpCompile::~CExpCompile()
{
}

CExpCompile *CExpCompile::Instance()
{
        static CExpCompile* _instance  = NULL;
        if(_instance == NULL)
        {
            _instance = new CExpCompile();
            _instance->initialize();
        }
        return _instance;
}

//有前后顺序,多字母的排前
void CExpCompile::initialize()
{
        CAdd* add = new CAdd();
        m_operatorList.append(add->keyWord().toUpper());
        m_funFactoryDic.insert(add->keyWord().toUpper(),add);


        CSubtract* subtract = new CSubtract();
        m_operatorList.append(subtract->keyWord().toUpper());
        m_funFactoryDic.insert(subtract->keyWord().toUpper(),subtract);

        CMultiplication* multiplication = new CMultiplication();
        m_operatorList.append(multiplication ->keyWord().toUpper());
        m_funFactoryDic.insert(multiplication ->keyWord().toUpper(),multiplication);

        CDivision* division = new CDivision();
        m_operatorList.append(division->keyWord().toUpper());
        m_funFactoryDic.insert(division->keyWord().toUpper(),division);

        CAAnd* aand = new CAAnd();
        m_operatorList.append(aand->keyWord().toUpper());
        m_funFactoryDic.insert(aand->keyWord().toUpper(),aand);

        CLOr* lOr = new CLOr();
        m_operatorList.append(lOr->keyWord().toUpper());
        m_funFactoryDic.insert(lOr->keyWord().toUpper(),lOr);

        CAOr* aor = new CAOr();
        m_operatorList.append(aor->keyWord().toUpper());
        m_funFactoryDic.insert(aor->keyWord().toUpper(),aor);

        CLAnd* land = new CLAnd();
        m_operatorList.append(land->keyWord().toUpper());
        m_funFactoryDic.insert(land->keyWord().toUpper(),land);

        CEq* eq = new CEq();
        m_operatorList.append(eq->keyWord().toUpper());
        m_funFactoryDic.insert(eq->keyWord().toUpper(),eq);

        CNEq* neq = new CNEq();
        m_operatorList.append(neq->keyWord().toUpper());
        m_funFactoryDic.insert(neq->keyWord().toUpper(),neq);
        CGreater* gr = new CGreater();
        m_operatorList.append(gr->keyWord().toUpper());
        m_funFactoryDic.insert(gr->keyWord().toUpper(),gr);
        CGTE* gt = new CGTE();
        m_operatorList.append(gt->keyWord().toUpper());
        m_funFactoryDic.insert(gt->keyWord().toUpper(),gt);
        CLess* less = new CLess();
        m_operatorList.append(less->keyWord().toUpper());
        m_funFactoryDic.insert(less->keyWord().toUpper(),less);
        CLTE* clte = new CLTE();
        m_operatorList.append(clte->keyWord().toUpper());
        m_funFactoryDic.insert(clte->keyWord().toUpper(),clte);

        CNot* _not = new CNot();
        // _operatorList.append(_not->keyWord().toUpper());
        m_funFactoryDic.insert(_not->keyWord().toUpper(),_not);


        CAbs* abs = new CAbs();
        // _operatorList.append(_not->keyWord().toUpper());
        m_funFactoryDic.insert(abs->keyWord().toUpper(),abs);

        CFormat* fmt = new CFormat();
        m_funFactoryDic.insert(fmt->keyWord().toUpper(),fmt);

        CVariableFun* vf = new CVariableFun();
        m_funFactoryDic.insert(vf->keyWord().toUpper(),vf);
}

void CExpCompile::release()
{
    QList<CFunctionBase*> list = m_funFactoryDic.values();
    for(int i = 0 ;i < list.length();i++)
    {
        delete list[i];
    }
    m_funFactoryDic.clear();
}

bool CExpCompile::compile(CFunctionBase *&root, QString exp)
{
    exp = exp.trimmed();
    int start = 0;
    int index = 0;
    CFunctionBase* currentFun = NULL;
    CFunctionBase* currentFunParent = NULL;
    bool isNeedmatch = false;
    while(index < exp.length())
    {
        QString str = exp.mid(start,index-start+1).trimmed();
         QString current = exp[index].toUpper();
        if(current.trimmed().isEmpty())
        {
            if(start == index)
            {
                ++start;
            }
            ++index;
            continue;
        }
        if(isNeedmatch&& "\\" == current)
        {
            index = index + 2;
            continue;
        }
        int endPos = getOptrKeyWordEndPos(exp,index);
        if(!isNeedmatch&& endPos >-1)//没括弧的操作符
        {
            bool bContinue = false;
            bool isok = computeOperator(exp,start,index,endPos,root,currentFun,currentFunParent,&bContinue);
            if(!isok)  return false;
            if(bContinue)
                continue;
        }
        else if( isKeyWord(str)) //函数处理
        {
            bool isok = true;
            bool bContinue = computeFunction(exp,str,start,index,root,currentFun,currentFunParent,isok);
            if(!isok) return false;
            if(bContinue)
                continue;
        }
        else if ( "(" == current && !isNeedmatch)  //()分组
        {
            if(index > start)
            {
                return false;// "分组";
            }
            CFunctionBase* tmp = NULL;
            bool isok = true;
            int end = findEndBracketsPos(exp,index,isok);
            if(!isok)return false;
            if(end>-1)
            {
                if(start+1 == end)
                {
                    return false;// "invalid fenzu";
                }
                compile(tmp,exp.mid(index+1,end-index-1));
                if(NULL != tmp)
                {
                    start = end +1;
                    index = start;
                    if(NULL == currentFun)
                    {
                        currentFun = root = tmp;
                    }
                    else
                    {
                        if(end + 1 < exp.length() && !exp.mid(end+1).trimmed().isEmpty())
                        {
                            bool isok = true;
                            start = index = appendFun(currentFun,root,tmp,currentFunParent, start, end + 1, exp,isok);
                            if(!isok)return false;
                        }
                        else
                        {
                             bool isok = true;
                            currentFun->pushArg(tmp,isok);
                            if(!isok)return false;
                            start = index = end +1;
                        }
                    }
                    continue;
                }
            }
        }
        else if(")" == current && !isNeedmatch)
        {
            return false;//("'(' and ')' not match");
        }
        else if("\"" == current)
        {
            if(isNeedmatch)
                isNeedmatch = false;
            else
                isNeedmatch = true;
        }
        ++index;
    }
    if (isNeedmatch)
    {
        return false;//"is not match";
    }
    if(start < exp.length() && NULL != currentFun)
    {
        QString temp = exp.mid(start);
        int paraType = isAvalableParameter(temp);
        if(paraType <= 0)
        {
            return false;// "is invalidate parameter";
        }
        if(paraType == pt_String)
        {
            CStringConstantFun* sf = new CStringConstantFun();
            bool isok = true;
            sf->pushValue(temp,isok);
            if(!isok)return false;
            currentFun->pushArg(sf,isok);
             if(!isok)return false;
        }
        else if(paraType == pt_Variable)
        {
            AddVariable(temp);
            CVariableFun* vf = new CVariableFun();
            bool isok = true;
            vf->pushValue(temp,isok);
            if(!isok)return false;
            currentFun->pushArg(vf,isok);
             if(!isok)return false;
        }
        else
        {
            bool isok = true;
            currentFun->pushValue(temp,isok);
            if(!isok)
                return false;
        }
    }
    else if(0 == start)
    {
        QString temp = exp;
        int paraType = isAvalableParameter(temp);
        if (paraType <= 0)
        {
           return false;// (temp + " is invalidate parameter!");
        }
        else if(paraType == pt_Variable)
        {
            AddVariable(temp);
            CVariableFun* vf = new CVariableFun();
            bool isok = true;
            vf->pushValue(temp,isok);if(!isok)return false;
            if(currentFun== NULL)
            {
                currentFun = root = vf;
            }
           else
            {
                bool isok = true;
                currentFun->pushArg(vf,isok);
                 if(!isok)return false;
            }
        }
        else
        {
            CStringConstantFun* sf = new CStringConstantFun();
            bool isok = true;
            sf->pushValue(temp,isok);if(!isok)return false;
            root = sf;
        }
    }
    return true;
}

void CExpCompile::AddVariable(const QString &varName)
{
}

//找m_operatorList里匹配的操作符
int CExpCompile::getOptrKeyWordEndPos(const QString& str, int p)
{
    int index = -1;
    for(int i = 0 ;i < m_operatorList.length();i++)
    {
        QString ptr = m_operatorList.at(i);
        int tmp = p+1;
        QString startStr = str.mid(p,tmp-p);
        if(ptr.startsWith(startStr.toUpper()))
        {
            if(startStr.toUpper()!= ptr)
            {
                while(tmp < str.length())
                {
                    QString key = str.mid(p, tmp - p + 1);
                    if(key == ptr)
                    {
                        index = tmp;
                    }
                    tmp++;
                }
            }
            else
            {
                if(p > index)
                {
                    index = p;
                }
            }
        }
    }
    return index;
}

CFunctionBase *CExpCompile::getFunction(QString keyWord)
{
    if(keyWord.isEmpty())return NULL;
    keyWord = keyWord.toUpper();
    if(isKeyWord(keyWord))
    {
        return m_funFactoryDic[keyWord]->create();
    }
    // ("not found function:{0}");
    return NULL;
}

bool CExpCompile::isKeyWord(QString key)
{
    if(key.isEmpty())return false;
    bool bContains = m_funFactoryDic.contains(key.toUpper());

    return bContains;
}

bool CExpCompile::computeOperator(QString exp,int&start,int& index,int& endPos,CFunctionBase*& rootFun,CFunctionBase*& currentFun,CFunctionBase*& currentFunParent)
{
    CFunctionBase* optr = getFunction(exp.mid(index,endPos-index+1));
    IOperator* optr1 = dynamic_cast<IOperator*>(optr);
    if(optr1 != NULL)
    {
       if (index == start && (NULL == currentFun ) ) //开始是操作符
       {
           if (NULL == currentFun ) //符号位
           {
               index = endPos + 1;
               return true;//continue
           }
           else if (NULL == currentFun)
           {
               rootFun = currentFun = optr; //单个元算符如：！
               index = endPos + 1;
               return true;
           }
       }
       if (NULL == rootFun)
         {
             currentFun = rootFun = optr;
             QString temp = exp.mid(start, index - start).trimmed();
             int paraType = isAvalableParameter(temp);
             if (paraType <= 0)
             {
                 return false;// (temp + " is invalidate parameter!");
             }
             if (paraType == pt_String) //无括号操作字符串处理
             {
                 CStringConstantFun* sf = new CStringConstantFun();
                 bool isok = true;
                 sf->pushValue(temp,isok);if(!isok)return false;
                 currentFun->pushArg(sf,isok);
                  if(!isok)return false;
             }
             else if(paraType == pt_Variable)
             {
                 AddVariable(temp);
                 CVariableFun* vf = new CVariableFun();
                 bool isok = true;
                 vf->pushValue(temp,isok);
                 if(!isok)return false;
                 currentFun->pushArg(vf,isok);
                  if(!isok)return false;
             }
             else
             {
                 bool isok = true;
                 currentFun->pushValue(temp,isok);
                 if(!isok)return false;
             }
             index = start = endPos + 1;//继续下一个参数匹配
             return true;
         }
       else
       {
           IOperator* curOptr = dynamic_cast<IOperator*>(currentFun);
           if(curOptr != NULL)
           {
               float currentLevel = curOptr->level();
               float optrLevel =  optr1->level();
               if(currentLevel >= optrLevel)
               {
                   if(index > start)
                   {
                       QString temp = exp.mid(start, index - start).trimmed();
                       int paraType = isAvalableParameter(temp);
                       if (paraType <= 0)
                       {
                          return false;// (temp + " is invalidate parameter!");
                       }
                       if (paraType == pt_String) //字符串处理
                       {
                           CStringConstantFun* sf = new CStringConstantFun();
                            bool isok = true;
                           sf->pushValue(temp,isok);
                          if(!isok)return false;
                           currentFun->pushArg(sf,isok);
                            if(!isok)return false;
                       }
                       else if(paraType == pt_Variable)
                       {
                           AddVariable(temp);
                           CVariableFun* vf = new CVariableFun();
                           bool isok = true;
                           vf->pushValue(temp,isok);
                           if(!isok)return false;
                           currentFun->pushArg(vf,isok);
                            if(!isok)return false;
                       }
                       else
                       {
                           bool isok = true;
                          currentFun->pushValue(temp,isok);
                          if(!isok)return false;
                       }
                   }
                   bool isok = true;
                   optr->pushArg(rootFun,isok);
                   if(!isok)return false;
                   currentFun = rootFun = optr;
               }
               else
               {
                   if(index > start)
                   {
                       IOperator* curOptr = dynamic_cast<IOperator*>(currentFun);
                       float currentLevel = curOptr->level();
                       float optrLevel =  optr1->level();
                       if(currentLevel < optrLevel)
                       {
                           bool isok = true;
                            currentFun->pushArg(optr,isok);
                            if(!isok)return false;
                            QString temp = exp.mid(start, index - start).trimmed();
                            int  paraType = isAvalableParameter(temp);
                            if (paraType <= 0)
                            {
                                return false;// (temp + " is invalidate parameter!");
                            }
                            if (paraType == pt_String) //字符串处理
                            {
                                CStringConstantFun* sf = new CStringConstantFun();
                                isok = true;
                                sf->pushValue(temp,isok);
                                if(!isok)return false;
                                optr->pushArg(sf,isok);
                                if(!isok)return false;
                            }
                            else if(paraType == pt_Variable)
                            {
                                AddVariable(temp);
                                CVariableFun* vf = new CVariableFun();
                                bool isok  = true;
                                vf->pushValue(temp,isok);
                                if(!isok)return false;
                                currentFun->pushArg(vf,isok);
                                if(!isok)return false;
                            }
                            else
                            {
                                bool isok  = true;
                                optr->pushValue(temp,isok);
                                if(!isok)return false;
                            }
                            currentFunParent = currentFun;
                            currentFun = optr;
                       }
                       else
                       {
                           QString temp = exp.mid(start,index-start).trimmed();
                           int paraType = isAvalableParameter(temp);
                            if (paraType <= 0)
                            {
                                return false;// (temp + " is invalidate parameter!");
                            }
                            if (paraType == pt_String) //字符串处理
                            {
                                CStringConstantFun* sf = new CStringConstantFun();
                                bool isok  = true;
                                sf->pushValue(temp,isok);
                                if(!isok)return false;
                                currentFun->pushArg(sf,isok);
                                if(!isok)return false;
                            }
                            else if(paraType == pt_Variable)
                            {
                                AddVariable(temp);
                                CVariableFun* vf = new CVariableFun();
                                bool isok  = true;
                                vf->pushValue(temp,isok);
                                if(!isok)return false;
                                currentFun->pushArg(vf,isok);
                                if(!isok)return false;
                            }
                            else
                            {
                                bool isok  = true;
                                currentFun->pushValue(temp,isok);
                                if(!isok)return false;
                            }
                            bool isok = true;
                            optr->pushArg(currentFun,isok);
                            if(!isok)return false;
                            if(NULL != currentFunParent)
                            {
                                IEmpty* v = currentFunParent->popArg();
                                //delete v;
                                bool isok = true;
                                currentFunParent->pushArg(optr,isok);
                                if(!isok)return false;
                                currentFun = optr;
                            }
                            else
                            {
                                bool isok = true;
                                currentFun = rootFun = optr;
                                if(!isok)return false;
                            }
                       }
                   }
                   else
                   {
                       bool isok = true;
                       optr->pushArg(currentFun,isok);
                       if(!isok)return false;
                       currentFun = rootFun = optr;
                   }
               }
               start = index = endPos+1;
               return true;
           }
           else
           {
                if (NULL != currentFun)
                {
                    bool isok = true;
                   optr->pushArg(currentFun,isok);
                   if(!isok)return false;
                }
                currentFun = rootFun = optr;
                start = index = endPos + 1;
                return true;
           }
       }
    }
    return false;
}

#include "Core/ExpUtil.h"
int CExpCompile::isAvalableParameter(QString str)
{
    if(str.isEmpty())
        return pt_Invalid;

    if(str.startsWith("\"") && str.endsWith("\""))
        return pt_String;

    bool bOK = false;
    str.toDouble(&bOK);
    if(bOK)
        return pt_Double;

    if (CExpUtil::IsVarNameValidate(str))
    {
         return pt_Variable;
    }
    return pt_Invalid;
}

bool CExpCompile::computeFunction(QString& exp,QString& str,int& start,int& index,CFunctionBase*& rootFun,CFunctionBase*& currentFun,CFunctionBase*& currentFunParent,bool& isok)
{
    ++index;
    QChar c = exp.at(index);
    while(' '==c)
    {
       ++index;
        c = exp.at(index);
    }
    CFunctionBase* fun = getFunction(str);
    if(fun == NULL)
    {
        isok = false;
        ::onError();
    }
    IOperator* op = dynamic_cast<IOperator*>(fun);
    bool b22 = !fun->inHerits("IOperator");
    bool b1 = '(' != c;
    bool b2 = !(op != NULL);
    bool b3 = fun->needBrackets();
    if(fun == NULL||(b1&&b2&&b3))
    {
        return true;
    }
    int end = -1;
    QList<QString> paramertList;
    if ('(' == c)
    {
        end = findEndBracketsPos(exp, index,isok);
        if(!isok)return false;
        getParameters(exp.mid(index + 1, end - index - 1),&paramertList);
    }
    else
    {
       end = findNextOptr(exp, index);
       getParameters(exp.mid(index , end - index+1),&paramertList);
    }
    for(int i = 0 ;i < paramertList.length();i++)
    {
       QString paramerter = paramertList.at(i);
        CFunctionBase* paraFun = NULL;
       compile(paraFun,paramerter);
       if(NULL != paraFun)
       {
           fun->pushArg(paraFun,isok);
           if(!isok)return false;
       }
    }
    if(NULL == currentFun)
    {
       currentFun = rootFun = fun;
       start = index = end+1;
    }
    else
    {
       if (end+1< exp.length() && !( exp.mid(end+1).trimmed().isEmpty()))
       {
           start = index = appendFun(currentFun, rootFun, fun, currentFunParent, start, end + 1, exp,isok);
           if(!isok)return false;
       }
       else //()后面无操作符,表示表达式最后一项
       {
           currentFun->pushArg(fun,isok);
           if(!isok)return false;
           start = index = end + 1;
       }
    }
    return true;
}

int CExpCompile::findEndBracketsPos(QString exp, int start,bool& isok)
{
    int count = 0;
    int prevEndBrackets = -1;
    while(start<exp.length())
    {
        QChar c = exp[start];
        if('('== c)
        {
            if(prevEndBrackets>-1)
            {
                if((prevEndBrackets+1) == start)
                {
                     // ("found ')(',please check your expression  ");
                    isok = false;
                    ::onError();
                    return -1;
                }
            }
            ++count;
        }
        else if(')'== c)
        {
            if(count >0)
            {
                prevEndBrackets = start;
                --count;
            }
            else
            {
                // ("'(' and ')' not match");
                isok = false;
                ::onError();
                return -1;
            }
        }
        if(0 == count)
        {
            return start;
        }
        ++start;
    }
    // "未发现与位置{0}的'('匹配的'）";
    isok = false;
    ::onError();
    return -1;
}

bool CExpCompile::getParameters(QString exp,QList<QString>* list)
{
     if(exp.isNull()||exp.isEmpty())return false;

     int start = 0;
     int index = 0;
     bool isNeedMatch = false;
     while(index<exp.length())
     {
         QChar c = exp.at(index);
         if('"'== c)
         {
             isNeedMatch = !isNeedMatch;
         }
         else if(','==c)
         {
             if(!isNeedMatch)
             {
                 QString str = exp.mid(start,index-start).trimmed();
                 if(str.isEmpty())
                     return false;//"isemptry";
                 list->append(str);
                 index++;
                 start = index;
                 continue;
             }
         }
         else if('('== c&&!isNeedMatch)
         {
             bool isok = true;
             int end = findEndBracketsPos(exp,index,isok);
             if(!isok)return false;
             if(end >-1)
             {
                 index = end;
             }
         }
         index++;
     }
     if(isNeedMatch)
        return false;//"not match";
     else
     {
         list->append(exp.mid(start));
     }
 }

int CExpCompile::appendFun(CFunctionBase *&currentFun, CFunctionBase *&rootFun, CFunctionBase *tmp, CFunctionBase *&currentFunParent, int start, int index, QString exp,bool& isok)
 {
     bool isAppend = false;
     while(start < exp.length())
     {
         QString current = "";
         current.append(exp[index]);
         if(current.trimmed()== "")
         {
             if(start == index)
             {
                 start++;
             }
             index++;
             continue;
         }
         int end = getOptrKeyWordEndPos(exp,index);
         CFunctionBase*optr = getFunction(exp.mid(index,end-index+1));
         index = end;
         if(NULL == optr)
         {
             isok = false;
             ::onError();
             return -1;
         }
         IOperator* op = dynamic_cast<IOperator*>(optr);
         if( op == NULL)
         {
             isok = false;
             ::onError();
             return -1;
         }
         IOperator* curOptr = dynamic_cast<IOperator*>(currentFun);
         int currentLevel = curOptr->level();
         int optrFunLevel = op->level();
         if(currentLevel> optrFunLevel)
         {
             currentFun->pushArg(tmp,isok);
             if(!isok)return false;
             optr->pushArg(currentFun,isok);
             if(!isok)return false;
             currentFun = rootFun = optr;
         }
         else
         {
             IOperator* curOptr = dynamic_cast<IOperator*>(currentFun);
             int currentLevel = curOptr->level();
             int optrFunLevel = op->level();
             if(currentLevel < optrFunLevel)
             {
                 optr->pushArg(tmp,isok);
                 if(!isok)return false;
                 currentFun->pushArg(optr,isok);
                 if(!isok)return false;
                 currentFun = optr;
             }
             else
             {
                 currentFun->pushArg(tmp,isok);
                 if(!isok)return false;
                 optr->pushArg(currentFun,isok);
                 if(!isok)return false;
                 if(currentFunParent != NULL)
                 {
                     IEmpty* v = currentFunParent->popArg();
                    // delete v;
                     currentFunParent->pushArg(optr,isok);
                     if(!isok)return false;
                     currentFun = optr;
                 }
                 else
                 {
                     currentFun = rootFun = optr;
                 }
             }
         }
         isAppend = true;
         index++;
         start = index;
         break;
     }
     if(!isAppend)
     {
         currentFun->pushArg(tmp,isok);
         if(!isok)return false;
     }
     return index;
 }

int CExpCompile::findNextOptr(QString str, int p)
{
    int index = -1;
    while(index == -1 && p < str.length())
    {
        for(int i = 0 ;i < m_operatorList.length();i++)
        {
            QString ptr = m_operatorList.at(i);

            int tmp = p+1;
            QString startStr = str.mid(p,tmp - p);
            if(ptr.startsWith(startStr.toUpper()))
            {
                if(startStr.toUpper() != ptr)
                {
                    while(tmp<str.length())
                    {
                        QString key = str.mid(p, tmp - p + 1);
                        if(key == ptr)
                        {
                            return p-1;
                        }
                        tmp++;
                    }
                }
            }
        }
        ++p;
    }
    if(index==-1&&p==str.length())
    {
        index = p-1;
    }
    return index;
}

bool CExpCompile::computeOperator(QString exp,int&start,int& index,int& endPos,CFunctionBase*& rootFun,CFunctionBase*& currentFun,CFunctionBase*& currentFunParent,bool* bContinue)
{
    bool isok = true;
    CFunctionBase* optr = getFunction(exp.mid(index,endPos-index+1));
    IOperator* optr1 = dynamic_cast<IOperator*>(optr);
    if(optr1 != NULL)
    {
       if (index == start && (NULL == currentFun ) ) //开始是操作符
       {
           if (NULL == currentFun ) //符号位
           {
               index = endPos + 1;
               *bContinue = true;
               return true;
           }
           else if (NULL == currentFun)
           {
               rootFun = currentFun = optr; //单个元算符如：！
               index = endPos + 1;
               *bContinue = true;

               return true;
           }
       }
       if (NULL == rootFun)
         {
             currentFun = rootFun = optr;
             QString temp = exp.mid(start, index - start).trimmed();
             int paraType = isAvalableParameter(temp);
             if (paraType <= 0)
             {
                 return false;//  (temp + " is invalidate parameter!");
             }
            if (paraType == pt_String)
            {
                CStringConstantFun* sf = new CStringConstantFun();
                sf->pushValue(temp,isok);
                currentFun->pushArg(sf,isok);
                if(!isok)return false;
            }
            else if(paraType == pt_Variable)
            {
                AddVariable(temp);
                CVariableFun* vf = new CVariableFun();
                vf->pushValue(temp,isok);
                if(!isok)return false;
                currentFun->pushArg(vf,isok);
                if(!isok)return false;
            }
             else//最后数字计算
             {
                 currentFun->pushValue(temp,isok);
                  if(!isok)return false;
             }
             index = start = endPos + 1;//继续下一个参数匹配
             *bContinue = true;
             return true;
         }
       else
       {
           IOperator* curOptr = dynamic_cast<IOperator*>(currentFun);
           if(curOptr != NULL)
           {
               float currentLevel = curOptr->level();
               float optrLevel =  optr1->level();

               if(currentLevel > optrLevel )
               {
                   if(index > start)
                   {
                       QString temp = exp.mid(start, index - start).trimmed();
                       int paraType = isAvalableParameter(temp);
                       if (paraType <= 0)
                       {
                          // (temp + " is invalidate parameter!");
                           return false;
                       }
                       if (paraType == pt_String)
                       {
                           CStringConstantFun* sf = new CStringConstantFun();
                            sf->pushValue(temp,isok);
                           currentFun->pushArg(sf,isok);
                           if(!isok)return false;
                       }
                       else if(paraType == pt_Variable)
                       {
                           AddVariable(temp);
                           CVariableFun* vf = new CVariableFun();
                           vf->pushValue(temp,isok);
                           if(!isok)return false;
                           currentFun->pushArg(vf,isok);
                           if(!isok)return false;
                       }
                       else
                       {
                               currentFun->pushValue(temp,isok);
                       }
                   }
                   optr->pushArg(rootFun,isok);
                   if(!isok)return false;
                   currentFun = rootFun = optr;
               }
               else
               {
                   if(index > start)
                   {
                       IOperator* curOptr = dynamic_cast<IOperator*>(currentFun);
                       float currentLevel = curOptr->level();
                       float optrLevel =  optr1->level();
                       if(currentLevel < optrLevel)
                       {
                            currentFun->pushArg(optr,isok);
                            if(!isok)return false;
                            QString temp = exp.mid(start, index - start).trimmed();
                            int  paraType = isAvalableParameter(temp);
                            if (paraType <= 0)
                            {
                                // (temp + " is invalidate parameter!");
                                return false;
                            }
                            if (paraType == QVariant::Type::String)
                            {
                                CStringConstantFun* sf = new CStringConstantFun();
                                sf->pushValue(temp,isok);
                                optr->pushArg(sf,isok);
                                if(!isok)return false;
                            }
                            else if(paraType == pt_Variable)
                            {
                                AddVariable(temp);
                                CVariableFun* vf = new CVariableFun();
                                vf->pushValue(temp,isok);
                                optr->pushArg(vf,isok);//zj add //currentFun->pushArg(vf);
                                if(!isok)return false;
                            }
                            else
                            {
                                optr->pushValue(temp,isok);
                            }
                            currentFunParent = currentFun;
                            currentFun = optr;
                       }
                       else
                       {
                           QString temp = exp.mid(start,index-start).trimmed();
                           int paraType = isAvalableParameter(temp);
                            if (paraType <= 0)
                            {
                                // (temp + " is invalidate parameter!");
                                return false;
                            }
                            if (paraType == QVariant::Type::String)
                            {
                                CStringConstantFun* sf = new CStringConstantFun();
                                sf->pushValue(temp,isok);
                                currentFun->pushArg(sf,isok);
                                if(!isok)return false;
                            }
                            else if(paraType == pt_Variable)
                            {
                                AddVariable(temp);
                                CVariableFun* vf = new CVariableFun();
                                vf->pushValue(temp,isok);
                                currentFun->pushArg(vf,isok);
                                if(!isok)return false;
                            }
                            else
                            {
                                currentFun->pushValue(temp,isok);
                            }
                            optr->pushArg(currentFun,isok);
                            if(!isok)return false;
                            if(NULL != currentFunParent)
                            {
                                currentFunParent->popArg();
                                currentFunParent->pushArg(optr,isok);
                                if(!isok)return false;
                                currentFun = optr;
                            }
                            else
                            {
                                currentFun = rootFun = optr;
                            }
                       }
                   }
                   else
                   {
                       optr->pushArg(currentFun,isok);
                       if(!isok)return false;
                       currentFun = rootFun = optr;
                   }
               }
               start = index = endPos+1;
               *bContinue = true;
               return true;
           }
           else
           {
                if (NULL != currentFun)
                {
                   optr->pushArg(currentFun,isok);
                   if(!isok)return false;
                }
                currentFun = rootFun = optr;
                start = index = endPos + 1;
                *bContinue = true;
                return true;
           }
       }
    }
    return true;
}
