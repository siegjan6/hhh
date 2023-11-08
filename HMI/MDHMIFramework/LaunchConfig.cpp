#include <QFile>
#include <QApplication>
#include <QDesktopWidget>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTextStream>
#include "LaunchConfig.h"
#include "HMIFramework.h"
//////////////////////////////////////////////////////////////////////////
CLaunchConfig::CLaunchConfig()
{
    m_launchFullScreen = false;//!全屏,启动时配置信息
    m_framework = NULL;
    FileName = "launch.config";
    m_designWidth = 1920;
    m_designHeight = 1080;
    m_scaleEnable = false;
}
//////////////////////////////////////////////////////////////////////////
void CLaunchConfig::init(CHMIFramework *framework)
{
    m_framework = framework;

    loadLaunchConfig();
}
void CLaunchConfig::saveLaunchConfig()
{
    QString name = m_framework->hmiPath() + "/" + FileName;
    QFile file(name);
    if (!file.open(QIODevice::WriteOnly))
        return;

    QJsonObject json;
    serializeHead(json);
    serialize(json);

    QJsonDocument doc;
    doc.setObject(json);
    QByteArray ba = doc.toJson();
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << ba;
    file.close();
}
void CLaunchConfig::loadLaunchConfig()
{
    QString name = m_framework->hmiPath() + "/" + FileName;
    QFile file(name);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray ba;
    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    ba = stream.readAll().toUtf8();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(ba, &error);
    if(error.error != QJsonParseError::NoError)
        return;

    deserialize(doc.object());

    launchForm();
}
void CLaunchConfig::launchForm()
{
    if (m_framework->isStudioMode())
        return;

    QPointF pf = QPointF(1, 1);
    if(m_scaleEnable)
        pf = calculateScale();
    m_framework->setFormScale(pf);

    QString path = m_framework->hmiPath();
    for (int i = 0; i < m_launchForms.count(); i++)
    {
        QString fullName = path + m_launchForms.at(i);
        m_framework->forms()->open(fullName);
    }
}
void CLaunchConfig::serialize(QJsonObject &json)
{
    json.insert("LaunchFullScreen", m_launchFullScreen);
    json.insert("DesignWidth", m_designWidth);
    json.insert("DesignHeight", m_designHeight);
    json.insert("ScaleEnable", m_scaleEnable);
    json.insert("LaunchForms", QJsonArray::fromStringList(m_launchForms));
}
void CLaunchConfig::deserialize(const QJsonObject &json)
{
    m_launchForms.clear();

    m_launchFullScreen = json.value("LaunchFullScreen").toBool();
    m_designWidth = json.value("DesignWidth").toInt();
    m_designHeight = json.value("DesignHeight").toInt();
    m_scaleEnable = json.value("ScaleEnable").toBool();
    QJsonArray array = json.value("LaunchForms").toArray();
    foreach(QJsonValue v, array)
        m_launchForms.append(v.toString());
}
void CLaunchConfig::serializeHead(QJsonObject &json)
{
    QJsonObject js;
    js.insert("Version", 1);
    js.insert("Type", QString("Mind.HMI.MDHMIFramework"));

    json.insert("Head", js);
}
//////////////////////////////////////////////////////////////////////////
qreal CLaunchConfig::limitScale(qreal value)
{
    const qreal Min = 0.2;
    const qreal Max = 5;
    value = (value < Min) ? Min : value;
    value = (value > Max) ? Max : value;
    return value;
}
QPointF CLaunchConfig::calculateScale()
{
    QRect screenRect = QApplication::desktop()->screenGeometry();
    qreal screenW = screenRect.width();
    qreal screenH = screenRect.height();

    qreal scaleW = screenW / m_designWidth;
    qreal scaleH = screenH / m_designHeight;
    scaleW = limitScale(scaleW);
    scaleH = limitScale(scaleH);

    return QPointF(scaleW, scaleH);
}
//////////////////////////////////////////////////////////////////////////
