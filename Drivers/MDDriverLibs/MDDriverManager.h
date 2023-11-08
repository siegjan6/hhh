/*!
\author enli 2015.01.08
\section 驱动管理实现类，由MDRTDBManager调用CMDDriverManagerFactory::createMDDriverManager()创建
*/
#ifndef MDDRIVERMANAGER_H
#define MDDRIVERMANAGER_H
#include <QLibrary>
#include <QMap>
#include <QMutexLocker>
#include <QMutex>
#include <QDomElement>
#include <QJsonDocument>
#include <QJsonParseError>

#include "IDriver.h"
#include "IDevice.h"
#include "IMDTreeItem.h"
#include "IMDRTDBVariable.h"
#include "IMDDriverManager.h"

#include "MDSwitchFactory/MDSwitchFactory.h"
#include "MDSwitchFactory/MDLogSwitchFactory.h"
#include "MDPropertyBrowser/PropertyBrowser.h"

class IMDRTDBManagerUI;

class  CMDDriverManager:public IMDDriverManager
{
public:
    CMDDriverManager();
    ~CMDDriverManager();
    /*!
    \section 项目相关
    */
public:
    void setProjectPath(const QString & projectPath);
    void setPropertyBrowse(CPropertyBrowser* propBrowser);
    void setRTDBManagerUI(IMDRTDBManagerUI* RTDBUI);
private:
    QString m_projectPath;
    //MDTreeItemType接口
public:
    /*!
    \section 实时数据库树列表跟节点
    */
    MDTreeItemType itemType();
    void setItemType(MDTreeItemType itemType);
    void *parentItem(MDTreeItemType parentItemType);
    void setParentItem(void* parent,MDTreeItemType parentItemType);
    /*!
    \section 关联设备
    */
public:
    //!添加设备
    bool addDevice(QString name,QString description,IMDDriverInformation * driverInformation,QString&error);
    bool addDevice(IDevice* device);
    //!移除设备
    bool removeDevice(IDevice* device);
    bool removeDevice(const QString & devName);
    //!删除所有设备
    void removeAllDevice();
    //!获取设备
    IDevice* getDeviceAt(int index);
    //!获取设备
    IDevice *getDevice(const QString & devName);
    //!获取设备个数
    qint32 getDeviceCount();
    //复制粘贴
    IDevice* copyPasteDevice(IDevice* device);
private:
    QList<IDevice*> m_deviceList;//设备链表
    QMutex m_deviceListMutex;
    CPropertyBrowser* m_propBrowser;
    IMDRTDBManagerUI* m_RTDBUI;
    /*!
    \section 驱动信息
    */
public:
    //!获取驱动息信
    IMDDriverInformation* getDriverInformationAt(int index);
    //!获取驱动信息
    IMDDriverInformation *getDriverInformation(const QString & driverID);
    //!获取驱动信息个数
    qint32 getDriverInformationCount();
private:
    //!初始化所有的驱动信息
    void initialAllDriverInformation();
    //!删除所有驱动信息
    void removeAllDriverInformation();
private:
    QList<IMDDriverInformation*> m_driverInformationList;//驱动信息链表
    QMutex m_driverInformationListMutex;
    /*!
    \section 驱动实例管理
    */
private:
     IDriver *addDriver(const QString &drvName);
     void removeDriver(const QString &drvName);
     IDriver *getDriver(const QString &drvName);
     QMap<QString,IDriver*> m_driversMap;//驱动实例集合
     QMutex   m_driversMapMutex;
     typedef IDriver* (*createDriverInstance)();
     /*!
     \section 初始化与释放接口
     */
public:
    void initial();
    void release();

    /*!
    \section 运行控制
    */
public:
    void startDevice(const QString &devName);
    void startAllDevices();
    void stopDevice(const QString &devName);
    void stopAllDevices();
    //!监控某个数据块收到的数据包
    void monitorDataBlock(bool bEnable,qint32 dataBlockID,QString devName,QString dbName,qint32 nFlag);

    /*!
    \section 读写变量
    */
public:
    bool asyncWriteVariables(QList<IMDRTDBVariable*>& nsVarList);
    bool asyncWriteVariable(IMDRTDBVariable *nsVar, QVariant data);
    /*!
    \section 读写配置
    */
public:
    void saveConfiguration(const QString &configFile);
    void readConfiguration(const QString &configFile);
public:
    //!移除所有变量
    bool removeAllVariables();

    /*!
    \section 运行模式
    */
public:
    //! Runtime模式:默认为编辑模式
    bool isRuntimeMode();
    void setRuntimeMode(bool isRuntime);

    bool isVirtualRunMode();
    void setVirtualRunMode(bool isVirtualRunMode);
private:
    bool m_isRuntimeMode;
    bool m_isVirtualRunMode;

/*!
\section 日志开关
*/
    CMDLogSwitch* m_logSwitch;

/*!
\section 运行时动态添加设备
*/
public:

    /*!
     动态删除设备变量
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    void  dynamicDeleteDevice(const QString &deviceID);

    /*!
     动态添加设备变量
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    void  dynamicAddDevice(const QString &deviceID,const QVariantList &deviceDriverList);
};
#endif // MDDRIVERMANAGER_H
