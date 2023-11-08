#include <QTextEdit>
#include "MDDrawProcDocker.h"
#include "DrawProcDockerData.h"
#include "PropertyDrawProcDocker.h"


CMDDrawProcDocker::CMDDrawProcDocker()
{
    m_procDockerData = new CDrawProcDockerData();
    QWidget *widget = &m_procDockerData->m_ProcDocker;
    // todo: 目前dock吸入的进程主窗体，在鼠标直接点击时不会自动顶到上层来，看是不是与事件过滤eventFilter有关系
    setWidgetDock(widget);
}
CMDDrawProcDocker::~CMDDrawProcDocker()
{
    if(m_procDockerData)
    {
        delete m_procDockerData;
        m_procDockerData = NULL;
    }
}

void CMDDrawProcDocker::run(const QString &cmd)
{
    m_procDockerData->m_ProcDocker.executeCommand("other", cmd);
    setCmd(cmd);
}

void CMDDrawProcDocker::setCmd(const QString& cmd)
{
    m_procDockerData->m_cmd = cmd;
}

QString CMDDrawProcDocker::getCmd()
{
    return m_procDockerData->m_cmd;
}

void CMDDrawProcDocker::changeLanguage()
{
    //m_report->m_report.changeLanguage();
}

void CMDDrawProcDocker::collectWords()
{
    //m_report->m_report.collectWords();
}

DrawType CMDDrawProcDocker::type()
{
    return DrawType::ProcDocker;
}

/*!
\section 返回属性框对象
*/
CMDPropertyDrawObj *CMDDrawProcDocker::createPropertyDrawObj()
{
    return new CPropertyDrawProcDocker(this);
}

/*!
\section 初始化
*/
void CMDDrawProcDocker::onInitialization()
{
    m_procDockerData->m_ProcDocker.setDrawEnvrionment(environment());

    CMDDrawWidget::onInitialization();
}

/*!
\section 创建自身
*/
CMDDrawObj *CMDDrawProcDocker::createSelf()
{
    return new CMDDrawProcDocker();
}


/*!
\section 克隆函数，复制控件
*/
CMDDrawObj *CMDDrawProcDocker::clone()
{
    CMDDrawProcDocker* obj = static_cast<CMDDrawProcDocker*>(CMDDrawWidget::clone());
    *obj->m_procDockerData = *m_procDockerData;
    return obj;
}

void CMDDrawProcDocker::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);

    QJsonObject js;
    m_procDockerData->serialize(js);
    json.insert("DrawProcDocker",js);
}

void CMDDrawProcDocker::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);

    QJsonObject js = json.value("DrawProcDocker").toObject();
    m_procDockerData->deserialize(js);
}


