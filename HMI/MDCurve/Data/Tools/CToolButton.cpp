#include "CToolButton.h"
#include <QJsonObject>
#include <QDebug>

CToolButton::CToolButton(QWidget *parent) :
    QPushButton(parent),
    m_isIcon(true),
    m_visible(true)
{
}
qint32 CToolButton::buttonPos() const
{
    return m_buttonPos;
}

void CToolButton::setButtonPos(const qint32 &buttonPos)
{
    m_buttonPos = buttonPos;
}
bool CToolButton::isIcon() const
{
    return m_isIcon;
}

void CToolButton::setIsIcon(bool isIcon)
{
    m_isIcon = isIcon;
}
QString CToolButton::buttonName() const
{
    return m_buttonName;
}

void CToolButton::setButtonName(const QString &buttonName)
{
    m_buttonName = buttonName;
}


void CToolButton::setButtonVisible(bool visible)
{
    m_visible = visible;
    QPushButton::setVisible(visible);
}

bool CToolButton::buttonVisible() const
{
    return m_visible;
}

CToolButton *CToolButton::clone()
{
    CToolButton * button = new CToolButton;
    button->m_isIcon = this->m_isIcon;
    button->setButtonVisible(this->buttonVisible());
    button->m_buttonName = this->m_buttonName;
    button->m_buttonPos = this->m_buttonPos;

    return button;
}

void CToolButton::serialize(QJsonObject &json)
{
    json.insert("ButtonPos",m_buttonPos);
    json.insert("ButtonName",m_buttonName);
    json.insert("Visible",m_visible);
}

void CToolButton::deserialize(const QJsonObject &json)
{
    m_buttonPos = json.value("ButtonPos").toInt();
    m_buttonName = json.value("ButtonName").toString();
    m_visible = json.value("Visible").toBool();
}



