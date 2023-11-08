#include <QtCore/qmath.h>
#include <QCoreApplication>

#include "MDRTDBBatchChangeVariable.h"
#include "IMDChangeVariable.h"
#include "IMDVariableGroup.h"
#include "PropertyBrowser.h"
#include "MDRTDBVariablesWidget.h"
#include "MDProjectConfiger.h"
#include "VariableSecurityPolicyDialog.h"

CMDRTDBBatchChangeVariable::CMDRTDBBatchChangeVariable()
{
    m_translateMap.insert("GUID_General", tr("通用"));
    m_translateMap.insert("GroupName", tr("变量组"));
    m_translateMap.insert("Description", tr("描述"));
    m_translateMap.insert("VariableType", tr("变量类型"));
    m_translateMap.insert("ProjectDataType", tr("数据类型"));
    m_translateMap.insert("Precision", tr("精度"));

    m_translateMap.insert("GUID_DriverInfomation", tr("驱动信息"));
    m_translateMap.insert("DeviceName", tr("设备"));
    m_translateMap.insert("DriverDataType", tr("数据类型"));

    m_translateMap.insert("GUID_Initialization", tr("初始化"));
    m_translateMap.insert("EnableChangeInit", tr("修改"));
    m_translateMap.insert("InitType", tr("初始化方式"));
    m_translateMap.insert("InitValue", tr("初始值"));

    m_translateMap.insert("GUID_Engineering_Transformation", tr("工程转换"));
    m_translateMap.insert("EnableChangeConvert", tr("修改"));
    //m_translateMap.insert("EnableValueConvert", tr("启用"));
    m_translateMap.insert("ConvertionType", tr("类型转换"));
    m_translateMap.insert("EnableLinearConvert", tr("启用线性转换"));
    m_translateMap.insert("ProjectMaxValue", tr("工程最大值"));
    m_translateMap.insert("ProjectMinValue", tr("工程最小值"));
    m_translateMap.insert("DriverMaxValue", tr("原始最大值"));
    m_translateMap.insert("DriverMinValue", tr("原始最小值"));
    m_translateMap.insert("ByteOrder", tr("字节顺序"));

    m_translateMap.insert("GUID_WriteProtection", tr("写保护"));
    m_translateMap.insert("EnableChangeWrite", tr("修改"));
    m_translateMap.insert("EnableWrite", tr("可写"));
    m_translateMap.insert("EnableWriteProtect", tr("写保护"));
    m_translateMap.insert("WriteProtectType", tr("超限处理"));
    m_translateMap.insert("MaxWriteValue", tr("最大值"));
    m_translateMap.insert("MinWriteValue", tr("最小值"));

    m_translateMap.insert("GUID_Archive", tr("存档规则"));
    m_translateMap.insert("EnableChangeArchive", tr("修改"));
    m_translateMap.insert("ArchiveTrigger", tr("触发方式"));
    m_translateMap.insert("ArchiveTo", tr("存储模式"));
    m_translateMap.insert("ArchiveInterval", tr("存储间隔"));
    m_translateMap.insert("MemoryHoldPeriod", tr("内存保存时间(分)"));

    m_translateMap.insert("GUID_Action", tr("安全策略"));
    m_translateMap.insert("EnableChangeAction", tr("修改"));
    m_translateMap.insert("DILActionList", tr(""));

    m_groupName= "";
    m_deviceName= "";
    m_dataType = MDDataTypeProperty::None;
    m_variableType = MDVariableTypeProperty::None;
    m_precision = -1;

    m_enableChangeInit = false;
    m_initializeParameter.m_initType = MDRTInitMode::None;
    m_initializeParameter.m_value = "";

    m_enableChangeConvert = false;
    m_dataConversionParameter.m_enableLinearConversion=false;
    m_dataConversionParameter.m_maxValue=0;
    m_dataConversionParameter.m_minValue=0;
    m_dataConversionParameter.m_originalMaxValue=0;
    m_dataConversionParameter.m_originalMinValue=0;
    m_dataConversionParameter.m_conversionType = MDVariableDataConversionType::None;

    m_enableChangeWrite = false;
    m_writeProtectParameter.m_enableWrite = false;
    m_writeProtectParameter.m_enableWriteProtect = true;
    m_writeProtectParameter.m_writeProtectType = 1;
    m_writeProtectParameter.m_maxValue = "0";
    m_writeProtectParameter.m_minValue = "0";

    m_enableChangeArchive = false;
    m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::None;
    m_dataSaveRulerParameter.m_saveMode = MDRTDataSaveMode::Memory;
    m_dataSaveRulerParameter.m_saveCycle = 1;
    m_dataSaveRulerParameter.m_timeRange = 360;

    m_enableChangeAction = false;

    m_mdRTDBManager = NULL;
    m_parentWidget = NULL;
    m_propertyBrowser = NULL;
    m_variableSecurityPolicyDialog = NULL;
}

