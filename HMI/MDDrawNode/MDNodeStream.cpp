#include "MDNodeStream.h"
#include "MyTools.h"

CMDNodeStream::CMDNodeStream(CMDDrawNode *node)
{
    m_direction = Direction::forward;
    m_enable = false;
    m_isFlow = true;
    m_interval = 500;
    m_dashSingleOffset = 1;
    m_dashOffset = 0;
    m_percent = 1;
    m_maxOffset = 0;
    m_dashPattern = {1,2};
    m_color = Qt::black;
    m_penManager.init(Qt::green,5);
    m_pen = m_penManager.getPen();
    m_data.clear();
    m_timer = new QTimer();
    m_object = node;

    connect(m_timer, &QTimer::timeout, this, &CMDNodeStream::onTimerOut);
}

CMDNodeStream::~CMDNodeStream()
{
    stop();
    delete m_timer;
}

CMDNodeStream *CMDNodeStream::clone()
{
    CMDNodeStream * s = new CMDNodeStream (m_object);
    s->m_enable = m_enable;
    s->m_color = m_color;
    s->m_interval = m_interval;
    s->m_percent = m_percent;
    s->m_isFlow = m_isFlow;
    s->m_penManager = m_penManager;
    s->setParent(parent());
    return s;
}

void CMDNodeStream::setObject(CMDDrawNode *value)
{
    m_object = value;
}

void CMDNodeStream::updatePercent(qreal percent)
{
    qreal sum = linesLength(); //总长度
    qreal currentDis = 0;
    qreal endDis = 0;
    int endIndex =-1;
    percent = percent<0?0:percent;
    percent = percent>1?1:percent;
    qreal m_percentLength =  sum * percent;

    if(direction() == Direction::forward)
    {
        m_data.clear();
        for(int i =0;i< m_object->nodeDatas()->count()-1;i++)
        {
            qreal dis = MyTools::distance(m_object->nodeDatas()->at(i),m_object->nodeDatas()->at(i+1));
            currentDis += dis;
            if(currentDis >= m_percentLength)
            {
                endIndex = i;
                endDis = currentDis - m_percentLength;
                endDis = dis - endDis;
                break;
            }
        }
        if(endIndex >= 0)
        {
            QPointF pointC = MyTools::getCbyPoint2Dis1(m_object->nodeDatas()->at(endIndex),m_object->nodeDatas()->at(endIndex+1),endDis);
            for(int i =1;i<= endIndex;i++)
            {
                m_data.append(QLineF(m_object->nodeDatas()->at(i-1),m_object->nodeDatas()->at(i)));
            }
            m_data.append(QLineF(m_object->nodeDatas()->at(endIndex),pointC));
        }
    }
    else
    {
        m_data.clear();
        for(int i = m_object->nodeDatas()->count()-1;i>0;i--)
        {
            qreal dis = MyTools::distance(m_object->nodeDatas()->at(i),m_object->nodeDatas()->at(i-1));
            currentDis += dis;
            if(currentDis >= m_percentLength)
            {
                endIndex = i;
                endDis = currentDis - m_percentLength;
                endDis = dis - endDis;
                break;
            }
        }
        if(endIndex > 0)
        {
            QPointF pointC = MyTools::getCbyPoint2Dis1(m_object->nodeDatas()->at(endIndex),m_object->nodeDatas()->at(endIndex-1),endDis);
            for(int i = m_object->nodeDatas()->count()-1; i > endIndex; i--)
            {
                m_data.append(QLineF(m_object->nodeDatas()->at(i),m_object->nodeDatas()->at(i-1)));
            }
            m_data.append(QLineF(m_object->nodeDatas()->at(endIndex),pointC));
        }
    }
}

void CMDNodeStream::onNodeChanged()
{
    updatePercent(percent());
}

void CMDNodeStream::start()
{
    if(!m_timer->isActive())
    {
        m_maxOffset = maxOffset();//*  linesLength();
        m_timer->start();
        m_timer->setInterval(interval());
    }
}

void CMDNodeStream::stop()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
    }
}

