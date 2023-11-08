#include "MDDrawPolygon.h"
#include "MDNodeStream.h"

CMDDrawPolygon::CMDDrawPolygon()
{

}

CMDDrawPolygon::~CMDDrawPolygon()
{

}

DrawType CMDDrawPolygon::type()
{
    return DrawType::Polygon;
}

CMDDrawObj *CMDDrawPolygon::createSelf()
{
    return new CMDDrawPolygon;
}

void CMDDrawPolygon::drawPath(QPainter *p)
{
    factBrush()->draw(p, rect(), basePath());

    CMDDrawNode::drawPath(p);

    if(!createSuccess() || this->pen() == NULL)
        return;

    if(!pipeBackGroundVisible() && !pLineStream()->streamLineVisible())
    {
      drawLine(p);
    }
}

void CMDDrawPolygon::generateNodePath(QPainterPath *path)
{
    if(nodeDatas()->count() > 0)
    {
        QPolygonF polygon;
        for(int i = 0; i < nodeDatas()->count(); i++)
            polygon.append(nodeDatas()->at(i));
        path->addPolygon(polygon);
        path->closeSubpath();
    }

    pLineStream()->onNodeChanged();
}

void CMDDrawPolygon::onDelete(int pos)
{
    if(this->nodeDatas()->count()>3)
        CMDDrawNode::onDelete(pos);
}

bool CMDDrawPolygon::isVisible(const QPointF &point)
{
    return CMDDrawVector::isVisible(point);
}

void CMDDrawPolygon::drawLine(QPainter *p)
{
    factPen()->draw(p, rect(), basePath());
}

QList<QStringList> CMDDrawPolygon::expNames()
{
    QList<QStringList> newlist;
    newlist.append(CMDDrawNode::expNames());
    newlist << QStringList({"$drawNode.percent"});

    return newlist;
}
