/*!
\author zyb 2015.01.08
\section 实时数据库管理类接口
*/
#ifndef IMDRTDBMANAGER_H
#define IMDRTDBMANAGER_H

#include <QAbstractItemModel>
#include "IMDRTDBManager.h"
#include "IMDVariableGroup.h"
#include "IMDVariableManager.h"
#include "../../include/MDDriverInterfaces/IMDDriverManager.h"
#include "IMDDeviceManager.h"
#include "IMDChangeVariable.h"
#include "IMDTreeModel.h"
#include "IMDVariableChangeNotifier.h"
#include "../../include/MDVariableArchives/IMDVariableArchivesManager.h"

class CMDSwitch;
class QUaServer;
class IMDRTDBManager
{
public:
    virtual ~IMDRTDBManager(){}
 /*!
section 项目管理
*/
public:
    virtual QString projectPath()=0;
    virtual void setProjectPath(const QString &path)=0;
    virtual void loadProject(const QString &path)=0;
    virtual void unLoadProject()=0;
    virtual void saveVariableGroupConfiguration()=0;
    virtual void readVariableGroupConfiguration()=0;
    virtual void saveDeviceConfiguration()=0;
    virtual void readDeviceConfiguration()=0;
/*!
section 变量管理
*/
public:
    virtual bool readVariableValue(IMDVariable *var)=0;
    virtual QVariant readVariableValue(const QString &varName)=0;
    /*!
    section 写变量
    */
    virtual bool writeVariableValue(const QString &varName,QVariant varValue)=0;
    //!批量写变量，DriverManager释放变量对象
    virtual bool writeVariables(QList<IMDVariable*>& nsVarList)=0;
    /*!
    section 变量变化通知
    */
    virtual void setVariableChangeNotifier(IMDVariableChangeNotifier *notifier)=0;
/*!
section 变量管理器
*/
public:
    //!变量管理器
    virtual IMDVariableManager *variableManager()=0;
    //!变量组管理器
    virtual IMDVariableGroup *variableGroup()=0;
    //!驱动管理器
    virtual IMDDriverManager  *driverManager()=0;
    //!变量归档管理器
    virtual IMDVariableArchivesManager *variableArchivesManager()=0;
    //!变量模拟器
public:
    virtual bool simulatiorIsRunning()=0;
    virtual void startSimulatior()=0;
    virtual void stopSimulatior()=0;
/*!
section 运行控制
*/
public:
    virtual bool isRunning()=0;
    virtual void start()=0;
    virtual void stop()=0;
/*!
section 变量安全策略
*/
public:
   virtual  CMDActionSPList *variableActionSPList(const QString &varName)=0;
/*!
section 中间变量
*/
public:
    //!中间变量管理器
    virtual IMDVariableManager *middleVariableManager()=0;
/*!
section 运行模式
*/
public:
    // !Runtime模式:默认为编辑模式
    virtual bool isRuntimeMode()=0;
    virtual void setRuntimeMode(bool isRuntime)=0;

    virtual bool isVirtualRunMode()=0;
    virtual void setVirtualRunMode(bool isVirtualRunMode)=0;


/*!
\section 运行时动态添加设备
*/
public:
    /*!
     动态添加修改设备参数
     \param deviceID 设备ID。
     \param deviceDriverList 设备驱动配置链表。
     \param deviceDriverList 设备变量配置链表。
     \returns 无。
     \note
     */
    virtual void  dynamicLoadDeviceParamter(const QString &deviceID,
                                    const QVariantList &deviceDriverList,
                                    const QVariantList &deviceVarList)=0;

/*!
\section OPC UA
*/
public:
        virtual QUaServer *uaServer() = 0;
        virtual void setUaServer(QUaServer* server) = 0;
};

#endif // IMDRTDBMANAGER_H
