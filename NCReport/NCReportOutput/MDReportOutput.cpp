#include "MDReport/MDReportOutput.h"
#include "ReportOutput.h"
#include <QDebug>


MDCNCReportOutputInc::MDCNCReportOutputInc()
{
    m_ReportOutput = new CNCReportOutput;
}

MDCNCReportOutputInc::~MDCNCReportOutputInc()
{
    if(m_ReportOutput)
    {
        delete m_ReportOutput;
        m_ReportOutput = NULL;
    }
}

/*!
\section 拷贝报表输出控件相关属性
*/
MDCNCReportOutputInc& MDCNCReportOutputInc::operator=(const MDCNCReportOutputInc &other)
{
    if(this == &other)
    {
        return *this;
    }
    m_ReportOutput->m_XMLfileName = other.m_ReportOutput->m_XMLfileName;
    m_ReportOutput->m_reportbackcolor = other.m_ReportOutput->m_reportbackcolor;
    return *this;
}

void MDCNCReportOutputInc::setQueryEngine(IMDDBQueryEngine * pEngine)
{
    m_ReportOutput->setQueryEngine(pEngine);
}

/*!
\section 设置报表文件路径
*/
void MDCNCReportOutputInc::setReportPath(const QString &strPath)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        pWidget->setReportPath(strPath);
    }
}

QString MDCNCReportOutputInc::getReportPath()
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        return pWidget->getReportPath();
    }
    return "";
}

/*!
\section 设置报表文件名
*/
void MDCNCReportOutputInc::setXMLfileName(const QString &filename)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        pWidget->setXMLfileName(filename);
    }
}

/*!
\section 获取报表文件名
*/
QString MDCNCReportOutputInc::getXMLfileName()
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        return pWidget->getXMLfileName();
    }
    else
    {
        return "";
    }
}

/*!
\section 设置报表文件背景色
*/
void MDCNCReportOutputInc::setReportBackColor(const QColor &color)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        pWidget->setReportBackColor(color);
    }
}

/*!
\section 查询报表文件背景色
*/
QColor MDCNCReportOutputInc::getReportBackColor()
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        return pWidget->getReportBackColor();
    }
    return Qt::gray;

}

void MDCNCReportOutputInc::setReportParameter(const QString &key, const QString &value)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        pWidget->setReportParameter(key,value);
    }
}

QString MDCNCReportOutputInc::getReportParameter(const QString &key)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        return pWidget->getReportParameter(key);
    }

    return QString::null;
}

void MDCNCReportOutputInc::setReportLanguage(const QString &langcode)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        pWidget->setReportLanguage(langcode);
    }
}

QString MDCNCReportOutputInc::getReportLanguage()
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        return pWidget->getReportLanguage();
    }

    return QString::null;
}

/*!
\section 获取当前窗体指针
*/
QWidget *MDCNCReportOutputInc::getWidget()
{
    return m_ReportOutput;
}

/*!
\section 设置项目管理参数
*/
void MDCNCReportOutputInc::setIDrawEnvrionment(IDrawEnvrionment *IDrawEnvrionment)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        (pWidget)->setIDrawEnvrionment(IDrawEnvrionment);
    }
}

/*!
\section 查询报表
*/
void MDCNCReportOutputInc::query()
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        QString m_NCReportFilePath = "";
        QString projectPath = (pWidget)->getIDrawEnvrionment()->projectPath();
        if(projectPath.isEmpty())
        {
            return ;
        }
        m_NCReportFilePath += projectPath;
        if(m_NCReportFilePath.right(0) != "/")
        {
            m_NCReportFilePath += "/";
        }
        m_NCReportFilePath += "Report";
        m_NCReportFilePath += "/";
        m_NCReportFilePath +=  (pWidget)->getXMLfileName();
        (pWidget)->runNCReport( m_NCReportFilePath);
    }
}

void MDCNCReportOutputInc::exportTo(const QString &fileName, const QString& password/* = ""*/, const QString& perm/* = ""*/)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        pWidget->exportTo(fileName, password/* = ""*/, perm/* = ""*/);
    }
}

void MDCNCReportOutputInc::serialize(QJsonObject &json)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        pWidget->serialize(json);
    }
}

void MDCNCReportOutputInc::deserialize(const QJsonObject &json)
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        (pWidget)->deserialize(json);
    }
}

void MDCNCReportOutputInc::changeLanguage()
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        (pWidget)->changeLanguage();
    }
}

void MDCNCReportOutputInc::collectWords()
{
    CNCReportOutput *pWidget = (CNCReportOutput*)getWidget();
    if(pWidget)
    {
        (pWidget)->collectWords();
    }
}
