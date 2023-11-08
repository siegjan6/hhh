/*!
\author zyb 2015.01.08
\section 驱动数据块接口类。IMDTreeItem：方便数据库HMI树列表定位
*/
#ifndef IDATABLOCK_H
#define IDATABLOCK_H

#include <QDateTime>
#include <QList>
#include <QString>
#include <QObject>
#include <QEvent>
#include <QByteArray>
#include <QtWidgets/QDialog>

#include "IMDRTDBVariable.h"
#include "IMDTreeItem.h"
class IDataBlock:public IMDTreeItem,public QObject
{
public:
    virtual ~IDataBlock(){}
public:
    enum MDMonitorType
    {
        Binary = 0,
        ASCII = 1,
    };
    /*!
    \section 数据块基本属性
    */
    //!名字
    virtual QString name()=0;
    virtual void setName(QString name)=0;
    //!描述
    virtual QString description()=0;
    virtual void setDescription(QString description)=0;
    //!使能
    virtual bool isEnabled()=0;
    virtual void setEnabled(bool enabled)=0;
    //!通信品质
    virtual MDVariableDataQuality quality()=0;
    virtual void setQuality(MDVariableDataQuality quality)=0;

    /*!
    \section 通信属性
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
    \section 变量操作
    */
    //!添加变量
    virtual bool addVariable(IMDRTDBVariable* nsVar,QString &error)=0;
    //!移除变量
    virtual bool removeVariable(IMDRTDBVariable* nsVar)=0;
    //!验证变量地址有效性
    virtual bool checkVarAddress(IMDRTDBVariable* nsVar,QString &error)=0;
    //!删除所有变量
    virtual void removeAllVariable()=0;
    //!获取变量
    virtual IMDRTDBVariable* getVariableAt(qint32 index)=0;
    //!获取变量
    virtual IMDRTDBVariable *getVariable(const QString & varName)=0;
    //!获取变量数量
    virtual int getVariableCount()=0;
    //!写变量
    virtual bool asyncWriteVariable(IMDRTDBVariable *nsVar)=0;
    virtual bool asyncWriteVariables(QList<IMDRTDBVariable*>& nsVarList)=0;
    //!拷贝
    virtual void copy(IDataBlock*otherDataBlock)= 0;
    //!数据块扫描结束更新变量
    virtual void refreshVariables()= 0;
    //!品质变化时更新变量
    virtual void refreshVariablesQuality()= 0;
    //!数据包截取
    virtual bool msgMonitor()= 0;
    virtual void setMsgMonitor(bool monitor)= 0;
    virtual MDMonitorType monitorMode()=0;
    virtual void setMonitorMode(MDMonitorType monitorType)= 0;
    virtual void setMsgWindow(QDialog* dialog)=0;
    virtual void emitPacket(bool bSend, quint8 *dataBuf,  qint16 startIndex, qint16 length)=0;
};
/*!
section 数据块监控 事件
*/
class CPacketEvent : public QEvent
{
public:
    CPacketEvent(Type packetEventType):QEvent(packetEventType){}
    virtual ~CPacketEvent(){}
    QString m_DataBlockPacket;
};

#endif // IDATABLOCK_H
