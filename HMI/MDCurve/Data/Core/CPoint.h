/*!

\author eHVr

\section 功能

    基本点，属性：
    \a visible
    \a color
    \a size
    \a pointStyle
    应用于，显示曲线上的数据点，特征标签的两端端点等
*/
#ifndef CPOINT_H
#define CPOINT_H

#include <QColor>
#include <QPainterPath>

class CPoint
{
public:
    CPoint();
    ~CPoint();

    //! 点类型
    enum class PointStyle{
        Circle = 0,
        Square,
        Triangle,
        Cross
    };
/*!
\section 基本属性
*/
public:
    QColor color() const;
    void setColor(const QColor &color);

    int size() const;
    void setSize(int size);

    PointStyle pointStyle() const;
    void setPointStyle(const PointStyle &pointStyle);

    bool visible() const;
    void setVisible(bool visible);
private:
    bool            m_visible;
    QColor          m_color ;
    qint32          m_size ;
    PointStyle      m_pointStyle;
/*!
\section 计算，绘制
*/
public:
    void clearPath(); //! 清除之前的绘制路径
public:
    void draw(QPainter *p);
    void figurePoint(const QPointF &p);
private:
    void figureCircle(const QPointF &p);  //! 圆
    void figureCross(const QPointF &p);   //! 十字
    void figureSquare(const QPointF &p);  //! 正方形
    void figureTriangle(const QPointF &p);//! 三角形
private:
    QPainterPath *m_drawPath;             //! 点的绘制路径
/*!
\section 序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
public:
    CPoint &operator = (const CPoint& other);

};

#endif // CPOINT_H
