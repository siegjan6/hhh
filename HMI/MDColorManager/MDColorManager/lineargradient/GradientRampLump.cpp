#include "GradientRampLump.h"
#include <QLinearGradient>
GradientRampLump::GradientRampLump(QWidget *parent) :
    QWidget(parent)
{
    _linearBr = NULL;
}

void GradientRampLump::setGradientStops(QGradientStops gradientStops)
{
    m_datas.clear();
    for(int i = 0 ;i < gradientStops.count();i++)
    {
        m_datas.append(GradientRamp(gradientStops[i]));
    }
}

QGradientStops GradientRampLump::gradientStops()
{
    QGradientStops stops;
    for(int i = 0 ;i < m_datas.count();i++)
    {
        QGradientStop s;
        s.first = m_datas[i].scale();
        s.second = m_datas[i].color();
        stops.append(s);
    }
    return stops;
}

void GradientRampLump::append(const QColor &clr, const qreal &scale)
{
    GradientRamp g;
    g.setColor(clr);
    g.setScale(scale);
    m_datas.append(g);
}

void GradientRampLump::append(GradientRamp &g)
{
    if(g.selected())
        setfullSelecte(false);
    m_datas.append(g);
}

void GradientRampLump::remove(GradientRamp &g)
{
    for(int i =0 ;i < m_datas.count();i++)
    {
        if(m_datas[i].color() == g.color() && m_datas[i].scale() == g.scale())
        {
            m_datas.removeAt(i);
            return;
        }
    }
}

void GradientRampLump::remove(const QColor &clr, const qreal &scale)
{
    for(int i =0 ;i < m_datas.count();i++)
    {
        if(clr == m_datas[i].color() && scale == m_datas[i].scale())
        {
            m_datas.removeAt(i);
            return;
        }
    }
}



void GradientRampLump::setfullSelecte(bool bSelect)
{
    for(int i =0 ;i < m_datas.count();i++)
        m_datas[i].setSelected(bSelect);
}

void GradientRampLump::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);  //消除锯齿
    QLinearGradient* br = lineargradient();
    p.setBrush(*br);
    p.drawRect(clientRect());
    for(int i =0 ;i < m_datas.count();i++)
        m_datas[i].Draw(&p,clientRect());
}

void GradientRampLump::mousePressEvent(QMouseEvent * event)
{
    QPointF pos(event->pos().x() - LINEAR_X,event->pos().y());
    int index = contains(pos);
    if(index == -2)
        return;
    if(index != -1)
    {
        setSelectIndex(index);
    }
    else
    {
        GradientRamp ramp;
        QColor c(rand()%256,rand()%256,rand()%256);
        ramp.setColor(c);
        qreal scale = (pos.x())/clientRect().width();
        ramp.setScale(scale);
        setfullSelecte(false);
        ramp.setSelected(true);
        append(ramp);
    }

    emit colorChange(current()->color());
    emit gradientStopsChange(gradientStops());
    update();
}

void GradientRampLump::mouseDoubleClickEvent(QMouseEvent * event)
{
    QPointF pos(event->pos().x() - LINEAR_X,event->pos().y());
    int index = contains(pos);
    if(index != -2 &&index != -1 && m_datas.count() > 2)
    {
        m_datas.removeAt(index);
        setfullSelecte(false);
        setSelectIndex(0);
        update();
    }
}

void GradientRampLump::mouseReleaseEvent(QMouseEvent * event)
{

}

void GradientRampLump::mouseMoveEvent(QMouseEvent *event)
{
    QPointF pos(event->pos().x() - LINEAR_X,event->pos().y());
    if(event->buttons() == Qt::MouseButton::LeftButton)
    {
        GradientRamp* g = current();
        if( g != NULL)
        {
            qreal f = pos.x()/clientRect().width();
            f >= 1 ? f = 1:f;
            f <= 0 ? f = 0:f;
            g->setScale(f);

            emit gradientStopsChange(gradientStops());
            update();
        }
    }

}

int GradientRampLump::selectIndex()const
{
    int temp = 0;
    int result = -1;
    for(int i =0 ;i < m_datas.count();i++)
    {
        if(m_datas[i].selected())
        {
            temp++;
            result = i;
        }
    }
    return temp == 1 ? result: -1;
}

void GradientRampLump::setSelectIndex(int index)
{
    setfullSelecte(false);
    m_datas[index].setSelected(true);
}

GradientRamp *GradientRampLump::current()
{
    int index = selectIndex();
    if(index != -1)
        return &m_datas[index];
    if(m_datas.count()>0)
    {
        setSelectIndex(0);
        return &m_datas[0];
    }
    return NULL;
}

bool GradientRampLump::containsChild(const GradientRamp& g,const QPointF &p)
{
    qreal minW = clientRect().width()* g.scale() - g.width()/2;
    qreal maxW = minW + g.width();
    qreal minH = clientRect().bottom();
    qreal maxH = minH + g.height();
    if(p.x() >= minW && p.x() <= maxW && p.y() >= minH && p.y() <= maxH)
        return true;
    return false;
}

int GradientRampLump::contains(const QPointF &p)
{
    QRectF rf = clientRect();
    qreal w = rf.width();
    qreal h = rf.height();

    if(p.x() >= 0  && p.x() <= w )
    {
        for(int i =0 ;i < m_datas.count();i++)
        {
                if(containsChild(m_datas.at(i),p))
                    return i;
        }
        return -1;
    }
    return -2;
}


QLinearGradient* GradientRampLump::lineargradient()
{
    if(_linearBr == NULL)
        _linearBr = new QLinearGradient(clientRect().x(),clientRect().y(),clientRect().right(),clientRect().y());

    QVector<QGradientStop> stops;
    for(int i =0 ;i < m_datas.count();i++)
        stops.append(m_datas[i].m_gradientStop);
    _linearBr->setStops(stops);
    return _linearBr;
}

QRectF GradientRampLump::clientRect()
{
    qreal w = rect().width();
    qreal h   = rect().height();
    QRectF rt(LINEAR_X,LINEAR_Y,w-LINEAR_WIDTH*2,h-LINEAR_HEIGHT);
    return rt;
}

void GradientRampLump::setCurrentColor(const QColor &clr)
{
    if(current() != NULL)
    {
        current()->setColor(clr);
        emit gradientStopsChange(gradientStops());
        update();
    }
}
