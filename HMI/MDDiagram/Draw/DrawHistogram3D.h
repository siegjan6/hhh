/*!
\section 绘制三维统计图
*/
#ifndef CDRAWHISTOGRAM3D_H
#define CDRAWHISTOGRAM3D_H
#include "DrawData.h"
#include <QRectF>
#include <QPainter>

class CDrawHistogram3D
{
public:
    CDrawHistogram3D();
    ~CDrawHistogram3D();

private:
    CDrawData* m_histogramData3D;
    QRectF m_drawRect;
    QRectF m_interRect;
    QList<QString>m_stringList1;
    QList<QString>m_stringList2;
    QList<QString>m_stringList3;
    QList<QColor>m_colorList;
    QList<qreal>m_valueList;
    QList<QRectF>m_drawRectList;
    QList<QRectF>m_interRectList;
public:
    QList<QRectF>m_pillarRectList;

public:
    void drawHistogram3D(QPainter* painter);
    void setHistogramData3D(CDrawData* data);
    void setDrawRect(QRectF rect);
    void setInterRect(QRectF rect);
    void setStringList1(QList<QString>stringList);
    void setStringList2(QList<QString>stringList);
    void setStringList3(QList<QString>stringList);
    void setColorList(QList<QColor>colorList); //! 获取矩形填充颜色
    void setValueList(QList<qreal>valueList); //! 获取统计值
    void setDrawRectList(QList<QRectF>rectList);
    void setInterRectList(QList<QRectF>rectList);

private:
    void drawHistogram3D1(QPainter* painter, QList<qreal> valueList, QRectF drawRect, QRectF interRect);//! 绘制一分组三维直方图
    void drawHistogram3D2(QPainter* painter); //! 绘制二分组三维直方图
    void drawHistogram3D3(QPainter* painter); //! 绘制三分组三维直方图
    void drawHistogram3D3Negative(QPainter* painter); //! 绘制有负值的三分组直方图
};

#endif // CDRAWHISTOGRAM3D_H
