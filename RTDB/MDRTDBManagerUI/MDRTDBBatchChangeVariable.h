/*!
author enli 2015.01.08
\section 实时数据库内部对象
*/
#ifndef CMDRTDBBATCHCHANGEVARIABLE_H
#define CMDRTDBBATCHCHANGEVARIABLE_H

#include <QList>
#include <QString>
#include <QMutex>
#include <QStringList>

#include "IMDRTDBVariable.h"
#include "IMDTreeItem.h"
#include "IMDRTDBManager.h"
#include "MDExpression.h"
#include "MDActionSP.h"
class IMDRTDBManager;
class IMDVariableGroup;
class CMDRTDBVariablesWidget;
class CPropertyBrowser;
class CVariableSecurityPolicyDialog;
//实时数据库变量
class CMDRTDBBatchChangeVariable:public QObject
{
    Q_OBJECT
    Q_ENUMS(MDGroupNameProperty)
    Q_ENUMS(MDDeviceNameProperty)
    Q_ENUMS(MDVariableTypeProperty)
    Q_ENUMS(MDDataTypeProperty)
    Q_ENUMS(MDRTInitModeProperty)
    Q_ENUMS(MDVariableDataConversionTypeProperty)
    Q_ENUMS(MDRTDataSaveModeProperty)
    Q_ENUMS(MDRTDataSaveRulerProperty)    

    Q_PROPERTY(QMap translateName READ translateName)

    //!General Prop
    Q_PROPERTY(QString GUID_General)
    Q_PROPERTY(MDGroupNameProperty GroupName READ groupNameIndex WRITE setGroupNameIndex)
    Q_PROPERTY(MDVariableTypeProperty VariableType READ variableTypeProp WRITE setVariableTypeProp)
    Q_PROPERTY(MDDataTypeProperty ProjectDataType READ dataTypeProp WRITE setDataTypeProp)
    Q_PROPERTY(int Precision READ precision WRITE setPrecision)

    //!Driver Prop
    Q_PROPERTY(QString GUID_DriverInfomation)    
    Q_PROPERTY(MDDeviceNameProperty DeviceName READ deviceNameIndex WRITE setDeviceNameIndex)
    Q_PROPERTY(MDDataTypeProperty DriverDataType READ originalDataTypeProp WRITE setOriginalDataTypeProp)

    //!初始化    
    Q_PROPERTY(QString GUID_Initialization)
    Q_PROPERTY(bool EnableChangeInit READ enableChangeInit WRITE setEnableChangeInit)
    Q_PROPERTY(MDRTInitModeProperty InitType READ initType WRITE setInitType)
    Q_PROPERTY(QString InitValue READ initValue WRITE setInitValue)

    //!工程转换
    Q_PROPERTY(QString GUID_Engineering_Transformation)
    Q_PROPERTY(bool EnableChangeConvert READ enableChangeConvert WRITE setEnableChangeConvert)
    //Q_PROPERTY(bool EnableValueConvert READ enableConvert WRITE setEnableConvert)
    Q_PROPERTY(MDVariableDataConversionTypeProperty ConvertionType READ convertionType WRITE setConvertionType)
    Q_PROPERTY(bool EnableLinearConvert READ linearConvertion WRITE setLinearConvertion)
    Q_PROPERTY(double ProjectMaxValue READ maxValue WRITE setMaxValue)
    Q_PROPERTY(double ProjectMinValue READ minValue WRITE setMinValue)
    Q_PROPERTY(double DriverMaxValue READ maxDriverValue WRITE setMaxDriverValue)
    Q_PROPERTY(double DriverMinValue READ minDriverValue WRITE setMinDriverValue)
    Q_PROPERTY(QString ByteOrder READ byteOrder WRITE setByteOrder)

