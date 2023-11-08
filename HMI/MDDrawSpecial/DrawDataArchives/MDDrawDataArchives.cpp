#include "MDDrawDataArchives.h"
#include "DrawDataArchivesData.h"
#include "PropertyDrawDataArchives.h"
#include <QMessageBox>
CMDDrawDataArchives::CMDDrawDataArchives()
{
    m_data = new CDrawDataArchivesData();
    QWidget* widget = m_data->m_dataArchives.widget();
    setWidget(widget);
}

CMDDrawDataArchives::~CMDDrawDataArchives()
{
    delete m_data;
}

bool CMDDrawDataArchives::openConfigDialog()
{
    m_data->m_dataArchives.setGridName(name());
    return m_data->m_dataArchives.openConfigDialog();
}

void CMDDrawDataArchives::onDoubleClick()
{
    openConfigDialog();
    parent()->setModify(true);
}

void CMDDrawDataArchives::query()
{
    m_data->m_dataArchives.query();
}

DrawType CMDDrawDataArchives::type()
{
    return DrawType::DataArchives;
}

CMDPropertyDrawObj *CMDDrawDataArchives::createPropertyDrawObj()
{
    return new CPropertyDrawDataArchives(this);
}

void CMDDrawDataArchives::onInitialization()
{
    CMDDrawWidget::onInitialization();
    m_data->m_dataArchives.setDrawEnvrionment(environment());
    m_data->m_dataArchives.onInitialization();
}

CMDDrawObj *CMDDrawDataArchives::createSelf()
{
    return new CMDDrawDataArchives();
}

void CMDDrawDataArchives::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawDataArchives",js);
}

void CMDDrawDataArchives::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);

    QJsonObject js = json.value("DrawDataArchives").toObject();
    m_data->deserialize(js);
}

CMDDrawObj *CMDDrawDataArchives::clone()
{
    CMDDrawDataArchives* obj = static_cast<CMDDrawDataArchives*>(CMDDrawWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}
