#include "CPushButton.h"
#include <QPainter>

CPushButton::CPushButton(QWidget *parent) :
    QPushButton(parent)
{
}

void CPushButton::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter.setPen(pen);
    painter.drawRect(0,0,size().width(),size().height());
    QPushButton::paintEvent(e);
}
