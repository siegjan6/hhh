#include "MDDrawSector.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawSector::CMDDrawSector()
{
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawSector::type()
{
    return DrawType::Sector;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawSector::createSelf()
{
    return new CMDDrawSector;
}
void CMDDrawSector::onGeneratePath()
{
    QPainterPath* path = basePath();
    QRectF rf = rect();

    path->arcMoveTo(rf, -startAngle());
    path->arcTo(rf, -startAngle(), -spanAngle());
    path->lineTo(rf.center());
    path->closeSubpath();

    generateCustom();
}
//////////////////////////////////////////////////////////////////////////

