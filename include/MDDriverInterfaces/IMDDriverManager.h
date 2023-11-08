/*!
\author zyb 2015.01.08
\section 驱动管理接口类 调用by MDRTDBManager IMDTreeItem是树列表的根节点
*/
#ifndef IMDDRIVERMANAGER_H
#define IMDDRIVERMANAGER_H
#include <QString>
#include <QList>
#include <QMutex>
#include "IDevice.h"
#include "IMDRTDBVariable.h"
#include "IMDDriverInformation.h"
class CPropertyBrowser;
class IMDRTDBManagerUI;

class IMDDriverManager:public IMDTreeItem
{
public:
    virtual ~IMDDriverManager(){}

    /*!
    \section 项目相关
    */
public:
    virtual void setProjectPath(const QString & projectPath) = 0;
    //!设置属性浏览框
    virtual void setPropertyBrowse(CPropertyBrowser* propBrowser) = 0;
    //!属性框信息同步通知
    virtual void setRTDBManagerUI(IMDRTDBManagerUI* RTDBUI) = 0;
    /*!
    \section 驱动相关
    */
public:
    //!获取驱动信息
    virtual IMDDriverInformation* getDriverInformationAt(qint32 index) = 0;
    //!获取驱动信息
    virtual IMDDriverInformation *getDriverInformation(const QString & driverID) = 0;
    //!获取驱动信息个数
    virtual qint32 getDriverInformationCount() = 0;

    /*!
    \section 设备相关
    */
public:
    //!添加设备
    virtual bool addDevice(QString name,QString description,IMDDriverInformation * driverInformation,QString&error) = 0;
    virtual bool addDevice(IDevice* device) = 0;
    //!移除设备
    virtual bool removeDevice(IDevice* device) = 0;
    virtual bool removeDevice(const QString & devName) = 0;
    //!删除所有设备
    virtual void removeAllDevice() = 0;
    //!获取设备
    virtual IDevice* getDeviceAt(int index) = 0;
    //!获取设备
    virtual IDevice *getDevice(const QString & devName) = 0;
    //!获取设备个数
    virtual int getDeviceCount() = 0;
    //!复制粘贴
    virtual IDevice* copyPasteDevice(IDevice* device) = 0;

    /*!
    \section 运行相关
    */
public:
    virtual void initial()=0;
    virtual void release()=0;
    virtual void startDevice(const QString & devName)=0;
    virtual void startAllDevices()=0;
    virtual void stopDevice(const QString & devName)=0;
    virtual void stopAllDevices()=0;
    //virtual bool checkValidVarAddress(IDevice *device,QString varAddress,MDDataType dataType,string resgister,QString StartAddr,QString endAddr,QString addressList)=0;
    //!数据块的数据包写文件
    virtual void monitorDataBlock(bool bEnable,int dataBlockID,QString devName,QString dbName,int nFlag)=0;
    //!批量快写变量
    virtual bool asyncWriteVariables(QList<IMDRTDBVariable*>& nsVarList)=0;
    //!单个写变量
    virtual bool asyncWriteVariable(IMDRTDBVariable *nsVar,QVariant data)=0;
public:
    //!读写配置
    virtual void saveConfiguration(const QString &configFile)=0;
    virtual void readConfiguration(const QString &configFile)=0;
public:
    //!移除所有变量
    virtual bool removeAllVariables()=0;
    /*!
    \section 运行模式
    */
public:
    //!Runtime模式:默认为编辑模式
    virtual bool isRuntimeMode()=0;
    virtual void setRuntimeMode(bool isRuntime)=0;

    virtual bool isVirtualRunMode()=0;
    virtual void setVirtualRunMode(bool isVirtualRunMode) = 0;

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
    virtual void  dynamicDeleteDevice(const QString &deviceID)=0;

    /*!
     动态添加设备变量
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    virtual void  dynamicAddDevice(const QString &deviceID,const QVariantList &deviceDriverList)=0;
};

#endif // IMDDRIVERMANAGER_H
