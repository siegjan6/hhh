#include "PropertyDrawGauge.h"
#include "MDDrawGauge.h"
//////////////////////////////////////////////////////////////////////////
CPropertyDrawGauge::CPropertyDrawGauge(CMDDrawObj *parent) :
    CMDPropertyDrawVector(parent)
{
    insertTranslateMap("config",tr("配置"));
}
CMDDrawGauge *CPropertyDrawGauge::object()
{
    return dynamic_cast<CMDDrawGauge*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
QString CPropertyDrawGauge::config()
{
    return "";
}
void CPropertyDrawGauge::setConfig(const QString &value)
{
    if (object()->openDesign())
        setModify(true);
}
//////////////////////////////////////////////////////////////////////////
