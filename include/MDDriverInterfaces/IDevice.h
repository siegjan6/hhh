/*!
\author zyb 2015.01.08
\section 驱动设备接口类。IMDTreeItem：数据库树列表节点
*/
#ifndef IDEVICE_H
#define IDEVICE_H
#include <QList>
#include <QString>
#include <QObject>
#include <QByteArray>
#include "IMDTreeItem.h"
#include "IDataBlock.h"
#include "ICommunicationComponent.h"
class CPropertyBrowser;
class IDriver;
class IMDRTDBManagerUI;

const QEvent::Type NameChangedEvent = (QEvent::Type)1000;

class IDevice:public IMDTreeItem,public QObject
{
public:
    virtual ~IDevice(){}
public:    
    /*!
    \section 设备基础属性
    */
    //!设备名称
    virtual QString name()=0;
    virtual void setName(const QString &  name)=0;
    //!描述
    virtual QString description()=0;
    virtual void setDescription(const QString &  description)=0;
    //!使能
    virtual bool isEnabled()=0;
    virtual void setEnabled(bool enabled)=0;

    /*!
    \section 设备统计所有数据块的通信参数
    */
    //!发送次数
    virtual qint64 transmitCount()=0;
    //!接收次数
    virtual qint64 receiveCount()=0;
    //!超时次数
    virtual qint64 timeoutCount()=0;
    //!重试次数
    virtual qint64 retryCount()=0;
    //!错误次数
    virtual qint64 errorCount()=0;
    //!通信时间属性
    virtual QDateTime lastReadTime()=0;
    virtual QDateTime lastWriteTime()=0;
    virtual QDateTime lastErrorTime()=0;

    /*!
    \section 设置父节点
    */
    virtual IDriver *driver()=0;
    virtual void setDriver(IDriver *driver)=0;

    /*!
    \section 数据块操作
    */
    virtual qint32 getDataBlockCount()=0;
    virtual bool addDataBlock(IDataBlock *db)=0;
    virtual bool removeDataBlock(const QString &  dbName)=0;
    virtual bool removeDataBlock(IDataBlock *db)=0;
    virtual void removeAllDataBlocks()=0;
    virtual IDataBlock *getDataBlockAt(qint32 index)=0;
    virtual IDataBlock *getDataBlock(const QString &  dbName)=0;
    virtual bool findDataBlock(const QString &  dbName)=0;
    //!配置设备，弹出对话框
    virtual void configDecice(void *parent = 0)=0;
    virtual IDataBlock *createDataBlock(void *parent = 0,bool config = true)=0;
    virtual bool configDataBlock(IDataBlock *dataBlock,void *parent = 0)=0;
    virtual CommunicationType commType()=0;
    virtual void setCommType(CommunicationType commType)=0;
    virtual QString commParameter()=0;
    virtual void setCommParameter(const QString &  commParameter)=0;
    //!获取当前通信参数 by DataBlock
    virtual QString getCurCommParameter()=0;
    /*!
    \section 变量操作
    */
    //!添加变量
    virtual bool addVariable(IMDRTDBVariable* nsVar,QString &error)=0;
    //!移除变量
    virtual bool removeVariable(IMDRTDBVariable* nsVar)=0;
    //!验证变量地址
    virtual bool checkVarAddress(IMDRTDBVariable* nsVar,QString &error)=0;
    //!复制粘贴
    virtual IDataBlock* copyPasteDataBlock(IDataBlock* db) = 0;
    //!拷贝
    virtual void copy(IDevice*otherDevice)= 0;
    //!获取可用的变量地址链表
    virtual void getVariableAddressList(QStringList &varAddressList)= 0;
    //!移除所有变量
    virtual bool removeAllVariables()=0;
    /*!
    \section 属性浏览框
    */
    virtual void setPropertyBrowse(CPropertyBrowser* propBrowser) = 0;
    virtual void setRTDBManagerUI(IMDRTDBManagerUI* RTDBUI) = 0;
/*!
\section 运行时动态添加设备
*/
public:

    /*!
     动态设置参数
     \param deviceParameterMap 设备参数。
     \returns 无。
     \note
     */
    virtual void dynamicSetParameter(const QVariantMap &deviceParameterMap)=0;
};

#endif // IDEVICE_H
