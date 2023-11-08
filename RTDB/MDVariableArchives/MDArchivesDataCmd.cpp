#include "MDArchivesDataCmd.h"

CMDArchivesDataCmd::CMDArchivesDataCmd()
{
    m_varName = "";
}

//名称
QString CMDArchivesDataCmd::varName()
{
    return m_varName;
}

void CMDArchivesDataCmd::setVarName(const QString & name)
{
    m_varName = name;
}
QVariant CMDArchivesDataCmd::value()
{
    return m_value;
}

void CMDArchivesDataCmd::setValue(const QVariant & value)
{
    m_value = value;
}
QDateTime CMDArchivesDataCmd::timeStamp()
{
    return m_timeStamp;
}

void CMDArchivesDataCmd::setTimeStamp(const QDateTime & timeStamp)
{
    m_timeStamp = timeStamp;
}

MDVariableDataQuality CMDArchivesDataCmd::quality()
{
    return m_quality;
}

void CMDArchivesDataCmd::setQuality(const MDVariableDataQuality & quality)
{
    m_quality = quality;
}
MDRTInitMode CMDArchivesDataCmd::initMode()
{
    return m_initMode;
}

void CMDArchivesDataCmd::setInitMode(MDRTInitMode mode)
{
    m_initMode = mode;
}

MDRTDataSaveRuler CMDArchivesDataCmd::saveRuler()
{
    return m_saveRuler;
}

void CMDArchivesDataCmd::setSaveRuler(MDRTDataSaveRuler ruler)
{
    m_saveRuler = ruler;
}
