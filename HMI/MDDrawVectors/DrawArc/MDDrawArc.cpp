#include "MDDrawArc.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawArc::CMDDrawArc()
{
    brush()->setBrushType(CMDBrushManager::BrushType::Nullptr);
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawArc::type()
{
    return DrawType::Arc;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawArc::createSelf()
{
    return new CMDDrawArc;
}
void CMDDrawArc::onGeneratePath()
{
    QPainterPath* path = basePath();
    QRectF rf = rect();

    path->arcMoveTo(rf, -startAngle());
    path->arcTo(rf, -startAngle(), -spanAngle());

    generateCustom();
}
//////////////////////////////////////////////////////////////////////////

