#ifndef CMDRTDBMANAGERFACTORY_H
#define CMDRTDBMANAGERFACTORY_H

#include "MDRTDBManager_global.h"
#include "IMDRTDBManager.h"
#include "IMDChangeVariable.h"
#include <QMutex>

class MDRTDBMANAGERSHARED_EXPORT CMDRTDBManagerFactory
{
public:
	//typedef std::shared_ptr<IMDRTDBManager> RTDBSharedPtr;
    CMDRTDBManagerFactory();
    ~CMDRTDBManagerFactory();
public:
   static IMDRTDBManager *createRTDBManager();
   static IMDChangeVariable *createChangeVariable();
   static IMDVariable *createVariable();
   static IMDTreeModel *createVariableRunTreeModel(IMDRTDBManager *nsRTDBManager);
   static IMDTreeModel *createDeviceRunTreeModel(IMDRTDBManager *nsRTDBManager);
   static IMDTreeModel *createVariableTreeModel(IMDRTDBManager *nsRTDBManager);
   static IMDTreeModel *createDeviceTreeModel(IMDRTDBManager *nsRTDBManager);
   static IMDTreeModel *createBrowseVariableTreeModel(IMDRTDBManager *nsRTDBManager);

private:
	static IMDRTDBManager* p_RTDBManagerSingleInstance;
	static QMutex m_mutex;
};
//CMDRTDBManagerFactory::RTDBSharedPtr CMDRTDBManagerFactory::p_RTDBManagerSingleInstance = nullptr;
//std::mutex CMDRTDBManagerFactory::m_mutex;

#endif // CMDRTDBMANAGERFACTORY_H
