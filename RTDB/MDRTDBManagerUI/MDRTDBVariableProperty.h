/*!
author enli 2015.01.08
\section 实时数据库内部对象
*/
#ifndef CMDRTDBVARIABLEPROP_H
#define CMDRTDBVARIABLEPROP_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <QMutexLocker>
#include <QDomElement>

#include "IMDRTDBManager.h"
#include "IMDRTDBVariable.h"
#include "IMDVariableGroup.h"
#include "IMDTreeItem.h"
#include "MDExpression.h"
#include "MDActionSP.h"

class CPropertyBrowser;
class CVariableSecurityPolicyDialog;

class CMDRTDBVariablesWidget;
//变量属性
class CMDRTDBVariableProperty:public QObject
{
    Q_OBJECT
    Q_ENUMS(MDVariableTypeProperty)
    Q_ENUMS(MDDataTypeProperty)
    Q_ENUMS(MDRTInitModeProperty)
    Q_ENUMS(MDVariableDataConversionTypeProperty)
    Q_ENUMS(MDRTDataSaveModeProperty)
    Q_ENUMS(MDDeviceNameEnum)
    Q_ENUMS(MDRTDataSaveRulerProperty)

    Q_PROPERTY(QMap translateName READ translateName)

    //!General Prop
    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(QString Name READ localName WRITE setLocalName)
    Q_PROPERTY(QString GroupName READ groupName)
    Q_PROPERTY(QString Description READ description WRITE setDescription)
    Q_PROPERTY(MDVariableTypeProperty VariableType READ variableTypeProp WRITE setVariableTypeProp)
    Q_PROPERTY(MDDataTypeProperty ProjectDataType READ dataTypeProp WRITE setDataTypeProp)
    Q_PROPERTY(int Precision READ precision WRITE setPrecision)

    //!Driver Prop
    Q_PROPERTY(QString GUID_DriverInfomation)
    Q_PROPERTY(QString DILAddress READ address WRITE setAddress)
    Q_PROPERTY(MDDeviceNameEnum DeviceName READ deviceName WRITE setDeviceName)
    Q_PROPERTY(MDDataTypeProperty DriverDataType READ originalDataTypeProp WRITE setOriginalDataTypeProp)

    /*
    //!Current Status
    Q_PROPERTY(QString GUID_CurrentStatus)
    Q_PROPERTY(QString CurrentValue READ currentValueProp WRITE setCurrentValueProp)
    Q_PROPERTY(QString Quality READ currentQualityDescription)
    Q_PROPERTY(QDateTime TimeStamp READ timeStamp)
    Q_PROPERTY(QString LastValue READ lastValueProp)
    Q_PROPERTY(QString LastQuality READ lastQualityDescription)
    */
    //!初始化
    Q_PROPERTY(QString GUID_Initialization)
    Q_PROPERTY(MDRTInitModeProperty InitType READ initType WRITE setInitType)
    Q_PROPERTY(QString InitValue READ initValue WRITE setInitValue)

    //!工程转换
    Q_PROPERTY(QString GUID_Engineering_Transformation)
   // Q_PROPERTY(bool EnableValueConvert READ enable WRITE setEnable)
    Q_PROPERTY(MDVariableDataConversionTypeProperty ConvertionType READ convertionType WRITE setConvertionType)
    Q_PROPERTY(bool EnableLinearConvert READ linearConvertion WRITE setLinearConvertion)
    Q_PROPERTY(double ProjectMaxValue READ maxValue WRITE setMaxValue)
    Q_PROPERTY(double ProjectMinValue READ minValue WRITE setMinValue)
    Q_PROPERTY(double DriverMaxValue READ maxDriverValue WRITE setMaxDriverValue)
    Q_PROPERTY(double DriverMinValue READ minDriverValue WRITE setMinDriverValue)
    Q_PROPERTY(QString ByteOrder READ byteOrder WRITE setByteOrdere)

    //!写保护
    Q_PROPERTY(QString GUID_WriteProtection)
    Q_PROPERTY(bool EnableWrite READ enableWrite WRITE setEnableWrite)
    Q_PROPERTY(bool EnableWriteProtect READ enableWriteProtect WRITE setEnableWriteProtect)
    Q_PROPERTY(int  WriteProtectType READ writeProtectType WRITE setWriteProtectType)
    Q_PROPERTY(QString MaxWrite READ maxWriteValue WRITE setWriteMaxValue)
    Q_PROPERTY(QString MinWrite READ minWriteValue WRITE setWriteMinValue)

