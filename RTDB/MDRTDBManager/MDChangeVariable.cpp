#include "MDChangeVariable.h"
CMDChangeVariable::CMDChangeVariable()
{
    m_name = "";
    m_unit = "";
    m_description = "";
    m_ext = "";
    m_dataType=MDDataType::Float;
    m_curQuality=MDVariableDataQuality::None;
    m_lastQuality = MDVariableDataQuality::None;
    m_timeStamp=QDateTime::currentDateTime();
    m_curValue = QVariant();
    m_lastValue = QVariant();
    m_variableChangeType = VariableChangeType::BothChanged;
    m_variableSaveRuler = MDRTDataSaveRuler::None;
}

CMDChangeVariable::~CMDChangeVariable()
{

}

//名称
QString CMDChangeVariable::name()
{
    return m_name;
}

void CMDChangeVariable::setName(QString name)
{
    m_name = name;
}
//描述
QString CMDChangeVariable::getUnit()
{
	return m_unit;
}

void CMDChangeVariable::setUnit(QString unit)
{
	m_unit = unit;
}
//描述
QString CMDChangeVariable::getDescription()
{
    return m_description;
}

void CMDChangeVariable::setDescription(QString description)
{
    m_description = description;
}

//扩展
QString CMDChangeVariable::getExt()
{
    return m_ext;
}
void CMDChangeVariable::setExt(QString ext)
{
    m_ext = ext;
}

//当前值
QVariant CMDChangeVariable::currentValue()
{
     return m_curValue;
}
void CMDChangeVariable::setCurrentValue(QVariant value)
{
    m_curValue = value;
}

//上次值
QVariant CMDChangeVariable::lastValue()
{
     return m_lastValue;
}
void CMDChangeVariable::setLastValue(QVariant value)
{
    m_lastValue = value;
}
//当前品质
MDVariableDataQuality CMDChangeVariable::currentQuality()
{
     return m_curQuality;
}
void CMDChangeVariable::setCurrentQuality(MDVariableDataQuality quality)
{
    m_curQuality = quality;
}
//上次的品质
MDVariableDataQuality CMDChangeVariable::lastQuality()
{
     return m_lastQuality;
}
void CMDChangeVariable::setLastQuality(MDVariableDataQuality quality)
{
    m_lastQuality = quality;
}

//时间戳
QDateTime CMDChangeVariable::timeStamp()
{
     return m_timeStamp;
}
void CMDChangeVariable::setTimeStamp(QDateTime timeStamp)
{
    m_timeStamp = timeStamp;
}

//数据类型
MDDataType CMDChangeVariable::dataType()
{
     return m_dataType;
}
void CMDChangeVariable::setDataType(MDDataType dataType)
{
    m_dataType = dataType;
}

//变化通知类型
void  CMDChangeVariable::setVariableChangeType(VariableChangeType varChangeType)
{
    m_variableChangeType = varChangeType;
}

VariableChangeType  CMDChangeVariable::variableChangeType()
{
    return m_variableChangeType;
}

MDRTDataSaveRuler CMDChangeVariable::dataSaveRuler()
{
    return m_variableSaveRuler;
}

void CMDChangeVariable::copy(IMDChangeVariable *otherChangeVariable)
{
    if(otherChangeVariable != NULL)
    {
        m_name = otherChangeVariable->name();
        m_dataType=otherChangeVariable->dataType();
        m_curQuality=otherChangeVariable->currentQuality();
        m_lastQuality=otherChangeVariable->lastQuality();
        m_timeStamp=otherChangeVariable->timeStamp();
        m_curValue = otherChangeVariable->currentValue();
        m_lastValue = otherChangeVariable->lastValue();
        m_variableChangeType = otherChangeVariable->variableChangeType();
        m_variableSaveRuler = otherChangeVariable->dataSaveRuler();
    }
}
void CMDChangeVariable::copyEx(IMDVariable *otherVariable)
{
    if(otherVariable != NULL)
    {
        m_name = otherVariable->name();
        m_dataType=otherVariable->dataType();
        m_curQuality=otherVariable->currentQuality();
        m_lastQuality=otherVariable->lastQuality();
        m_timeStamp=otherVariable->timeStamp();
        m_curValue = otherVariable->currentValue();
        m_lastValue = otherVariable->lastValue();
        m_variableSaveRuler = otherVariable->dataSaveRuler();
    }
}


