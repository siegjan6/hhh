#include "MDDrawLine.h"
#include "MDNodeStream.h"
#include "MDPropertyDrawNode.h"

CMDDrawLine::CMDDrawLine()
{

}

CMDDrawLine::~CMDDrawLine()
{

}

DrawType CMDDrawLine::type()
{
    return DrawType::StraightLine;
}

CMDDrawObj *CMDDrawLine::createSelf()
{
    return new CMDDrawLine;
}

CMDPropertyDrawObj *CMDDrawLine::createPropertyDrawObj()
{
   return new CMDPropertyDrawLine(this);
}

void CMDDrawLine::drawPath(QPainter *p)
{
    CMDDrawNode::drawPath(p);

    if(!createSuccess())
        return;

    if(!pipeBackGroundVisible() && !pLineStream()->streamLineVisible())
    {
      drawLine(p);
    }
}

void CMDDrawLine::generateNodePath(QPainterPath *path)
{
    CMDDrawNode::generateNodePath(path);
    pLineStream()->onNodeChanged();
}

bool CMDDrawLine::createFinish()
{
    return this->nodeDatas()->count() >= 2;
}

void CMDDrawLine::drawLine(QPainter *p)
{
    factPen()->draw(p, rect(), basePath());
}

QList<QStringList> CMDDrawLine::expNames()
{
    QList<QStringList> list;
    list.append(CMDDrawNode::expNames());
    list << QStringList({"$drawvector.fill"});
    list << QStringList({"$drawvector.fillBrush"});
    list << QStringList({"$drawvector.brush"});

    return list;
}
