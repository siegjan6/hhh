#ifndef MDUSER_H
#define MDUSER_H

#include <QString>
#include <QMap>
#include "../../include/MDScriptEngine/MDObjectWrapper.h"

class CMDUser
{
public:
    CMDUser();
public:
    int m_uid;
    QString m_name;
public:
    QString doWork(const QString& actionName, int repeat);
};

class CMDUserMgrWrapper;
class CMDUserMgr
{
public:
    CMDUserMgr();
    ~CMDUserMgr();
public:
    bool add(const QString& name);
    bool delete1(const QString& name);
    void deleteAll();
public:
    bool login(const QString& name);
    void logout();
public:
    void enableScript();
    void setPID(const QString& PID);
    QString m_PID;
    CMDUserMgrWrapper* m_wrapper;
protected:
    QMap<QString, CMDUser*> m_userMap;
};

#endif // MDUSER_H
