/*!

\author eHVr

\section 功能

    单一标签
    曲线绘制区域左上角标签显示曲线的信息
    当鼠标移动时,显示鼠标指向标签的信息（游标）
*/
#ifndef CLABELITEM_H
#define CLABELITEM_H

#include <QRectF>
#include <QString>
#include <QList>
#include <QColor>
#include <QFont>

class QPainter;
class CLabelItem
{
public:
    CLabelItem();
    ~CLabelItem();
/*!
\section 基本属性
*/
public:
    //! 值的字符串值
    QString valueString() const;
    void setValueString(const QString &valueString);

    //! 变量名称
    QString text() const;
    void setText(const QString &text);

    //! 色块颜色(曲线色)
    QColor color() const;
    void setColor(const QColor &color);

    //! 时间值字符串
    QString timeString() const;
    void setTimeString(const QString &timeString);

    void setFont(const QFont &font); //! 设置色块框边长为字体高
    QFont font() const;
private:
    QColor          m_color;
    QString         m_valueString;
    QString         m_timeString;
    QString         m_text;
    QFont           m_font;

/*!
\section 计算，绘制
*/
public:
    //! 绘制区域
    QRectF drawArea() const;
    void setDrawArea(const QRectF &drawArea);

    //! 绘制起点
    void setTopLeft(const QPointF &topLeft);

    void calculate(double valueWidth, double textWidth, double textHeight, double timeWidth = 0);
    void draw(QPainter *p);
private:
    QRectF          m_drawArea;      //! 标签区域
    QRectF          m_colorRect;     //! 曲线色彩框
    QRectF          m_valueRect;     //! 值的绘制区域
    QRectF          m_timeRect;      //! 时间区域
    QRectF          m_textRect;      //! 名称绘制区域
};

#endif // CLABELITEM_H
