#include <QtCore/qmath.h>
#include<QCoreApplication>
#include<QJsonObject>
#include<QJsonDocument>
#include <QJsonArray>
#include <QDialog>
#include <QMessageBox>

#include "MDRTDBVariableProperty.h"
#include "IMDChangeVariable.h"
#include "IMDVariableGroup.h"
#include "PropertyBrowser.h"
#include "MDMultiLanguage.h"
#include "MDSelectAddressDialog.h"
#include "MDRTDBVariablesWidget.h"
#include "MDProjectConfiger.h"
#include "VariableSecurityPolicyDialog.h"

CMDRTDBVariableProperty::CMDRTDBVariableProperty()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("Name", tr("变量名"));
    m_translateMap.insert("GroupName", tr("变量组"));
    m_translateMap.insert("Description", tr("描述"));
    m_translateMap.insert("VariableType", tr("变量类型"));
    m_translateMap.insert("ProjectDataType", tr("数据类型"));
    m_translateMap.insert("Precision", tr("精度"));

    m_translateMap.insert("GUID_DriverInfomation", tr("驱动配置"));
    m_translateMap.insert("DeviceName", tr("设备"));
    m_translateMap.insert("Address", tr("地址"));
    m_translateMap.insert("DriverDataType", tr("数据类型"));

    m_translateMap.insert("GUID_Initialization", tr("初始化"));
    m_translateMap.insert("InitType", tr("初始化方式"));
    m_translateMap.insert("InitValue", tr("固定初始值"));

    m_translateMap.insert("GUID_Engineering_Transformation", tr("工程转换"));
    m_translateMap.insert("ConvertionType", tr("类型转换"));
    m_translateMap.insert("EnableLinearConvert", tr("启用线性转换"));
    m_translateMap.insert("ProjectMaxValue", tr("工程最大值"));
    m_translateMap.insert("ProjectMinValue", tr("工程最小值"));
    m_translateMap.insert("DriverMaxValue", tr("原始最大值"));
    m_translateMap.insert("DriverMinValue", tr("原始最小值"));
    m_translateMap.insert("ByteOrder", tr("字节顺序"));

    m_translateMap.insert("GUID_WriteProtection", tr("写保护"));
    m_translateMap.insert("EnableWrite", tr("可写"));
    m_translateMap.insert("EnableWriteProtect", tr("写保护"));
    m_translateMap.insert("WriteProtectType", tr("超限处理"));
    m_translateMap.insert("MaxWrite", tr("最大值"));
    m_translateMap.insert("MinWrite", tr("最小值"));

    m_translateMap.insert("GUID_Archive", tr("存档规则"));
    m_translateMap.insert("ArchiveTrigger", tr("触发方式"));
    m_translateMap.insert("ArchiveTo", tr("存储模式"));
    m_translateMap.insert("ArchiveInterval", tr("存储间隔"));
    m_translateMap.insert("MemoryHoldPeriod", tr("内存保存时间(分)"));

    m_translateMap.insert("GUID_Action", tr("安全策略"));
    m_translateMap.insert("ActionList", tr("动作列表"));

    m_localName= "";
    m_groupName= "";
    m_deviceName= "";
    m_address="";
    m_changeCount =0;
    m_dataType=MDDataType::Int16;
    m_curQuality=MDVariableDataQuality::None;
    m_lastQuality=MDVariableDataQuality::None;
    m_timeStamp=QDateTime::fromString("1900:01:01 00:00:00.000");

    m_variableType = MDVariableType::IOVariable;
    m_originalValue  = QVariant();
    m_curValue = QVariant();
    m_lastValue = QVariant();
    m_isValid = true;
    m_itemType = MDTreeItemType::Variable;
    m_precision =3;
    m_RTDBManager = NULL;
    m_propertyBrowser = NULL;
    m_parentWidget = NULL;
    m_variableSecurityPolicyDialog = NULL;

    m_initializeParameter.m_initType = MDRTInitMode::None;
    m_initializeParameter.m_value = "";


    m_dataConversionParameter.m_enableLinearConversion=false;
    m_dataConversionParameter.m_maxValue=0;
    m_dataConversionParameter.m_minValue=0;
    m_dataConversionParameter.m_originalMaxValue=0;
    m_dataConversionParameter.m_originalMinValue=0;
    m_dataConversionParameter.m_conversionType = MDVariableDataConversionType::None;


    m_writeProtectParameter.m_enableWrite = true;
    m_writeProtectParameter.m_enableWriteProtect = true;
    m_writeProtectParameter.m_writeProtectType = 1;
    m_writeProtectParameter.m_maxValue = "0";
    m_writeProtectParameter.m_minValue = "0";

    m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::None;
    m_dataSaveRulerParameter.m_saveMode = MDRTDataSaveMode::Memory;
    m_dataSaveRulerParameter.m_saveCycle = 1;
    m_dataSaveRulerParameter.m_timeRange = 360;
}

