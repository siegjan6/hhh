/*!
\author aGRw 2015.05.06
\section 节点控制点类
*/
#ifndef NODEPOINT_H
#define NODEPOINT_H

#include <QList>
#include "DrawObjDefine.h"
#include "ControlPointContainer.h"

class CNodePoint
{
public:
    CNodePoint(CSelectObjectManager* objects);
/*!
\section field
*/
private:
    //!数据数组
    QList<QPointF> m_datas;
    //!路径数组
    QList<QPainterPath> m_paths;
    CSelectObjectManager* m_objects;
/*!
\section private function
*/
private:
    bool isVisible(const QPointF& point, ControlState& state, int& index, bool isDeleteNode = false);
/*!
\section public function
*/
public:
    bool isVisible(const QPointF& point);
    bool canOperate(const QPointF& pf, ControlState& state, int& index);
    void draw(QPainter* p);
/*!
\section calculate
*/
public:
    void calculate(qreal scale, QRectF& invalidateRect);
private:
    //!设置数据
    void generateData(qreal scale);
    //!设置路径
    void generatePath();
    //!设置区域
    void generateRect(QRectF& invalidateRect);

};

#endif // NODEPOINT_H
