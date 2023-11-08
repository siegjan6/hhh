#include "MDDiagram.h"
#include "DrawWidget.h"
CMDDiagram::CMDDiagram()
{
    m_widget = new CDrawWidget;
}

CMDDiagram::~CMDDiagram()
{
    if(m_widget)
    {
        delete m_widget;
        m_widget = NULL;
    }
}

QWidget *CMDDiagram::widget()
{
    return m_widget;
}

CMDDiagram &CMDDiagram::operator =(const CMDDiagram &other)
{
    if(this == &other)
    {
        return *this;
    }
    *m_widget =*other. m_widget;
}

void CMDDiagram::serialize(QJsonObject &json)
{
    m_widget->serialize(json);
}

void CMDDiagram::deserialize(const QJsonObject &json)
{
    m_widget->deserialize(json);
}

void CMDDiagram::setRect(const QRectF &value)
{
    m_widget->setRect(value);
}

bool CMDDiagram::openDesign()
{
    return m_widget->openDesign();
}

void CMDDiagram::onDoubleClick()
{
    m_widget->openDesign();
}

void CMDDiagram::setString1(const QString &str)
{
    m_widget->setString1(str);
}

void CMDDiagram::setString2(const QString &str)
{
    m_widget->setString2(str);
}

void CMDDiagram::setString3(const QString &str)
{
    m_widget->setString3(str);
}

void CMDDiagram::setValue(const QString &str)
{
    m_widget->setValue(str);
}

