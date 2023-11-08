#include "MDRTDBVariableUaBase.h"
#include "MDChangeVariable.h"
#include "MDVariableGroup.h"
#include "MDVariableDataConverter.h"
#include <QtCore/qmath.h>
#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

CMDRTDBVariable::CMDRTDBVariable(QUaServer *server)
    : QUaBaseObject(server)
{
    m_name = "";
    m_localName= "";
    m_groupName= "";
    m_deviceName= "";
    m_address="";
    m_dataBlockIndexID=-1;
    m_changeCount =0;
    m_dataType=MDDataType::Int16;
    m_curQuality=MDVariableDataQuality::None;
    m_lastQuality=MDVariableDataQuality::None;
    m_timeStamp=QDateTime::fromString("1900:01:01 00:00:00.000");
    m_internalAddress=-1;
    m_internalSubAddress=-1;
    m_variableType = MDVariableType::IOVariable;
    m_originalValue  = QVariant();
    m_curValue = QVariant();
    m_lastValue = QVariant();
    m_rtdbManager =NULL;
    m_isValid = true;
    m_itemType = MDTreeItemType::Variable;
    m_parentDataBlock = NULL;
    m_parentVariableGroup = NULL;
    m_precision =3;

    m_initializeParameter.m_initType = MDRTInitMode::None;
    m_initializeParameter.m_value = "";


    m_dataConversionParameter.m_enable=false;
    m_dataConversionParameter.m_enableLinearConversion=false;
    m_dataConversionParameter.m_maxValue=0;
    m_dataConversionParameter.m_minValue=0;
    m_dataConversionParameter.m_originalMaxValue=0;
    m_dataConversionParameter.m_originalMinValue=0;
    m_dataConversionParameter.m_conversionType = MDVariableDataConversionType::None;


    m_writeProtectParameter.m_enableWrite = true;
    m_writeProtectParameter.m_enableWriteProtect = true;
    m_writeProtectParameter.m_writeProtectType = 1;

    m_dataSaveRulerParameter.m_saveRuler = MDRTDataSaveRuler::None;
    m_dataSaveRulerParameter.m_saveMode = MDRTDataSaveMode::Memory;
    m_dataSaveRulerParameter.m_saveCycle = 1;
    m_dataSaveRulerParameter.m_timeRange = 360;
    //m_actionSPList.append(MDActionType::variable_WriteValue );

    type()->setDataTypeEnum(QMetaEnum::fromType<MDDataValueType>());
    value()->setWriteAccess(true);

    connect(value(), &QUaBaseDataVariable::valueChanged, this, &CMDRTDBVariable::OnValueChanged);
}

void CMDRTDBVariable::OnValueChanged(const QVariant &value, const bool m_bInternalWrite)
{
	/*if (m_rtdbManager->getSwitchState(MDLogicSwitchType::Log))
	{
		QString str = QString("write var(fromClient:%1): %2=%3").arg(m_bInternalWrite).arg(name()).arg(value.toString());
		m_rtdbManager->m_logSwitch->LOG_INFO(str);
	}*/
    //qDebug() << "OnValueChanged New value:" << value << "m_bInternalWrite: " << m_bInternalWrite;
	if (m_bInternalWrite)
	{
		// write request from ua client
		// need to write to actual variable
		writeValue(value);
	}
}

QUaProperty * CMDRTDBVariable::type() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Type");
}

QUaProperty * CMDRTDBVariable::unit() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Unit");
}

QUaProperty * CMDRTDBVariable::description() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("Description");
}

QUaBaseDataVariable * CMDRTDBVariable::value() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaBaseDataVariable>("Value");
}

QUaBaseDataVariable * CMDRTDBVariable::lastError() const
{
	return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaBaseDataVariable>("LastError");
}

QUaProperty * CMDRTDBVariable::hl() const
{
    return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("HL");
}

QUaProperty * CMDRTDBVariable::ll() const
{
    return const_cast<CMDRTDBVariable*>(this)->browseChild<QUaProperty>("LL");
}



CMDRTDBVariable::~CMDRTDBVariable()
{
	
}
void CMDRTDBVariable::copy(IMDRTDBVariable *otherVariable)
{
     if(otherVariable== NULL)
     {
         return;
     }
     m_name = otherVariable->name();
     m_localName= otherVariable->localName();
     m_groupName= ((CMDRTDBVariable*)otherVariable)->groupName();
     m_deviceName= otherVariable->deviceName();
     m_address=otherVariable->address();
     m_dataBlockIndexID=otherVariable->dataBlockIndexID();
     m_dataType=otherVariable->dataType();
     m_originalDataType=otherVariable->originalDataType();
     m_curQuality=otherVariable->currentQuality();
     m_lastQuality=otherVariable->lastQuality();
     m_timeStamp=otherVariable->timeStamp();
     m_internalAddress=otherVariable->internalAddress();
     m_internalSubAddress=otherVariable->internalSubAddress();
     m_variableType = otherVariable->variableType();
     m_originalValue =otherVariable->originalValue();
     m_curValue = otherVariable->currentValue();
     m_lastValue = otherVariable->lastValue();
     m_rtdbManager =((CMDRTDBVariable*)otherVariable)->getRTDBManager();
     m_isValid= otherVariable->isValid();
     m_itemType = otherVariable->itemType();
     m_precision =otherVariable->precision();
	 m_unit = otherVariable->getUnit();
     m_description = otherVariable->getDescription();
     m_initializeParameter = otherVariable->initializeParameter();
     m_dataConversionParameter = otherVariable->dataConversionParameter();
     m_dataSaveRulerParameter = otherVariable->dataSaveRulerParameter();
     m_writeProtectParameter = otherVariable->writeProtectParameter();
     m_actionSPList = *otherVariable->actionSPList();

}
void CMDRTDBVariable::initialze()
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
    removeAllRefrenceVariables();
}
/*!
\section IMDRTDBTreeItem 接口
*/
MDTreeItemType CMDRTDBVariable::itemType()
{
    return m_itemType;
}

void CMDRTDBVariable::setItemType(MDTreeItemType itemType)
{
    m_itemType = itemType;
}
void *CMDRTDBVariable::parentItem(MDTreeItemType parentItemType)
{
    if(parentItemType == MDTreeItemType::VariableGroup)
    {
        return m_parentVariableGroup;
    }
    else if(parentItemType == MDTreeItemType::DataBlock)
    {
        return m_parentDataBlock;
    }
    return NULL;
}
void CMDRTDBVariable::setParentItem(void* parent,MDTreeItemType parentItemType)
{
    if(parentItemType == MDTreeItemType::VariableGroup)
    {
        m_parentVariableGroup = (CMDVariableGroup *)parent;
    }
    else if(parentItemType == MDTreeItemType::DataBlock)
    {
        m_parentDataBlock = (IDataBlock*)parent;
    }
}


//名称
QString CMDRTDBVariable::name()
{
    if(NULL!=m_parentVariableGroup&& m_name.isEmpty())
    {
       QString parentName=m_parentVariableGroup->name();
       if(!parentName.isEmpty()&&!parentName.isNull())
       {
         m_name = QString("%1.%2").arg(parentName).arg(m_localName);
       }
    }
    return m_name;
}

void CMDRTDBVariable::setName(QString name)
{
    m_name = name;
}

QString CMDRTDBVariable::localName()
{
    return m_localName;
}

void CMDRTDBVariable::setLocalName(QString name)
{
    if(0!=QString::compare(m_localName, name,Qt::CaseSensitive))
    {
        m_localName = name;
        setName("");
    }
}

//单位
//QString CMDRTDBVariable::getUnit() const
//{
//	return m_unit;
//}

//void CMDRTDBVariable::setUnit(QString unit)
//{
//	m_unit = unit;
//}

//描述
//QString CMDRTDBVariable::getDescription() const
//{
//	return m_description;
//}

//void CMDRTDBVariable::setDescription(QString description)
//{
//	m_description = description;
//}

