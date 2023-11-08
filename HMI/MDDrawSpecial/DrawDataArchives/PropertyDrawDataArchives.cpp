#include "PropertyDrawDataArchives.h"
#include "MDDrawDataArchives.h"
void CPropertyDrawDataArchives::setConfig(const QString &value)
{
    if(object()->openConfigDialog())
        setModify(true);
}
QString CPropertyDrawDataArchives::config()
{
    return "";
}
CPropertyDrawDataArchives::CPropertyDrawDataArchives(CMDDrawDataArchives *parent):
    CMDPropertyDrawObj(parent)
{
        insertTranslateMap("config",tr("配置"));
}
CMDDrawDataArchives *CPropertyDrawDataArchives::object()
{
    return dynamic_cast<CMDDrawDataArchives*>(m_object);
}
