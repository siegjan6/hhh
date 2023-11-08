#include "CFeatureTagItem.h"
#include "CTagPosition.h"
#include <QPainter>
#include <QJsonObject>
#include "CTranslator.h"
#include "MDJsonConvert.h"
using namespace Translator;

CFeatureTagItem::CFeatureTagItem():
    m_visible(true)
{
    //    QFontMetricsF metrics(font());
    //    double textHeight = metrics.ascent() + metrics.descent();
    //    m_drawArea.setHeight(textHeight);
}

CFeatureTagItem::CFeatureTagItem(const QString &itemName, const QString &cm, const QString &fm):
    m_itemName(itemName),
    m_comment(cm),
    m_format(fm)
{
    m_visible = true;
    //    QFontMetricsF metrics(font());
    //    double textHeight = metrics.ascent() + metrics.descent();
    //    m_drawArea.setHeight(textHeight);
}

CFeatureTagItem::~CFeatureTagItem()
{

}

QString CFeatureTagItem::itemName() const
{
    return m_itemName;
//    return m_translateName.isEmpty()? m_itemName:m_translateName;
}

void CFeatureTagItem::setItemName(const QString &itemName)
{
    m_itemName = itemName;
}

QString CFeatureTagItem::comment() const
{
    return m_comment;
//    return m_translateComment.isEmpty()? m_comment:m_translateComment;
}

void CFeatureTagItem::setComment(const QString &comment)
{
    m_comment = comment;
}

QString CFeatureTagItem::format() const
{
    return m_format;
}

void CFeatureTagItem::setFormat(const QString &format)
{
    m_format = format;
}

QFont CFeatureTagItem::font() const
{
    return m_font;
}

void CFeatureTagItem::setFont(const QFont &font)
{
    m_font = font;
}

QPointF CFeatureTagItem::point() const
{
    return m_point;
}

void CFeatureTagItem::setPoint(const QPointF &point)
{
    m_point = point;
}

bool CFeatureTagItem::visible() const
{
    return m_visible;
}

void CFeatureTagItem::setVisible(bool visible)
{
    m_visible = visible;
}

QRectF CFeatureTagItem::drawArea() const
{
    return m_drawArea;
}

void CFeatureTagItem::setDrawArea(const QRectF &drawArea)
{
    m_drawArea = drawArea;
}

double CFeatureTagItem:: textHeight () const
{
    QFontMetricsF metrics(m_font);
    return metrics.ascent() + metrics.descent();
}

void CFeatureTagItem::setDrawTopLeft(const QPointF &topLeft)
{
    m_drawArea.moveTopLeft(topLeft);
}

void CFeatureTagItem::serialize(QJsonObject &json)
{
    json.insert("Visible",m_visible);
    json.insert("ItemName",m_itemName);
    json.insert("Comment",m_comment);
    json.insert("Format",m_format);
    json.insert("Font",CMDJsonConvert::toJson(m_font));
    json.insert("DrawArea",CMDJsonConvert::toJson(m_drawArea));
    json.insert("Point",CMDJsonConvert::toJson(m_point));
    QJsonObject hJson;
    m_hPos.serialize(hJson);
    json.insert("HPos",hJson);
    QJsonObject vJson;
    m_vPos.serialize(vJson);
    json.insert("VPos",vJson);
}

void CFeatureTagItem::deserialize(const QJsonObject &json)
{
    m_visible = json.value("Visible").toBool();
    m_itemName = json.value("ItemName").toString();
    m_comment = json.value("Comment").toString();
    m_format = json.value("Format").toString();
    m_font =CMDJsonConvert::toQFont(json.value("Font"));
    m_drawArea =CMDJsonConvert::toQRectF(json.value("DrawArea"));
    m_point = CMDJsonConvert::toQPointF(json.value("Point"));
    QJsonObject hJson = json.value("HPos").toObject();
    m_hPos.deserialize(hJson);
    QJsonObject vJson = json.value("VPos").toObject();
    m_vPos.deserialize(vJson);
}

void CFeatureTagItem::changeLanguage()
{
    m_translateComment = translatorString(m_comment);
    m_translateName = translatorString(m_itemName);
}

void CFeatureTagItem::collectWords()
{
    addTranslatorWord(m_comment);
    addTranslatorWord(m_itemName);
}

CTagVPos CFeatureTagItem::vPos() const
{
    return m_vPos;
}

void CFeatureTagItem::setVPos(const CTagVPos &vPos)
{
    m_vPos = vPos;
}

CTagHPos CFeatureTagItem::hPos() const
{
    return m_hPos;
}

void CFeatureTagItem::setHPos(const CTagHPos &hPos)
{
    m_hPos = hPos;
}
