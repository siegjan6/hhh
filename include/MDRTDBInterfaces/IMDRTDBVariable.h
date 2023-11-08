/*!
\author zyb 2015.01.08
\section 驱动变量类。底层驱动刷新变量用
*/
#ifndef IMDRTDBVARIABLE_H
#define IMDRTDBVARIABLE_H

#include "IMDVariable.h"
#include "IMDTreeItem.h"
//!变量类型
enum  class MDVariableType
{
    VirtualVariable = 0,    //虚拟变量，内部变量
    IOVariable = 1,         //IO变量，设备变量
    MiddleVariable = 2,     //中间变量
    SystemVariable = 3,     //系统变量
};
//Q_ENUMS(MDVariableType)

/*chp move to IMDVariable.h
//!变量数据归档规则
enum class  MDRTDataSaveRuler
{
    None = 0,                     //不存储
    DataChanged = 1,              //数据变化
    Cycle = 2,                    //周期存储
};*/

//!变量数据归档模式
enum class  MDRTDataSaveMode
{
    Memory = 0,                         //内存存储模式
    DiskFile = 1,                          //磁盘文件存储模式
    Both = 2,                               //内存与磁盘文件存储模式
};

/*luyj move to IMDVariable.h
enum class  MDRTInitMode
{
    None = 0,                         //无
    LastValue = 1,                          //保存值
    FixedValue = 2,                               //指定值
};
*/

//!变量数据归档规则参数
struct MDRTDataSaveRulerParameter
{
    MDRTDataSaveMode m_saveMode;                //归档模式
    MDRTDataSaveRuler m_saveRuler;                  //变量数据归档规则
    int m_saveCycle;                                                //周期存储时的存储周期
    int m_timeRange;                                             //存储的时间范围(秒）
    const MDRTDataSaveRulerParameter& operator=( const MDRTDataSaveRulerParameter& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_saveMode= other.m_saveMode;
        m_saveRuler= other.m_saveRuler;
        m_saveCycle= other.m_saveCycle;
        m_timeRange= other.m_timeRange;
        return other;
    }
};


//!变量初始化参数
struct MDVariableInitializeParameter
{
    //bool m_enable;//启用初始化
    bool m_enableEvent;//启用初始化事件
    MDRTInitMode  m_initType;//初始化方式：0：无，1：保存值、2：指定值
    QString m_value;//指定值表达式

    const MDVariableInitializeParameter& operator=( const MDVariableInitializeParameter& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_enableEvent= other.m_enableEvent;
        m_initType= other.m_initType;
        m_value= other.m_value;
        return other;
    }
};
//!变量写保护参数
struct MDVariableWriteProtectParameter
{
    bool m_enableWrite;//启用写:true:可写、false：不可写
    bool m_enableWriteProtect;//启用写保护
    QString m_maxValue;//最大值：最大值用字符串主要是允许使用表达式
    QString m_minValue;//最小值 ：最小值用字符串主要是允许使用表达式
    int  m_writeProtectType;//写保护的模式：0:超限时放弃写；1:超限时使用限值
    const MDVariableWriteProtectParameter& operator=( const MDVariableWriteProtectParameter& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_enableWrite= other.m_enableWrite;
        m_enableWriteProtect= other.m_enableWriteProtect;
        m_maxValue= other.m_maxValue;
        m_minValue= other.m_minValue;
        m_writeProtectType= other.m_writeProtectType;
        return other;
    }
};
//!变量限值报警参数
struct MDVariableAlarmParameter
{
    bool m_enableAlarm;//使能: true:启用、false：禁用
    QString m_HHLValue;//上上限：上上限用字符串主要是允许使用表达式
    QString m_HLValue;//上限：上限用字符串主要是允许使用表达式
    QString m_LLLValue;//下下限：下下限用字符串主要是允许使用表达式
    QString m_LLValue;//下限：下限用字符串主要是允许使用表达式
    const MDVariableAlarmParameter& operator=( const MDVariableAlarmParameter& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_enableAlarm= other.m_enableAlarm;
        m_HHLValue= other.m_HHLValue;
        m_HLValue= other.m_HLValue;
        m_LLLValue= other.m_LLLValue;
        m_LLValue= other.m_LLValue;
        return other;
    }
};
//!变量品质错误数值处理参数
struct MDVariableQualityIsBadParameter
{
    int  m_type;//品质错误数值处理方式：0：保持原值、1：指定值
    QString m_value;//指定值表达式
};

//!变量工程转换类型
enum class  MDVariableDataConversionType
{
    None  = 0,                            //线性转换
    BCD3 = 1,                               //3位BCD码
    BCD4 = 2,                               //4位BCD码：数值为无符号的整数
    BCD8 = 3,                               //8位BCD码
    OMRONBCD4 = 4,                          //OMRON 4位BCD码：如果是有符号整数，最高4位二进制位为符号位
    Bit8BinaryWithoutAlarm = 5,             //8位二进制无报警
    Bit8BinaryWithAlarm = 6,                //8位二进制有报警
    Bit12BinaryWithoutAlarm = 7,            //12位二进制有报警
    Bit12BinaryWithAlarm = 8,               //12位二进制有报警
    Bit13BinaryWithoutAlarm = 9,           //13位二进制无报警
    Bit13BinaryWithAlarm = 10,              //13位二进制有报警
    Bit15BinaryWithoutAlarm = 11,           //15位二进制无报警
    Bit15BinaryWithAlarm = 12,              //15位二进制有报警
    ByteOrderChange = 13,                   //数据字节顺序转换
    SiemensBCD4 = 14,                       //Siemens 4位BCD码：如果是有符号整数，最高4位二进制位为符号位
};
//!变量工程转换参数
struct MDVariableDataConversionParameter
{
    bool m_enable;//启用数据转换
    MDVariableDataConversionType  m_conversionType; //数据转换类型
    bool m_enableLinearConversion;//启用线性转换：除了线性转换之外的转换同时启用线性转换
    double m_maxValue; //工程值最大值
    double m_minValue;//工程值最小值
    double m_originalMaxValue;//原始最大值
    double m_originalMinValue; //原始最小值
    QString m_byteOrder;//数据字节顺序:2-1-3-4
    const MDVariableDataConversionParameter& operator=( const MDVariableDataConversionParameter& other )
    {
        if ( this == &other )
        {
            return *this;
        }
        m_enable= other.m_enable;
        m_enableLinearConversion= other.m_enableLinearConversion;
        m_conversionType= other.m_conversionType;
        m_maxValue= other.m_maxValue;
        m_minValue= other.m_minValue;
        m_originalMaxValue= other.m_originalMaxValue;
        m_originalMinValue= other.m_originalMinValue;
        return other;
    }
};

