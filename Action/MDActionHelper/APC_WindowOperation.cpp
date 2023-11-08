#include "APC_WindowOperation.h"
#include "MDActionPropertyData.h"

// 窗口动作
CAPC_WindowOperation::CAPC_WindowOperation()
    : CMDActionProperty_Config(MDActionType::window_Open)
{
    m_data1 = new CAPCData_WindowOperation;
}

CAPC_WindowOperation::~CAPC_WindowOperation()
{
    if(m_data1)
        delete m_data1;
}

CAPC_WindowOperation::CAPC_WindowOperation(const CAPC_WindowOperation& obj)
 : CMDActionProperty_Config(obj)
{
    m_data1 = new CAPCData_WindowOperation;

    copy(&obj); // 基类属性被重复拷贝1次
}

CAPC_WindowOperation& CAPC_WindowOperation::operator = (const CAPC_WindowOperation& obj)
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
QString CAPC_WindowOperation::second() const
{
    return (m_data1 ? m_data1->m_second : "");
}
void CAPC_WindowOperation::setSecond(const QString& windowName)
{
    if(m_data1)
        m_data1->m_second = windowName;
}

int CAPC_WindowOperation::left() const
{
    return (m_data1 ? m_data1->m_left : 0);
}
void CAPC_WindowOperation::setLeft(int left)
{
    if(m_data1)
        m_data1->m_left = left;
}

int CAPC_WindowOperation::top() const
{
    return (m_data1 ? m_data1->m_top : 0);
}
void CAPC_WindowOperation::setTop(int top)
{
    if(m_data1)
        m_data1->m_top = top;
}

int CAPC_WindowOperation::width() const
{
    return (m_data1 ? m_data1->m_width : 100);
}
void CAPC_WindowOperation::setWidth(int width)
{
    if(m_data1)
        m_data1->m_width = width;
}

int CAPC_WindowOperation::height() const
{
    return (m_data1 ? m_data1->m_height : 100);
}
void CAPC_WindowOperation::setHeight(int height)
{
    if(m_data1)
        m_data1->m_height = height;
}

int CAPC_WindowOperation::pwid() const
{
    return (m_data1 ? m_data1->m_pwid : 0);
}
void CAPC_WindowOperation::setPwid(int pwid)
{
    if(m_data1)
        m_data1->m_pwid = pwid;
}

QRect CAPC_WindowOperation::geometry() const
{
    return (m_data1 ? m_data1->m_gRect : QRect(0,0,0,0));
}
void CAPC_WindowOperation::setGeometry(const QRect grect)
{
    if(m_data1)
        m_data1->m_gRect = grect;
}

QString CAPC_WindowOperation::meta() const
{
    return (m_data1 ? m_data1->m_metadata : "");
}
void CAPC_WindowOperation::setMeta(const QString& meta)
{
    if(m_data1)
        m_data1->m_metadata = meta;
}

bool CAPC_WindowOperation::copy(const CMDActionProperty_Config* source)
{
    CMDActionProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CAPC_WindowOperation*)source)->m_data1) : false);
}

/*!
\section 序列化
*/
bool CAPC_WindowOperation::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDActionProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CAPC_WindowOperation::deserialize(QDataStream& stream)
{
    CMDActionProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CAPC_WindowOperation::serialize(QJsonObject& json)
{
    CMDActionProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CAPC_WindowOperation::deserialize(const QJsonObject& json)
{
    CMDActionProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