QString CMDRTDBVariable::groupName()
{
    if(NULL!=m_parentVariableGroup)
    {
        m_groupName=m_parentVariableGroup->name();
    }
    return m_groupName;
}

void CMDRTDBVariable::setGroupName(QString name)
{
    m_groupName = name;
}

//地址
QString CMDRTDBVariable::address()
{
    return m_address;
}
void CMDRTDBVariable::setAddress(QString address)
{
    m_address = address;
}

//品质

QString CMDRTDBVariable::qualityDescription(const MDVariableDataQuality &quality)
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
QString CMDRTDBVariable::currentQualityDescription()
{
    QMutexLocker locker(&m_objectMutex);
     return qualityDescription(m_curQuality);
}
MDVariableDataQuality CMDRTDBVariable::currentQuality()
{
     QMutexLocker locker(&m_objectMutex);
     return m_curQuality;
}
void CMDRTDBVariable::setCurrentQuality(MDVariableDataQuality quality)
{
    QMutexLocker locker(&m_objectMutex);
    m_curQuality = quality;
}
QString CMDRTDBVariable::lastQualityDescription()
{
    QMutexLocker locker(&m_objectMutex);
     return qualityDescription(m_lastQuality);
}
MDVariableDataQuality CMDRTDBVariable::lastQuality()
{
    QMutexLocker locker(&m_objectMutex);
     return m_lastQuality;
}
void CMDRTDBVariable::setLastQuality(MDVariableDataQuality quality)
{
    QMutexLocker locker(&m_objectMutex);
    m_lastQuality = quality;
}

//设备名称
QString CMDRTDBVariable::deviceName()
{
     return m_deviceName;
}
void CMDRTDBVariable::setDeviceName(QString deviceName)
{
    m_deviceName= deviceName;
}

//时间戳
QDateTime CMDRTDBVariable::timeStamp()
{
    QMutexLocker locker(&m_objectMutex);
     return m_timeStamp;
}
void CMDRTDBVariable::setTimeStamp(QDateTime timeStamp)
{
    QMutexLocker locker(&m_objectMutex);
    m_timeStamp = timeStamp;
}

MDDataType CMDRTDBVariable::originalDataType()
{
    return m_originalDataType;
}

void CMDRTDBVariable::setOriginalDataType(MDDataType dataType)
{
    m_originalDataType=dataType;
}

//原始值
QVariant CMDRTDBVariable::originalValue()
{
    QMutexLocker locker(&m_objectMutex);
     return m_originalValue;
}
void CMDRTDBVariable::setOriginalValue(QVariant value)
{
    QMutexLocker locker(&m_objectMutex);
    m_originalValue = value;
}

//当前值
QVariant CMDRTDBVariable::currentValue()
{
    QMutexLocker locker(&m_objectMutex);
     return m_curValue;
}
void CMDRTDBVariable::setCurrentValue(QVariant value)
{
    QMutexLocker locker(&m_objectMutex);
    m_curValue = value;
	setValue(value);
}

//上次值
QVariant CMDRTDBVariable::lastValue()
{
    QMutexLocker locker(&m_objectMutex);
     return m_lastValue;
}
void CMDRTDBVariable::setLastValue(QVariant value)
{
    QMutexLocker locker(&m_objectMutex);
    m_lastValue = value;
}

CMDActionSPList * CMDRTDBVariable::actionSPList()
{
    return &m_actionSPList;
}
void CMDRTDBVariable::setActionSPList(CMDActionSPList *actionSPList)
{
    m_actionSPList = *actionSPList;
}

//变量类型
MDVariableType CMDRTDBVariable::variableType()
{
     return m_variableType;
}
void CMDRTDBVariable::setVariableType(MDVariableType varType)
{
    m_variableType = varType;
}

QString CMDRTDBVariable::variableTypeDescription()
{
    QString ret = "";
    switch (m_variableType)
    {
    case MDVariableType::IOVariable:
        ret =QObject::tr("IO变量");
        break;
    case MDVariableType::VirtualVariable:
        ret =QObject::tr("虚拟变量");
        break;
    case MDVariableType::MiddleVariable:
        ret =QObject::tr("中间变量");
        break;
    default:
        break;
    }
    return ret;
}
//数据类型
MDDataType CMDRTDBVariable::dataType()
{
     return m_dataType;
}
void CMDRTDBVariable::setDataType(MDDataType dataType)
{
    m_dataType = dataType;
}

QString CMDRTDBVariable::dataTypeDescription()
{
    return nsDataTypeDescription(m_dataType);
}
QString CMDRTDBVariable::originalDataTypeDescription()
{
    return nsDataTypeDescription(m_originalDataType);
}
QString CMDRTDBVariable::nsDataTypeDescription(const MDDataType &dataType)
{
    QString ret = "";
    switch (dataType)
    {
    case MDDataType::Bool:
        ret ="Bool";
        break;
    case MDDataType::SByte:
        ret ="Char";
        break;
    case MDDataType::Byte:
        ret ="Byte";
        break;
    case MDDataType::Int16:
        ret ="Int16";
        break;
    case MDDataType::UInt16:
        ret ="UInt16";
        break;
    case MDDataType::Int32:
        ret ="Int32";
        break;
    case MDDataType::UInt32:
        ret ="UInt32";
        break;
    case MDDataType::Int64:
        ret ="Int64";
        break;
    case MDDataType::UInt64:
        ret ="UInt64";
        break;
    case MDDataType::Float:
        ret ="Float";
        break;
    case MDDataType::Double:
        ret ="Double";
        break;
    case MDDataType::DateTime:
        ret ="DateTime";
        break;
    case MDDataType::String:
        ret ="String";
        break;
    default:
        break;
    }
    return ret;
}

int CMDRTDBVariable::precision()
{
  return m_precision;
}

void CMDRTDBVariable::setPrecision(int precision)
{
    m_precision = precision;
}

//数据块名称
QString CMDRTDBVariable::dataBlockName()
{
    return m_dataBlockName;
}

void CMDRTDBVariable::setDataBlockName(QString dbName)
{
    m_dataBlockName = dbName;
}

//关联数据块编号
int CMDRTDBVariable::dataBlockIndexID()
{
     return m_dataBlockIndexID;
}
void CMDRTDBVariable::setDataBlockIndexID(int dataBlockIndexID)
{
    m_dataBlockIndexID = dataBlockIndexID;
}

//关联数据区地址
int CMDRTDBVariable::internalAddress()
{
     return m_internalAddress;
}
void CMDRTDBVariable::setInternalAddress(int internalAddress)
{
    m_internalAddress = internalAddress;
}

//关联数据区子地址
int CMDRTDBVariable::internalSubAddress()
{
     return m_internalSubAddress;
}
void CMDRTDBVariable::setInternalSubAddress(int internalSubAddress)
{
     m_internalSubAddress = internalSubAddress;
}
//有效性
bool CMDRTDBVariable::isValid()
{
    return m_isValid;
}
void CMDRTDBVariable::setValid(bool isValid)
{
    m_isValid = isValid;
}

MDRTDataSaveRuler CMDRTDBVariable::dataSaveRuler()
{
    return m_dataSaveRulerParameter.m_saveRuler;
}

MDRTDataSaveRulerParameter  CMDRTDBVariable::dataSaveRulerParameter()
{
    return m_dataSaveRulerParameter;
}

void  CMDRTDBVariable::setDataSaveRulerParameter(const MDRTDataSaveRulerParameter  &dataSaveRulerParameter)
{
    m_dataSaveRulerParameter = dataSaveRulerParameter;
}

//初始化参数
MDVariableInitializeParameter  CMDRTDBVariable::initializeParameter()
{
    return m_initializeParameter;
}

void  CMDRTDBVariable::setInitializeParameter(const MDVariableInitializeParameter  &parameter)
{
    m_initializeParameter = parameter;
}

//品质错误参数
MDVariableQualityIsBadParameter  CMDRTDBVariable::qualityIsBadParameter()
{
    return m_qualityIsBadParameter;
}

