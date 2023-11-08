#include "CHeader.h"
#include "Data.h"
#include <QPointF>
#include <QFont>
#include <QColor>
#include <QJsonObject>
#include "MDJsonConvert.h"
#include "CTranslator.h"
using namespace Translator;

CHeader::CHeader():
    m_startPoint(100,127),
    m_caption(QObject::tr("测试曲线")),
    m_fontColor(Qt::black)
{
    m_font.setPointSize(20);        
}

CHeader::~CHeader()
{
}

void CHeader::setStart(const QPoint &point)
{
    m_startPoint = point;
    QFontMetrics metrics(m_font);
    int captionHeight = metrics.ascent() + metrics.descent();
    int y = m_startPoint.y();
    m_startPoint.setY(y+captionHeight);
}

QPoint CHeader::start() const
{
    return m_startPoint;
}

void CHeader::setText(const QString &string)
{
    m_caption = string;
}

QString CHeader::text() const
{
    return m_caption;
}

void CHeader::setFont(const QFont &font)
{
    m_font = font;
}

QFont CHeader::font() const
{
    return m_font;
}

void CHeader::draw(QPainter *p)
{
    p->save();

    p->setPen(m_fontColor);
    p->setFont(m_font);
    p->drawText(m_startPoint,(m_translateCaption.isEmpty() ? m_caption : m_translateCaption));

    p->restore();
}

void CHeader::serialize(QJsonObject &json)
{
    json.insert("StartPoint",CMDJsonConvert::toJson(m_startPoint));
    json.insert("Caption",m_caption);
    json.insert("Font",CMDJsonConvert::toJson(m_font));
    json.insert("FontColor",(double)m_fontColor.rgba());

}

void CHeader::deserialize(const QJsonObject &json)
{
    m_startPoint = CMDJsonConvert::toQPoint(json.value("StartPoint"));
    m_caption = json.value("Caption").toString();
    m_font = CMDJsonConvert::toQFont(json.value("Font"));
    m_fontColor = QColor::fromRgba(json.value("FontColor").toDouble());
}

void CHeader::changeLanguage()
{
    m_translateCaption = translatorString(m_caption);
}

void CHeader::collectWords()
{
    addTranslatorWord(m_caption);
}
QColor CHeader::fontColor() const
{
    return m_fontColor;
}

void CHeader::setFontColor(const QColor &fontColor)
{
    m_fontColor = fontColor;
}

