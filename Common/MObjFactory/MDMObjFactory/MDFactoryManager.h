#ifndef MDFACTORYMANAGER_H
#define MDFACTORYMANAGER_H

#include <QMap>
#include <QMutex>
#include "MDMObjPara.h"

class CMDMObjFactory;
class CMDMObj;
class CMDFactoryManager
{
public:
    CMDFactoryManager();
    ~CMDFactoryManager();
    CMDMObj* createMObj(CMDMObjFactory* staticFactoryInstance, const CMDMObjPara& para);
    CMDMObjFactory* createFactory(CMDMObjFactory* staticFactoryInstance, const CMDMObjPara& para);
    CMDMObjFactory* findFactory(const CMDMObjPara& para);
public:
    void deleteSome(const QString& pid);
    void deleteAll();
    void enumAll(QList<CMDMObjFactory*>& fList);
    void saveAll();
protected:
    QMap<QString, CMDMObjFactory*> m_fMap;
    QMutex m_mutex;
};

#endif // MDFACTORYMANAGER_H
