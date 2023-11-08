#include <QPen>
#include "MDDrawText.h"
#include "DrawTextData.h"
#include "MDPropertyDrawText.h"

CMDDrawText::CMDDrawText()
{
    m_data = new CDrawTextData;

    m_data->m_option.setWrapMode(QTextOption::WrapAnywhere);
    m_data->m_textBrushManager.init(Qt::black);

    brush()->setBrushType(CMDBrushManager::BrushType::Nullptr);
    pen()->setWidth(0);
}
CMDDrawText::~CMDDrawText()
{
    delete m_data;
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawText::type()
{
    return DrawType::Text;
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawText::setFont(const QFont *value)
{
    m_data->m_font = *value;
    invalidate();
}
QFont *CMDDrawText::font()
{
    return &m_data->m_font;
}
void CMDDrawText::setText(const QString &value)
{
    m_data->m_text = value;
    m_data->m_showText = language()->value(m_data->m_text, false);
    invalidate();
}
QString CMDDrawText::text()
{
    return m_data->m_text;
}
CMDBrushManager *CMDDrawText::textBrush()
{
    return &m_data->m_textBrushManager;
}
void CMDDrawText::setHAlignment(Qt::Alignment value)
{
    m_data->m_alignment &= 0xF0;
    m_data->m_alignment |= value;

    initTextOption();
    invalidate();
}
Qt::Alignment CMDDrawText::hAlignment()
{
    return m_data->m_alignment & 0x0F;
}
void CMDDrawText::setVAlignment(Qt::Alignment value)
{
    m_data->m_alignment &= 0x0F;
    m_data->m_alignment |= value;

    initTextOption();
    invalidate();
}
Qt::Alignment CMDDrawText::vAlignment()
{
    return m_data->m_alignment & 0xF0;
}
void CMDDrawText::setIsWrap(bool value)
{
    if (value)
        m_data->m_wrapMode = QTextOption::WrapAnywhere;
    else
        m_data->m_wrapMode = QTextOption::NoWrap;

    initTextOption();
    invalidate();
}
bool CMDDrawText::isWrap()
{
    return !(m_data->m_wrapMode == QTextOption::NoWrap);
}
void CMDDrawText::setButtonFrame(bool value)
{
    m_data->m_isButtonFrame = value;

    if (m_data->m_isButtonFrame)
    {
        brush()->setBrushType(CMDBrushManager::BrushType::Solid);
        brush()->setColor(Qt::gray);
        pen()->setWidth(0);
    }
    else
        brush()->setBrushType(CMDBrushManager::BrushType::Nullptr);
}
bool CMDDrawText::buttonFrame()
{
    return m_data->m_isButtonFrame;
}
void CMDDrawText::setDecimal(int value)
{
    if (value >= 0 && value <= 10)
        m_data->m_decimal = value;
}
int CMDDrawText::decimal()
{
    return m_data->m_decimal;
}
void CMDDrawText::setDateFormat(const QString &value)
{
    m_data->m_dateFormat = value;
    m_data->m_showDateFormat = language()->value(m_data->m_dateFormat, false);
}
QString CMDDrawText::dateFormat()
{
    return m_data->m_dateFormat;
}
void CMDDrawText::setMaxLength(int value)
{
    if(value >= 0 && value <= 99999)
        m_data->m_maxLength = value;
}
int CMDDrawText::maxLength()
{
    return m_data->m_maxLength;
}
void CMDDrawText::setIsDownFrame(bool value)
{
    m_data->m_isDownFrame = value;
}
bool CMDDrawText::isDownFrame()
{
    return m_data->m_isDownFrame;
}
QString CMDDrawText::textFormatString()
{
    return QString("%1,%2,%3").arg(maxLength()).arg(decimal()).arg(dateFormat());
}
//////////////////////////////////////////////////////////////////////////
CMDDrawObj *CMDDrawText::createSelf()
{
    return new CMDDrawText();
}
void CMDDrawText::onPaint(QPainter *p)
{
    CMDDrawVector::onPaint(p);

    p->setFont(m_data->m_font);
    QPen pen;
    pen.setBrush(m_data->m_textBrushManager.getBrush());
    p->setPen(pen);
    p->drawText(rect(), m_data->m_showText, m_data->m_option);
}
void CMDDrawText::generatePath()
{
    CMDDrawVector::generatePath();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawText::generateContent()
{
    CMDDrawVector::generateContent();

    m_data->m_textBrushManager.generate(rect());
}
CMDPropertyDrawObj *CMDDrawText::createPropertyDrawObj()
{
    return new CMDPropertyDrawText(this);
}
void CMDDrawText::onInitialization()
{
    initTextOption();
}
void CMDDrawText::getBrushes(QList<CMDBrushManager *> &list)
{
    CMDDrawVector::getBrushes(list);
    list.append(&m_data->m_textBrushManager);
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawText::serialize(QJsonObject &json)
{
    CMDDrawVector::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawText", js);
}
void CMDDrawText::deserialize(const QJsonObject &json)
{
    CMDDrawVector::deserialize(json);

    QJsonObject js = json.value("DrawText").toObject();
    m_data->deserialize(js);
}
CMDDrawObj* CMDDrawText::clone()
{
    CMDDrawText* obj = static_cast<CMDDrawText*>(CMDDrawVector::clone());
    *obj->m_data = *m_data;

    return obj;
}

//////////////////////////////////////////////////////////////////////////
QList<QStringList> CMDDrawText::expNames()
{
    QList<QStringList> list;
    list << QStringList({"#appearance"});
    list << QStringList({"drawtext.text",   QObject::tr("文本")});
    return list;
}
void CMDDrawText::onDataChanged(CMDExpInfo *exp)
{
    switch (exp->index())
    {
    case 0:     //text
        setText(exp->stringValue(
                    m_data->m_decimal, m_data->m_showDateFormat, m_data->m_maxLength));
        break;
    default:
        break;
    }
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawText::initTextOption()
{
    m_data->m_option.setAlignment(m_data->m_alignment);
    m_data->m_option.setWrapMode(m_data->m_wrapMode);
}
void CMDDrawText::undoTextBrush(CMDBrushManager *value)
{
    *textBrush() = *value;
    generateContent();
}
//////////////////////////////////////////////////////////////////////////
void CMDDrawText::changeLanguage()
{
    m_data->m_showText = language()->value(m_data->m_text, false);
    m_data->m_showDateFormat = language()->value(m_data->m_dateFormat, false);
}
void CMDDrawText::collectWords()
{
    language()->addWord(m_data->m_text, false);
    language()->addWord(m_data->m_dateFormat, false);
}
//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawText::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawText", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawText(this);

    return CMDDrawVector::createPropertyDrawObj(className);
}
void CMDDrawText::initClassInfo(QStringList &classInfo)
{
    CMDDrawVector::initClassInfo(classInfo);
    classInfo.append("DrawText");
}
//////////////////////////////////////////////////////////////////////////