CMDRTDBVariableProperty::~CMDRTDBVariableProperty()
{
    if(m_variableSecurityPolicyDialog != NULL)
        delete m_variableSecurityPolicyDialog;
    m_translateMap.clear();
}

void CMDRTDBVariableProperty::setRTDBManager(IMDRTDBManager* RTDBManager)
{
    m_RTDBManager = RTDBManager;
}

void CMDRTDBVariableProperty::setPropertyBrowser(CPropertyBrowser* propertyBrowser)
{
    m_propertyBrowser = propertyBrowser;
}

void CMDRTDBVariableProperty::setParentWidget(CMDRTDBVariablesWidget* parentWidget)
{
    m_parentWidget = parentWidget;
}

void CMDRTDBVariableProperty::copy(IMDRTDBVariable *otherVariable)
{
     m_variable = NULL;
     m_variableGroup = NULL;
     if(otherVariable== NULL)
     {
         return;
     }
     m_variable = otherVariable;
     m_localName= otherVariable->localName();
     m_variableGroup = (IMDVariableGroup*)otherVariable->parentItem(MDTreeItemType::VariableGroup);
     m_groupName= m_variableGroup->name();
     m_deviceName= otherVariable->deviceName();
     m_address=otherVariable->address();
     m_dataType=otherVariable->dataType();
     m_originalDataType=otherVariable->originalDataType();
     m_curQuality=otherVariable->currentQuality();
     m_lastQuality=otherVariable->lastQuality();
     m_timeStamp=otherVariable->timeStamp();
     m_variableType = otherVariable->variableType();
     m_originalValue =otherVariable->originalValue();
     m_curValue = otherVariable->currentValue();
     m_lastValue = otherVariable->lastValue();
     m_isValid= otherVariable->isValid();
     m_itemType = otherVariable->itemType();
     m_precision =otherVariable->precision();
     m_description = otherVariable->getDescription();
     m_initializeParameter = otherVariable->initializeParameter();
     m_dataConversionParameter = otherVariable->dataConversionParameter();
     m_dataSaveRulerParameter = otherVariable->dataSaveRulerParameter();
     m_writeProtectParameter = otherVariable->writeProtectParameter();
     m_actionSPList = *otherVariable->actionSPList();
}
void CMDRTDBVariableProperty::initialze()
{
    switch (m_originalDataType)
    {
    case MDDataType::Bool:
        m_originalValue = QVariant(bool(false));
        break;
    case MDDataType::SByte:
        m_originalValue = QVariant(qint8(0));
        break;
    case MDDataType::Byte:
        m_originalValue = QVariant(quint8(0));
        break;
    case MDDataType::Int16:
        m_originalValue = QVariant(qint16(0));
        break;
    case MDDataType::UInt16:
        m_originalValue = QVariant(quint16(0));
        break;
    case MDDataType::Int32:
        m_originalValue = QVariant(qint32(0));
        break;
    case MDDataType::UInt32:
        m_originalValue = QVariant(quint32(0));
        break;
    case MDDataType::Int64:
        m_originalValue = QVariant(qint64(0));
        break;
    case MDDataType::UInt64:
        m_originalValue = QVariant(quint64(0));
        break;
    case MDDataType::Float:
        m_originalValue = QVariant(float(0));
        break;
    case MDDataType::Double:
        m_originalValue = QVariant(double(0));
        break;
    case MDDataType::DateTime:
        m_originalValue = QVariant(QDateTime::fromString("1900:01:01 00:00:00.000"));
        break;
    case MDDataType::String:
        m_originalValue = QVariant(QString(""));
        break;
    default:
        break;
    }
    switch (m_dataType)
    {
    case MDDataType::Bool:
        m_curValue = QVariant(bool(false));
        break;
    case MDDataType::SByte:
        m_curValue = QVariant(qint8(0));
        break;
    case MDDataType::Byte:
        m_curValue = QVariant(quint8(0));
        break;
    case MDDataType::Int16:
        m_curValue = QVariant(qint16(0));
        break;
    case MDDataType::UInt16:
        m_curValue = QVariant(quint16(0));
        break;
    case MDDataType::Int32:
        m_curValue = QVariant(qint32(0));
        break;
    case MDDataType::UInt32:
        m_curValue = QVariant(quint32(0));
        break;
    case MDDataType::Int64:
        m_curValue = QVariant(qint64(0));
        break;
    case MDDataType::UInt64:
        m_curValue = QVariant(quint64(0));
        break;
    case MDDataType::Float:
        m_curValue = QVariant(float(0));
        break;
    case MDDataType::Double:
        m_curValue = QVariant(double(0));
        break;
    case MDDataType::DateTime:
        m_curValue = QVariant(QDateTime::fromString("1900:01:01 00:00:00.000"));
        break;
    case MDDataType::String:
        m_curValue = QVariant(QString(""));
        break;
    default:
        break;
    }
    m_lastValue = m_curValue;
    switch (m_variableType)
    {
    case MDVariableType::IOVariable:
        m_lastQuality = m_curQuality=MDVariableDataQuality::None;
        m_timeStamp= QDateTime::fromString("1900:01:01 00:00:00.000");
        break;
    case MDVariableType::VirtualVariable:
        m_lastQuality = m_curQuality=MDVariableDataQuality::Good;
        m_timeStamp= QDateTime::currentDateTime();
        break;
    case MDVariableType::SystemVariable:
        m_lastQuality = m_curQuality=MDVariableDataQuality::Good;
        m_timeStamp= QDateTime::currentDateTime();
        break;
    case MDVariableType::MiddleVariable:
        m_lastQuality = m_curQuality=MDVariableDataQuality::None;
        m_timeStamp= QDateTime::fromString("1900:01:01 00:00:00.000");
        break;
    default:
        break;
    }
}
//名称
QString CMDRTDBVariableProperty::localName()
{
    return m_localName;
}

