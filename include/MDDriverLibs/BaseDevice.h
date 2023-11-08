/*!
\author enli 2015.01.08
\section 设备实现基类，由DriverManager管理
*/
#ifndef BASEDEVICE_H
#define BASEDEVICE_H

#include <QString>
#include <QMutex>
#include <QObject>
#include <QMap>
#include <QDomDocument>

#include "MDDriverLibs_global.h"
#include "IDriver.h"
#include "IMDDriverManager.h"
#include "IDataBlock.h"
#include "BaseChannel.h"
#include "BaseDataBlock.h"
#include "IDevice.h"
#include "SortJsonObject.h"

class IMDRTDBManagerUI;
class MDDRIVERLIBSSHARED_EXPORT CBaseDevice:public IDevice
{
    Q_OBJECT
    Q_ENUMS(MDCommunicationTypeProp)
    Q_PROPERTY(QMap translateName READ translateName)
    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(bool Enable READ isEnabled WRITE setEnabled)
    Q_PROPERTY(QString Name READ name WRITE setNameProp)
    Q_PROPERTY(QString Description READ description WRITE setDescription)    

public:
    CBaseDevice();
    ~CBaseDevice();

    enum class MDCommunicationTypeProp
    {
        Serial = 0,     //串口通信
        TCP = 1,        //TCP通信
        UDP = 2,        //UDP通信
        USB = 3,		//USB
        Board = 4,      //板卡
        Other = 5,	    //其它
    };
/*!
\section IMDTreeItem接口
*/
public:
    MDTreeItemType itemType();
    void setItemType(MDTreeItemType itemType);
    void *parentItem(MDTreeItemType parentItemType);
    void setParentItem(void* parent,MDTreeItemType parentItemType);
private:
    MDTreeItemType m_itemType;
    IMDDriverManager *m_driverManager;

/*!
\section 属性浏览框
*/
public:
    void setPropertyBrowse(CPropertyBrowser* propBrowser);
    CPropertyBrowser* getPropertyBrowse();
    void setRTDBManagerUI(IMDRTDBManagerUI* RTDBUI);
    IMDRTDBManagerUI* getRTDBManagerUI();

private:
     CPropertyBrowser* m_propBrowser;
     IMDRTDBManagerUI* m_RTDBUI;

/*!
\section 基本属性
*/
public:
    //!名称
    QString name();
    void setName(const QString & name);
    //!名称属性，加载后台数据时无需检验重复性，无弹出提示对话框
    void setNameProp(const QString & name);
    //!描述
    QString description();
    void setDescription(const QString & description);

    //!关联Driver
    IDriver *driver();
    void setDriver(IDriver *driver);
    //!关联Channel
    CBaseChannel *channel();
    void setChannel(CBaseChannel *channel);
    //!启用状态
    bool isEnabled();
    void setEnabled(bool enabled);
    //!超时时间（毫秒）
    quint32 replyTimeout();
    void setReplyTimeout(quint32 replyTimeout);

    //!命令重试次数
    quint32 retries();
    void setRetries(quint32 retries);
    //!动态设备ID
    QString dynamicDeviceID();
    void setDynamicDeviceID(const QString & deviceID);

    //!获取当前通信参数
        QString getCurCommParameter();
/*!
\section 拷贝
*/
public:
    virtual void copy(IDevice*otherDevice);
private:
     QString m_name;
     QString m_dynamicDeviceID;
     QString m_description;
     IDriver *m_driver;
     CBaseChannel *m_channel;
     quint32 m_replyTimeout;
     quint16 m_retries;
     bool m_enabled;
/*!
\section 数据块管理
*/
public:
    bool addDataBlock(IDataBlock *db);
    bool removeDataBlock(const QString &dbName);
    bool removeDataBlock(IDataBlock *db);
    void removeAllDataBlocks();
    qint32 getDataBlockCount();
    IDataBlock *getDataBlockAt(qint32 index);
    IDataBlock *getDataBlock(const QString & dbName);
    bool findDataBlock(const QString & dbName);
    IDataBlock *copyPasteDataBlock(IDataBlock* db);
private:
    QList<IDataBlock*> m_dataBlockList;
    QMutex      m_datablockListMutex;
/*!
\section 变量管理
*/
public:
   bool addVariable(IMDRTDBVariable* nsVar,QString &error);
   bool removeAllVariables();
   //!维护变量品质
   IMDRTDBVariable* m_qualityVariable;
/*!
\section 通信
*/
public:
    virtual bool start();
    virtual bool stop();
    void setupReadCommand(bool bForce = false);
    void onTimeout(bool retry);