void  CMDRTDBVariable::setQualityIsBadParameter(const MDVariableQualityIsBadParameter  &parameter)
{
    m_qualityIsBadParameter = parameter;
}

//数据转换参数
MDVariableDataConversionParameter  CMDRTDBVariable::dataConversionParameter()
{
    return m_dataConversionParameter;
}

void  CMDRTDBVariable::setDataConversionParameter(const MDVariableDataConversionParameter  &parameter)
{
    m_dataConversionParameter = parameter;
    m_dataConversionParameter.m_byteOrder = parameter.m_byteOrder;
}
//变量写保护参数
MDVariableWriteProtectParameter  CMDRTDBVariable::writeProtectParameter()
{
    return m_writeProtectParameter;
}

void  CMDRTDBVariable::setWriteProtectParameter(const MDVariableWriteProtectParameter  &parameter)
{
    m_writeProtectParameter = parameter;
}

//变量限值报警参数
MDVariableAlarmParameter CMDRTDBVariable::variableAlarmParameter()
{
    return m_variableAlarmParameter;
}

void CMDRTDBVariable::setVariableAlarmParameter(const MDVariableAlarmParameter &parameter)
{
    m_variableAlarmParameter = parameter;
}

void CMDRTDBVariable::simulate()
{
    if (m_dataType == MDDataType::Bool)
    {
        bool bval = true;
        if (!currentValue().isNull())
        {
            bval = !currentValue().toBool();
        }
        refreshData(QVariant(bval),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false);
    }
    else if (m_dataType == MDDataType::String)
    {
         refreshData(QVariant(name()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false);
    }

    else if (m_dataType == MDDataType::DateTime)
    {
         refreshData(QVariant(QDateTime::currentDateTime()),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false);
    }
    else
    {
        double value = 0.0;
        if (!currentValue().isNull())
        {
            value = currentValue().toDouble()+1;
        }
        refreshData(QVariant(value),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false);
    }

}
qint64 CMDRTDBVariable::changeCount()
{
    return m_changeCount;
}

bool CMDRTDBVariable::getWriteLimitValue(QVariant valueIn, QVariant& valueOut)
{
    bool bManualLimit = true;
    double dbMaxManual = m_writeProtectParameter.m_maxValue.toDouble();
    double dbMinManual = m_writeProtectParameter.m_minValue.toDouble();

    //无限定工程值
    if(!m_writeProtectParameter.m_enableWriteProtect || (dbMaxManual == dbMinManual && dbMaxManual == 0))
        bManualLimit = false;

    //找出限制
    //设定边界值
    if(bManualLimit)
    {
        if(dbMinManual > dbMaxManual)//int64ManualMax as temp value here
        {
            double temp = dbMinManual;
            dbMinManual = dbMaxManual;
            dbMaxManual = temp;
        }
    }
    double dbMin = 0;
    double dbMax = 0;
    //获取类型边界值
    switch (m_dataType)
    {
       case MDDataType::Byte:
            dbMin = 0;
            dbMax = 255;
           break;
        case MDDataType::SByte:
            dbMin = -128;
            dbMax = 127;
            break;
        case MDDataType::Int16:
            dbMin = -32768;
            dbMax = 32767;
            break;
        case MDDataType::UInt16:
            dbMin = 0;
            dbMax = 65535;
            break;
        case MDDataType::Int32:
            dbMin = -2147483648;
            dbMax = 2147483647;
            break;
        case MDDataType::UInt32:
            dbMin = 0;
            dbMax = 4294967295;
            break;
        case MDDataType::Int64:
            dbMin = -9223372036854775808;
            dbMax = 9223372036854775807;
            break;
        case MDDataType::UInt64:
            dbMin = 0;
            dbMax = 0xFFFFFFFFFFFFFFFF;
            break;
        default:
            break;
     }
    //比较手动设置与类型边界值
    if(bManualLimit)
    {
        if(m_dataType != MDDataType::Float ||
           m_dataType != MDDataType::Double ||
           m_dataType != MDDataType::DateTime )
        {
            if(dbMinManual > dbMin && dbMinManual < dbMax)
                dbMin = dbMinManual;

            if(dbMaxManual > dbMin && dbMaxManual < dbMax)
                dbMax = dbMaxManual;
        }
        else
        {
            dbMin = dbMinManual;
            dbMax = dbMaxManual;
        }
    }
    double dbVarValueTemp = valueIn.toDouble();
    //与类型边界值比较
    if(dbMin != dbMax)
    {
        if(dbVarValueTemp < dbMin || dbVarValueTemp > dbMax)
        {
            //0：超限不写
            if(m_writeProtectParameter.m_writeProtectType == 0)
            {
                return false;
            }
            else
            {
                if(dbVarValueTemp < dbMin)
                    dbVarValueTemp = dbMin;
                else
                    dbVarValueTemp = dbMax;
            }
        }
    }
    switch (m_dataType)
    {
        case MDDataType::Bool:
           valueOut = QVariant(valueIn.toBool());
           break;
       case MDDataType::Byte:
           valueOut = QVariant(qint16(dbVarValueTemp));
           break;
        case MDDataType::SByte:
            valueOut = QVariant(qint16(dbVarValueTemp));
            break;
        case MDDataType::Int16:
            valueOut = QVariant(qint16(dbVarValueTemp));
            break;
        case MDDataType::UInt16:
            valueOut = QVariant(qint16(dbVarValueTemp));
            break;
        case MDDataType::Int32:
            valueOut = QVariant(qint32(dbVarValueTemp));
            break;
        case MDDataType::UInt32:
            valueOut = QVariant(quint32(dbVarValueTemp));
            break;
        case MDDataType::Int64:
            valueOut = QVariant(qint64(dbVarValueTemp));
            break;
        case MDDataType::UInt64:
            valueOut = QVariant(quint64(dbVarValueTemp));
            break;
        case MDDataType::DateTime:
            valueOut = QVariant(valueIn.toDateTime());
            break;
        case MDDataType::Float:
        case MDDataType::Double:
            if(dbMin != dbMax)
                valueOut = dbVarValueTemp;
            else
                valueOut = QVariant(double(qRound64( valueIn.toFloat()*qPow(10, m_precision))/(qPow(10, m_precision))));
           break;
        default:
            break;
     }
    return true;
}

bool CMDRTDBVariable::writeValue(QVariant varValue)
{
    if(variableType()==MDVariableType::MiddleVariable)
      // ||variableType()==MDVariableType::SystemVariable)
    {
        return false;
    }
    //写保护
    if(!m_writeProtectParameter.m_enableWrite)
    {
        return false;
    }
    QVariant originalValue,writeVarValue;
    //字符串、无需比较限值
    if(m_dataType != MDDataType::String)
    {
        if(!getWriteLimitValue(varValue,originalValue))
            return false;
    }
    else
    {
        originalValue = varValue;
    }


    if (m_dataConversionParameter.m_enable)//工程转换
    {
          QVariant originalValue;
          MDDataType originalDataType = m_originalDataType;
          if(variableType() ==MDVariableType::VirtualVariable)
          {
              originalDataType = m_dataType;
          }
          if(CMDVariableDataConverter::getOriginalValue(m_dataConversionParameter,originalDataType,m_dataType,m_precision,varValue,originalValue))
          {
                writeVarValue = originalValue;
          }
          else
          {
               return false;
          }
    }
    else
    {
        writeVarValue = varValue;
    }

    MDDataType originalDataType = m_originalDataType;
    if(variableType()!=MDVariableType::IOVariable)
    {
        originalDataType = m_dataType;
    }
    if(!CMDVariableDataConverter::getOriginalValue(m_dataConversionParameter,originalDataType,m_dataType,m_precision,originalValue,writeVarValue))
    {
          return false;
    }
    if(variableType()==MDVariableType::IOVariable)
    {
        if(m_parentDataBlock!=NULL)
        {
			IMDRTDBVariable* writeVar = m_rtdbManager->variableManager()->createVariable();
			if (writeVar == NULL)
				return false;
            // CMDRTDBVariable *writeVar = new CMDRTDBVariable();
            writeVar->copy(this);
            writeVar->setOriginalValue(writeVarValue);
            if(m_rtdbManager->getSwitchState(MDLogicSwitchType::Log))
            {
                QString str = QString("write var: %1=%2").arg(name()).arg(writeVarValue.toString());
                m_rtdbManager->m_logSwitch->LOG_INFO(str);
            }
            if(m_parentDataBlock->asyncWriteVariable(writeVar))
            {
               return true;
            }
            else
            {
                delete writeVar;
               return false;
            }
        }
        return false;
    }
    else if(variableType()==MDVariableType::VirtualVariable)
    {
        refreshData(writeVarValue,QDateTime::currentDateTime(),MDVariableDataQuality::Good,false);
        return true;
    }
    else
    {
        return false;
    }
}

//更新数据
bool CMDRTDBVariable::refreshData(QVariant dataValue,QDateTime timeStamp,MDVariableDataQuality quality,bool onlyChangedQuality)
{
    bool bRet = false;
    bool bLock = true;
    bool bValueChanged = false;//云通知时只需要数值变化
    m_objectMutex.lock();
    if (true == onlyChangedQuality)//只改变品质
    {
        if(m_curQuality != quality)
        {
            m_lastQuality = m_curQuality;
            m_timeStamp = timeStamp;
            m_curQuality = quality;
            m_changeCount++;
            if(m_rtdbManager != NULL)
            {
                m_objectMutex.unlock();
                bLock = false;
                bRet = true;
                m_rtdbManager->notifyNewChangeVariable(this,VariableChangeType::QualityChanged);
            }
        }
    }
    else//改变数值
    {
        if (m_originalValue !=dataValue && dataValue.isValid())
        {
             m_originalValue = dataValue;
             bool rightData = true;

             QVariant engineeringValue ;
             if(CMDVariableDataConverter::getEngineeringValue(m_dataConversionParameter,m_originalDataType,m_dataType,m_precision,m_originalValue,engineeringValue))
             {
                dataValue = engineeringValue;
             }
             else
             {
                rightData = false;

                //qDebug() << "DataConverter err: last value= " << m_lastValue << " current value= " << m_curValue;

               //!如果工程转换为6（Bit8BinaryWithAlarm），8，10，12，改变品质
               if(m_dataConversionParameter.m_conversionType == MDVariableDataConversionType::Bit8BinaryWithAlarm ||
                  m_dataConversionParameter.m_conversionType == MDVariableDataConversionType::Bit12BinaryWithAlarm ||
                  m_dataConversionParameter.m_conversionType == MDVariableDataConversionType::Bit13BinaryWithAlarm ||
                  m_dataConversionParameter.m_conversionType == MDVariableDataConversionType::Bit15BinaryWithAlarm)
                     quality = MDVariableDataQuality::DataConvertError;
             }

             if(rightData)
             {
                 switch (m_dataType)
                 {
                     case MDDataType::Float:
                     case MDDataType::Double:
                        dataValue= QVariant(double(qRound64( dataValue.toFloat()*qPow(10, m_precision))/(qPow(10, m_precision))));
                        break;
                     default:
                         break;
                 }
                 if (m_curValue !=dataValue)
                 {
                     m_lastQuality = m_curQuality;
                     m_lastValue = m_curValue;
                     m_timeStamp =timeStamp;
                     m_curValue = dataValue;
                     if(m_dataType == MDDataType::String)
                        qDebug() << "value change from " << m_lastValue << "to " << m_curValue;
                     bValueChanged = true;
                     m_changeCount++;
                     if (m_curQuality != quality)
                     {
                         m_curQuality = quality;
                         if(m_rtdbManager != NULL)
                         {
                             m_objectMutex.unlock();
                             bLock = false;
                             bRet = true;
                             m_rtdbManager->notifyNewChangeVariable(this,VariableChangeType::BothChanged);  
                             /*
                             if(m_rtdbManager->getSwitchState(MDLogicSwitchType::Archive) && m_dataSaveRulerParameter.m_saveRuler != MDRTDataSaveRuler::None)
                                m_rtdbManager->variableArchivesManager()->triggerChangeDataArchives(m_name,m_curValue,m_curQuality,m_timeStamp);
                             */
                          }
                     }
                     else
                     {
                         if(m_rtdbManager != NULL)
                         {
                             m_objectMutex.unlock();
                             bLock = false;
                             bRet = true;
                             m_rtdbManager->notifyNewChangeVariable(this,VariableChangeType::ValueChanged);
                             /*
                             if(m_rtdbManager->getSwitchState(MDLogicSwitchType::Archive) && m_dataSaveRulerParameter.m_saveRuler != MDRTDataSaveRuler::None)
                                m_rtdbManager->variableArchivesManager()->triggerChangeDataArchives(m_name,m_curValue,m_curQuality,m_timeStamp);
                              */
                          }
                     }

					 setValue(m_curValue);


                     if(bLock)
                     {
                         m_objectMutex.unlock();
                         bLock = false;
                     }
                     //计算关联中间变量
                     if(m_rtdbManager->getSwitchState(MDLogicSwitchType::Middle))//允许中间变量
                     {
                         for (int i = 0; i < getRefrenceVariableCount(); i++)
                         {
                               CMDRTDBVariable* nsVar = (CMDRTDBVariable* )getRefrenceVariable(i);
                               nsVar->doExpression(name(),m_curValue);
                         }
                     }
                 }
             }
             else//not right data
             {
                 if (m_curQuality != quality)
                 {
                     m_curQuality = quality;
                     if(m_rtdbManager != NULL)
                     {
                         m_objectMutex.unlock();
                         bLock = false;
                         bRet = true;
                         m_rtdbManager->notifyNewChangeVariable(this,VariableChangeType::QualityChanged);
                         /*
                         if(m_rtdbManager->getSwitchState(MDLogicSwitchType::Archive) && m_dataSaveRulerParameter.m_saveRuler != MDRTDataSaveRuler::None)
                            m_rtdbManager->variableArchivesManager()->triggerChangeDataArchives(m_name,m_curValue,m_curQuality,m_timeStamp);
                         */
                      }
                 }
             }
        }
        else if(m_curQuality != quality)
        {
            //!工程转换失败是上层转换时赋的品质。
            if(m_curQuality != MDVariableDataQuality::DataConvertError)
            {
                m_lastQuality = m_curQuality;
                m_timeStamp = timeStamp;
                m_curQuality = quality;
                m_changeCount++;
                if(m_rtdbManager != NULL)
                {
                    m_objectMutex.unlock();
                    bLock = false;
                    bRet = true;
                    m_rtdbManager->notifyNewChangeVariable(this,VariableChangeType::QualityChanged);
                }
            }
        }
    }

    //********************************* Y2hw ****************************
    //! 中间变量变化的云通知
   if(MDVariableType::VirtualVariable == variableType() ||
      (m_rtdbManager->getSwitchState(MDLogicSwitchType::NotifyDeviceVarChange) && variableType() == MDVariableType::MiddleVariable))
    {
        if(bRet && bValueChanged)
        {
            QStringList varNameList;
            QStringList varValueList;
            //name
            varNameList.append(localName());
            //value
            if(dataType() == MDDataType::Bool)
            {
                if(m_curValue.toBool())
                    varValueList.append("1");
                else
                    varValueList.append("0");
            }
            else
            {
                 varValueList.append(m_curValue.toString());
            }
            sendChangeVariablesToCloud(varNameList,varValueList);
        }
    }
    if(bLock)
    {
        m_objectMutex.unlock();
        bLock = false;
    }
    //------------------------------------------------------------------
    return bRet;
}
/*!
section 表达式等关联变量管理（中间变量）
*/
void CMDRTDBVariable::doExpression(const QString& varName,QVariant varValue)
{
    if(variableType()==MDVariableType::MiddleVariable)
    {
         m_varExpression.setVariableValue(varName,varValue);
         refreshData(m_varExpression.excute(),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false);
    }
}
void CMDRTDBVariable::doExpression()
{
    if(variableType()==MDVariableType::MiddleVariable && m_rtdbManager != NULL)
    {
        QList<QString>  referencedVarList =m_varExpression.referencedVarList();
        for(int i=0;i<referencedVarList.count();i++)
        {
            m_varExpression.setVariableValue(referencedVarList[i],m_rtdbManager->readVariableValue(referencedVarList[i]));
        }
        refreshData(m_varExpression.excute(),QDateTime::currentDateTime(),MDVariableDataQuality::Good,false);

    }
}

void CMDRTDBVariable::initialMiddleVariableExpression()
{
    if(variableType()==MDVariableType::MiddleVariable && m_rtdbManager != NULL)
    {
            m_varExpression.compile(address());
            QList<QString>  referencedVarList =m_varExpression.referencedVarList();
            for(int i=0;i<referencedVarList.count();i++)
            {
                CMDRTDBVariable *nsVar = (CMDRTDBVariable*)m_rtdbManager->variableManager()->getVariable(referencedVarList[i]);
                if(nsVar)
                {
                     nsVar->addRefrenceVariable(this);
                }
            }
    }
}


void CMDRTDBVariable::dynamicInitialMiddleVariableExpression()
{
    if(variableType()==MDVariableType::MiddleVariable && m_rtdbManager != NULL)
    {
        m_varExpression.compile(address());
        QList<QString>  referencedVarList =m_varExpression.referencedVarList();
        for(int i=0;i<referencedVarList.count();i++)
        {
            QString varName = referencedVarList[i];
            if(varName.indexOf(".")<=0)
            {
                varName = QString("%1.%2").arg(groupName()).arg(varName);

                m_varExpression.repaceVariableName(referencedVarList[i],varName);
            }
            CMDRTDBVariable *nsVar = (CMDRTDBVariable*)m_rtdbManager->variableManager()->getVariable(varName);
            if(nsVar)
            {
                 nsVar->addRefrenceVariable(this);
            }
        }
    }
}


void CMDRTDBVariable::removeAllRefrenceVariables()
{
    QMutexLocker locker(&m_refrenceVarListMutex);
    m_refrenceVarList.clear();
}
void CMDRTDBVariable::addRefrenceVariable(IMDVariable* refVar)
{
    if(NULL != refVar)
    {
        QMutexLocker locker(&m_refrenceVarListMutex);
        m_refrenceVarList.append(refVar);
    }
}
void CMDRTDBVariable::removeRefrenceVariable(IMDVariable* refVar)
{
    if(NULL != refVar)
    {
        QMutexLocker locker(&m_refrenceVarListMutex);
        m_refrenceVarList.removeOne(refVar);
    }
}
IMDVariable* CMDRTDBVariable::getRefrenceVariable(int index)
{
    IMDVariable* refVar = NULL;
    QMutexLocker locker(&m_refrenceVarListMutex);
    if (index < m_refrenceVarList.count() && index>=0)
    {
        refVar = m_refrenceVarList[index];
    }
    return refVar;
}
int CMDRTDBVariable::getRefrenceVariableCount()
{
    int iCount = 0;
    QMutexLocker locker(&m_refrenceVarListMutex);
    iCount = m_refrenceVarList.count();
    return iCount;
}
bool CMDRTDBVariable::hasRefrenceVariable()
{
    bool bRet = false;
    QMutexLocker locker(&m_refrenceVarListMutex);

    if (m_refrenceVarList.count() > 0)
    {
      bRet =true;
    }
    return bRet;
}
/*!
\section MDRTDBManager
*/
void CMDRTDBVariable::setRTDBManager(CMDRTDBManager *rtdbManager)
{
    m_rtdbManager= rtdbManager;
}

CMDRTDBManager * CMDRTDBVariable::getRTDBManager()
{
    return m_rtdbManager;
}
/*!
\section 读写变量属性
*/
void CMDRTDBVariable::readConfiguration(QDomElement &domElement )
{
    if(!domElement.isNull())
    {

        QDomElement element = domElement.firstChildElement("address");
        QString dd1 = element.text();

        element = domElement.firstChildElement("deviceName");
        QString dd2 = element.text();

         element = element.nextSiblingElement();
         QString dd4 = element.text();
        element = domElement.firstChildElement("name");
        QString dd3 = element.text();

        while ( !element.isNull() )
        {
            if(0==QString::compare(element.tagName(),"Name", Qt::CaseInsensitive))
            {
                this->setLocalName(element.text());
            }
            else if(0==QString::compare(element.tagName(),"Description", Qt::CaseInsensitive))
            {
                this->setDescription(element.text());
            }
            else if(0==QString::compare(element.tagName(),"GroupName", Qt::CaseInsensitive))
            {
                this->setGroupName(element.text());
            }
            else if(0==QString::compare(element.tagName(),"DeviceName", Qt::CaseInsensitive))
            {
                this->setDeviceName(element.text());
            }
            else if(0==QString::compare(element.tagName(),"Address", Qt::CaseInsensitive))
            {
                this->setAddress(element.text());
            }
            else if(0==QString::compare(element.tagName(),"DataType", Qt::CaseInsensitive))
            {
                this->setDataType((MDDataType)element.text().toInt());
            }
            else if(0==QString::compare(element.tagName(),"VariableType", Qt::CaseInsensitive))
            {
                this->setVariableType((MDVariableType)element.text().toInt());
            }
            else if(0==QString::compare(element.tagName(),"OriginalDataType", Qt::CaseInsensitive))
            {
                this->setOriginalDataType((MDDataType)element.text().toInt());
            }
            element = element.nextSiblingElement();
        }
    }
}
void CMDRTDBVariable::saveConfiguration(QDomDocument &doc,QDomElement &parentElement)
{
    QDomElement variableElement = doc.createElement("Variable");
    parentElement.appendChild(variableElement);

    QDomElement element = doc.createElement("Name");
    variableElement.appendChild(element);
    QDomText elementText = doc.createTextNode(m_localName);
    element.appendChild(elementText);

    element = doc.createElement("Description");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(m_description);
    element.appendChild(elementText);

    element = doc.createElement("GroupName");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(groupName());
    element.appendChild(elementText);

    element = doc.createElement("DeviceName");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(m_deviceName);
    element.appendChild(elementText);

    element = doc.createElement("Address");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(m_address);
    element.appendChild(elementText);

    element = doc.createElement("DataType");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString::number((int)m_dataType,10));
    element.appendChild(elementText);


    element = doc.createElement("VariableType");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString::number((int)m_variableType,10));
    element.appendChild(elementText);


    element = doc.createElement("OriginalDataType");
    variableElement.appendChild(element);
    elementText = doc.createTextNode(QString::number((int)m_originalDataType,10));
    element.appendChild(elementText);

    element = doc.createElement("DataSaveRulerParameter");
    variableElement.appendChild(element);
    {
        QDomAttr elementAttr = doc.createAttribute("Mode");
        elementAttr.setValue(QString::number((int)m_dataSaveRulerParameter.m_saveMode,10));
        element.setAttributeNode(elementAttr);

        elementAttr = doc.createAttribute("Ruler");
        elementAttr.setValue(QString::number((int)m_dataSaveRulerParameter.m_saveRuler,10));
        element.setAttributeNode(elementAttr);

        elementAttr = doc.createAttribute("Cycle");
        elementAttr.setValue(QString::number((int)m_dataSaveRulerParameter.m_saveCycle,10));
        element.setAttributeNode(elementAttr);

        elementAttr = doc.createAttribute("TimeRange");
        elementAttr.setValue(QString::number((int)m_dataSaveRulerParameter.m_timeRange,10));
        element.setAttributeNode(elementAttr);
    }

}