void CMDRTDBVariableProperty::setLocalName(QString name)
{
    //检验地址重复
    if(m_variableGroup != NULL)
    {
        if(m_variableGroup->getVariable(name) == NULL)
        {
            m_localName = name;
            m_variable->setLocalName(name);
            m_parentWidget->refreshItemProp(0);
        }
    }
}
//描述
QString CMDRTDBVariableProperty::description()
{
    return m_description;
}

void CMDRTDBVariableProperty::setDescription(QString description)
{
    m_description = description;
    m_variable->setDescription(description);
    m_parentWidget->refreshItemProp(0);
}
QString CMDRTDBVariableProperty::groupName()
{
    return m_groupName;
}

//地址
QString CMDRTDBVariableProperty::address()
{
    IDevice* dev = m_RTDBManager->driverManager()->getDevice(m_deviceName);
    if(dev != NULL)
    {
        if(m_address == "")
        {
            m_propertyBrowser->setPropertyBackColor("Address",QColor(255,0,0));
        }
        else
        {
            QString strErr;
            if(!checkAddress(strErr))
                m_propertyBrowser->setPropertyBackColor("Address",QColor(255,0,0));
            else
                m_propertyBrowser->setPropertyBackColor("Address",QColor(255,255,255));
        }
    }
    return m_address;
}
void CMDRTDBVariableProperty::setAddress(QString address)
{
    IDevice* dev = m_RTDBManager->driverManager()->getDevice(m_deviceName);
    if(dev != NULL)
    {
        QStringList dbList;
        dev->getVariableAddressList(dbList);

        //device
        IDevice* dev = m_RTDBManager->driverManager()->getDevice(m_deviceName);
        if(dev == NULL)
            return;
        CMDSelectAddressDialog dlg;
        dlg.setDeviceAndVariable(dev,m_variable);
        dlg.setDataBlockAddressList(dbList);
        int reply = dlg.exec();
        if(reply == QMessageBox::Yes)
        {
            m_parentWidget->refreshItemProp(0);
        }
    }
}

