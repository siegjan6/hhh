#include "ExpCompileMgr.h"

CExpCompileMgr::CExpCompileMgr()
{
    _fun = NULL;
}

CExpCompileMgr::~CExpCompileMgr()
{
    if(_fun != NULL)
        _fun->Release();
}

bool CExpCompileMgr::compile(const QString &exp)
{
    if(_exp != exp)
    {
        if(_fun != NULL)
        {
            _varList.clear();
            _fun->Release();
            _fun = NULL;
        }
        bool b = CExpCompile::Instance()->compile(_fun,exp);
        if(!b)
            return b;
        referencedVarHead(_fun);
        _exp = exp;
    }
    return true;
}

void CExpCompileMgr::setVariableValue(const QString &varName, const QVariant &varValue)
{
    QString varNameLower = varName.toLower();
    if(_varList.contains(varNameLower))
    {
        //for(CVariableFun* fun:_varList[varNameLower])
        //{
        //    fun->setVarValue(varValue);
        //}
        for (int i = 0 ;i < _varList[varNameLower].count();i++)
        {
             _varList[varNameLower][i]->setVarValue(varValue);
        }
    }
}

QList<QString> CExpCompileMgr::referencedVarList()
{
    return _varList.keys();
}

void CExpCompileMgr::repaceVariableName(const QString& varName,const QString& newVarName)
{
    QList<QString> varList = this->referencedVarList();
    QString varNameLower = varName.toLower();
    QString newVarNameLower = newVarName.toLower();
    if(varList.contains(varNameLower))
    {
        if(_varList.count() != 0)
        {
            QList<CVariableFun*> varfuns = _varList[varNameLower];
            _varList.insert(newVarNameLower,varfuns);
            _varList.remove(varNameLower);
        }
    }
}


QVariant CExpCompileMgr::excute(bool *isok)
{
    if(isok != NULL)
        *isok = true;
    if(_fun != NULL)
    {
        IEmpty* pty = _fun->excute(isok);
        if(pty != NULL)
        {
            if(pty->inHerits("CVariant"))
            {
                QVariant v = *(CVariant*)pty;
                delete pty;
                return v;
            }
        }
    }
    return QVariant();
}

void CExpCompileMgr::referencedVarHead(CFunctionBase* fun)
{
    bool bVar = _fun->inHerits("CVariableFun");
    if(bVar)
    {
        CVariableFun* var = (CVariableFun*)_fun;
        const QString& varName = var->m_args.at(0)->toString();
        insertVar(varName,var);
    }
    else
    {
        referencedVarList(fun);
    }
}

void CExpCompileMgr::referencedVarList(CFunctionBase* fun)
{
    if(fun == NULL)
        return;

    //for(IEmpty* obj:fun->m_args)
    for(int i = 0; i< fun->m_args.count();i++)
    {
        IEmpty* obj = fun->m_args[i];
        bool bFun = obj->inHerits("CFunctionBase");
        if(bFun)
        {
            referencedVarList((CFunctionBase*)obj);
            bool bVar = obj->inHerits("CVariableFun");
            if(bVar)
            {
                CVariableFun* var = (CVariableFun*)obj;
                const QString& varName = var->m_args.at(0)->toString();
                insertVar(varName,var);
            }
        }
    }
}

void CExpCompileMgr::insertVar(const QString &varName, CVariableFun *fun)
{
    QString varNameLower = varName.toLower();

    bool bContains = _varList.contains(varNameLower);
    if(bContains)
    {
        _varList[varNameLower].append(fun);
    }
    else
    {
        QList<CVariableFun*> funList;
        funList.append(fun);
        _varList.insert(varNameLower,funList);
    }
}