void CMDRTDBVariable::read(QDataStream &in)
{
    int version=0;
   in>>version;  //版本
   in>>m_localName;
   in>>m_description;
   in>>m_deviceName;
   in>>m_address;
   int intTemp=0;
   in>>intTemp;
   m_dataType = (MDDataType)intTemp;
   in>>intTemp;
   m_variableType = (MDVariableType)intTemp;
   in>>intTemp;
   m_originalDataType = (MDDataType)intTemp;
   in>>intTemp;
   m_dataSaveRulerParameter.m_saveMode = (MDRTDataSaveMode)intTemp;
   in>>intTemp;
   m_dataSaveRulerParameter.m_saveRuler = (MDRTDataSaveRuler)intTemp;

   in>>m_dataSaveRulerParameter.m_saveCycle;
   in>>m_dataSaveRulerParameter.m_timeRange;
   if(version>=2)
   {
       in>>m_writeProtectParameter.m_enableWrite;
       in>>m_writeProtectParameter.m_enableWriteProtect;
       in>>m_writeProtectParameter.m_maxValue;
       in>>m_writeProtectParameter.m_minValue;
       in>>m_writeProtectParameter.m_writeProtectType;
   }
    if(version>=3)
    {
        in>>m_dataConversionParameter.m_enable;
        in>>m_dataConversionParameter.m_maxValue;
        in>>m_dataConversionParameter.m_minValue;
        in>>m_dataConversionParameter.m_originalMaxValue;
        in>>m_dataConversionParameter.m_originalMinValue;
        int conversionType;
        in>>conversionType;
        m_dataConversionParameter.m_conversionType = (MDVariableDataConversionType)conversionType;
        if(version>=5)
        {
          in>>m_dataConversionParameter.m_enableLinearConversion;
        }
    }

    if(version>=4)
    {
       m_actionSPList.deleteAll();
       m_actionSPList.deserialize(in);       
    }
    if(version>=6)
    {
        in>>intTemp;
        m_initializeParameter.m_initType = (MDRTInitMode)intTemp;
        if(m_initializeParameter.m_initType == MDRTInitMode::LastValue)
            m_rtdbManager->enableSaveConfigBeforeUnLoad();
        in>>m_initializeParameter.m_value;
    }

 /*
if(m_localName == "aa__adaf")
{
    MDVariableDataConversionParameter varDataConversionParameter ;
    varDataConversionParameter.m_enable = true;
    varDataConversionParameter.m_enableLinearConversion=true;
    varDataConversionParameter.m_maxValue=999.9;
    varDataConversionParameter.m_minValue=0;
    varDataConversionParameter.m_originalMaxValue=9999;
    varDataConversionParameter.m_originalMinValue=0;
    varDataConversionParameter.m_conversionType = MDVariableDataConversionType::OMRONBCD4;




     setDataConversionParameter(varDataConversionParameter);
}
else
{
    MDVariableDataConversionParameter varDataConversionParameter ;
    varDataConversionParameter.m_enable = false;
    varDataConversionParameter.m_enableLinearConversion=true;
    varDataConversionParameter.m_maxValue=999.9;
    varDataConversionParameter.m_minValue=0;
    varDataConversionParameter.m_originalMaxValue=9999;
    varDataConversionParameter.m_originalMinValue=0;
    varDataConversionParameter.m_conversionType = MDVariableDataConversionType::BCD4;




     setDataConversionParameter(varDataConversionParameter);
} */

}

