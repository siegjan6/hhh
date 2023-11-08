#include "PropertyDrawLog.h"
#include "MDDrawLog.h"

void CPropertyDrawLog::setConfig(const QString &value)
{
    if(object()->openConfigDialog())
        setModify(true);
}

QString CPropertyDrawLog::config()
{
    return "";
}

CPropertyDrawLog::CPropertyDrawLog(CMDDrawLog *parent) :
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("config",tr("配置"));
}

CMDDrawLog *CPropertyDrawLog::object()
{
    return dynamic_cast<CMDDrawLog*>(m_object);
}