    //!存档规则
    Q_PROPERTY(QString GUID_Archive)    
    Q_PROPERTY(MDRTDataSaveRulerProperty ArchiveTrigger READ dataSaveRulerProp WRITE setDataSaveRulerProp)
    Q_PROPERTY(MDRTDataSaveModeProperty ArchiveTo READ dataSaveModeProp WRITE setDataSaveModeProp)
    Q_PROPERTY(int ArchiveInterval READ saveInterval WRITE setSaveInterval)
    Q_PROPERTY(int MemoryHoldPeriod READ memorySavePeriod WRITE setMemorySavePeriod)


    //!安全策略
    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(QString DILActionList READ actionList WRITE setActionList)

public:
    enum class MDDeviceNameEnum
    {
        None = 0,
    };
    //!支持Prop，另外定义以下接口及类型
    enum class MDVariableTypeProperty
    {
        VirtualVariable = 0,    //虚拟变量，内部变量
        IOVariable = 1,         //IO变量，设备变量
        MiddleVariable = 2,     //中间变量
    };

    enum class MDDataTypeProperty
    {
        Bool = 0,
        SByte = 1,
        Byte = 2,
        Int16 = 3,
        UInt16 = 4,
        Int32 = 5,
        UInt32 = 6,
        Int64 = 7,
        UInt64 = 8,
        Float = 9,
        Double = 10,
        DateTime = 11,
        String = 12,
    };

    enum class MDRTInitModeProperty
    {
        None = 0,              //内存存储模式
        LastValue = 1,         //磁盘文件存储模式
        FixedValue = 2,        //内存与磁盘文件存储模式
    };

    enum class MDVariableDataConversionTypeProperty
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

    enum class MDRTDataSaveModeProperty
    {
        Memory = 0,                         //内存存储模式
        DiskFile = 1,                          //磁盘文件存储模式
        Both = 2,                               //内存与磁盘文件存储模式
    };

    enum class MDRTDataSaveRulerProperty
    {
        None = 0,                     //不存储
        DataChanged = 1,              //数据变化
        Cycle = 2,                    //周期存储
    };
public:
    CMDRTDBVariableProperty();
    virtual ~CMDRTDBVariableProperty();
    //!设置RTDBManager
    void setRTDBManager(IMDRTDBManager* RTDBManager);
    //!设置属性框
    void setPropertyBrowser(CPropertyBrowser* propertyBrowser);
    //!便于及时刷新修改的变量属性
    void setParentWidget(CMDRTDBVariablesWidget* parentWidget);

/*!
\section IMDRTDBVariable 接口
*/
public:
    //!组名
    QString groupName();
    
    //!不带Group的名
    QString localName();
    void setLocalName(QString name);

    //!描述
    QString description();
    void setDescription(QString description);

    //!变量类型
    MDVariableTypeProperty variableTypeProp();
    void setVariableTypeProp(MDVariableTypeProperty varType);
    QString variableTypeDescription();

    //!数据类型
    MDDataTypeProperty dataTypeProp();
    void setDataTypeProp(MDDataTypeProperty dataType);

    //!精度
    int precision();
    void setPrecision(int precision);

    //!当前值
    QVariant currentValue();
    void setCurrentValue(QVariant value);
    QString currentValueProp();
    void setCurrentValueProp(QString value);

    //!上次的值
    QVariant lastValue();
    void setLastValue(QVariant value);
    QString lastValueProp();

    //!当前品质
    QString currentQualityDescription();
    MDVariableDataQuality  currentQuality();
    void setCurrentQuality(MDVariableDataQuality quality);

    //!上次的品质
    QString lastQualityDescription();
    MDVariableDataQuality  lastQuality();
    void setLastQuality(MDVariableDataQuality quality);

    //!时间戳
    QDateTime timeStamp();
    void setTimeStamp(QDateTime timeStamp);

    //!地址
    QString address();
    void setAddress(QString address);

    //!设备名称
    MDDeviceNameEnum deviceName();
    void setDeviceName(MDDeviceNameEnum deviceNameIndex);
    void setDeviceNameList(QStringList& deviceNameList);
    void getDeviceNameList(QStringList& deviceNameList);

    //!原始值数据类型
    //!!property 不支持原来定义的enum
    MDDataTypeProperty originalDataTypeProp();
    void setOriginalDataTypeProp(MDDataTypeProperty dataType);

    //!原始值
    QVariant originalValue();
    void setOriginalValue(QVariant value);

    //!初始化参数
    MDRTInitModeProperty initType();//初始化方式：0：无，1：保存值、2：指定值
    void setInitType(MDRTInitModeProperty type);
    QString initValue();
    void setInitValue(QString value);