void CMDRTDBVariable::serialize(CSortJsonObject& json)
{
    // Name
    json.insertSimpleValue("@N",m_localName);
    // Unit
    json.insertSimpleValue("@UNIT", m_unit);
    // Description
    json.insertSimpleValue("@D",m_description);
    // DeviceName
    json.insertSimpleValue("@DN",m_deviceName);
    // Address
    json.insertSimpleValue("@AD",m_address);
    // DataType
    json.insertSimpleValue("@DT",QString::number((int)m_dataType));
    // VariableType
    json.insertSimpleValue("@VT",QString::number((int)m_variableType));
    // DriverDataType or OriginalDataType
    json.insertSimpleValue("@ODT",QString::number((int)m_originalDataType));
    // Precision
    json.insertSimpleValue("@PCS",QString::number(m_precision));

    //archive
    CSortJsonObject archive;
    // ArchiveMode
    archive.insertSimpleValue("@AM",QString::number((int)m_dataSaveRulerParameter.m_saveMode));
    // ArchiveRuler
    archive.insertSimpleValue("@AR",QString::number((int)m_dataSaveRulerParameter.m_saveRuler));
    // ArchiveInterval
    archive.insertSimpleValue("@AI",QString::number(m_dataSaveRulerParameter.m_saveCycle));
    // ArchiveTimePeriod
    archive.insertSimpleValue("@ATP",QString::number(m_dataSaveRulerParameter.m_timeRange));
    json.insertObject("ACV",archive);

    //write proof
    CSortJsonObject write_proof;
    // EnableWrite
    write_proof.insertSimpleValue("@EW",m_writeProtectParameter.m_enableWrite?QString("1"):QString("0"));
    // EnableWriteProtect
    write_proof.insertSimpleValue("@EWP",m_writeProtectParameter.m_enableWriteProtect?QString("1"):QString("0"));
    // WriteMaxValue
    write_proof.insertSimpleValue("@WMAV",m_writeProtectParameter.m_maxValue);
    // WriteMinValue
    write_proof.insertSimpleValue("@WMIV",m_writeProtectParameter.m_minValue);
    // OutOfLimitProtect
    if(m_writeProtectParameter.m_writeProtectType == 0)
        write_proof.insertSimpleValue("@ELP","0");
    else
        write_proof.insertSimpleValue("@ELP","1");
    json.insertObject("WP",write_proof);

    //Data Convertion
    CSortJsonObject convert;
    //json.insertSimpleValue("EnableConvertion",m_dataConversionParameter.m_enable?QString("1"):QString("0"));
    // EnableLinearConvert
    convert.insertSimpleValue("@ELC",m_dataConversionParameter.m_enableLinearConversion?QString("1"):QString("0"));
    //增加类型描述
//    CSortJsonObject jsonTypeDescription;
//    jsonTypeDescription.insertSimpleValue("0","None"); // 无
//    jsonTypeDescription.insertSimpleValue("1","BCD3"); // 3位BCD码
//    jsonTypeDescription.insertSimpleValue("2","BCD4"); // 4位BCD码
//    jsonTypeDescription.insertSimpleValue("3","BCD8"); // 8位BCD码
//    jsonTypeDescription.insertSimpleValue("4","OMRONBCD4"); // OMR有符号4位BCD码
//    jsonTypeDescription.insertSimpleValue("5","Bit8BinaryWithoutAlarm"); // 8位二进制无报警
//    jsonTypeDescription.insertSimpleValue("6","Bit8BinaryWithAlarm"); // 8位二进制有报警
//    jsonTypeDescription.insertSimpleValue("7","Bit12BinaryWithoutAlarm"); // 12位二进制无报警
//    jsonTypeDescription.insertSimpleValue("8","Bit12BinaryWithAlarm"); // 12位二进制有报警
//    jsonTypeDescription.insertSimpleValue("9","Bit13BinaryWithoutAlarm"); // 13位二进制无报警
//    jsonTypeDescription.insertSimpleValue("10","Bit13BinaryWithAlarm"); // 13位二进制有报警
//    jsonTypeDescription.insertSimpleValue("11","Bit15BinaryWithoutAlarm"); // 15位二进制无报警
//    jsonTypeDescription.insertSimpleValue("12","Bit15BinaryWithAlarm"); // 15位二进制有报警
//    jsonTypeDescription.insertSimpleValue("13","ByteOrderChange");// 数据字节顺序转换
//    jsonTypeDescription.insertSimpleValue("14","SiemensBCD4"); // 西门子有符号4位BCD码
//    json.insertObject("ConvertionTypeDescription",jsonTypeDescription);
    // ConvertionType
    convert.insertSimpleValue("@CT",QString::number((int)m_dataConversionParameter.m_conversionType));
    // ProjectMaxValue
    convert.insertSimpleValue("@PMAV",QString::number(m_dataConversionParameter.m_maxValue));
    // ProjectMinValue
    convert.insertSimpleValue("@PMIV",QString::number(m_dataConversionParameter.m_minValue));
    // DriverMaxValue
    convert.insertSimpleValue("@OMAV",QString::number(m_dataConversionParameter.m_originalMaxValue));
    // DriverMinValue
    convert.insertSimpleValue("@OMIV",QString::number(m_dataConversionParameter.m_originalMinValue));
    json.insertObject("CVT",convert);

    QJsonArray actionArr;
    m_actionSPList.serialize(actionArr);
    CSortJsonArray* jsonArr = CSortJsonObject::converetQJsonArray2JsonArray(actionArr);
    if(jsonArr != NULL)
    {
        // Actions
        json.insertArray("@AUTH",*jsonArr);
        delete jsonArr;
        jsonArr = NULL;
    }
    //Initialization parameter
    //version=6
    // InitType
    json.insertSimpleValue("@IV",QString::number((int)m_initializeParameter.m_initType));
    if(m_initializeParameter.m_initType == MDRTInitMode::LastValue)
        m_initializeParameter.m_value = m_curValue.toString();
    // LastValue
    json.insertSimpleValue("@LV",m_initializeParameter.m_value);
}

