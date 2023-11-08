/*!
\author aGRw 2014.12.23
\section 自定义控制点类
*/
#ifndef CUSTOMPOINT_H
#define CUSTOMPOINT_H

#include <QPainter>
#include "DrawObjDefine.h"

class CSelectObjectManager;

class CCustomPoint
{
public:
    CCustomPoint(CSelectObjectManager* objects);
    ~CCustomPoint();
/*!
\section field
*/
private:
    //!数据列表
    QList<QPointF> m_datas;
    //!路径列表
    QList<QPainterPath*> m_paths;
    //!中心点
    QPointF m_center;

    CSelectObjectManager* m_objects;
/*!
\section public function
*/
public:
    void draw(QPainter* p);
    bool canOperate(const QPointF& point, ControlState& state, int& index);
/*!
\section calculate
*/
public:
    void calculate(qreal scale, QRectF& invalidateRect);
    void generatePath();
    void generateRect(QRectF& invalidateRect);
private:
    void generateData(qreal scale);
/*!
\section private function
*/
private:
    void clearPath();

};

#endif // CUSTOMPOINT_H