bool CMDRTDBVariableProperty::checkAddress(QString& strErr)
{
    if(m_RTDBManager == NULL)
        return false;

    bool bCheckOK = false;
    //获取device
    IDevice* dev = m_RTDBManager->driverManager()->getDevice(m_deviceName);
    if(dev != NULL)
    {
        bCheckOK = dev->checkVarAddress(m_variable,strErr);
    }
    return bCheckOK;
}

//品质
QString CMDRTDBVariableProperty::qualityDescription(const MDVariableDataQuality &quality)
{
    QString ret = "";
    switch (quality)
    {
    case MDVariableDataQuality::AddressError:
        ret =QObject::tr("地址错误");
        break;
    case MDVariableDataQuality::ConnectDeviceFailed:
        ret =QObject::tr("无法与设备建立通信");
        break;
    case MDVariableDataQuality::StopCommunication:
        ret =QObject::tr("停止设备通信");
        break;
    case MDVariableDataQuality::DataTypeError:
        ret =QObject::tr("数据类型错误");
        break;
    case MDVariableDataQuality::DataBlockConfigError:
        ret =QObject::tr("数据块配置错误");
        break;
    case MDVariableDataQuality::Good:
        ret =QObject::tr("正确");
        break;
    case MDVariableDataQuality::None:
        ret =QObject::tr("没有初始化");
        break;
    case MDVariableDataQuality::ProtocolError:
        ret =QObject::tr("通信协议错误");
        break;
    case MDVariableDataQuality::TimeoutError:
        ret =QObject::tr("命令超时");
        break;
    case MDVariableDataQuality::DataConvertError:
        ret =QObject::tr("工程转换失败");
        break;
    default:
        break;
    }
    return ret;
}
QString CMDRTDBVariableProperty::currentQualityDescription()
{
    QMutexLocker locker(&m_objectMutex);
     return qualityDescription(m_curQuality);
}
MDVariableDataQuality CMDRTDBVariableProperty::currentQuality()
{
     QMutexLocker locker(&m_objectMutex);
     return m_curQuality;
}
void CMDRTDBVariableProperty::setCurrentQuality(MDVariableDataQuality quality)
{
    QMutexLocker locker(&m_objectMutex);
    m_curQuality = quality;
}
QString CMDRTDBVariableProperty::lastQualityDescription()
{
    QMutexLocker locker(&m_objectMutex);
     return qualityDescription(m_lastQuality);
}
MDVariableDataQuality CMDRTDBVariableProperty::lastQuality()
{
    QMutexLocker locker(&m_objectMutex);
     return m_lastQuality;
}
void CMDRTDBVariableProperty::setLastQuality(MDVariableDataQuality quality)
{
    QMutexLocker locker(&m_objectMutex);
    m_lastQuality = quality;
}

