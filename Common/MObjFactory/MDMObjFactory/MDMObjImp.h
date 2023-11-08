#ifndef MDMOBJIMP_H
#define MDMOBJIMP_H

#include "MDMObjPara.h"
#include "../../../include/MDSysLogExpert/MDSysLogger.h"

class CMDMObjFactory;
class CMDMObjImp
{
public:
    CMDMObjImp();
    CMDMObjImp(const CMDMObjPara& para, CMDMObjFactory* factory);
    ~CMDMObjImp();
    CMDMObjPara m_para;
    CMDMObjFactory* m_factory;
};

class CMDMObjParaImp
{
public:
    CMDMObjParaImp();
    CMDMObjParaImp(const QString& pid, const QString& type, const QString& group, const QString& comment);
    CMDMObjParaImp(const CMDMObjParaImp& obj);
    CMDMObjParaImp& operator = (const CMDMObjParaImp& obj);
    ~CMDMObjParaImp();
    bool copy(const CMDMObjParaImp* source);
    QString m_name;
    QString m_pid;
    QString m_type;
    QString m_group;
    QString m_comment;
    void* m_ptr;
};

#endif // MDMOBJIMP_H
