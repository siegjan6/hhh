#include "MDDrawLog.h"
#include "DrawLogData.h"
#include "PropertyDrawLog.h"

CMDDrawLog::CMDDrawLog()
{
    m_data = new CDrawLogData();

    QWidget *widget = m_data->m_log.widget();
    setWidget(widget);
}

CMDDrawLog::~CMDDrawLog()
{
    delete m_data;
}

bool CMDDrawLog::openConfigDialog()
{
    m_data->m_log.setGridName(name());
    return m_data->m_log.openConfigDialog();
}

void CMDDrawLog::onDoubleClick()
{
    openConfigDialog();
    parent()->setModify(true);
}

//void CMDDrawLog::setGridType(GridType type)
//{
////    m_data->m_log.setGridType (GridType::Log);
//}

DrawType CMDDrawLog::type()
{
    return DrawType::Log;
}

CMDPropertyDrawObj *CMDDrawLog::createPropertyDrawObj()
{
    return new CPropertyDrawLog(this);
}

void CMDDrawLog::onInitialization()
{
    CMDDrawWidget::onInitialization();
    m_data->m_log.setDrawEnvrionment(environment());
    m_data->m_log.onInitialization();
}
void CMDDrawLog::notify(NotifyType type)
{
    if (type == NotifyType::Log)
    {
        m_data->m_log.setNotify (true);
    }
}
CMDDrawObj *CMDDrawLog::createSelf()
{
    return new CMDDrawLog();
}
void CMDDrawLog::serialize(QJsonObject &json)
{  
    CMDDrawWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawLog",js);
}

void CMDDrawLog::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);

    QJsonObject js = json.value("DrawLog").toObject();
    m_data->deserialize(js);
}

CMDDrawObj *CMDDrawLog::clone()
{
    CMDDrawLog* obj = static_cast<CMDDrawLog*>(CMDDrawWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}
