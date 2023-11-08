/*!
\section 绘制二维统计图
*/
#ifndef CDRAWHISTOGRAM_H
#define CDRAWHISTOGRAM_H
#include "DrawData.h"
#include <QRectF>
#include <QPainter>

class CDrawHistogram
{
public:
    CDrawHistogram();
    ~CDrawHistogram();

private:
    CDrawData* m_histogramData;
    QList<QRectF>m_rectList;
    QList<QColor>m_colorList;
    QList<qreal>m_valueList;
    QList<QString>m_stringList2;
    QList<QString>m_stringList3;
public:
    QList<QRectF>m_pillarRectList;

public:
    void setHistogramData(CDrawData* data);
    void setHistogramRectList(QList<QRectF>rectList);  //! 获取绘制直方图矩形区域
    void setHistogramValueList(QList<qreal>valueList); //! 获取统计值
    void setHistogramColorList(QList<QColor>colorList); //! 获取矩形填充颜色
    void setHistogramStringList2(QList<QString>stringList);
    void setHistogramStringList3(QList<QString>stringList);

    /*!
     \section 根据数据分组情况进行绘图
    */
    void drawHistogram(QPainter* painter);
    void drawHistogram1(QPainter* painter); //! 一分组
    void drawHistogram2(QPainter* painter); //! 二分组
    void drawPillar2(QPainter* painter,QList<qreal>valueList,QList<QColor>colorList,QRectF rect);
    void drawHistogram3(QPainter* painter); //! 三分组
    void drawHistogram3Negative(QPainter *painter);

};

#endif // CDRAWHISTOGRAM_H