//设备名称
CMDRTDBVariableProperty::MDDeviceNameEnum CMDRTDBVariableProperty::deviceName()
{
    int nIndex = m_deviceNameList.indexOf(m_deviceName);
    if(nIndex == -1)
    {
        nIndex = 0;
        m_deviceName = "InvalidDeviceName";
        m_propertyBrowser->updateProperty();
    }
    else
    {
        if(nIndex == 0)
        {
            if(m_variableType == MDVariableType::IOVariable)
                m_propertyBrowser->setPropertyBackColor("DeviceName",QColor(255,0,0));
            else
                m_propertyBrowser->setPropertyBackColor("DeviceName",QColor(255,255,255));
        }
        else
        {
            m_propertyBrowser->setPropertyBackColor("DeviceName",QColor(255,255,255));
        }
    }

    return (CMDRTDBVariableProperty::MDDeviceNameEnum)nIndex;
}

void CMDRTDBVariableProperty::setDeviceName(MDDeviceNameEnum deviceNameIndex)
{
    int nindex = (int)deviceNameIndex;
    if(nindex == 0)
    {
        if(m_variableType == MDVariableType::IOVariable)
            return;
    }
    m_propertyBrowser->setPropertyBackColor("DeviceName",QColor(255,255,255));
    if(nindex < m_deviceNameList.count())
    {
        m_deviceName= m_deviceNameList[nindex];
        m_variable->setDeviceName(m_deviceName);
        //验证地址是否依然正确
        QString strErr;
        //!刷新控件颜色
        if(!checkAddress(strErr) && m_propertyBrowser != NULL)
        {
            m_propertyBrowser->setPropertyBackColor("Address",QColor(255,0,0));
        }
        else
        {
            m_propertyBrowser->setPropertyBackColor("Address",QColor(255,255,255));
            m_parentWidget->refreshItemProp(0);
        }
    }
}

void CMDRTDBVariableProperty::setDeviceNameList(QStringList& deviceNameList)
{
    m_deviceNameList.clear();
    m_deviceNameList.append(deviceNameList);
}

void CMDRTDBVariableProperty::getDeviceNameList(QStringList& deviceNameList)
{
    deviceNameList.append(m_deviceNameList);
}

//时间戳
QDateTime CMDRTDBVariableProperty::timeStamp()
{
    QMutexLocker locker(&m_objectMutex);
     return m_timeStamp;
}
void CMDRTDBVariableProperty::setTimeStamp(QDateTime timeStamp)
{
    QMutexLocker locker(&m_objectMutex);
    m_timeStamp = timeStamp;
}

CMDRTDBVariableProperty::MDDataTypeProperty CMDRTDBVariableProperty::originalDataTypeProp()
{
    return (MDDataTypeProperty)m_originalDataType;
}

void CMDRTDBVariableProperty::setOriginalDataTypeProp(MDDataTypeProperty dataType)
{
    m_originalDataType = (MDDataType)dataType;
    m_variable->setOriginalDataType((MDDataType)dataType);
}

//原始值
QVariant CMDRTDBVariableProperty::originalValue()
{
    QMutexLocker locker(&m_objectMutex);
     return m_originalValue;
}
void CMDRTDBVariableProperty::setOriginalValue(QVariant value)
{
    QMutexLocker locker(&m_objectMutex);
    m_originalValue = value;
}

//当前值
QVariant CMDRTDBVariableProperty::currentValue()
{
    QMutexLocker locker(&m_objectMutex);
     return m_curValue;
}
void CMDRTDBVariableProperty::setCurrentValue(QVariant value)
{
    QMutexLocker locker(&m_objectMutex);
    m_curValue = value;
}
QString CMDRTDBVariableProperty::currentValueProp()
{
    return m_curValue.toString();
}

void CMDRTDBVariableProperty::setCurrentValueProp(QString value)
{

}

//上次值
QVariant CMDRTDBVariableProperty::lastValue()
{
    QMutexLocker locker(&m_objectMutex);
     return m_lastValue;
}
void CMDRTDBVariableProperty::setLastValue(QVariant value)
{
    QMutexLocker locker(&m_objectMutex);
    m_lastValue = value;
}
QString CMDRTDBVariableProperty::lastValueProp()
{
    return m_lastValue.toString();
}

