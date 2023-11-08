#include "CLabelItem.h"
#include <QPainter>
#include <QDebug>

CLabelItem::CLabelItem()
{

}

CLabelItem::~CLabelItem()
{

}
QString CLabelItem::timeString() const
{
    return m_timeString;
}

void CLabelItem::setTimeString(const QString &timeString)
{
    m_timeString = timeString;
}

void CLabelItem::setFont(const QFont &font)
{
    m_font = font;
}

QFont CLabelItem::font() const
{
    return m_font;
}

QColor CLabelItem::color() const
{
    return m_color;
}

void CLabelItem::setColor(const QColor &color)
{
    m_color = color;
}

QString CLabelItem::valueString() const
{
    return m_valueString;
}

void CLabelItem::setValueString(const QString &valueString)
{
    m_valueString = valueString;
}
QString CLabelItem::text() const
{
    return m_text;
}

void CLabelItem::setText(const QString &text)
{
    m_text = text;
}

/*!
\section
*/
QRectF CLabelItem::drawArea() const
{
    return m_drawArea;
}

void CLabelItem::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
}

void CLabelItem::setTopLeft(const QPointF &topLeft)
{
    m_drawArea.setTopLeft(topLeft);
}
void CLabelItem::calculate(double valueWidth,double textWidth,double textHeight,double timeWidth)
{
    enum {Internal = 2};

    double colorWidth = textHeight;
    m_drawArea.setWidth(valueWidth+ colorWidth+textWidth + timeWidth+3*Internal);
    m_drawArea.setHeight(textHeight);

    QRectF area = m_drawArea;
    QPointF topLeft = area.topLeft();

    // 时间
    m_timeRect.setTopLeft(topLeft);
    m_timeRect.setSize(QSizeF(timeWidth,textHeight));
    // 色块
    topLeft.setX(topLeft.x() + timeWidth + Internal);
    m_colorRect.setTopLeft(topLeft);
    QFontMetrics metrics(m_font);
    double colorHeight = metrics.height();
    m_colorRect.setSize(QSizeF(colorHeight,colorHeight));

    // 变量描述文本
    topLeft.setX(topLeft.x()+colorHeight+Internal);
    m_textRect.setTopLeft(topLeft);
    m_textRect.setSize(QSizeF(textWidth,textHeight));

    // 值文本
    topLeft.setX(topLeft.x()+textWidth+Internal);
    m_valueRect.setTopLeft(topLeft);
    m_valueRect.setSize(QSizeF(valueWidth,textHeight));
}

void CLabelItem::draw(QPainter *p)
{
    p->save();
    QRectF colorFrameRect = QRectF(m_colorRect.x()-1,m_colorRect.y()-1,m_colorRect.width()+1,m_colorRect.height()+1);
    p->drawRect(colorFrameRect);//!色块绘制边框

    p->setPen(m_color);
    if(!m_timeString.isNull())
        p->drawText(m_timeRect,m_timeString);
    p->fillRect(m_colorRect,m_color);
    p->drawText(m_textRect,m_text);
    p->drawText(m_valueRect,m_valueString);

    p->restore();
}


