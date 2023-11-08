#include "MDUser.h"
#include "MDUiWrappers.h"
#include "MDScriptEngine.h"

CMDUser::CMDUser()
{
    m_name = "";
    m_uid = 0;
}

QString CMDUser::doWork(const QString& actionName, int repeat)
{
    return "I am function [doWork]";
}

// CMDUserMgr
CMDUserMgr::CMDUserMgr()
{
    m_wrapper = NULL;
}

CMDUserMgr::~CMDUserMgr()
{
    deleteAll();

    if(m_wrapper)
        delete m_wrapper;
}

bool CMDUserMgr::add(const QString& name)
{
    if(m_userMap.contains(name))
    {
        return false;
    }

    CMDUser* newUser = new CMDUser;
    newUser->m_name = name;
    newUser->m_uid = rand();
    m_userMap.insert(name, newUser);
    return true;
}

bool CMDUserMgr::delete1(const QString& name)
{
    QMap<QString, CMDUser*>::const_iterator iPosition = m_userMap.find(name);
     if(m_userMap.constEnd() != iPosition)
     {
         delete iPosition.value();
         return true;
     }
     return false;
}

void CMDUserMgr::deleteAll()
{
    QMap<QString, CMDUser*>::const_iterator iPosition = m_userMap.constBegin();
    while(m_userMap.constEnd() != iPosition)
    {
        delete iPosition.value();
        ++ iPosition;
    }
    m_userMap.clear();
}

bool CMDUserMgr::login(const QString& name)
{
    QList<QVariant> paras;
    paras.append(QVariant(name));
    CMDScriptEngine::callFuncInPool("g_UserMgr_OnLogin", paras, m_PID);

    return false;
}

void CMDUserMgr::logout()
{

}

void CMDUserMgr::enableScript()
{
    m_wrapper = new CMDUserMgrWrapper(this, "g_UserMgr");
}

void CMDUserMgr::setPID(const QString& PID)
{
    m_PID = PID;
}