void CMDRTDBVariable::deserialize(const QJsonObject &json)
{
    // Name
    m_localName = json.value("@N").toString();
    // Unit
    if(json.contains("@UNIT"))
        m_unit = json.value("@UNIT").toString();
    // Description
    m_description = json.value("@D").toString();
    // DeviceName
    m_deviceName = json.value("@DN").toString();
    // Address
    m_address = json.value("@AD").toString();

    // DataType
    m_dataType = (MDDataType)json.value("@DT").toString("3").toInt();//default:MDDataType::Int16
    // VariableType
    m_variableType = (MDVariableType)json.value("@VT").toString("1").toInt();
    // DriverDataType or OriginalDataType
    m_originalDataType = (MDDataType)json.value("@ODT").toString("3").toInt();//default:MDDataType::Int16
    m_precision = json.value("@PCS").toString("3").toInt();

    //archive
    if(json.contains("ACV"))
    {
        QJsonObject archive = json.value("ACV").toObject();
        m_dataSaveRulerParameter.m_saveMode = (MDRTDataSaveMode)archive.value("@AM").toString().toInt((int)MDRTDataSaveMode::Memory);
        m_dataSaveRulerParameter.m_saveRuler = (MDRTDataSaveRuler)archive.value("@AR").toString().toInt((int)MDRTDataSaveRuler::None);
        m_dataSaveRulerParameter.m_saveCycle = archive.value("@AI").toString("1").toInt();
        m_dataSaveRulerParameter.m_timeRange = archive.value("@ATP").toString("360").toInt();
    }
    //write proof
    if(json.contains("WP"))
    {
        QJsonObject write_proof = json.value("WP").toObject();
        if(write_proof.value("@EW").toString("1") == "1")
            m_writeProtectParameter.m_enableWrite = true;
        else
            m_writeProtectParameter.m_enableWrite = false;

        m_writeProtectParameter.m_enableWriteProtect = write_proof.value("@EWP").toBool();
        m_writeProtectParameter.m_maxValue = write_proof.value("@WMAV").toString();
        m_writeProtectParameter.m_minValue = write_proof.value("@WMIV").toString();
        if(write_proof.value("@ELP").toString("1") == "1")
            m_writeProtectParameter.m_writeProtectType  = 1;
        else
            m_writeProtectParameter.m_writeProtectType  = 0;
    }

    //Data Convertion
    /*
    if(json.value("EnableConvertion").toString("0") == "1")
        m_dataConversionParameter.m_enable = true;
    else
        m_dataConversionParameter.m_enable = false;
    */
    if(json.contains("CVT"))
    {
        QJsonObject convert = json.value("CVT").toObject();
        m_dataConversionParameter.m_conversionType = (MDVariableDataConversionType)convert.value("@CT").toString().toInt((int)MDVariableDataConversionType::None);
        m_dataConversionParameter.m_maxValue = convert.value("@PMAV").toString().toDouble();
        m_dataConversionParameter.m_minValue = convert.value("@PMIV").toString().toDouble();
        m_dataConversionParameter.m_originalMaxValue = convert.value("@OMAV").toString().toDouble();
        m_dataConversionParameter.m_originalMinValue = convert.value("@OMIV").toString().toDouble();
        if(convert.value("@ELC").toString("0") == "1")
            m_dataConversionParameter.m_enableLinearConversion = true;
        else
            m_dataConversionParameter.m_enableLinearConversion = false;
    }

    m_actionSPList.deserialize(json.value("@AUTH").toArray());

    //Initialization
    m_initializeParameter.m_initType = (MDRTInitMode)json.value("@IV").toString().toInt((int)MDRTInitMode::None);
    m_initializeParameter.m_value = json.value("@LV").toString();

	// uaserver
    setType((MDDataValueType)json.value("@DT").toString("3").toInt());
	setUnit(m_unit);
	setDescription(m_description);
    if(m_writeProtectParameter.m_enableWrite)
    {
        this->value()->setWriteAccess(m_writeProtectParameter.m_enableWrite);
        setHL(m_writeProtectParameter.m_maxValue.toDouble());
        setLL(m_writeProtectParameter.m_minValue.toDouble());
    }
}

