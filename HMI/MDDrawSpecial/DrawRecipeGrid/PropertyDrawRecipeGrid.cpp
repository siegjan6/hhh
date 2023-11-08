#include "PropertyDrawRecipeGrid.h"
#include "MDDrawRecipeGrid.h"
//////////////////////////////////////////////////////////////////////////
CPropertyDrawRecipeGrid::CPropertyDrawRecipeGrid(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    insertTranslateMap("config",tr("配置"));
}
//////////////////////////////////////////////////////////////////////////
CMDDrawRecipeGrid *CPropertyDrawRecipeGrid::object()
{
    return dynamic_cast<CMDDrawRecipeGrid*>(m_object);
}
QString CPropertyDrawRecipeGrid::DILconfig() const
{
    return QString();
}
void CPropertyDrawRecipeGrid::setDILconfig(QString)
{
}