    //!品质错误参数
    MDVariableQualityIsBadParameter qualityIsBadParameter();
    void setQualityIsBadParameter(const MDVariableQualityIsBadParameter &parameter);

    //!数据转换参数    
    MDVariableDataConversionTypeProperty convertionType();
    void setConvertionType(MDVariableDataConversionTypeProperty type);
    bool linearConvertion();
    void setLinearConvertion(bool linearConvertion);
    double maxValue();
    void setMaxValue(double value);
    double minValue();
    void setMinValue(double value);
    double maxDriverValue();
    void setMaxDriverValue(double value);
    double minDriverValue();
    void setMinDriverValue(double value);
    QString byteOrder();
    void setByteOrdere(QString byteorder);

    //!变量写保护参数
    bool enableWrite();
    void setEnableWrite(bool enable);
    QString maxWriteValue();
    void setWriteMaxValue(QString value);
    QString minWriteValue();
    void setWriteMinValue(QString value);
    bool enableWriteProtect();
    void setEnableWriteProtect(bool enable);
    int writeProtectType();
    void setWriteProtectType(int protectType);

    //!存档
    MDRTDataSaveRulerProperty dataSaveRulerProp();
    void setDataSaveRulerProp(MDRTDataSaveRulerProperty ruler);
    MDRTDataSaveModeProperty dataSaveModeProp();
    void setDataSaveModeProp(MDRTDataSaveModeProperty mode);
    int saveInterval();
    void setSaveInterval(int interval);
    int memorySavePeriod();
    void setMemorySavePeriod(int period);

    //!关联数据区地址
    bool checkAddress(QString& strErr);

    //!动作安全策略
    QString actionList();
    void setActionList(QString actionList);
    //!复制
    void copy(IMDRTDBVariable *otherVariable);

private:
    QString qualityDescription(const MDVariableDataQuality &quality);
    //!属性框组名及属性名翻译
    QMap<QString,QVariant> translateName(){return m_translateMap;}

    //!弹出错误信息框
    void promptErrorMsg(const QString& errMsg);
private:
    QString m_groupName;
    QString m_localName;
    QVariant m_curValue;
    QVariant m_lastValue;
    QVariant m_originalValue;
    QString m_description;

    CMDActionSPList  m_actionSPList;

    QString m_address;
    QString m_deviceName;
    QString m_dataBlockName;
    int m_dataBlockIndexID;
    int m_internalAddress;
    int m_internalSubAddress;


    MDVariableType m_variableType;

    int m_precision;
    MDDataType m_dataType;
    MDDataType m_originalDataType;
    MDVariableDataQuality m_curQuality;
    MDVariableDataQuality m_lastQuality;
    QDateTime m_timeStamp;
    bool m_isValid;

    MDVariableInitializeParameter m_initializeParameter;
    MDVariableQualityIsBadParameter m_qualityIsBadParameter ;
    MDVariableDataConversionParameter m_dataConversionParameter;
    MDRTDataSaveRulerParameter  m_dataSaveRulerParameter;
    MDVariableWriteProtectParameter m_writeProtectParameter;
    
    qint64 m_changeCount;//变化更新次数

    QMutex m_objectMutex;

private:
    MDTreeItemType m_itemType;

/*!
\section 表达式等关联变量管理（中间变量）
*/
public:
    //!移除所有关联变量
    void removeAllRefrenceVariables();
    //!添加关联变量
    void addRefrenceVariable(IMDVariable* refVar);
    //!移除关联变量
    void removeRefrenceVariable(IMDVariable* refVar);
    //!获取关联变量
    IMDVariable* getRefrenceVariable(int index);
    //!获取关联变量数量
    int getRefrenceVariableCount();
    //!是否有关联变量
    bool hasRefrenceVariable();
    //!表达式计算
    void doExpression(const QString& varName,QVariant varValue);
    void doExpression();
    void initialMiddleVariableExpression();
    void initialze();

private:
    QStringList m_deviceNameList;

private:
    QMap<QString,QVariant>  m_translateMap;
    IMDRTDBVariable*        m_variable;
    IMDVariableGroup*       m_variableGroup;
    IMDRTDBManager*         m_RTDBManager;
    CPropertyBrowser*       m_propertyBrowser;
    CMDRTDBVariablesWidget* m_parentWidget;
    CVariableSecurityPolicyDialog*m_variableSecurityPolicyDialog;
};
#endif // CMDRTDBVARIABLEPROP_H
