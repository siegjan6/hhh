#include "ViewVarMgr.h"
#include "utils.h"

namespace DBQE
{
bool ViewVarMgr::Init(const QString & viewVarsFileName)
{
    if(m_ViewVarsMap.size() != 0) //初始化时 必须为空
    {
        DBQE::WriteInfoLog(QString("ViewVarMgr::Init:already initialized with %1").arg(viewVarsFileName));
        return false;
    }

    QList<DBQ::ViewVariable *> viewVarsList;
    QString errStr;
    if(!DBQ::ViewVariable_Load(viewVarsFileName, DBQ::FILE_TYPE_JSON, viewVarsList, errStr))
    {
        DBQE::WriteInfoLog(QString("ViewVarMgr::Init:cann't load file %1").arg(viewVarsFileName));
        return false;
    }

    foreach(DBQ::ViewVariable * pViewVar, viewVarsList)
    {
        if(!CheckViewVariable(pViewVar)) //如果ViewVariable不合法 则continue
        {
            DBQE::WriteInfoLog(QString("ViewVarMgr::Init:varialbe is invaild in  %1 view variable").arg(viewVarsFileName));
            continue;
        }

        m_ViewVarsMap[pViewVar->name] = pViewVar;
    }

    return true;
}

bool ViewVarMgr::FindValue(const QString & key, QString & valueOut)
{
    if(!m_ViewVarsMap.contains(key))
        return false;
    valueOut = m_ViewVarsMap[key]->value;
    return true;
}

DBQ::ViewVariable * ViewVarMgr::Find(const QString & key)
{
    if(!m_ViewVarsMap.contains(key))
        return 0;
    return m_ViewVarsMap[key];
}

bool ViewVarMgr::CheckViewVariable(DBQ::ViewVariable * pViewVar)
{
    if(!pViewVar)
    {
        DBQE::WriteInfoLog("ViewVarMgr::CheckViewVariable: view variable is epmty");
        return false;
    }

    if(pViewVar->name.isEmpty())
    {
        DBQE::WriteInfoLog("ViewVarMgr::CheckViewVariable: view variable name is epmty");
        return false ;
    }

    if(m_ViewVarsMap.contains(pViewVar->name))
    {
        DBQE::WriteInfoLog(QString("ViewVarMgr::CheckViewVariable:the %1 view variable is already exsisted").arg(pViewVar->name));
        return false ;
    }

    return true;
}
}
