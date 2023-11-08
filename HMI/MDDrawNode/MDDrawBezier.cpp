#include "MDDrawBezier.h"
#include "MyTools.h"
#include "MDNodeStream.h"

CMDDrawBezier::CMDDrawBezier():CMDDrawNode()
{
    brush()->setBrushType(CMDBrushManager::BrushType::Nullptr);
}

CMDDrawBezier::~CMDDrawBezier()
{

}

DrawType CMDDrawBezier::type()
{
    return DrawType::Bezier;
}

CMDDrawObj *CMDDrawBezier::createSelf()
{
    return new CMDDrawBezier;
}

void CMDDrawBezier::drawPath(QPainter *p)
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

void CMDDrawBezier::generateNodePath(QPainterPath *path)
{
    int count = nodeDatas()->count();
    if(count <= 1)
        return;

    int mo = count - 1 % 3;

    QList<QPointF> list = MyTools::convertBeziers(*nodeDatas(),false);

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

void CMDDrawBezier::drawLine(QPainter *p)
{
    factPen()->draw(p, rect(), basePath());
}

QList<QStringList> CMDDrawBezier::expNames()
{
    QList<QStringList> list;
    list.append(CMDDrawNode::expNames());
    list << QStringList({"$drawNode.percent"});

    return list;
}
