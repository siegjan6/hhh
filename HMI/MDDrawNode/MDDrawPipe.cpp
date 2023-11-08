#include "MDDrawPipe.h"
#include "MDNodeStream.h"
#include "MDPropertyDrawNode.h"

CMDDrawPipe::CMDDrawPipe()
{
    setPipeBackGroundVisible(true);
}

CMDDrawPipe::~CMDDrawPipe()
{

}

DrawType CMDDrawPipe::type()
{
    return DrawType::Pipe;
}

CMDDrawObj *CMDDrawPipe::createSelf()
{
    return new CMDDrawPipe;
}

CMDPropertyDrawObj *CMDDrawPipe::createPropertyDrawObj()
{
    return new CMDPropertyDrawPipe(this);
}

void CMDDrawPipe::drawPath(QPainter *p)
{
    if(!pipeBackGroundVisible() && !pLineStream()->streamLineVisible())
    {
      drawLine(p);
    }
    CMDDrawNode::drawPath(p);
}

void CMDDrawPipe::generateNodePath(QPainterPath *path)
{
    CMDDrawNode::generateNodePath(path);
    pLineStream()->onNodeChanged();
}

void CMDDrawPipe::drawLine(QPainter *p)
{
    factPen()->draw(p, rect(), basePath());
}
QList<QStringList> CMDDrawPipe::expNames()
{
    QList<QStringList> list;
    list.append(CMDDrawNode::expNames());
    list << QStringList({"$drawvector.fill"});
    list << QStringList({"$drawvector.fillBrush"});
    list << QStringList({"$drawvector.brush"});

    return list;
}
