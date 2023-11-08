﻿#include "MDDrawShear.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawShear::CMDDrawShear()
{
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawShear::type()
{
    return DrawType::Shear;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawShear::createSelf()
{
    return new CMDDrawShear;
}
void CMDDrawShear::onGeneratePath()
{
    QPainterPath* path = basePath();
    QRectF rf = rect();

    path->arcMoveTo(rf, -startAngle());
    path->arcTo(rf, -startAngle(), -spanAngle());
    path->closeSubpath();

    generateCustom();
}
//////////////////////////////////////////////////////////////////////////

