/*!
\author aGRw 2015.08.27
\section 启动配置类
*/
#ifndef LAUNCHCONFIG_H
#define LAUNCHCONFIG_H

#include <QStringList>
#include <QJsonObject>

class CHMIFramework;

class CLaunchConfig
{
public:
    CLaunchConfig();
//////////////////////////////////////////////////////////////////////////
public:
    void setLaunchForms(const QStringList& value){m_launchForms = value;}
    QStringList launchForms(){return m_launchForms;}
    void setLaunchFullScreen(bool value){m_launchFullScreen = value;}
    bool launchFullScreen(){return m_launchFullScreen;}
private:
    QStringList m_launchForms;//!启动时窗体
    bool m_launchFullScreen;//!全屏,启动时配置信息
private:
    CHMIFramework* m_framework;
    QString FileName;
//////////////////////////////////////////////////////////////////////////
public:
    void init(CHMIFramework* framework);
    void saveLaunchConfig();
private:
    void loadLaunchConfig();
    void launchForm();//!加载窗体
    void serialize(QJsonObject &json);
    void deserialize(const QJsonObject &json);
    void serializeHead(QJsonObject &json);
/*!
\section form scale
*/
public:
    void setDesignWidth(int value){m_designWidth = value;}
    int designWidth(){return m_designWidth;}
    void setDesignHeight(int value){m_designHeight = value;}
    int designHeight(){return m_designHeight;}
    void setScaleEnable(bool value){m_scaleEnable = value;}
    bool scaleEnable(){return m_scaleEnable;}
private:
    //!设计分辨率
    qint32 m_designWidth;
    qint32 m_designHeight;
    //!是否缩放
    bool m_scaleEnable;
private:
    qreal limitScale(qreal value);
    QPointF calculateScale();
};

#endif // LAUNCHCONFIG_H