void CMDNodeStream::drawStreamLine(QPainter *painter)
{
    if(m_object == NULL)
        return;

    painter->setPen(m_pen);
    if(m_data.count() != 0)
    {
        if(m_object->type() == DrawType::StraightLine || m_object->type() == DrawType::FoldLine || m_object->type() == DrawType::Pipe)
        {
            painter->drawLines(m_data);
        }
        if(m_object->type() == DrawType::Bezier || m_object->type() == DrawType::ClosedBezier || m_object->type() == DrawType::Polygon)
        {
            painter->drawPath(*(m_object->basePath()));
        }
    }
}

void CMDNodeStream::serialize(QJsonObject &json)
{
    QJsonObject NodeStreamJson;

    NodeStreamJson.insert("Enable",m_enable);
    NodeStreamJson.insert("IsFlow",m_isFlow);
    NodeStreamJson.insert("Direction",(int)m_direction);
    NodeStreamJson.insert("DashSingleOffset",m_dashSingleOffset);
    NodeStreamJson.insert("Color", (qreal)m_color.rgba());
    NodeStreamJson.insert("Interval", m_interval);
    NodeStreamJson.insert("Percent", m_percent);
    json.insert("NodeStream",NodeStreamJson);
    m_penManager.serialize(json);
}

void CMDNodeStream::deserialize(const QJsonObject &json)
{
    QJsonObject NodeStreamJson = json.value("NodeStream").toObject();

    m_enable = NodeStreamJson.value("Enable").toBool();
    m_isFlow = NodeStreamJson.value("IsFlow").toBool();
    m_direction = (Direction)NodeStreamJson.value("Direction").toInt();
    m_dashSingleOffset = NodeStreamJson.value("DashSingleOffset").toDouble();
    m_color = QColor::fromRgba(NodeStreamJson.value("Color").toDouble());
    m_interval = NodeStreamJson.value("Interval").toInt();
    m_percent = NodeStreamJson.value("Percent").toInt();
    m_penManager.deserialize(json);
}

void CMDNodeStream::setDashSingleOffset(qreal offset)
{
    qreal max = m_maxOffset;
    while(max >= offset)
        max-= offset;
    qreal mo =  max ;
    if(mo == 0 && offset < max)
    {
        m_dashSingleOffset = offset;
    }
}

int CMDNodeStream::dashSingleOffset()
{
    return m_dashSingleOffset;
}

QVector<qreal> CMDNodeStream::dashPattern() const
{
    return m_dashPattern;
}

void CMDNodeStream::setDashPattern(QVector<qreal> pattern)
{
    m_dashPattern.clear();
    for(int i = 0; i < pattern.count(); i++)
        m_dashPattern.append(pattern.at(i));
}

QColor CMDNodeStream::color() const
{
    return m_color;
}

void CMDNodeStream::setColor(const QColor &color)
{
    m_color = color;
}

qreal CMDNodeStream::percent()
{
    return m_percent;
}

void CMDNodeStream::setPercent(qreal p)
{
    if(p >= 0 && p <= 1)
    {
        m_percent = p;
    }
    updatePercent(m_percent);
}

void CMDNodeStream::onTimerOut()
{
    //显示流动
    if(!streamLineVisible())
        return;

    if(streamLineFlow() && !m_object->isStudioMode())
    {
        m_dashOffset += m_dashSingleOffset;
        if (m_direction == Direction::forward)
            m_pen.setDashOffset(-m_dashOffset);
        else
            m_pen.setDashOffset(m_dashOffset);
        while(m_dashOffset >= m_maxOffset*2)
            m_dashOffset-= m_maxOffset;

        if(m_object != NULL)
            m_object->invalidate();
    }
}

QPen CMDNodeStream::updatePen()
{
    m_penManager.generate(m_object->basePath()->boundingRect());
    m_pen = m_penManager.getPen();
    m_pen.setStyle(Qt::PenStyle::CustomDashLine);
    m_pen.setDashPattern(m_dashPattern);

    return m_pen;
}

qreal CMDNodeStream::maxOffset()
{
    qreal maxOffset = 0;
    for(int i = 0; i < m_dashPattern.count(); i++)
    {
        maxOffset += m_dashPattern.at(i);
    }
    return maxOffset;
}

qreal CMDNodeStream::linesLength()
{
    qreal linesDis= 0;
    for(int i = 1;i< m_object->nodeDatas()->count();i++)
    {
        qreal dis = MyTools::distance(m_object->nodeDatas()->at(i-1),m_object->nodeDatas()->at(i));
        linesDis+=dis;
    }
    return linesDis;
}
