#include "PropertyDrawAlarm.h"
#include "MDDrawAlarm.h"

void CPropertyDrawAlarm::setConfig(const QString &value)
{
    if(object()->openConfigDialog())
        setModify(true);
}

QString CPropertyDrawAlarm::config()
{
    return "";
}

CPropertyDrawAlarm::CPropertyDrawAlarm(CMDDrawAlarm *parent) :
    CMDPropertyDrawObj(parent)
{
        insertTranslateMap("config",tr("配置"));
}
CMDDrawAlarm *CPropertyDrawAlarm::object()
{
    return dynamic_cast<CMDDrawAlarm*>(m_object);
}