CMDRTDBBatchChangeVariable::~CMDRTDBBatchChangeVariable()
{
    if(m_variableSecurityPolicyDialog != NULL)
        delete m_variableSecurityPolicyDialog;
    m_translateMap.clear();
}

void CMDRTDBBatchChangeVariable::setParentWidget(CMDRTDBVariablesWidget* parentWidget)
{
    m_parentWidget = parentWidget;
}

void CMDRTDBBatchChangeVariable::setRTDBManager(IMDRTDBManager  *RTDBManager)
{
    m_mdRTDBManager = RTDBManager;
    if(m_mdRTDBManager != NULL)
    {
        //变量组
        m_groupNameList.append("None");
        getGroupNameList(m_mdRTDBManager->variableGroup(),m_groupNameList);        

        //设备名
        m_deviceNameList.append("None");
        getDeviceNameList(m_deviceNameList);
    }
}

void CMDRTDBBatchChangeVariable::setPropertyBrowser(CPropertyBrowser* propertyBrowser)
{
    m_propertyBrowser = propertyBrowser;
    m_propertyBrowser->setObject(this);
    m_propertyBrowser->resetEnums("GroupName",m_groupNameList);
    m_propertyBrowser->resetEnums("DeviceName",m_deviceNameList);
}

void CMDRTDBBatchChangeVariable::getGroupNameList(IMDVariableGroup* group,QStringList& groupList)
{
    if(group == NULL)
        return;

    IMDVariableGroup* childgroup = NULL;
    int nGroupCnt = group->getChildVariableGroupCount();
    for(int i = 0; i<nGroupCnt; i++)
    {
        childgroup = group->getChildVariableGroupAt(i);
        if(childgroup->name() == "SystemVariableGroup")
            continue;
        else
            groupList.append(childgroup->name());
        //if has child group
        if(childgroup->getChildVariableGroupCount() > 0)
            getGroupNameList(childgroup,groupList);
    }
}

void CMDRTDBBatchChangeVariable::getDeviceNameList(QStringList& deviceList)
{
    IMDDriverManager* rootdevice = m_mdRTDBManager->driverManager();
    if(rootdevice != NULL)
    {
        IDevice* childdev = NULL;
        for(int i = 0; i<rootdevice->getDeviceCount(); i++)
        {
            childdev = rootdevice->getDeviceAt(i);
            deviceList.append(childdev->name());
        }
    }
}

/*
void CMDRTDBBatchChangeVariable::copy(IMDRTDBVariable *otherVariable)
{
     if(otherVariable== NULL)
     {
         return;
     }
     //m_groupName= ((CMDRTDBBatchChangeVariable*)otherVariable)->groupName();
     m_deviceName= otherVariable->deviceName();
     m_dataType=otherVariable->dataType();
     m_originalDataType=otherVariable->originalDataType();
     m_curQuality=otherVariable->currentQuality();
     m_lastQuality=otherVariable->lastQuality();
     m_timeStamp=otherVariable->timeStamp();
     m_variableType = otherVariable->variableType();
     m_precision =otherVariable->precision();
     m_writeProtectParameter = otherVariable->writeProtectParameter();
     m_actionSPList = *otherVariable->actionSPList();
}
*/

