#include "APC_PlayTTS.h"
#include "MDActionPropertyData.h"

// 播放文本
CAPC_PlayTTS::CAPC_PlayTTS()
    : CMDActionProperty_Config(MDActionType::tts_Play)
{
    m_data1 = new CAPCData_PlayTTS;
}

CAPC_PlayTTS::~CAPC_PlayTTS()
{
    if(m_data1)
        delete m_data1;
}

CAPC_PlayTTS::CAPC_PlayTTS(const CAPC_PlayTTS& obj)
 : CMDActionProperty_Config(obj)
{
    m_data1 = new CAPCData_PlayTTS;

    copy(&obj); // 基类属性被重复拷贝1次
}

CAPC_PlayTTS& CAPC_PlayTTS::operator = (const CAPC_PlayTTS& obj)
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
qint8 CAPC_PlayTTS::mode() const
{
    return (m_data1 ? m_data1->m_mode : -1);
}
void CAPC_PlayTTS::setMode(qint8 mode)
{
    if(m_data1)
        m_data1->m_mode = mode;
}

qint8 CAPC_PlayTTS::repeatCount() const
{
    return (m_data1 ? m_data1->m_repeat : 1);
}
void CAPC_PlayTTS::setRepeatCount(qint8 repeatCount)
{
    if(m_data1)
        m_data1->m_repeat = repeatCount;
}

QString CAPC_PlayTTS::ttsText() const
{
    return (m_data1 ? m_data1->m_ttsText : "");
}
void CAPC_PlayTTS::setTtsText(const QString& ttsText)
{
    if(m_data1)
        m_data1->m_ttsText = ttsText;
}

QString CAPC_PlayTTS::ttsTextFile() const
{
    return (m_data1 ? m_data1->m_ttsTextFile : "");
}
void CAPC_PlayTTS::setTtsTextFile(const QString& ttsTextFile)
{
    if(m_data1)
        m_data1->m_ttsTextFile = ttsTextFile;
}

bool CAPC_PlayTTS::copy(const CMDActionProperty_Config* source)
{
    CMDActionProperty_Config::copy(source);

    return (m_data1 ? m_data1->copy(((CAPC_PlayTTS*)source)->m_data1) : false);
}

/*!
\section 序列化
*/
bool CAPC_PlayTTS::serialize(QDataStream& stream, bool useCurrentVersion)
{
    if(!CMDActionProperty_Config::serialize(stream, useCurrentVersion))
        return false;

    return (m_data1 ? m_data1->serialize(stream, useCurrentVersion) : false);
}

void CAPC_PlayTTS::deserialize(QDataStream& stream)
{
    CMDActionProperty_Config::deserialize(stream);

    if(m_data1)
        m_data1->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CAPC_PlayTTS::serialize(QJsonObject& json)
{
    CMDActionProperty_Config::serialize(json);

    if(m_data1)
        m_data1->serialize(json);
}

void CAPC_PlayTTS::deserialize(const QJsonObject& json)
{
    CMDActionProperty_Config::deserialize(json);

    if(m_data1)
        m_data1->deserialize(json);
}
