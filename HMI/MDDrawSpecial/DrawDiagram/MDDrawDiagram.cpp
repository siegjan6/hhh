#include "MDDrawDiagram.h"
#include "DrawDiagramData.h"
#include "PropertyDrawDiagram.h"
#include "MDDrawVector.h"
CMDDrawDiagram::CMDDrawDiagram()
{
    m_data = new CDrawDiagramData();
    QWidget *widget = m_data->m_diagram.widget();
    setWidget(widget);
}
CMDDrawDiagram::~CMDDrawDiagram()
{
    delete m_data;
}

bool CMDDrawDiagram::openDesign()
{
    return m_data->m_diagram.openDesign();
}

void CMDDrawDiagram::onDoubleClick()
{
    m_data->m_diagram.onDoubleClick();
    parent()->setModify(true);
}
//////////////////////////////////////////////////////////////////////////
DrawType CMDDrawDiagram::type()
{
    return DrawType::Diagram;
}
CMDPropertyDrawObj *CMDDrawDiagram::createPropertyDrawObj()
{
    return new CPropertyDrawDiagram(this);
}
void CMDDrawDiagram::onInitialization()
{
    CMDDrawWidget::onInitialization();
}
CMDDrawWidget *CMDDrawDiagram::createSelf()
{
    return new CMDDrawDiagram();
}
void CMDDrawDiagram::onGeneratePath()
{
    CMDDrawWidget::onGeneratePath();
    QRectF rect1 = QRectF(0,0,rect().width(),rect().height());
    m_data->m_diagram.setRect(/*rect().toRect()*/rect1);
}

CMDDrawObj *CMDDrawDiagram::clone()
{
    CMDDrawDiagram* obj = static_cast<CMDDrawDiagram*>(CMDDrawWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

void CMDDrawDiagram::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);
    QJsonObject myJson;
    m_data->serialize(myJson);
    json.insert("MDDrawDiagram",myJson);
}

void CMDDrawDiagram::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);
    QJsonObject myJson = json.value("MDDrawDiagram").toObject();
    m_data->deserialize(myJson);
}

void CMDDrawDiagram::setString1(const QString &str)
{
   m_data->m_diagram.setString1(str);
}

void CMDDrawDiagram::setString2(const QString &str)
{
     m_data->m_diagram.setString2(str);
}

void CMDDrawDiagram::setString3(const QString &str)
{
     m_data->m_diagram.setString3(str);
}

void CMDDrawDiagram::setValue(const QString &str)
{
     m_data->m_diagram.setValue(str);
}
//////////////////////////////////////////////////////////////////////////