CMDRTDBBatchChangeVariable::MDGroupNameProperty CMDRTDBBatchChangeVariable::groupNameIndex()
{
    int index = m_groupNameList.indexOf(m_groupName);
    return (CMDRTDBBatchChangeVariable::MDGroupNameProperty)index;
}

void CMDRTDBBatchChangeVariable::setGroupNameIndex(MDGroupNameProperty nameIndex)
{
    int nindex = (int)nameIndex;
    if(nindex < m_groupNameList.count() && nindex > -1)
        m_groupName= m_groupNameList[nindex];
}

QString CMDRTDBBatchChangeVariable::groupName()
{
    return m_groupName;
}

void CMDRTDBBatchChangeVariable::setGroupName(const QString& name)
{
    m_groupName = name;
}

//设备名称
CMDRTDBBatchChangeVariable::MDDeviceNameProperty CMDRTDBBatchChangeVariable::deviceNameIndex()
{
    int index = m_deviceNameList.indexOf(m_deviceName);
    return (CMDRTDBBatchChangeVariable::MDDeviceNameProperty)index;
}

void CMDRTDBBatchChangeVariable::setDeviceNameIndex(MDDeviceNameProperty nameIndex)
{
    int nIndex = (int)nameIndex;
    if(nIndex < m_deviceNameList.count() && nIndex > -1)
        m_deviceName = m_deviceNameList[nIndex];
}


QString CMDRTDBBatchChangeVariable::deviceName()
{
    return m_deviceName;
}


void CMDRTDBBatchChangeVariable::setDeviceName(const QString& name)
{
    m_deviceName = name;
}

CMDRTDBBatchChangeVariable::MDDataTypeProperty CMDRTDBBatchChangeVariable::originalDataTypeProp()
{
    return m_originalDataType;
}

void CMDRTDBBatchChangeVariable::setOriginalDataTypeProp(MDDataTypeProperty dataType)
{
    m_originalDataType = dataType;
}

//安全策略
bool CMDRTDBBatchChangeVariable::enableChangeAction()
{
    m_propertyBrowser->setSinglePropertyHide("DILActionList",!m_enableChangeAction);
    return m_enableChangeAction;
}

void CMDRTDBBatchChangeVariable::setEnableChangeAction(bool enable)
{
    m_enableChangeAction = enable;    
}

QString CMDRTDBBatchChangeVariable::actionList()
{
    return "";
}

void CMDRTDBBatchChangeVariable::setActionList(QString actionList)
{
    CMDProjectConfiger* projectConcfg = m_parentWidget->getProjectConfiger();
    if(projectConcfg != NULL)
    {
        if(m_variableSecurityPolicyDialog == NULL)
            m_variableSecurityPolicyDialog = new CVariableSecurityPolicyDialog();

        m_variableSecurityPolicyDialog->setRunModel(m_parentWidget->getRunningStatus());
        m_variableSecurityPolicyDialog->initial(projectConcfg->powerGroupConfiger(),NULL);
        m_variableSecurityPolicyDialog->exec();
        m_actionSPList = m_variableSecurityPolicyDialog->m_actionSPList;
    }
}

CMDActionSPList CMDRTDBBatchChangeVariable::getActionPara()
{
    return m_actionSPList;
}

//变量类型
CMDRTDBBatchChangeVariable::MDVariableTypeProperty CMDRTDBBatchChangeVariable::variableTypeProp()
{
    QStringList propList;
    propList.append("DeviceName");
    propList.append("DriverDataType");
    if(m_variableType == MDVariableTypeProperty::IOVariable)
        m_propertyBrowser->setPropertyHide(propList,false);
    else
        m_propertyBrowser->setPropertyHide(propList,true);

    return MDVariableTypeProperty(m_variableType);
}

void CMDRTDBBatchChangeVariable::setVariableTypeProp(MDVariableTypeProperty varType)
{
    m_variableType = varType;    
}

