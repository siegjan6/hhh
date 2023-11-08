#include "MDDrawClosedBezier.h"
#include "MyTools.h"
#include "MDNodeStream.h"

CMDDrawClosedBezier::CMDDrawClosedBezier():CMDDrawNode()
{

}

CMDDrawClosedBezier::~CMDDrawClosedBezier()
{

}

DrawType CMDDrawClosedBezier::type()
{
    return DrawType::ClosedBezier;
}

CMDDrawObj *CMDDrawClosedBezier::createSelf()
{
    return new CMDDrawClosedBezier;
}

void CMDDrawClosedBezier::drawPath(QPainter *p)
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

void CMDDrawClosedBezier::generateNodePath(QPainterPath *path)
{
    int count = nodeDatas()->count();
    if(count <= 1)
        return;
    int mo = count - 1 % 3;

    QList<QPointF> list = MyTools::convertBeziers(*nodeDatas(),true);

    for(int i = 0  ;i < mo;i+=3)
    {
        list.append(nodeDatas()->last());
    }

    path->moveTo(list.at(0));
    for(int i = 1;i + 3 < list.count();i+=3)
    {
        path->cubicTo(list.at(i),list.at(i+1),list.at(i+2));
    }

    pLineStream()->onNodeChanged();
}

bool CMDDrawClosedBezier::isVisible(const QPointF &point)
{
    return CMDDrawVector::isVisible(point);
}

void CMDDrawClosedBezier::drawLine(QPainter *p)
{
     factPen()->draw(p, rect(), basePath());
}

QList<QStringList> CMDDrawClosedBezier::expNames()
{
    QList<QStringList> list;
    list.append(CMDDrawNode::expNames());
    list << QStringList({"$drawNode.percent"});

    return list;
}
