#include "MDAlarmPara.h"
#include "MDAlarmParaImp.h"

CMDAlarmPara::CMDAlarmPara()
{
    m_data = new CMDAlarmParaImp;
}

CMDAlarmPara::~CMDAlarmPara()
{
    if(m_data)
        delete m_data;
}

CMDAlarmPara::CMDAlarmPara(const CMDAlarmPara& obj)
{
    m_data = new CMDAlarmParaImp(*obj.m_data);

    copy(&obj);
}

CMDAlarmPara& CMDAlarmPara::operator = (const CMDAlarmPara& obj)
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
void CMDAlarmPara::enumModes(QStringList& modeList)
{
   modeList.append(modeToString(MDAlarmMode::common));
   modeList.append(modeToString(MDAlarmMode::extend));
}

QString CMDAlarmPara::modeToString(MDAlarmMode mode)
{
   QString str;
   switch (mode) {
   case MDAlarmMode::common: str = "common";
       break;
   case MDAlarmMode::extend: str = "extend";
       break;
   default:
       break;
   }
   return str;
}

MDAlarmMode CMDAlarmPara::stringToMode(const QString& str)
{
   MDAlarmMode m = MDAlarmMode::common;
   if(0 == str.compare("common"))
       m = MDAlarmMode::common;
   else if(0 == str.compare("extend"))
       m = MDAlarmMode::extend;
   else{};
   return m;
}

MDAlarmMode CMDAlarmPara::alarmMode() const
{
    return (m_data ? m_data->m_mode : MDAlarmMode::common);
}
void CMDAlarmPara::setAlarmMode(MDAlarmMode mode)
{
    if(m_data)
        m_data->m_mode = mode;
}

bool CMDAlarmPara::copy(const CMDAlarmPara* source)
{
    return (m_data ? m_data->copy(source->m_data) : false);
}

/*!
\section 序列化
*/
bool CMDAlarmPara::serialize(QDataStream& stream, bool useCurrentVersion)
{
    return (m_data ? m_data->serialize(stream, useCurrentVersion) : false);
}

void CMDAlarmPara::deserialize(QDataStream& stream)
{
    if(m_data)
        m_data->deserialize(stream);
}

/*!
\section 序列化：JSON格式
*/
void CMDAlarmPara::serialize(QJsonObject& json)
{
    if(m_data)
        m_data->serialize(json);
}

void CMDAlarmPara::deserialize(const QJsonObject& json)
{
    if(m_data)
        m_data->deserialize(json);
}
