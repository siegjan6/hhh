#include <QPen>
#include "MDDrawButton.h"

//////////////////////////////////////////////////////////////////////////
CMDDrawButton::CMDDrawButton()
{
    setText(QObject::tr("按钮"));
    setHAlignment(Qt::AlignHCenter);
    setVAlignment(Qt::AlignVCenter);
    setButtonFrame(true);
}
CMDDrawButton::~CMDDrawButton()
{
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawButton::type()
{
    return DrawType::Button;
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawButton::createSelf()
{
    return new CMDDrawButton();
}
void CMDDrawButton::onPaint(QPainter *p)
{
    CMDDrawText::onPaint(p);

    //buttonStyle
    QRectF rf = rect();
    QPointF a = rf.topLeft();
    QPointF b = rf.topRight();
    QPointF c = rf.bottomLeft();
    QPointF d = rf.bottomRight();

    QPen penWhite = QPen(QColor(248, 248, 245));
    QPen penBlack = QPen(QColor(60, 60, 60));
    QPen penGray = QPen(QColor(120, 120, 120));

    if (isDownFrame())
    {
        p->setPen(penWhite);
        p->drawLine(b, d);
        p->drawLine(c, d);
        p->setPen(penBlack);
        p->drawLine(a, b);
        p->drawLine(a, c);
        p->setPen(penGray);
        p->drawLine(QPointF(a.x(),a.y()+1), QPoint(b.x(),b.y()+1));
        p->drawLine(QPointF(a.x()+1,a.y()), QPoint(c.x()+1,c.y()));
    }
    else
    {
        p->setPen(penWhite);
        p->drawLine(a, b);
        p->drawLine(a, c);
        p->setPen(penBlack);
        p->drawLine(b, d);
        p->drawLine(c, d);
        p->setPen(penGray);
        p->drawLine(QPointF(b.x()-1,b.y()), QPoint(d.x()-1,d.y()));
        p->drawLine(QPointF(c.x(),c.y()-1), QPoint(d.x(),d.y()-1));
    }
}
void CMDDrawButton::onMouseDown(Qt::MouseButton button, qreal x, qreal y)
{
    setIsDownFrame(true);
    invalidate();

    CMDDrawText::onMouseDown(button, x, y);
}
void CMDDrawButton::onMouseUp(Qt::MouseButton button, qreal x, qreal y)
{
    setIsDownFrame(false);
    invalidate();

    CMDDrawText::onMouseUp(button, x, y);
}
void CMDDrawButton::onMouseLeave()
{
    setIsDownFrame(false);
    invalidate();

    CMDDrawText::onMouseLeave();
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj* CMDDrawButton::clone()
{
    CMDDrawButton* obj = static_cast<CMDDrawButton*>(CMDDrawText::clone());
    return obj;
}
//////////////////////////////////////////////////////////////////////////



