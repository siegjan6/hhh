#include "APC_VariableWriteValue.h"
#include "MDActionPropertyData.h"

// 写变量
CAPC_VariableWriteValue::CAPC_VariableWriteValue()
    : CMDActionProperty_Config(MDActionType::variable_WriteValue)
{
    m_data1 = new CAPCData_VariableWriteValue;
}

CAPC_VariableWriteValue::~CAPC_VariableWriteValue()
{
    if(m_data1)
        delete m_data1;
}

CAPC_VariableWriteValue::CAPC_VariableWriteValue(const CAPC_VariableWriteValue& obj)
    : CMDActionProperty_Config(obj)
{
    m_data1 = new CAPCData_VariableWriteValue;

    copy(&obj); // 基类属性被重复拷贝1次
}

CAPC_VariableWriteValue& CAPC_VariableWriteValue::operator = (const CAPC_VariableWriteValue& obj)
{
    if( this != &obj )
    {
        copy(&obj);
    }
    return *this;
}

/*!
\section 属性
*/
void CAPC_VariableWriteValue::enumMode(QStringList& modeList)
{
    modeList.clear();

    modeList.append(modeToString(MDWriteMode::mode_fixed));
    modeList.append(modeToString(MDWriteMode::mode_relative));
    modeList.append(modeToString(MDWriteMode::mode_switch_0_1));
}

QString CAPC_VariableWriteValue::modeToString(MDWriteMode mode)
{
    QString str = "";
    switch (mode) {
    case MDWriteMode::null_mode:
        break;
    case MDWriteMode::mode_fixed:
        str = "fixed";
        break;
    case MDWriteMode::mode_relative:
        str = "relative";
        break;
    case MDWriteMode::mode_switch_0_1:
        str = "0/1";
        break;
    default:
        break;
    }
    return str;
}

MDWriteMode CAPC_VariableWriteValue::stringToMode(const QString& str)
{
    MDWriteMode mode = MDWriteMode::null_mode;
    if(0 == str.compare(""))
        mode = MDWriteMode::null_mode;
    else if(0 == str.compare("fixed"))
        mode = MDWriteMode::mode_fixed;
    else if(0 == str.compare("relative"))
        mode = MDWriteMode::mode_relative;
    else if(0 == str.compare("0/1"))
        mode = MDWriteMode::mode_switch_0_1;
    else{};
    return mode;
}

MDWriteMode CAPC_VariableWriteValue::mode() const
{
    return (m_data1 ? m_data1->m_mode : MDWriteMode::null_mode);
}
void CAPC_VariableWriteValue::setMode(MDWriteMode mode)
{
    if(m_data1)
        m_data1->m_mode = mode;
}

QVariant CAPC_VariableWriteValue::value() const
{
    return (m_data1 ? m_data1->m_value : QVariant());
}
void CAPC_VariableWriteValue::setValue(const QVariant& value)
{
    if(m_data1)
        m_data1->m_value = value;
}

qint8 CAPC_VariableWriteValue::valueSource() const
{
    return (m_data1 ? m_data1->m_valueSource : -1);
}
void CAPC_VariableWriteValue::setValueSource(qint8 valueSource)
{
    if(m_data1)
        m_data1->m_valueSource = valueSource;
}

QString CAPC_VariableWriteValue::sourceName() const
{
    return (m_data1 ? m_data1->m_sourceName : "");
}
void CAPC_VariableWriteValue::setSourceName(const QString& sourceName)
{
    if(m_data1)
        m_data1->m_sourceName = sourceName;
}

bool CAPC_VariableWriteValue::copy(const CMDActionProperty_Config* source)
{
    CMDActionProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CAPC_VariableWriteValue*)source)->m_data1) : false);
}

/*!
\section 序列化
*/
bool CAPC_VariableWriteValue::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDActionProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CAPC_VariableWriteValue::deserialize(QDataStream& stream)
{
    CMDActionProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CAPC_VariableWriteValue::serialize(QJsonObject& json)
{
    CMDActionProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CAPC_VariableWriteValue::deserialize(const QJsonObject& json)
{
    CMDActionProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
