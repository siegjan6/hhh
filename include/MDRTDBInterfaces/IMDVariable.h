/*!
\author enli 2015.01.08
\section 所有变量类的基类，写变量也基于此类型
*/
#ifndef IMDVARIABLE_H
#define IMDVARIABLE_H

#include <QDateTime>
#include <qvariant.h>
#include "../../include/interface/Common/IMDDataType.h"

//变量数据品质
enum class MDVariableDataQuality
{
    None = 0,                                           //没有通信或者初始化
    Good = 192,                                         //正确
    AddressError = 260,                                 //地址错误
    ConnectDeviceFailed = 792,                          //无法与设备建立通信
    TimeoutError = 24,                                  //命令超时
    ProtocolError = 12,                                 //通信协议错误
    DataTypeError = 516,                                //数据类型错误
    StopCommunication= 1024,                            //停止设备通信
    DataBlockConfigError = 2048,                        //数据块配置错误
    DataConvertError = 4192,
};

//!变量数据归档规则
enum class  MDRTDataSaveRuler
{
    None = 0,                     //不存储
    DataChanged = 1,              //数据变化
    Cycle = 2,                    //周期存储
};
enum class  MDRTInitMode
{
    None = 0,                     //无
    LastValue = 1,                //保存值
    FixedValue = 2,               //指定值
};
class IMDVariable
{    
public:
    virtual ~IMDVariable(){}
public:
    //!名称
    virtual  QString name()=0;
    virtual  void setName(QString name)=0;

    //!单位
    virtual  QString getUnit() = 0;
    virtual  void setUnit(QString unit) = 0;

    //!描述
    virtual  QString getDescription() = 0;
    virtual  void setDescription(QString description) = 0;

    //!扩展
    virtual  QString getExt() = 0;
    virtual  void setExt(QString ext) = 0;

    //!数据类型
    virtual  MDDataType dataType()=0;
    virtual  void setDataType(MDDataType dataType)=0;

    //!当前值
    virtual  QVariant currentValue()=0;
    virtual  void setCurrentValue(QVariant value)=0;

    //!上次的值
    virtual  QVariant lastValue()=0;
    virtual  void setLastValue(QVariant value)=0;

    //!当前品质
    virtual MDVariableDataQuality  currentQuality()=0;
    virtual  void setCurrentQuality(MDVariableDataQuality quality)=0;

    //!上次的品质
    virtual MDVariableDataQuality  lastQuality()=0;
    virtual  void setLastQuality(MDVariableDataQuality quality)=0;

    //!时间戳
    virtual  QDateTime timeStamp()=0;
    virtual  void setTimeStamp(QDateTime timeStamp)=0;

    //!归档属性
    virtual  MDRTDataSaveRuler dataSaveRuler()=0;

    virtual void* uaBaseDataVariable(){return NULL;}
    virtual void setUaBaseDataVariable(void* pUaBaseDataVariable){Q_UNUSED(pUaBaseDataVariable);}
};

#endif // IMDVARIABLE_H
