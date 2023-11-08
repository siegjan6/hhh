/*!
author enli 2015.01.08
\section 实时数据库管理类（加载/卸载项目配置，仿真，读写变量，变量变化通知，运行/停止）
*/
#ifndef CMDRTDBMANAGER_H
#define CMDRTDBMANAGER_H

#include "IMDRTDBManager.h"
#include "MDRTDBSimulatiorEngine.h"
#include "MDVariableQueueManager.h"
#include "MDRTDBDataExchangeEngine.h"
#include "MDSystemVariableTimer.h"
#include <QDomDocument>
#include <QDomElement>
#include "MDSwitchFactory/MDSwitchFactory.h"
#include "MDSwitchFactory/MDFuncSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include <QUaServer>
#include <QProcess>
class  CMDRTDBManager:public IMDRTDBManager
{
   // Q_OBJECT
public:
    CMDRTDBManager();
    ~CMDRTDBManager();
/*!
\section 项目管理
*/
public:
    //!项目
    QString projectPath();
    void setProjectPath(const QString &path);
    void loadProject(const QString &path);
    void unLoadProject();

    //!读写配置
    void saveVariableGroupConfiguration();
    void readVariableGroupConfiguration();
    void saveDeviceConfiguration();
    void readDeviceConfiguration();
private:
    void analyzeConfiguration(QDomElement&domElement );
    void createSystemVariableGroup();
    //写系统变量
    void writeSystemValue();
    //项目start后写初始值
    void writeInitValue();
private:
    QString  m_projectPath;
    QString  m_rtdbPath;

    QString  m_vhash;

/*!
\section 变量管理,读写
*/
public:
    bool readVariableValue(IMDVariable *var);
    QVariant readVariableValue(const QString &varName);
    bool writeVariableValue(const QString &varName,QVariant varValue);
    bool writeVariables(QList<IMDVariable*>& nsVarList);    
/*!
\section 变量安全策略
*/
public:
    CMDActionSPList *variableActionSPList(const QString &varName);
/*!
\section 运行控制
*/
public:
    bool isRunning();
    void start();
    void stop();
/*!
\section 变量仿真
*/
public:
    bool simulatiorIsRunning();
    void startSimulatior();
    void stopSimulatior();
private:
    bool  m_isRunning;
    bool  m_simulatiorIsRunning;
/*!
\section 变量管理器
*/
public:

    //变量管理器
    IMDVariableManager *variableManager();
    //中间变量管理器
    IMDVariableManager *middleVariableManager();
    //变量组管理器
    IMDVariableGroup *variableGroup();
    //驱动管理器
    IMDDriverManager  *driverManager();
    //变量归档管理器
    IMDVariableArchivesManager *variableArchivesManager();
    //启用安全策略的变量管理器
   // IMDVariableManager *middleVariableManager();
    QUaServer *uaServer() override;

    void setUaServer(QUaServer* server) override;

private:
    //变量管理器
    IMDVariableManager *m_mdVariableManager;
    //中间变量管理器
    IMDVariableManager *m_mdMiddleVariableManager;
    //变量组管理器
    IMDVariableGroup * m_mdVariableGroup;
    //驱动管理器
    IMDDriverManager *m_mdDriverManager;
     //变量归档管理器
    IMDVariableArchivesManager *m_mdVariableArchivesManager;
private:
     CMDRTDBSimulatiorEngine m_mdRTDBSimulatiorEngine;
     CMDSystemVariableTimer m_systemVariableTimer;

     QUaServer* m_pServer;

/*!
\section 变量变化通知
*/
public:
     void notifyNewChangeVariable(IMDRTDBVariable *changedVar,const VariableChangeType &varChangeType);
     void setVariableChangeNotifier(IMDVariableChangeNotifier *notifier);
     //!变量有初始化值为上次保存值时，项目退出时保存配置
     void enableSaveConfigBeforeUnLoad();
private:
     IMDVariableChangeNotifier *m_variableChangeNotifier;
private:
     void initialMiddleVariableManager();
     void middleVariableDoExpression();
     //!有初始化变量为上次保存值时，退出前保存变量值
     bool m_saveVariableConfigBfrQuit;
/*!
section 多语言
*/
public:
     void ChangeLanguage();
 /*!
\section 运行模式
*/
public:
     // Runtime模式:默认为编辑模式
     bool isRuntimeMode();
     void setRuntimeMode(bool isRuntime);

     bool isVirtualRunMode();
     void setVirtualRunMode(bool isVirtualRunMode);
private:
    bool m_isRuntimeMode;
    bool m_isVirtualRunMode;

/*!
\\section 运行时动态添加设备
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
    void  dynamicLoadDeviceParamter(const QString &deviceID,
                                    const QVariantList &deviceDriverList,
                                    const QVariantList &deviceVarList);

    /*!
     动态删除设备变量
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    void  dynamicDeleteDeviceVariable(const QString &deviceID);

    /*!
     动态添加设备变量
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    void  dynamicAddDeviceVariable(const QString &deviceID,const QVariantList &deviceVarList);
/*!
\\section 关联数据块存在变量变化时通知实时数据管理器
*/
public:
    /*!
     设备多个变量变化通知
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
     void  notifyDeviceVariablesChange(const QString& device,const QList<QByteArray>& jsonMsgList);
     void  notifyDeviceVariablesWithHmset(const QString& device,const QList<QByteArray>& redisCmdList);

     void notifyDeviceVariablesToKVCache(QStringList& varNameList, QList<QVariant>& varValueList);
/*!
\section 开关
*/
public:
     //!获取开关状态
     bool getSwitchState(MDLogicSwitchType swType);
     //!日志开关
     CMDLogSwitch* m_logSwitch;
private:
     //!中间变量，设计表达式及计算
     CMDSwitch* m_middleVarSwitch;
     //!变量归档
     CMDSwitch* m_varArchiveSwitch;   
     //!单写变量
     CMDSwitch* m_writeSingleVarSwitch;
     //!批量写变量
     CMDSwitch* m_writeMultiVarsSwitch;
     //!动态添加设备
     CMDSwitch* m_dynamicAddDeviceSwitch;
     //!动态添加变量
     CMDSwitch* m_dynamicAddVarSwitch;
     //!启动动态添加的设备
     CMDSwitch* m_dynamicStartDeviceSwitch;
     //!变量变化通知
     CMDSwitch* m_notifyVarChangeSwitch;
     //!设备变量变化通知
     CMDSwitch* m_notifyDeviceVarChangeSwitch;


     //!创建所有开关
     void createAllSwitches();
     //!释放所有开关
     void releaseAllLogicSwitches();

     QDateTime getUpTime();
};

#endif // CMDRTDBMANAGER_H