    //!写保护
    Q_PROPERTY(QString GUID_WriteProtection)
    Q_PROPERTY(bool EnableChangeWrite READ enableChangeWrite WRITE setEnableChangeWrite)
    Q_PROPERTY(bool EnableWrite READ enableWrite WRITE setEnableWrite)
    Q_PROPERTY(bool EnableWriteProtect READ enableWriteProtect WRITE setEnableWriteProtect)
    Q_PROPERTY(int WriteProtectType READ writeProtectType WRITE setWriteProtectType)
    Q_PROPERTY(QString MaxWriteValue READ maxWriteValue WRITE setWriteMaxValue)
    Q_PROPERTY(QString MinWriteValue READ minWriteValue WRITE setWriteMinValue)

    //!存档规则
    Q_PROPERTY(QString GUID_Archive)
    Q_PROPERTY(bool EnableChangeArchive READ enableChangeArchive WRITE setEnableChangeArchive)
    Q_PROPERTY(MDRTDataSaveRulerProperty ArchiveTrigger READ dataSaveRulerProp WRITE setDataSaveRulerProp)
    Q_PROPERTY(MDRTDataSaveModeProperty ArchiveTo READ dataSaveModeProp WRITE setDataSaveModeProp)
    Q_PROPERTY(int ArchiveInterval READ saveInterval WRITE setSaveInterval)
    Q_PROPERTY(int MemoryHoldPeriod READ memorySavePeriod WRITE setMemorySavePeriod)


    //!安全策略
    Q_PROPERTY(QString GUID_Action)
    Q_PROPERTY(bool EnableChangeAction READ enableChangeAction WRITE setEnableChangeAction)
    Q_PROPERTY(QString DILActionList READ actionList WRITE setActionList)

public:
    CMDRTDBBatchChangeVariable();
    virtual ~CMDRTDBBatchChangeVariable();
/*!
\section IMDRTDBVariable 接口
*/
public:
    //!支持Prop，另外定义以下接口及类型
    enum class MDGroupNameProperty
    {
        invalid = 0,
    };

    enum class MDDeviceNameProperty
    {
        invalid = 0,
    };

    enum class MDVariableTypeProperty
    {
        None = 0,
        VirtualVariable = 1,    //虚拟变量，内部变量
        IOVariable = 2,         //IO变量，设备变量
        MiddleVariable = 3,     //中间变量
    };

    enum class MDDataTypeProperty
    {
        None = 0,
        Bool = 1,
        SByte = 2,
        Byte = 3,
        Int16 = 4,
        UInt16 = 5,
        Int32 = 6,
        UInt32 = 7,
        Int64 = 8,
        UInt64 = 9,
        Float = 10,
        Double = 11,
        DateTime = 12,
        String = 13,
    };

    enum class MDRTInitModeProperty
    {
        None = 0,              //无
        LastValue = 1,         //保留上次值
        FixedValue = 2,        //固定值
    };

    enum class MDVariableDataConversionTypeProperty
    {
        None = 0,
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
        None = 0,
        Memory = 1,                         //内存存储模式
        DiskFile = 2,                          //磁盘文件存储模式
        Both = 3,                               //内存与磁盘文件存储模式
    };

    enum class MDRTDataSaveRulerProperty
    {
        None = 0,                     //不存储
        DataChanged = 1,              //数据变化
        Cycle = 2,                    //周期存储
    };
    //!组名
    MDGroupNameProperty groupNameIndex();
    void setGroupNameIndex(MDGroupNameProperty nameIndex);
    QString groupName();
    void setGroupName(const QString& name);

    //!描述
    QString description();
    void setDescription(QString description);

    //!变量类型;
    QString variableTypeDescription();
    MDVariableTypeProperty variableTypeProp();
    void setVariableTypeProp(MDVariableTypeProperty varType);

    //!数据类型
    MDDataTypeProperty dataTypeProp();
    void setDataTypeProp(MDDataTypeProperty dataType);

    //!精度
    int precision();
    void setPrecision(int precision);

    //!地址
    QString address();
    void setAddress(QString address);

    //!设备名称
    MDDeviceNameProperty deviceNameIndex();
    void setDeviceNameIndex(MDDeviceNameProperty nameIndex);
    QString deviceName();
    void setDeviceName(const QString& name);