//!功能逻辑开关。最好放到IMDRTDBManager.h中，考虑到DriverLib也需引用，放到这里。
enum class  MDLogicSwitchType
{
    Middle = 0,
    Archive = 1,
    SingleWrite = 2,
    MultiWrite = 3,
    DynamicAddDevice = 4,
    DynamicAddVar = 5,
    DynamicRunDevice = 6,
    NotifyVarChange = 7,
    NotifyDeviceVarChange = 8,
    Log = 9,
};
class CMDActionSPList;
class  IMDRTDBVariable:public IMDVariable,public IMDTreeItem
{
public:
    virtual ~IMDRTDBVariable(){}
public:
    //!不带Group的名
    virtual  QString localName()=0;
    virtual  void setLocalName(QString name)=0;
    //!数据精度
    virtual  int precision()=0;
    virtual  void setPrecision(int precision)=0;
    //!有效性
    virtual  bool isValid()=0;
    virtual  void setValid(bool isValid)=0;

    //!变量类型
    virtual MDVariableType variableType()=0;
    virtual void setVariableType(MDVariableType varType)=0;

    //!地址
    virtual  QString address()=0;
    virtual  void setAddress(QString address)=0;

    //!设备名称
    virtual  QString deviceName()=0;
    virtual  void setDeviceName(QString deviceName)=0;

    //驱动名称：当写时在MDDriverManager::AsyncWriteVariable（）中找到对应的驱动
    //virtual  QString getDriverName()=0;
    //virtual  void setDriverName(QString driverName)=0;

    //!原始值数据类型
    virtual  MDDataType originalDataType()=0;
    virtual  void setOriginalDataType(MDDataType dataType)=0;
    //!原始值
    virtual  QVariant originalValue()=0;
    virtual  void setOriginalValue(QVariant value)=0;

    //!关联数据块编号
    virtual  int dataBlockIndexID()=0;
    virtual  void setDataBlockIndexID(int dataBlockIndexID)=0;

    //!关联数据区地址
    virtual  int internalAddress()=0;
    virtual  void setInternalAddress(int internalAddress)=0;

    //!关联数据区子地址
    virtual  int internalSubAddress()=0;
    virtual  void setInternalSubAddress(int internalSubAddress)=0;
    //!更新数据:返回值 true：数值变化或者品质变化
    virtual  bool refreshData(QVariant dataValue,QDateTime TimeStamp,MDVariableDataQuality quality,bool OnlyChangedQuality=false)=0;
    //!模拟数据
    virtual void simulate()=0;
    //!复制
    virtual void copy(IMDRTDBVariable *otherVariable)=0;
    //!写变量
    virtual bool writeValue(QVariant varValue)=0;
    //!数据归档规则
    virtual MDRTDataSaveRulerParameter dataSaveRulerParameter()=0;
    virtual void setDataSaveRulerParameter(const MDRTDataSaveRulerParameter  &dataSaveRulerParameter)=0;

    //!初始化参数
    virtual MDVariableInitializeParameter initializeParameter()=0;
    virtual void setInitializeParameter(const MDVariableInitializeParameter  &dataInitialParameter)=0;

    //!变量写保护参数
    virtual MDVariableWriteProtectParameter writeProtectParameter()=0;
    virtual void setWriteProtectParameter(const MDVariableWriteProtectParameter &parameter)=0;

    //!变量限值报警参数
    virtual MDVariableAlarmParameter variableAlarmParameter()=0;
    virtual void setVariableAlarmParameter(const MDVariableAlarmParameter &parameter)=0;

    //!变量工程转换参数
    virtual MDVariableDataConversionParameter dataConversionParameter()=0;
    virtual void setDataConversionParameter(const MDVariableDataConversionParameter &parameter)=0;
    //!动作安全策略
    virtual CMDActionSPList * actionSPList()=0;
    virtual void setActionSPList(CMDActionSPList *actionSPList)=0;

/*!
\section 关联数据块存在变量变化时通知实时数据管理器
*/
public:
    /*!
     设备多个变量变化通知
     \param deviceID 设备ID。
     \returns 无。
     \note
     */
    //virtual void  notifyDeviceVariablesChange(bool OnlyChangedQuality=false)=0;
    virtual void  notifyDeviceVariablesChange(QList<IMDRTDBVariable*>& varlist,bool OnlyChangedQuality=false)=0;

/*!
\section OPC UA
*/
	virtual void removeFromUa()=0;
};
#endif // IMDRTDBVARIABLE_H
