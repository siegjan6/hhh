#include "CFeatureTagCommentItem.h"
#include "CTagPosition.h"
#include <QJsonObject>
#include <QPainter>

CFeatureTagCommentItem::CFeatureTagCommentItem()
{
    setItemName(QObject::tr("标签说明"));
    setComment(QObject::tr("总标签"));
}

CFeatureTagCommentItem::~CFeatureTagCommentItem()
{

}

CFeatureTagItem::ItemType CFeatureTagCommentItem::type() const
{
    return CFeatureTagItem::ItemType::Comment;
}

void CFeatureTagCommentItem::calculate()
{
    if(visible())
    {
        QString  drawText = comment();

        QFontMetricsF metrics(font());
        double textHeight = metrics.ascent() + metrics.descent();
        double textWidth = metrics.width(drawText);

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

void CFeatureTagCommentItem::drawItem(QPainter *p)
{
    if(visible()) {
        p->save();
        p->setFont(font());
        p->drawText(drawArea(),comment());
        p->restore();
    }
}

void CFeatureTagCommentItem::serialize(QJsonObject &json)
{
    json.insert("Type",(int)type());
    CFeatureTagItem::serialize(json);
}