    //!原始值数据类型
    MDDataTypeProperty originalDataTypeProp();
    void setOriginalDataTypeProp(MDDataTypeProperty dataType);
    QString originalDataTypeDescription();

    //!初始化参数
    bool enableChangeInit();
    void setEnableChangeInit(bool enable);
    MDRTInitModeProperty initType();//初始化方式：0：无，1：保存值、2：指定值
    void setInitType(MDRTInitModeProperty type);
    QString initValue();
    void setInitValue(QString value);
    MDVariableInitializeParameter getInitPara();

    //!数据转换参数
    bool enableChangeConvert();
    void setEnableChangeConvert(bool enable);
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
    void setByteOrder(QString byteorder);
    MDVariableDataConversionParameter getDataConvertPara();

    //!变量写保护参数
    bool enableChangeWrite();
    void setEnableChangeWrite(bool enable);
    bool enableWrite();
    void setEnableWrite(bool enable);
    bool enableWriteProtect();
    void setEnableWriteProtect(bool enable);
    QString maxWriteValue();
    void setWriteMaxValue(QString value);
    QString minWriteValue();
    void setWriteMinValue(QString value);
    int writeProtectType();
    void setWriteProtectType(int writeType);
    MDVariableWriteProtectParameter getWriteProtectPara();

    //!存档
    bool enableChangeArchive();
    void setEnableChangeArchive(bool enable);
    MDRTDataSaveRulerProperty dataSaveRulerProp();
    void setDataSaveRulerProp(MDRTDataSaveRulerProperty ruler);
    MDRTDataSaveModeProperty dataSaveModeProp();
    void setDataSaveModeProp(MDRTDataSaveModeProperty mode);
    int saveInterval();
    void setSaveInterval(int interval);
    int memorySavePeriod();
    void setMemorySavePeriod(int period);
    MDRTDataSaveRulerParameter getArchivePara();

    //!动作安全策略
    bool enableChangeAction();
    void setEnableChangeAction(bool enable);
    QString actionList();
    void setActionList(QString actionList);
    CMDActionSPList getActionPara();

private:
    //!属性框组名及属性名翻译
    QMap<QString,QVariant> translateName(){return m_translateMap;}
    //!属性框中设备名更改时，刷新DataBlockName的列表
    void getDataBlockNameList(const QString& devName, QStringList& datablockNameList);
    void getGroupNameList(IMDVariableGroup* group,QStringList& groupList);
    void getDeviceNameList(QStringList& deviceList);
public:    
    //!设置RTDB管理器
    void setRTDBManager(IMDRTDBManager  *RTDBManager);
    //!获取projectcfg->获取安全策略管理对象
    void setParentWidget(CMDRTDBVariablesWidget* parentWidget);
    //!属性框
    void setPropertyBrowser(CPropertyBrowser* propertyBrowser);
public:
    QString         m_groupName;
    QString         m_deviceName;
    MDVariableTypeProperty  m_variableType;
    int                     m_precision;
    MDDataTypeProperty      m_dataType;
    MDDataTypeProperty      m_originalDataType;

    bool                    m_enableChangeInit;
    MDVariableInitializeParameter m_initializeParameter;

    bool                    m_enableChangeConvert;
    MDVariableDataConversionParameter m_dataConversionParameter;

    bool                    m_enableChangeWrite;
    MDVariableWriteProtectParameter m_writeProtectParameter;

    bool                    m_enableChangeArchive;
    MDRTDataSaveRulerParameter  m_dataSaveRulerParameter;

private:
    bool                    m_enableChangeAction;
    CVariableSecurityPolicyDialog* m_variableSecurityPolicyDialog;
    CMDActionSPList  m_actionSPList;


    QMap<QString,QVariant>  m_translateMap;
    QStringList         m_groupNameList;
    QStringList         m_deviceNameList;
    CMDRTDBVariablesWidget* m_parentWidget;
    IMDRTDBManager *    m_mdRTDBManager;
    CPropertyBrowser*   m_propertyBrowser;
};
#endif // CMDRTDBBATCHCHANGEVARIABLE_H
