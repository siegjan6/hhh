#include <QFrame>
#include <QDebug>
#include "MDDrawBasicWidget.h"
#include "DrawBasicWidgetData.h"
#include "MDPropertyDrawBasicWidget.h"

CMDDrawBasicWidget::CMDDrawBasicWidget()
{
    m_data = new CDrawBasicWidgetData;
}
CMDDrawBasicWidget::~CMDDrawBasicWidget()
{
    if (widget() != NULL)
        delete widget();
    delete m_data;
}

/*!
\section  属性设置函数
*/
void CMDDrawBasicWidget::setTextColor(const QColor &value)
{
    m_data->m_textColor = value;

    resetColor();
}

QColor CMDDrawBasicWidget::textColor()
{
    return m_data->m_textColor;
}

void CMDDrawBasicWidget::setBackColor(const QColor &value)
{
    m_data->m_backColor = value;
    resetColor();
}
QColor CMDDrawBasicWidget::backColor()
{
    return m_data->m_backColor;
}

void CMDDrawBasicWidget::setDisableBackColor(const QColor &value)
{
    m_data->m_disableBackColor = value;
    resetColor();
}
QColor CMDDrawBasicWidget::disableBackColor()
{
    return m_data->m_disableBackColor;
}

void CMDDrawBasicWidget::setFont(const QFont *value)
{
    m_data->m_font = *value;
    widget()->setFont(*value);
}
QFont *CMDDrawBasicWidget::font()
{
    return &m_data->m_font;
}
void CMDDrawBasicWidget::setEnable(bool value)
{
    m_data->m_enable = value;
    if(!isStudioMode())
        widget()->setEnabled(finalEnabled());

    resetColor();
}

bool CMDDrawBasicWidget::enable()
{
    return m_data->m_enable;
}
bool CMDDrawBasicWidget::finalEnabled()
{
    if(!CMDDrawWidget::finalEnabled())
        return false;

    return enable();
}
/*!
\section  初始化
*/
void CMDDrawBasicWidget::onInitialization()
{
    CMDDrawWidget::onInitialization();

    QWidget* w = widget();
    if(!w)
        return;
    w->setFont(m_data->m_font);

    resetColor();
}

/*!
\section  序列化
*/
CMDDrawObj *CMDDrawBasicWidget::clone()
{
    CMDDrawBasicWidget* obj = static_cast<CMDDrawBasicWidget*>(CMDDrawObj::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawBasicWidget::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawBasicWidget",js);
}

void CMDDrawBasicWidget::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);

    QJsonObject js = json.value("DrawBasicWidget").toObject();
    m_data->deserialize(js);
}

/*!
\section  颜色设置函数
*/
void CMDDrawBasicWidget::resetColor()
{
    QColor backColor;
    if(m_data->m_enable)
        backColor = m_data->m_backColor;
    else
        backColor = m_data->m_disableBackColor;

    QColor textColor = m_data->m_textColor;
    QString style = QString("background-color:rgb(%1,%2,%3,%4); color:rgb(%5,%6,%7,%8)")
            .arg(backColor.red())
            .arg(backColor.green())
            .arg(backColor.blue())
            .arg(backColor.alpha())
            .arg(textColor.red())
            .arg(textColor.green())
            .arg(textColor.blue())
            .arg(textColor.alpha());
    widget()->setStyleSheet(style);
}

QString CMDDrawBasicWidget::getBasicWidgetColor()
{
    QColor backcolor;
    QColor disablebackcolor;
    QColor textcolor = textColor();
    if(enable())
    {
        backcolor = backColor();
        QString backColorStyle = QString("background-color:rgb(%1,%2,%3,%4); color:rgb(%5,%6,%7,%8)")
                .arg(backcolor.red())
                .arg(backcolor.green())
                .arg(backcolor.blue())
                .arg(backcolor.alpha())
                .arg(textcolor.red())
                .arg(textcolor.green())
                .arg(textcolor.blue())
                .arg(textcolor.alpha());

        return backColorStyle;
    }
    else
    {
        disablebackcolor = disableBackColor();
        QString disableBackColorStyle = QString("background-color:rgb(%1,%2,%3,%4); color:rgb(%5,%6,%7,%8)")
                .arg(disablebackcolor.red())
                .arg(disablebackcolor.green())
                .arg(disablebackcolor.blue())
                .arg(disablebackcolor.alpha())
                .arg(textcolor.red())
                .arg(textcolor.green())
                .arg(textcolor.blue())
                .arg(textcolor.alpha());

        return disableBackColorStyle;
    }
}

//////////////////////////////////////////////////////////////////////////
CMDPropertyDrawObj *CMDDrawBasicWidget::createPropertyDrawObj(const QString &className)
{
    if(className.compare("DrawBasicWidget", Qt::CaseInsensitive) == 0)
        return new CMDPropertyDrawBasicWidget(this);

    return CMDDrawObj::createPropertyDrawObj(className);
}

void CMDDrawBasicWidget::initClassInfo(QStringList &classInfo)
{
    CMDDrawObj::initClassInfo(classInfo);
    classInfo.append("DrawBasicWidget");
}
//////////////////////////////////////////////////////////////////////////
