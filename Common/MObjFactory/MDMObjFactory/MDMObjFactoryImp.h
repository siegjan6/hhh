#ifndef MDMOBJFACTORYIMP_H
#define MDMOBJFACTORYIMP_H

#include <QList>
#include <QMutex>
#include "MDMObjPara.h"
#include "MDFactoryFile.h"

class CMDMObj;

class CMDMObjFactoryImp
{
public:
    CMDMObjFactoryImp(CMDMObj* theMObj);
    CMDMObjFactoryImp(const CMDMObjPara& para, CMDMObj* theMObj);
    ~CMDMObjFactoryImp();
    bool add(CMDMObj* mobj);
    bool delete1(CMDMObj* mobj);
    void deleteAll();
    void enumAll(QList<CMDMObj*>& sList);
    int count();
    CMDMObj* find(int index);
    CMDMObj* findByGroup(const QString& group);
public:
    bool save(CMDMObjFactory* f);
    bool load(CMDMObjFactory* f);
    QString groupValue(const QString& group);
    CMDFactoryFile m_fFile;
public:
    CMDMObjPara m_para;
    QList<CMDMObj*> m_mobjList;
    QMutex m_mutex;
public:
    CMDMObj* m_theMObj;
};

#endif // MDMOBJFACTORYIMP_H