QString CMDRTDBVariableProperty::actionList()
{
    return "";
}

void CMDRTDBVariableProperty::setActionList(QString actionList)
{
    CMDProjectConfiger* projectConcfg = m_parentWidget->getProjectConfiger();
    if(projectConcfg != NULL)
    {
        if(m_variableSecurityPolicyDialog == NULL)
            m_variableSecurityPolicyDialog = new CVariableSecurityPolicyDialog();

        m_variableSecurityPolicyDialog->setRunModel(m_parentWidget->getRunningStatus());
        m_variableSecurityPolicyDialog->initial(projectConcfg->powerGroupConfiger(),m_variable);
        m_variableSecurityPolicyDialog->exec();
        m_actionSPList = m_variableSecurityPolicyDialog->m_actionSPList;
        m_variable->setActionSPList(&m_actionSPList);
    }
}

//变量类型
CMDRTDBVariableProperty::MDVariableTypeProperty CMDRTDBVariableProperty::variableTypeProp()
{
    //隐藏一些属性
    QStringList propList;
    propList.append("GUID_DriverInfomation");
    propList.append("DeviceName");
    propList.append("Address");
    propList.append("DriverDataType");
    if(m_variableType == MDVariableType::IOVariable)
        m_propertyBrowser->setPropertyHide(propList,false);
    else
        m_propertyBrowser->setPropertyHide(propList,true);
    return MDVariableTypeProperty(m_variableType);
}

void CMDRTDBVariableProperty::setVariableTypeProp(MDVariableTypeProperty varType)
{
    m_variableType = (MDVariableType)varType;
    m_variable->setVariableType(m_variableType);
    m_parentWidget->refreshItemProp(0);    
}

//数据类型
CMDRTDBVariableProperty::MDDataTypeProperty CMDRTDBVariableProperty::dataTypeProp()
{
    return (MDDataTypeProperty)m_dataType;
}

void CMDRTDBVariableProperty::setDataTypeProp(MDDataTypeProperty dataType)
{
    m_dataType = (MDDataType)dataType;
    m_variable->setDataType(m_dataType);
    m_parentWidget->refreshItemProp(0);
}

int CMDRTDBVariableProperty::precision()
{
  return m_precision;
}

void CMDRTDBVariableProperty::setPrecision(int precision)
{
    m_precision = precision;
    m_variable->setPrecision(m_precision);
}

//存储策略
CMDRTDBVariableProperty::MDRTDataSaveRulerProperty CMDRTDBVariableProperty::dataSaveRulerProp()
{
    return (MDRTDataSaveRulerProperty)m_dataSaveRulerParameter.m_saveRuler;
}

void CMDRTDBVariableProperty::setDataSaveRulerProp(MDRTDataSaveRulerProperty ruler)
{
    m_dataSaveRulerParameter.m_saveRuler = (MDRTDataSaveRuler)ruler;
}
CMDRTDBVariableProperty::MDRTDataSaveModeProperty CMDRTDBVariableProperty::dataSaveModeProp()
{
    return (MDRTDataSaveModeProperty)m_dataSaveRulerParameter.m_saveMode;
}

void CMDRTDBVariableProperty::setDataSaveModeProp(MDRTDataSaveModeProperty mode)
{
    m_dataSaveRulerParameter.m_saveMode = (MDRTDataSaveMode)mode;
    m_variable->setDataSaveRulerParameter(m_dataSaveRulerParameter);
}

int CMDRTDBVariableProperty::saveInterval()
{
    return m_dataSaveRulerParameter.m_saveCycle;
}

void CMDRTDBVariableProperty::setSaveInterval(int interval)
{
    m_dataSaveRulerParameter.m_saveCycle = interval;
    m_variable->setDataSaveRulerParameter(m_dataSaveRulerParameter);
}
int CMDRTDBVariableProperty::memorySavePeriod()
{
    return m_dataSaveRulerParameter.m_timeRange;
}
void CMDRTDBVariableProperty::setMemorySavePeriod(int period)
{
    m_dataSaveRulerParameter.m_timeRange = period;
    m_variable->setDataSaveRulerParameter(m_dataSaveRulerParameter);
}