//数据类型
CMDRTDBBatchChangeVariable::MDDataTypeProperty CMDRTDBBatchChangeVariable::dataTypeProp()
{
    return m_dataType;
}

void CMDRTDBBatchChangeVariable::setDataTypeProp(MDDataTypeProperty dataType)
{
    m_dataType = dataType;
}

int CMDRTDBBatchChangeVariable::precision()
{
  return m_precision;
}

void CMDRTDBBatchChangeVariable::setPrecision(int precision)
{
    m_precision = precision;
}

//数据块名称
/*
QString CMDRTDBBatchChangeVariable::dataBlockName()
{
    return "";//m_dataBlockName;
}

void CMDRTDBBatchChangeVariable::setDataBlockName(QString dbName)
{
    //m_dataBlockName = dbName;
}
*/
//关联数据区地址
bool CMDRTDBBatchChangeVariable::enableChangeArchive()
{
    QStringList propList;
    propList.append("ArchiveTrigger");
    propList.append("ArchiveTo");
    propList.append("ArchiveInterval");
    propList.append("MemoryHoldPeriod");
    if(m_enableChangeArchive)
        m_propertyBrowser->setPropertyHide(propList,false);
    else
        m_propertyBrowser->setPropertyHide(propList,true);

    return m_enableChangeArchive;
}

void CMDRTDBBatchChangeVariable::setEnableChangeArchive(bool enable)
{
    m_enableChangeArchive = enable;    
}

CMDRTDBBatchChangeVariable::MDRTDataSaveRulerProperty CMDRTDBBatchChangeVariable::dataSaveRulerProp()
{
    return (MDRTDataSaveRulerProperty)m_dataSaveRulerParameter.m_saveRuler;
}

void CMDRTDBBatchChangeVariable::setDataSaveRulerProp(MDRTDataSaveRulerProperty ruler)
{
    m_dataSaveRulerParameter.m_saveRuler = (MDRTDataSaveRuler)ruler;
}

CMDRTDBBatchChangeVariable::MDRTDataSaveModeProperty CMDRTDBBatchChangeVariable::dataSaveModeProp()
{
    int n = (int)m_dataSaveRulerParameter.m_saveMode;
    return (MDRTDataSaveModeProperty)(n+1);
}

void CMDRTDBBatchChangeVariable::setDataSaveModeProp(MDRTDataSaveModeProperty mode)
{
    int n = (int)mode;
    if(n > 1)
        m_dataSaveRulerParameter.m_saveMode = (MDRTDataSaveMode)(n-1);
}

int CMDRTDBBatchChangeVariable::saveInterval()
{
    return m_dataSaveRulerParameter.m_saveCycle;
}

void CMDRTDBBatchChangeVariable::setSaveInterval(int interval)
{
    m_dataSaveRulerParameter.m_saveCycle = interval;
}
int CMDRTDBBatchChangeVariable::memorySavePeriod()
{
    return m_dataSaveRulerParameter.m_timeRange;
}
void CMDRTDBBatchChangeVariable::setMemorySavePeriod(int period)
{
    m_dataSaveRulerParameter.m_timeRange = period;
}

MDRTDataSaveRulerParameter CMDRTDBBatchChangeVariable::getArchivePara()
{
    return m_dataSaveRulerParameter;
}

//初始化参数
bool CMDRTDBBatchChangeVariable::enableChangeInit()
{
    QStringList propList;
    propList.append("InitType");
    propList.append("InitValue");

    if(m_enableChangeInit)
    {
        m_propertyBrowser->setPropertyHide(propList,false);
    }
    else
    {
        m_propertyBrowser->setPropertyHide(propList,true);
    }
    return m_enableChangeInit;
}

void CMDRTDBBatchChangeVariable::setEnableChangeInit(bool enable)
{
    m_enableChangeInit = enable;
}

CMDRTDBBatchChangeVariable::MDRTInitModeProperty CMDRTDBBatchChangeVariable::initType()//初始化方式：0：无，1：保存值、2：指定值
{
    if( m_initializeParameter.m_initType == MDRTInitMode::FixedValue && m_enableChangeInit)
        m_propertyBrowser->setSinglePropertyHide("InitValue",false);
    else
        m_propertyBrowser->setSinglePropertyHide("InitValue",true);
    return (MDRTInitModeProperty)(m_initializeParameter.m_initType);
}

