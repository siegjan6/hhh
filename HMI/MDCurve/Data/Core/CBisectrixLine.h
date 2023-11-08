/*!

\author eHVr

\section 功能

    基础线，属性
    \a lineColor
    \a lineWidth
    \a lineStyle

    使用：曲线绘制区域的分割线。警戒线和特征标签线的基类
*/

#ifndef CBISECTRIXLINE_H
#define CBISECTRIXLINE_H

#include <QPen>

class CBisectrixLine
{
public:
    CBisectrixLine();
    virtual ~CBisectrixLine();
/*!
\section   基础属性
*/
public:
    void setLineColor(const QColor &color);
    QColor lineColor() const;

    void  setLineStyle (Qt::PenStyle style);
    Qt::PenStyle lineStyle() const;

    void setLineWidth(int wid);
    int lineWidth () const;
private:
    QColor       m_lineColor;  //! 线色
    qint32       m_lineWidth;  //! 线宽
    Qt::PenStyle m_lineStyle;  //! 线型

/*!
\section   序列化和反序列化
*/
public:
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

};

#endif // CBISECTRIXLINE_H
