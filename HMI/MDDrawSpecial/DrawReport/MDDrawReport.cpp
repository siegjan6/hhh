#include <QTextEdit>
#include "MDDrawReport.h"
#include "DrawReportData.h"
#include "PropertyDrawReport.h"

CMDDrawReport::CMDDrawReport()
{
    m_report = new CDrawReportData();
    QWidget *widget = m_report->m_report.getWidget();
    setWidget(widget);
}
CMDDrawReport::~CMDDrawReport()
{
    if(m_report)
    {
        delete m_report;
        m_report = NULL;
    }

}

void CMDDrawReport::setReportPath(const QString &value)
{
    m_report->m_report.setReportPath(value);
}

QString CMDDrawReport::getReportPath()
{
    return m_report->m_report.getReportPath();
}

/*!
\section 设置报表文件名
*/
void CMDDrawReport::setXMLfileName(const QString &value)
{
    m_report->m_report.setXMLfileName(value);
}

/*!
\section 查询报表文件名
*/
QString CMDDrawReport::getXMLfileName()
{
    return m_report->m_report.getXMLfileName();
}

void CMDDrawReport::setReportParameter(const QString &key, const QString &value)
{
    m_report->m_report.setReportParameter(key, value);
}

QString CMDDrawReport::getReportParameter(const QString &key)
{
    return m_report->m_report.getReportParameter(key);
}

void CMDDrawReport::setReportLanguage(const QString &langcode)
{
    m_report->m_report.setReportLanguage(langcode);
}
QString CMDDrawReport::getReportLanguage()
{
    return m_report->m_report.getReportLanguage();
}

/*!
\section 设置报表xml文件数据库参数
*/
void CMDDrawReport::setDataSourceParameter(const QString &dataSourceName, int type, const QString &parameter)
{
//    if(m_report->m_report.getNCReportOutputDSSetting())
//    {
//        m_report->m_report.getNCReportOutputDSSetting()->setDataSourceParameter(dataSourceName,type,parameter);
//    }
}

void CMDDrawReport::setQueryEngine(IMDDBQueryEngine *pEngine)
{
    m_report->m_report.setQueryEngine(pEngine);
}

/*!
\section 查询报表
*/
void CMDDrawReport::query()
{
    m_report->m_report.query();
}

void CMDDrawReport::exportTo(const QString &fileName, const QString& password/* = ""*/, const QString& perm/* = ""*/)
{
    m_report->m_report.exportTo(fileName, password/* = ""*/, perm/* = ""*/);
}

void CMDDrawReport::changeLanguage()
{
    m_report->m_report.changeLanguage();
}

void CMDDrawReport::collectWords()
{
    m_report->m_report.collectWords();
}

/*!
\section 设置报表文件背景色
*/
void CMDDrawReport::setReportBackColor(const QColor &color)
{
    m_report->m_report.setReportBackColor(color);
}

/*!
\section 查询报表文件背景色
*/
QColor CMDDrawReport::getReportBackColor()
{
    return m_report->m_report.getReportBackColor();
}

DrawType CMDDrawReport::type()
{
    return DrawType::Report;
}

/*!
\section 返回属性框对象
*/
CMDPropertyDrawObj *CMDDrawReport::createPropertyDrawObj()
{
    return new CPropertyDrawReport(this);
}

/*!
\section 初始化
*/
void CMDDrawReport::onInitialization()
{
    m_report->m_report.setIDrawEnvrionment(environment());

    CMDDrawWidget::onInitialization();
}

/*!
\section 创建自身
*/
CMDDrawObj *CMDDrawReport::createSelf()
{
    return new CMDDrawReport();
}


/*!
\section 克隆函数，复制报表控件
*/
CMDDrawObj *CMDDrawReport::clone()
{
    CMDDrawReport* obj = static_cast<CMDDrawReport*>(CMDDrawWidget::clone());
    *obj->m_report = *m_report;
    return obj;
}

void CMDDrawReport::serialize(QJsonObject &json)
{
    CMDDrawWidget::serialize(json);

    QJsonObject js;
    m_report->serialize(js);
    json.insert("DrawReport",js);
}

void CMDDrawReport::deserialize(const QJsonObject &json)
{
    CMDDrawWidget::deserialize(json);

    QJsonObject js = json.value("DrawReport").toObject();
    m_report->deserialize(js);
}
