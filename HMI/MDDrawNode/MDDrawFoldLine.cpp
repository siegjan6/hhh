#include "MDDrawFoldLine.h"
#include "MDNodeStream.h"
#include "MDPropertyDrawNode.h"

CMDDrawFoldLine::CMDDrawFoldLine()
{
    brush()->setBrushType(CMDBrushManager::BrushType::Nullptr);
}

CMDDrawFoldLine::~CMDDrawFoldLine()
{

}

DrawType CMDDrawFoldLine::type()
{
    return DrawType::FoldLine;
}

CMDDrawObj *CMDDrawFoldLine::createSelf()
{
    return new CMDDrawFoldLine;
}

void CMDDrawFoldLine::drawPath(QPainter *p)
{
    factBrush()->draw(p, rect(), basePath());

    CMDDrawNode::drawPath(p);

    if(!createSuccess())
        return;

    if(!pipeBackGroundVisible() && !pLineStream()->streamLineVisible())
    {
        drawLine(p);
    }
}

void CMDDrawFoldLine::generateNodePath(QPainterPath *path)
{
    CMDDrawNode::generateNodePath(path);
    pLineStream()->onNodeChanged();
}

void CMDDrawFoldLine::drawLine(QPainter *p)
{
    factPen()->draw(p, rect(), basePath());
}