void CMDRTDBVariable::save(QDataStream &out)
{
    out<<int(6);  //版本
    out<<m_localName;
    out<<m_description;
    out<<m_deviceName;
    out<<m_address;
    out<<(int)m_dataType;
    out<<(int)m_variableType;
    out<<(int)m_originalDataType;
    out<<(int)m_dataSaveRulerParameter.m_saveMode;
    out<<(int)m_dataSaveRulerParameter.m_saveRuler;
    out<<m_dataSaveRulerParameter.m_saveCycle;
    out<<m_dataSaveRulerParameter.m_timeRange;
    //version=2
    out<<m_writeProtectParameter.m_enableWrite;
    out<<m_writeProtectParameter.m_enableWriteProtect;
    out<<m_writeProtectParameter.m_maxValue;
    out<<m_writeProtectParameter.m_minValue;
    out<<m_writeProtectParameter.m_writeProtectType;
    //version=3
    out<<m_dataConversionParameter.m_enable;
    out<<m_dataConversionParameter.m_maxValue;
    out<<m_dataConversionParameter.m_minValue;
    out<<m_dataConversionParameter.m_originalMaxValue;
    out<<m_dataConversionParameter.m_originalMinValue;
    out<<int(m_dataConversionParameter.m_conversionType);
    //version=5
    out<<m_dataConversionParameter.m_enableLinearConversion;
    //version=4
    m_actionSPList.serialize(out);
    //Initialization parameter
    //version=6
    out<<(int)m_initializeParameter.m_initType;
    if(m_initializeParameter.m_initType == MDRTInitMode::LastValue)
        m_initializeParameter.m_value = m_curValue.toString();
    out<<m_initializeParameter.m_value;
}


/*!
\section 关联数据块存在变量变化时通知实时数据管理器
*/
/*!
 * \brief 设备多个变量变化通知
 * 先通知Redis的实时，后publish设备的变化信息
 */
