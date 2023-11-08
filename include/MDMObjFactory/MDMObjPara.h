#ifndef MDMOBJPARA_H
#define MDMOBJPARA_H

#include "MDMObjFactory_Global.h"
#include <QString>

class CMDMObjParaImp;
class MDMOBJFACTORYSHARED_EXPORT CMDMObjPara
{
public:
    CMDMObjPara();
    CMDMObjPara(const QString& pid, const QString& type);
    CMDMObjPara(const QString& pid, const QString& type, const QString& group, const QString& comment);
    CMDMObjPara(const CMDMObjPara& obj);
    CMDMObjPara& operator = (const CMDMObjPara& obj);
    ~CMDMObjPara();
public:
    QString pid() const;
    void setPid(const QString& pid);
    QString pidToPath() const;
    static QString pidToPath(const QString& pid);
public:
    QString type() const;
    void setType(const QString& type);
public:
    QString name_from_pid_type() const;
public:
    QString group() const;
    void setGroup(const QString& group);
    QString comment() const;
    void setComment(const QString& comment);
public:
    QString name() const;
    void setName(const QString& name);
public:
    void* ptr() const;
    void setPtr(void* ptr);
protected:
    CMDMObjParaImp* m_data;
};

#endif // MDMOBJPARA_H
