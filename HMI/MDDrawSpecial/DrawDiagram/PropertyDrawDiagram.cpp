#include "PropertyDrawDiagram.h"
#include "MDDrawDiagram.h"
//////////////////////////////////////////////////////////////////////////
CPropertyDrawDiagram::CPropertyDrawDiagram(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("config",tr("配置"));
}
CMDDrawDiagram *CPropertyDrawDiagram::object()
{
    return dynamic_cast<CMDDrawDiagram*>(m_object);
}
//////////////////////////////////////////////////////////////////////////
QString CPropertyDrawDiagram::config()
{
    return "";
}
void CPropertyDrawDiagram::setConfig(const QString &value)
{
    if (object()->openDesign())
        setModify(true);
}
//////////////////////////////////////////////////////////////////////////