void CMDRTDBVariable::notifyDeviceVariablesChange(QList<IMDRTDBVariable*>& varlist,bool OnlyChangedQuality)
{
    if(NULL != m_rtdbManager)
    {
        bool bonlineChanged = false;
        //! 允许设备变量变化通知
        if(!m_rtdbManager->getSwitchState(MDLogicSwitchType::NotifyDeviceVarChange))
        {
            return;
        }
        QString varName = QString("%1.onLine").arg(groupName());
        IMDRTDBVariable * nsVar = m_rtdbManager->variableManager()->getVariable(varName);
        //********************************* Y2hw *******************************
        //OnlyChangedQuality为TRUE时：stop（）+notifyNotConnectDevice()，说明断线
        //OnlyChangedQuality为FALSE时：有设备变量变化，设备在线
        if(nsVar != NULL)
        {
            QVariant varValue = nsVar->currentValue();
            if(OnlyChangedQuality)
            {
                if(varValue.toBool())
                {
                   nsVar->writeValue(QVariant(false));
                   bonlineChanged = true;

                }
            }
            else
            {
                if(!varValue.toBool())
                {
                   nsVar->writeValue(QVariant(true));
                   bonlineChanged = true;
                }
            }
        }
        /*
        if(OnlyChangedQuality)
        {
            if(nsVar!= NULL)
            {
                QVariant varValue = nsVar->currentValue();

                switch(m_curQuality)
                {
                 case MDVariableDataQuality::None:
                 case MDVariableDataQuality::ConnectDeviceFailed:
                 case MDVariableDataQuality::StopCommunication:
                     {
                        if(varValue.toBool())
                        {
                           nsVar->writeValue(QVariant(false));
                           bonlineChanged = true;
                           //m_rtdbManager->notifyDeviceVariablesChange(groupName());
                        }
                     }
                     break;
                 default:
                    {
                      if(!varValue.toBool())
                      {
                         nsVar->writeValue(QVariant(true));
                         bonlineChanged = true;
                         //m_rtdbManager->notifyDeviceVariablesChange(groupName());
                      }
                    }
                    break;
                }
            }
        }
        else
        {
            if(nsVar!= NULL)
            {
                QVariant varValue = nsVar->currentValue();
                switch(m_curQuality)
                {
                 case MDVariableDataQuality::None:
                 case MDVariableDataQuality::ConnectDeviceFailed:
                 case MDVariableDataQuality::StopCommunication:
                     {
                        if(varValue.toBool())
                        {
                           nsVar->writeValue(QVariant(false));
                           bonlineChanged = true;
                        }
                     }
                     break;
                 default:
                    {
                      if(!varValue.toBool())
                      {
                         nsVar->writeValue(QVariant(true));
                         bonlineChanged = true;
                      }
                    }
                    break;
                }
            }
            //m_rtdbManager->notifyDeviceVariablesChange(groupName());
        }*/


        QStringList varNameList;
        QStringList varValueList;
        //check change variable
        if(!varlist.isEmpty())
        {
            CMDRTDBVariable* pvar = NULL;
            for(int i = 0; i<varlist.count(); i++)
            {
                pvar = (CMDRTDBVariable*)varlist.at(i);
                if(pvar == NULL)
                    continue;
                varNameList.append(pvar->localName());
                if(pvar->dataType() == MDDataType::Bool)
                {
                    if(pvar->currentValue().toBool())
                        varValueList.append("1");
                    else
                        varValueList.append("0");
                }
                else
                {
                     varValueList.append(pvar->currentValue().toString());
                }
            }
        }

        if(bonlineChanged && nsVar != NULL)
        {
            varNameList.append("onLine");
            if(nsVar->currentValue().toBool())
            {
                varValueList.append("1");
            }
            else
            {
                varValueList.append("0");
            }
        }
        sendChangeVariablesToCloud(varNameList,varValueList);
        //----------------------------------------------------------
    }
}
void CMDRTDBVariable::sendChangeVariablesToCloud(QStringList& varNameList, QStringList& varValueList)
{
    if(varNameList.isEmpty())
        return;

    //redis hmset 命令
    QList<QByteArray> redisCmdList;
    redisCmdList.append("hmset");
    QString skey = "RTData.Devices." + groupName();
    redisCmdList.append(skey.toUtf8());
    //redis publish 命令
    QList<QByteArray> jsonMsgList;

    QVariantMap devcontent;
    int ncnt = varNameList.count();
    for(int i = 0; i<ncnt; i++)
    {
        devcontent.insert(varNameList.at(i),varValueList.at(i));
        redisCmdList.append(varNameList.at(i).toUtf8());
        redisCmdList.append(varValueList.at(i).toUtf8());
    }
    //通知实时数据
    m_rtdbManager->notifyDeviceVariablesWithHmset(groupName(),redisCmdList);
    QJsonObject jsondevcontent = QJsonObject::fromVariantMap(devcontent);

    QVariantMap devcontent1;
    devcontent1.insert(groupName(),jsondevcontent.toVariantMap());
    QJsonObject devjson = QJsonObject::fromVariantMap(devcontent1);
    QJsonDocument msgout(devjson);
    jsonMsgList.append(msgout.toJson());
    //publish数据
    m_rtdbManager->notifyDeviceVariablesChange(groupName(),jsonMsgList);
    varNameList.clear();
    varValueList.clear();
}

// uaserver
MDDataValueType CMDRTDBVariable::getType() const
{
    return this->type()->value().value<MDDataValueType>();
}

void CMDRTDBVariable::setType(const MDDataValueType & type)
{
	if (this->getType() == type)
	{
		return;
	}
	this->type()->setValue(type);
	//this->on_typeChanged(type, true);
}

QString CMDRTDBVariable::getUnit()
{
	return this->unit()->value().value<QString>();
}

void CMDRTDBVariable::setUnit(QString unit)
{
	/*if (this->getUnit() == unit)
	{
		return;
	}*/
	this->unit()->setValue(unit);
	m_unit = unit;
	//this->on_unitChanged(unit, true);
}

QString CMDRTDBVariable::getDescription()
{
	return this->description()->value().value<QString>();
}

void CMDRTDBVariable::setDescription(QString description)
{
	/*if (this->getDescription() == description)
	{
		return;
	}*/
	this->description()->setValue(description);
	m_description = description;
	//this->on_descriptionChanged(description, true);
}

QVariant CMDRTDBVariable::getValue() const
{
	return this->value()->value();
}

void CMDRTDBVariable::setValue(const QVariant & value)
{
    //this->value()->setDataTypeEnum(QMetaEnum::fromType<CMDRTDBVariable::MDDataValueType>());
	this->value()->setDataType(dataType2MetaType());
	this->value()->setValue(value);
	//this->on_valueChanged(value, true);
}


QVariant CMDRTDBVariable::getHL() const
{
    return this->hl()->value();
}

void CMDRTDBVariable::setHL(const QVariant & value)
{
    this->hl()->setDataType(dataType2MetaType());
    this->hl()->setValue(value);
}

QVariant CMDRTDBVariable::getLL() const
{
    return this->ll()->value();
}

void CMDRTDBVariable::setLL(const QVariant & value)
{
    this->ll()->setDataType(dataType2MetaType());
    this->ll()->setValue(value);
}

//#include <QMessageBox>
void CMDRTDBVariable::setValue(const QUaBaseDataVariable *value)
{
	//QMessageBox::information(0,"", value->value().toString());
}

void CMDRTDBVariable::removeFromUa()
{
	/*QUaBaseObject* obj = (QUaBaseObject*)vartemp;
	if (obj)
		obj->*/
		//removeBaseObject();
	/*if ((QUaBaseObject*)this)
		delete (QUaBaseObject*)this;*/
}

QMetaType::Type CMDRTDBVariable::dataType2MetaType()
{
	QMetaType::Type ret;
	switch (m_dataType)
	{
	case MDDataType::Bool:
		ret = QMetaType::Bool;
		break;
	case MDDataType::SByte:
		ret = QMetaType::Char;
		break;
	case MDDataType::Byte:
		ret = QMetaType::UChar;
		break;
	case MDDataType::Int16:
		ret = QMetaType::Short;
		break;
	case MDDataType::UInt16:
		ret = QMetaType::UShort;
		break;
	case MDDataType::Int32:
		ret = QMetaType::Int;
		break;
	case MDDataType::UInt32:
		ret = QMetaType::UInt;
		break;
	case MDDataType::Int64:
		ret = QMetaType::Long;
		break;
	case MDDataType::UInt64:
		ret = QMetaType::ULong;
		break;
	case MDDataType::Float:
		ret = QMetaType::Float;
		break;
	case MDDataType::Double:
		ret = QMetaType::Double;
		break;
	case MDDataType::DateTime:
		ret = QMetaType::QDateTime;
		break;
	case MDDataType::String:
		ret = QMetaType::QString;
		break;
	default:
		break;
	}
	return ret;
}