void CMDRTDBBatchChangeVariable::setInitType(MDRTInitModeProperty type)
{
    m_initializeParameter.m_initType = (MDRTInitMode)type;
}

QString CMDRTDBBatchChangeVariable::initValue()
{
    return m_initializeParameter.m_value;
}

void CMDRTDBBatchChangeVariable::setInitValue(QString value)
{
    m_initializeParameter.m_value = value;
}

MDVariableInitializeParameter CMDRTDBBatchChangeVariable::getInitPara()
{
    return m_initializeParameter;
}

//数据转换参数
bool CMDRTDBBatchChangeVariable::enableChangeConvert()
{
    QStringList propList;
    propList.append("ConvertionType");
    propList.append("EnableLinearConvert");
    propList.append("ProjectMaxValue");
    propList.append("ProjectMinValue");
    propList.append("DrivrMaxValue");
    propList.append("DrivrMinValue");
    propList.append("ByteOrder");
    if(m_enableChangeConvert)
    {
        m_propertyBrowser->setPropertyHide(propList,false);
    }
    else
    {
        m_propertyBrowser->setPropertyHide(propList,true);
    }

    return m_enableChangeConvert;
}

void CMDRTDBBatchChangeVariable::setEnableChangeConvert(bool enable)
{
    m_enableChangeConvert = enable;    
}

CMDRTDBBatchChangeVariable::MDVariableDataConversionTypeProperty CMDRTDBBatchChangeVariable::convertionType()
{
    if(m_dataConversionParameter.m_conversionType == MDVariableDataConversionType::ByteOrderChange &&
           m_enableChangeConvert)
        m_propertyBrowser->setSinglePropertyHide("ByteOrder",false);
    else
        m_propertyBrowser->setSinglePropertyHide("ByteOrder",true);

    return (MDVariableDataConversionTypeProperty)m_dataConversionParameter.m_conversionType;
}

void CMDRTDBBatchChangeVariable::setConvertionType(MDVariableDataConversionTypeProperty type)
{
    m_dataConversionParameter.m_conversionType = (MDVariableDataConversionType)type;
}
bool CMDRTDBBatchChangeVariable::linearConvertion()
{
    //刷新属性框
    QStringList propList;
    propList.append("ProjectMaxValue");
    propList.append("ProjectMinValue");
    propList.append("DrivrMaxValue");
    propList.append("DrivrMinValue");
    if(m_dataConversionParameter.m_enableLinearConversion && m_enableChangeConvert)
        m_propertyBrowser->setPropertyHide(propList,false);
    else
        m_propertyBrowser->setPropertyHide(propList,true);

    return m_dataConversionParameter.m_enableLinearConversion;
}

void CMDRTDBBatchChangeVariable::setLinearConvertion(bool linearConvertion)
{
    m_dataConversionParameter.m_enableLinearConversion = linearConvertion;
}
double CMDRTDBBatchChangeVariable::maxValue()
{
    return m_dataConversionParameter.m_maxValue;
}

void CMDRTDBBatchChangeVariable::setMaxValue(double value)
{
    m_dataConversionParameter.m_maxValue = value;
}

double CMDRTDBBatchChangeVariable::minValue()
{
    return m_dataConversionParameter.m_minValue;
}

void CMDRTDBBatchChangeVariable::setMinValue(double value)
{
    m_dataConversionParameter.m_minValue = value;
}

double CMDRTDBBatchChangeVariable::maxDriverValue()
{
    return m_dataConversionParameter.m_originalMaxValue;
}

void CMDRTDBBatchChangeVariable::setMaxDriverValue(double value)
{
    m_dataConversionParameter.m_originalMaxValue = value;
}

double CMDRTDBBatchChangeVariable::minDriverValue()
{
    return m_dataConversionParameter.m_originalMinValue;
}

