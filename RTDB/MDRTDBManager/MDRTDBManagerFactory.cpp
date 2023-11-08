#include "MDRTDBManagerFactory.h"
#include "MDRTDBManager.h"
#include "MDChangeVariable.h"
#include "MDRTDBVariableUaBase.h"
#include "MDDeviceRunTreeModel.h"
#include "MDVariableRunTreeModel.h"
#include "MDDeviceTreeModel.h"
#include "MDVariableTreeModel.h"
#include "MDRTDBVariable.h"

IMDRTDBManager* CMDRTDBManagerFactory::p_RTDBManagerSingleInstance = nullptr;
QMutex CMDRTDBManagerFactory::m_mutex;

CMDRTDBManagerFactory::CMDRTDBManagerFactory()
{

}

CMDRTDBManagerFactory::~CMDRTDBManagerFactory()
{

}

//! 总管理类
IMDRTDBManager *CMDRTDBManagerFactory::createRTDBManager()
{
	//CMDRTDBManager *pRTDBManager = new CMDRTDBManager();
	//p_RTDBManagerSingleInstance = (IMDRTDBManager*)pRTDBManager;
	//return (IMDRTDBManager*)pRTDBManager;

	if (p_RTDBManagerSingleInstance == nullptr)
	{
		QMutexLocker locker(&m_mutex);
		if (p_RTDBManagerSingleInstance == nullptr)
		{
			p_RTDBManagerSingleInstance = new CMDRTDBManager();
		}
	}

	// "double checked lock"
	//if (p_RTDBManagerSingleInstance == nullptr) {
	//	std::lock_guard<std::mutex> lk(m_mutex);
	//	if (p_RTDBManagerSingleInstance == nullptr) {
	//		p_RTDBManagerSingleInstance = std::shared_ptr<IMDRTDBManager>(new CMDRTDBManager);
	//	}
	//}


	return p_RTDBManagerSingleInstance;
}

//! 变化变量
IMDChangeVariable *CMDRTDBManagerFactory::createChangeVariable()
{
    return new CMDChangeVariable();
}
//! 基本变量（用于写变量）
IMDVariable *CMDRTDBManagerFactory::createVariable()
{
	//if(p_RTDBManagerSingleInstance == nullptr)
        return new CMDRTDBVariableT();
	//else
	//{
	//	return p_RTDBManagerSingleInstance->variableManager()->createVariable();
	//}
}
//! 运行时变量管理控件
IMDTreeModel *CMDRTDBManagerFactory::createVariableRunTreeModel(IMDRTDBManager *nsRTDBManager)
{
    if(nsRTDBManager==NULL)
    {
        return NULL;
    }
    CMDVariableRunTreeModel *treeModel = new CMDVariableRunTreeModel();
    treeModel->initial(nsRTDBManager->variableGroup());
    return treeModel;
}
//! 运行时设备管理控件
IMDTreeModel *CMDRTDBManagerFactory::createDeviceRunTreeModel(IMDRTDBManager *nsRTDBManager)
{
    if(nsRTDBManager==NULL)
    {
        return NULL;
    }
    CMDDeviceRunTreeModel *treeModel = new CMDDeviceRunTreeModel();
    treeModel->initial(nsRTDBManager->driverManager());
    return treeModel;
}

//! 编辑时变量管理控件
IMDTreeModel *CMDRTDBManagerFactory::createVariableTreeModel(IMDRTDBManager *nsRTDBManager)
{
    if(nsRTDBManager==NULL)
    {
        return NULL;
    }
    CMDVariableTreeModel *treeModel = new CMDVariableTreeModel();
    treeModel->initial(nsRTDBManager->variableGroup());
    return treeModel;
}
//! 编辑时设备管理控件
IMDTreeModel *CMDRTDBManagerFactory::createDeviceTreeModel(IMDRTDBManager *nsRTDBManager)
{
    if(nsRTDBManager==NULL)
    {
        return NULL;
    }
    CMDDeviceTreeModel *treeModel = new CMDDeviceTreeModel();
    treeModel->initial(nsRTDBManager->driverManager());
    return treeModel;
}
 //! 编辑时设备地址浏览控件
IMDTreeModel *CMDRTDBManagerFactory::createBrowseVariableTreeModel(IMDRTDBManager *nsRTDBManager)
{
    if(nsRTDBManager==NULL)
    {
        return NULL;
    }
    CMDVariableTreeModel *treeModel = new CMDVariableTreeModel();
    treeModel->setCheckedable(true);
    treeModel->initial(nsRTDBManager->variableGroup());
    return treeModel;
}

