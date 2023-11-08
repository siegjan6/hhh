#include "ColorExample.h"
#include <qpainter.h>

ColorExample::ColorExample(QWidget *parent) :
    QWidget(parent)
{
    m_brushManager = new   CMDBrushManager();
    m_penMgr = new CMDPenManager();
    m_penMgr->setWidth(3);
    m_isPen = false;
}

ColorExample::~ColorExample()
{
    delete m_brushManager;
    delete m_penMgr;
}

void ColorExample::paintEvent(QPaintEvent *)
{
    if(int(m_brushManager->brushType()) == 5)
        return;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    QBrush back;
    back.setTexture(QPixmap(QLatin1String(":/color_widgets/alphaback.png")));
    p.setBrush(back);
    p.drawRect(rect().x()+2,rect().y()+2,rect().width()-4,rect().height()-4);

    if(!m_isPen)
    {
        QBrush br = m_brushManager->brush(this->rect());
        p.setBrush(br);
        p.drawEllipse(rect().x()+2,rect().y()+2,rect().width()-4,rect().height()-4);
    }
    else
    {
        m_penMgr->setBrushManager(*m_brushManager);
        QPen pen = m_penMgr->pen(rect());
        p.setPen(pen);
        p.setBrush(Qt::NoBrush);

        qreal left = rect().left()+rect().width()/3;
        qreal right = rect().right()- rect().width()/3;
        qreal top = rect().top()+rect().height()/3;
        qreal bottom = rect().bottom()- rect().height()/3;
        QPainterPath path;
        path.moveTo(right,top);
        path.lineTo(right,bottom);
        path.lineTo(left,bottom);
        p.drawPath(path);
    }
}

void ColorExample::setColor(const QColor &clr)
{
    m_brushManager->setColor(clr);
}

void ColorExample::setAngle(int angle)
{
    m_brushManager->setAngle(angle);
}

void ColorExample::setStops(QGradientStops stops)
{
    m_brushManager->setStops(stops);
}

void ColorExample::setType(CMDBrushManager::BrushType type)
{
    m_brushManager->setBrushType(type);
}

void ColorExample::setCapStyle(Qt::PenCapStyle capStyle)
{
    m_penMgr->setCapStyle(capStyle);
}

void ColorExample::setJoinStyle(Qt::PenJoinStyle joinStyle)
{
    m_penMgr->setJoinStyle(joinStyle);
}

void ColorExample::setPenStyle(Qt::PenStyle penStyle)
{
    m_penMgr->setStyle(penStyle);
}

void ColorExample::setWidth(int width)
{
    m_penMgr->setWidth(width);
}