void CMDRTDBBatchChangeVariable::setMinDriverValue(double value)
{
    m_dataConversionParameter.m_originalMinValue = value;
}

QString CMDRTDBBatchChangeVariable::byteOrder()
{
    return m_dataConversionParameter.m_byteOrder;
}

void CMDRTDBBatchChangeVariable::setByteOrder(QString byteorder)
{
    m_dataConversionParameter.m_byteOrder = byteorder;
}

MDVariableDataConversionParameter CMDRTDBBatchChangeVariable::getDataConvertPara()
{
    return m_dataConversionParameter;
}

//变量写保护参数
bool CMDRTDBBatchChangeVariable::enableChangeWrite()
{
//    QStringList propList;
//    propList.append("EnableWrite");
//    propList.append("EnableWriteProtect");
//    propList.append("WriteProtectType");
//    propList.append("MaxWriteValue");
//    propList.append("MinWriteValue");
//    if(m_enableChangeWrite)
//    {
//        m_propertyBrowser->setPropertyHide(propList,false);
//    }
//    else
//    {
//        m_propertyBrowser->setPropertyHide(propList,true);
//    }
    return m_enableChangeWrite;
}
void CMDRTDBBatchChangeVariable::setEnableChangeWrite(bool enable)
{
    m_enableChangeWrite = enable;
}

bool CMDRTDBBatchChangeVariable::enableWrite()
{
//    //刷新属性
//    QStringList propList;
//    //propList.append("EnableWriteProtect");
//    propList.append("WriteProtectType");
//    propList.append("MaxWriteValue");
//    propList.append("MinWriteValue");
//    if(m_writeProtectParameter.m_enableWrite && m_enableChangeWrite)
//    {
//        m_propertyBrowser->setSinglePropertyHide("EnableWriteProtect",false);
//        m_propertyBrowser->setPropertyHide(propList,!m_writeProtectParameter.m_enableWriteProtect);
//    }
//    else
//    {
//        propList.append("EnableWriteProtect");
//        m_propertyBrowser->setPropertyHide(propList,true);
//    }
    return m_writeProtectParameter.m_enableWrite;
}

void CMDRTDBBatchChangeVariable::setEnableWrite(bool enable)
{
    m_writeProtectParameter.m_enableWrite = enable;
}

bool CMDRTDBBatchChangeVariable::enableWriteProtect()
{
//    QStringList propList;
//    propList.append("WriteProtectType");
//    propList.append("MaxWriteValue");
//    propList.append("MinWriteValue");
//    if(m_writeProtectParameter.m_enableWriteProtect && m_writeProtectParameter.m_enableWrite && m_enableChangeWrite)
//    {
//        m_propertyBrowser->setPropertyHide(propList,false);
//    }
//    else
//    {
//        m_propertyBrowser->setPropertyHide(propList,true);
//    }

    return m_writeProtectParameter.m_enableWriteProtect;
}

void CMDRTDBBatchChangeVariable::setEnableWriteProtect(bool enable)
{
    m_writeProtectParameter.m_enableWriteProtect = enable;
}

int CMDRTDBBatchChangeVariable::writeProtectType()
{
    return m_writeProtectParameter.m_writeProtectType;
}

void CMDRTDBBatchChangeVariable::setWriteProtectType(int writeType)
{
    m_writeProtectParameter.m_writeProtectType = writeType;
}

QString CMDRTDBBatchChangeVariable::maxWriteValue()
{
    return m_writeProtectParameter.m_maxValue;
}

void CMDRTDBBatchChangeVariable::setWriteMaxValue(QString value)
{
    m_writeProtectParameter.m_maxValue = value;
}

QString CMDRTDBBatchChangeVariable::minWriteValue()
{
    return m_writeProtectParameter.m_minValue;
}

void CMDRTDBBatchChangeVariable::setWriteMinValue(QString value)
{
    m_writeProtectParameter.m_minValue = value;
}

MDVariableWriteProtectParameter CMDRTDBBatchChangeVariable::getWriteProtectPara()
{
    return m_writeProtectParameter;
}
