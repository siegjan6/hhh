#include <math.h>
#include "MyTools.h"
MyTools::MyTools()
{
}

QPointF MyTools::getCbyPoint2Dis1(QPointF a, QPointF b, qreal d)
{
    //如Xa≠Xb且Ya≠Yb,点(Xa,Ya)与(Xb,Yb)之间的距离为D Xc=Xa+d*(Xb-Xa)/D Yc=Ya+d*(Yb-Ya)/d
    QPointF c ;
  //  if(a.x() != b.x() && a.y() != b.y())
    qreal x = b.x() - a.x();
    qreal y = b.y() - a.y();
    qreal l = sqrt( x*x + y*y);
    qreal sin = y / l;
    qreal cos = x / l;
    qreal xOff = d*cos;
    qreal yOff = d*sin;

    c = a + QPointF(xOff, yOff);
    return c;
}

qreal MyTools::distance(const QPointF& a, const QPointF& b)
{
    return qreal((qreal)sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y())));
}

QList<QPointF> MyTools::convertBeziers(const QList<QPointF>& nodeList, bool close)
{
    QList<QPointF> list;
    QPointF ptfs[4];
    ptfs[0] = nodeList[nodeList.count() - 1];
    ptfs[3] = nodeList[0];
    for (int i = 0; i < nodeList.count(); i++)
    {
        QPointF data = nodeList[i]; //数据点
        QPointF prev = nodeList[i]; //上一次数据点
        QPointF next = nodeList[i];//下一次数据点
        if (i != 0)
            prev = nodeList[i - 1];
        else if (close)
            prev = nodeList[nodeList.count() - 1];
        if (i != nodeList.count() - 1)
            next = nodeList[i + 1];
        else if (close)
            next = nodeList[0];
        //数据准备完毕、开始计算控制点
        //控制点要求:  控制线 controlpoint1 与controlpoint2 。   数据线 数据点 prev、next所连成的线。
        //1.控制线与数据线平行。
        //2.控制线的长度是数据线的二分之一。

        //求数据线长度
        float dataDis = distance(prev, next);
        //求数据线的夹角
        float dataAngle = getLineHorizontalAngle(prev, next);
        //获取控制点
        QPointF controlPoint1 = centerRadiusPoint(data, dataAngle, dataDis / 4);
        QPointF controlPoint2 = centerRadiusPoint(data, dataAngle - 180, dataDis / 4);
        //添加控制点
        if (i != 0)
            list.append(controlPoint1);
        else if (close)
        {
            ptfs[2] = controlPoint1;
        }
        list.append(data);
        if (i != nodeList.count() - 1)
            list.append(controlPoint2);
        else if (close)
        {
            ptfs[1] = controlPoint2;
        }
    }
    if (close)
    {
        list.append(ptfs[1]);
        list.append(ptfs[2]);
        list.append(ptfs[3]);
    }
    return list;
}

qreal MyTools::getLineHorizontalAngle(const QPointF& pointA,const  QPointF& pointB)
{
    qreal angle = 0;
    if (pointA.y() == pointB.y())
    {
        if (pointA.x() > pointB.x())
            return 0;
        return 180;
    }
    if (pointA.x() == pointB.x())
    {
        if (pointA.y() > pointB.y())
            return 90;
        return 270;
    }
    QPointF pt(pointA.x(), pointB.y());
    qreal dui = distance(pt, pointA);
    qreal xie = distance(pointA, pointB);
    angle = (qreal)asin(dui / xie);
    angle = angle / (qreal)3.1415 * (qreal)180.0;   //弧度转角度

    if (pointA.x() > pointB.x() && pointA.y() > pointB.y())
    {
        return angle;
    }
    else if (pointA.x() < pointB.x() && pointA.y() > pointB.y())
    {
        angle = 180 - angle;
        return angle;
    }
    else if (pointA.x() < pointB.x() && pointA.y() < pointB.y())
    {
        angle += 180;
        return angle;
    }
    else if (pointA.x() > pointB.x() && pointA.y() < pointB.y())
    {
        angle = 360 - angle;
        return angle;
    }
    return angle;
}

QPointF MyTools::centerRadiusPoint(const QPointF& center, qreal angle, qreal radius)
{
    QPointF p;
    qreal angleHude = angle * 3.1415 / 180.0;/*角度变成弧度*/
    p.setX((qreal)(radius * cos(angleHude)) + center.x());
    p.setY((qreal)(radius * sin(angleHude)) + center.y());
    return p;
}