//初始化参数
CMDRTDBVariableProperty::MDRTInitModeProperty CMDRTDBVariableProperty::initType()//初始化方式：0：无，1：保存值、2：指定值
{
    if( m_initializeParameter.m_initType == MDRTInitMode::FixedValue)
        m_propertyBrowser->setSinglePropertyHide("InitValue",false);
    else
        m_propertyBrowser->setSinglePropertyHide("InitValue",true);
    return (MDRTInitModeProperty)(m_initializeParameter.m_initType);
}

void CMDRTDBVariableProperty::setInitType(MDRTInitModeProperty type)
{
    m_initializeParameter.m_initType = (MDRTInitMode)type;
    m_variable->setInitializeParameter(m_initializeParameter);
}

QString CMDRTDBVariableProperty::initValue()
{
    return m_initializeParameter.m_value;
}

void CMDRTDBVariableProperty::setInitValue(QString value)
{
    m_initializeParameter.m_value = value;
    m_variable->setInitializeParameter(m_initializeParameter);
}

//品质错误参数
MDVariableQualityIsBadParameter  CMDRTDBVariableProperty::qualityIsBadParameter()
{
    return m_qualityIsBadParameter;
}

void  CMDRTDBVariableProperty::setQualityIsBadParameter(const MDVariableQualityIsBadParameter  &parameter)
{
    m_qualityIsBadParameter = parameter;
}

//数据转换参数
CMDRTDBVariableProperty::MDVariableDataConversionTypeProperty CMDRTDBVariableProperty::convertionType()
{
    if(m_dataConversionParameter.m_conversionType == MDVariableDataConversionType::ByteOrderChange)
        m_propertyBrowser->setSinglePropertyHide("ByteOrder",false);
    else
        m_propertyBrowser->setSinglePropertyHide("ByteOrder",true);

    return (MDVariableDataConversionTypeProperty)m_dataConversionParameter.m_conversionType;
}

void CMDRTDBVariableProperty::setConvertionType(MDVariableDataConversionTypeProperty type)
{
    m_dataConversionParameter.m_conversionType = (MDVariableDataConversionType)type;
    m_variable->setDataConversionParameter(m_dataConversionParameter);
}

bool CMDRTDBVariableProperty::linearConvertion()
{
    //刷新属性框
    QStringList propList;
    propList.append("ProjectMaxValue");
    propList.append("ProjectMinValue");
    propList.append("DrivrMaxValue");
    propList.append("DrivrMinValue");
    if(m_dataConversionParameter.m_enableLinearConversion)
    {
        m_propertyBrowser->setPropertyHide(propList,false);
    }
    else
    {
        m_propertyBrowser->setPropertyHide(propList,true);
    }
    return m_dataConversionParameter.m_enableLinearConversion;
}

void CMDRTDBVariableProperty::setLinearConvertion(bool linearConvertion)
{
    m_dataConversionParameter.m_enableLinearConversion = linearConvertion;
    m_variable->setDataConversionParameter(m_dataConversionParameter);    
}

double CMDRTDBVariableProperty::maxValue()
{
    return m_dataConversionParameter.m_maxValue;
}

void CMDRTDBVariableProperty::setMaxValue(double value)
{
    m_dataConversionParameter.m_maxValue = value;
    m_variable->setDataConversionParameter(m_dataConversionParameter);
}

double CMDRTDBVariableProperty::minValue()
{
    return m_dataConversionParameter.m_minValue;
}

void CMDRTDBVariableProperty::setMinValue(double value)
{
    m_dataConversionParameter.m_minValue = value;
    m_variable->setDataConversionParameter(m_dataConversionParameter);
}

double CMDRTDBVariableProperty::maxDriverValue()
{
    return m_dataConversionParameter.m_originalMaxValue;
}