    void resetStatistics();
    qint64 transmitCount();
    void   incrementTransmitCount();
    qint64 receiveCount();
    void   incrementReceiveCount();
    qint64 timeoutCount();
    void   incrementTimeoutCount();
    qint64 retryCount();
    void   incrementRetryCount();
    qint64 errorCount();
    void   incrementErrorCount();

    //!通信时间属性
    void setLastReadTime(QDateTime readTime);
    void setLastWriteTime(QDateTime writeTime);
    void setLastErrorTime(QDateTime errorTime);

    QDateTime lastReadTime();
    QDateTime lastWriteTime();
    QDateTime lastErrorTime();

    bool  isRunning();
    void notifyNotConnectDevice();
private:
    bool   m_running;
    QMutex m_objMutex;
    //!属性框组名及属性名翻译
    QMap<QString,QVariant> translateName(){return m_translateMap;}
    QMap<QString,QVariant>  m_translateMap;
protected:
    qint64 m_cmdTransimitCount;
    qint64 m_cmdReceiveCount;
    qint64 m_cmdTimeoutCount;
    qint64 m_cmdRetryCount;
    qint64 m_cmdErrorCount;

    QDateTime m_lastReadTime;
    QDateTime m_lastWriteTime;
    QDateTime m_lastErrorTime;
/*!
\section 具体驱动需要实现的函数
*/
public:
    /*/
    //创建数据块
    virtual IDataBlock *createDataBlock(void *parent = 0)=0;
    //配置数据块
    virtual bool configDataBlock(IDataBlock *dataBlock,void *parent = 0)=0;
    //配置设备
    virtual void configDecice(void *parent = 0)=0;
    //通信类型
    virtual CommunicationType commType()=0;
    virtual void setCommType(CommunicationType commType)=0;
    //通信参数（如串口：COM口，Baud等）
    virtual QString commParameter()=0;
    virtual void setCommParameter(const QString & commParameter)=0;
   /*/
    virtual void saveConfiguration(QDomDocument &doc,QDomElement &parentElement)=0;
    virtual void readConfiguration(QDomElement &domElement)=0;

    virtual void serialize(CSortJsonObject& json)=0;
    virtual void deserialize(const QJsonObject& json)=0;

//运行模式
public:
    //!Runtime模式:默认为编辑模式
    bool isRuntimeMode();


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
    virtual void dynamicSetParameter(const QVariantMap &deviceParameterMap);
};


/*
#include <QString>
#include "MDDriverLibs_global.h"
#include "IDriver.h"
#include "BaseChannel.h"
#include "BaseDataBlock.h"
#include "IDevice.h"
class MDDRIVERLIBSSHARED_EXPORT CBaseDevice:public IDevice
{
public:
    CBaseDevice();
    ~CBaseDevice();
//IMDTreeItem接口
public:
    MDTreeItemType itemType();
    void setItemType(MDTreeItemType itemType);
    void *parent(MDTreeItemType parentItemType);
    void setParent(void* parent,MDTreeItemType parentItemType);
private:
    MDTreeItemType m_itemType;
//关联变量
public:
//基本属性
public:
    //名称
    QString name();
    void setName(QString name);
    //关联Driver
    IDriver *getDriver();
    void setDriver(IDriver *driver);
    //关联Channel
    CBaseChannel *getChannel();
    void setChannel(CBaseChannel *channel);
    //启用状态
    bool getEnabled();
    void setEnabled(bool enabled);
    //超时时间（毫秒）
    quint32 getReplyTimeout();
    void setReplyTimeout(quint32 replyTimeout);
    //命令重试次数
    quint32 getRetries();
    void setRetries(quint32 retries);
private:
     QString m_name;
     IDriver *m_driver;
     CBaseChannel *m_channel;
     quint32 m_replyTimeout;
     quint16 m_retries;
     bool m_enabled;
//数据块管理
public:
    bool addDataBlock(CBaseDataBlock *db);
    bool removeDataBlock(QString dbName);
    bool removeDataBlock(CBaseDataBlock *db);
    void removeAllDataBlocks();
    int getDataBlockCount();
    CBaseDataBlock *getDataBlock(int iDBIndex);
    CBaseDataBlock *getDataBlock(QString dbName);
    bool findDataBlock(QString dbName);
private:
    QList<CBaseDataBlock*> m_dataBlockList;
    QMutex      m_datablockListMutex;
//通信控制
public:
    bool start();
    bool stop();
    void setupReadCommand();
    void onTimeout(bool retry);
private:
    bool   m_running;
protected:
    long m_cmdTransimitCount;
    long m_cmdReceiveCount;
    long m_cmdTimeoutCount;
    long m_cmdRetryCount;
    long m_cmdErrorCount;

//具体驱动需要实现的函数
public:
    //初始化参数
    virtual void initialParameters(IDevice *dev)=0;
};
*/

#endif // BASEDEVICE_H
