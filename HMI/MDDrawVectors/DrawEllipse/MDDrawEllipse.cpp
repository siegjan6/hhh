﻿#include "MDDrawEllipse.h"
//////////////////////////////////////////////////////////////////////////
CMDDrawEllipse::CMDDrawEllipse()
{
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawEllipse::type()
{
    return DrawType::Ellipse;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawEllipse::createSelf()
{
    return new CMDDrawEllipse;
}
void CMDDrawEllipse::onGeneratePath()
{
    QPainterPath* path = basePath();
    path->addEllipse(rect());
}
//////////////////////////////////////////////////////////////////////////