void CMDRTDBVariableProperty::setMaxDriverValue(double value)
{
    m_dataConversionParameter.m_originalMaxValue = value;
    m_variable->setDataConversionParameter(m_dataConversionParameter);
}

double CMDRTDBVariableProperty::minDriverValue()
{
    return m_dataConversionParameter.m_originalMinValue;
}

void CMDRTDBVariableProperty::setMinDriverValue(double value)
{
    m_dataConversionParameter.m_originalMinValue = value;
    m_variable->setDataConversionParameter(m_dataConversionParameter);
}

QString CMDRTDBVariableProperty::byteOrder()
{
    return m_dataConversionParameter.m_byteOrder;
}

void CMDRTDBVariableProperty::setByteOrdere(QString byteorder)
{
    m_dataConversionParameter.m_byteOrder = byteorder;
    m_variable->setDataConversionParameter(m_dataConversionParameter);
}

//变量写保护参数
bool CMDRTDBVariableProperty::enableWrite()
{
    //刷新属性
    QStringList propList;
    propList.append("WriteProtectType");
    propList.append("MaxWrite");
    propList.append("MinWrite");
    if(m_writeProtectParameter.m_enableWrite)
    {
        m_propertyBrowser->setSinglePropertyHide("EnableWriteProtect",false);
        if(m_writeProtectParameter.m_enableWriteProtect)
        {
            m_propertyBrowser->setPropertyHide(propList,false);
        }
        else
        {
            m_propertyBrowser->setPropertyHide(propList,true);
        }
    }
    else
    {
        propList.append("EnableWriteProtect");
        m_propertyBrowser->setPropertyHide(propList,true);
    }
    return m_writeProtectParameter.m_enableWrite;
}

void CMDRTDBVariableProperty::setEnableWrite(bool enable)
{
    m_writeProtectParameter.m_enableWrite = enable;
    m_variable->setWriteProtectParameter(m_writeProtectParameter);
}

QString CMDRTDBVariableProperty::maxWriteValue()
{
    return m_writeProtectParameter.m_maxValue;
}

void CMDRTDBVariableProperty::setWriteMaxValue(QString value)
{
    m_writeProtectParameter.m_maxValue = value;
    m_variable->setWriteProtectParameter(m_writeProtectParameter);
}

QString CMDRTDBVariableProperty::minWriteValue()
{
    return m_writeProtectParameter.m_minValue;
}

void CMDRTDBVariableProperty::setWriteMinValue(QString value)
{
    m_writeProtectParameter.m_minValue = value;
    m_variable->setWriteProtectParameter(m_writeProtectParameter);
}

bool CMDRTDBVariableProperty::enableWriteProtect()
{
    QStringList propList;
    propList.append("WriteProtectType");
    propList.append("MaxWrite");
    propList.append("MinWrite");
    if(m_writeProtectParameter.m_enableWriteProtect && m_writeProtectParameter.m_enableWrite)
    {
        m_propertyBrowser->setPropertyHide(propList,false);
    }
    else
    {
        m_propertyBrowser->setPropertyHide(propList,true);
    }
    return m_writeProtectParameter.m_enableWriteProtect;
}

void CMDRTDBVariableProperty::setEnableWriteProtect(bool enable)
{
     m_writeProtectParameter.m_enableWriteProtect = enable;
}

int CMDRTDBVariableProperty::writeProtectType()
{
    return m_writeProtectParameter.m_writeProtectType;
}

void CMDRTDBVariableProperty::setWriteProtectType(int protectType)
{
    m_writeProtectParameter.m_writeProtectType = protectType;
}

void CMDRTDBVariableProperty::promptErrorMsg(const QString& errMsg)
{
    QMessageBox box;
    box.setText(errMsg);
    box.setWindowTitle(tr("警告"));
    box.setStandardButtons(QMessageBox::Ok);

    CMDMultiLanguage::instance()->translateWidget(&box);
    box.exec();
}
