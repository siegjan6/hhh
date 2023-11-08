#include "CFeatureTagValueItem.h"
#include "CTagPosition.h"
#include <QJsonObject>
#include <QPainter>
#include <QDebug>

CFeatureTagValueItem::CFeatureTagValueItem():
    m_value(0)
{
    setComment(QObject::tr("数值"));
    setFormat("0");
}

CFeatureTagValueItem::~CFeatureTagValueItem()
{

}
double CFeatureTagValueItem::value() const
{
    return m_value;
}

void CFeatureTagValueItem::setValue(double value)
{
    m_value = value;
}

CFeatureTagItem::ItemType CFeatureTagValueItem::type() const
{
    return CFeatureTagItem::ItemType::Value;
}

void CFeatureTagValueItem::calculate()
{
    if(visible())
    {
        m_drawText = comment()+" "+ QString::number(m_value,'f',format().toInt());

        QFontMetricsF metrics(font());
        double textHeight = metrics.ascent() + metrics.descent();
        double textWidth = metrics.width(m_drawText);

        QRectF dArea(drawArea());
        dArea.setSize(QSizeF(textWidth,textHeight));

        switch (hPos().position()) {
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

void CFeatureTagValueItem::drawItem(QPainter *p)
{
    if(visible()) {
        p->save();
        p->setFont(font());
//        p->drawText(drawArea(),m_drawText);
        p->drawText(drawArea(),comment()+" "+ QString::number(m_value,'f',format().toInt())); //!多语言
        p->restore();
    }
}

void CFeatureTagValueItem::serialize(QJsonObject &json)
{
    CFeatureTagItem::serialize(json);
    json.insert("Type",(int)type());
}
