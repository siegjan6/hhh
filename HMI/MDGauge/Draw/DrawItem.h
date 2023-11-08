/*!
\author emhhbmdsZQ==
\section  绘制仪表的各部分组件
*/

#ifndef DRAWITEM_H
#define DRAWITEM_H
#include "DrawData.h"

class CDrawItem
{
public:
    CDrawItem();
    ~CDrawItem();
public: 
    /*!
         绘制组件。
         \param moveRgn为NULL时，正常绘图；不为NULL，不绘图，输出组件包含的region。
         \param m  用于传递旋转缩放数据。
         \returns 空。
     */
    void draw(QPainter* painter, const QRectF& rect, CData* data, QRegion* moveRgn = NULL, QTransform* m = NULL);
    //!  获取Pen，可以为NULL，获取的Pen需要delete
    QPen* createPen(CEdgeLine& edgeLine);
    //!  获取Brush，可以为NULL，获取的Brush需要delete
    QBrush* createBrush(const CBackColor& backColor,const QRectF& rect,const QPainterPath* path = NULL,bool bRotate = false);
    //!  根据style
    /*!
         绘制形状路径。
         \param style 用于设置RMarker和RScale对应的形状。
         \returns 空。
     */
    void setPath(QPainterPath& path, const QRectF& rf, qint32 style);
    void setLayer(CDataLayer* layer);  //!  获取层数据
    void setRectAct(const QRectF& rAct);  //!  设置非正方形坐标
private:
    /*!
    \section  绘制圆表的各部分组件
    */
    void drawCap();   //!  绘制中心点
    void drawNeedle(QRegion* moveRgn = NULL, QTransform* m = NULL);  //!  绘制指针
    void drawRange();   //!  绘制进度条
    void drawScale();   //!  绘制刻度
    void drawScaleBack();    //!  绘制刻度背景
    void drawScaleText();    //!  绘制刻度文本
    void drawBack();     //!  绘制背景
    void drawHalfBack();    //!  绘制半圆背景
    void drawLabel();     //!  绘制标签
    void drawLed();     //!  绘制Led数字
    void drawTemperature(QRegion* moveRgn = NULL, QTransform* m = NULL);    //!  绘制温度计
private:
    /*!
    \section  绘制直表的各部分组件
    */
    void drawRNeedle(QRegion* moveRgn = NULL, QTransform* m = NULL);    //!  绘制直表指针
    void drawRMarker(QRegion* moveRgn = NULL, QTransform* m = NULL);   //!  绘制直表显示块
    void drawRRange();    //!  绘制直表进度条
    void drawRScaleText();    //!  绘制直表刻度文本
    void drawRScale();    //!  绘制直表刻度
    void drawRScaleBack();    //!  绘制直表刻度背景
private:
     QPainter *m_g;
     QRectF m_r;    //!  正方形绘图区域
     QRectF m_rAct;    //!  实际绘图区域，主要用于直表组件
     double m_shortLen;    //!  m_rAct的width和height中较短的长度
     CData* m_d;
     CDataLayer* m_layer;
     qint32 m_nIndex;
};

#endif // DRAWITEM_H
