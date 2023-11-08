#include "MDDrawAlarm.h"
#include "DrawAlarmData.h"
#include "PropertyDrawAlarm.h"

CMDDrawAlarm::CMDDrawAlarm()
{
    m_data = new CDrawAlarmData();

    QWidget *widget = m_data->m_alarm.widget();
    setWidget(widget);
}
CMDDrawAlarm::~CMDDrawAlarm()
{
    if(m_data)
    {
        delete m_data;
        m_data = NULL;
    }
}
bool CMDDrawAlarm::openConfigDialog()
{
    m_data->m_alarm.setGridName(name());
    return m_data->m_alarm.openConfigDialog();
}

void CMDDrawAlarm::onDoubleClick()
{
    openConfigDialog();
    parent()->setModify(true);
}

//void CMDDrawAlarm::setGridType(GridType type)
//{
////    m_data->m_alarm.setGridType (GridType::Alarm);
//}

DrawType CMDDrawAlarm::type()
{
    return DrawType::Alarm;
}
CMDPropertyDrawObj *CMDDrawAlarm::createPropertyDrawObj()
{
    return new CPropertyDrawAlarm(this);
}
void CMDDrawAlarm::onInitialization()
{
    CMDDrawWidget::onInitialization();
    m_data->m_alarm.setDrawEnvrionment(environment());
    m_data->m_alarm.onInitialization();
}
void CMDDrawAlarm::notify(NotifyType type)
{
    if (type == NotifyType::Alarm)
    {
        m_data->m_alarm.setNotify(true);
    }
}
CMDDrawObj *CMDDrawAlarm::createSelf()
{
    return new CMDDrawAlarm();
}
void CMDDrawAlarm::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);

    QJsonObject js;
    m_data->serialize(js);
    json.insert("DrawAlarm",js);
}

void CMDDrawAlarm::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);

    QJsonObject js = json.value("DrawAlarm").toObject();
    m_data->deserialize(js);
}

CMDDrawObj *CMDDrawAlarm::clone()
{
    CMDDrawAlarm* obj = static_cast<CMDDrawAlarm*>(CMDDrawWidget::clone());
    *obj->m_data = *m_data;
    return obj;
}

