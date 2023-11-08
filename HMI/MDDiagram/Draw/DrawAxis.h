/*!
\section 绘制统计图的坐标轴类
*/
#ifndef CDRAWAXIS_H
#define CDRAWAXIS_H
#include "DrawData.h"
#include <QRectF>
class CDrawAxis
{
public:
    CDrawAxis();
    ~CDrawAxis();

private:
    CDrawData* m_axisData;
    QList<QString>m_stringList;
    QList<QRectF>m_rectList; //! 连续的绘图矩形区域
public:
    //! 三维直方图两个绘图区域
    QRectF m_drawRect;
    QRectF m_interRect;
    QList<QRectF>m_drawRectList; //! 连续的绘图矩形区域
    QList<QRectF>m_interRectList; //! 连续的绘图矩形区域

public:
    void setAxisData(CDrawData* data);
    void drawAxis(QPainter* painter,QRectF rectf);
    void setXString(QList<QString>stringList); //! 获取X轴显示文本
    QList<QRectF>getRectList();

private:
    void drawYAxis(QPainter* painter, QRectF rectf); //! 绘制横向主次刻度线
    void drawScale(QPainter* painter, QRectF rect);   //! 绘制刻度值
    qreal scaleCoord(qreal data, QRectF rect); //! 每个像素对应的单位值
    QRectF getDiagramRect(QRectF rect); //! 绘制刻度线的矩形区域
    void drawXAxis(QPainter* painter,QRectF rectf); //! 绘制纵向主次刻度线

};

#endif // CDRAWAXIS_H
