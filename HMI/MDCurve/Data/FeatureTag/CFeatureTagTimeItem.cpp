#include "CFeatureTagTimeItem.h"
#include "CTagPosition.h"
#include <QJsonObject>
#include <QPainter>
#include <QDebug>

CFeatureTagTimeItem::CFeatureTagTimeItem():
    CFeatureTagItem(QObject::tr("RecordTime"),QObject::tr("记录时间"),"hh-mm-ss")
{
    m_dateTime = QDateTime::currentDateTime();
}

CFeatureTagTimeItem::~CFeatureTagTimeItem()
{

}

QDateTime CFeatureTagTimeItem::dateTime() const
{
    return m_dateTime;
}

void CFeatureTagTimeItem::setDateTime(const QDateTime &dateTime)
{
    m_dateTime = dateTime;
}

CFeatureTagItem::ItemType CFeatureTagTimeItem::type() const
{
    return CFeatureTagItem::ItemType::Time;
}

void CFeatureTagTimeItem::calculate()
{
    if(visible())
    {
        m_drawText = comment()+" "+ m_dateTime.toString(format());
        QFontMetricsF metrics(font());
        double textHeight = metrics.ascent() + metrics.descent();
        double textWidth = metrics.width(m_drawText);

        QRectF dArea(drawArea());
        dArea.setSize(QSizeF(textWidth,textHeight));

        switch (hPos().position())
        {
        case CTagHPos::Position::Left:
            dArea.moveTopLeft(point());
            break;
        case CTagHPos::Position::Right:
            dArea.moveTopRight(point());
            break;
        case CTagHPos::Position::Middle:
        {
            QPointF pointF = point();
            pointF.setX(point().x() - textWidth/2);
            dArea.moveTopLeft(pointF);
            break;
        }
        default:
            break;
        }
        setDrawArea(dArea);
    }
}

void CFeatureTagTimeItem::drawItem(QPainter *p)
{
    if(visible())
    {
        p->save();
        p->setFont(font());
        p->drawText(drawArea(),comment()+" "+ m_dateTime.toString(format())); //!多语言
        p->restore();
    }
}

void CFeatureTagTimeItem::serialize(QJsonObject &json)
{
    CFeatureTagItem::serialize(json);
    json.insert("Type",(int)type());// would be deserialize in the Tag class

}
