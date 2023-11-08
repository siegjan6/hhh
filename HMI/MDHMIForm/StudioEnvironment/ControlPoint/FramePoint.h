/*!
\author aGRw 2014.09.15
\section 控件控制点类
*/
#ifndef FRAMEPOINT_H
#define FRAMEPOINT_H

#include <QPainterPath>

#include "DrawObjDefine.h"
#include "ControlPointContainer.h"

class CSelectObjectManager;

class CFramePoint
{
public:
    CFramePoint(CSelectObjectManager* objects);
/*!
\section const
*/
private:
    //!边框控制点数目
    static int FrameCount;
    //!旋转控制点数目
    static int RotateCount;
    //!倾斜控制点数目
    static int ShearCount;
    static int PointCount;
    static int FrameSize;
    QRectF m_pointRect;
/*!
\section field
*/
private:
    //!边框控制点路径
    QPainterPath m_framePath[8];
    QPainterPath m_rotateCenterPath;
    QPainterPath m_rotatePath[4];
    QPainterPath m_shearPath[2];

    //!边框线点
    QPointF m_framePoint[5];
    CSelectObjectManager* m_objects;
/*!
\section calculate
*/
public:
    void calculate(qreal scale, QRectF& invalidateRect, QRectF& rotateRect);
private:
    //!设置坐标
    void generateData(qreal scale);
    void generatePath();
    void generateRect(QRectF& invalidateRect, QRectF& rotateRect);
private:
    QPointF m_rotatePointPos;
    //!边框控制点坐标数组
    QPointF m_frameArray[8];
/*!
\section public function
*/
public:
    bool canOperate(const QPointF& pf, ControlState& state, int& index);
    void draw(QPainter* p);
};

#endif // FRAMEPOINT_H

