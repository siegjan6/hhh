#include "PropertyDrawReport.h"
#include "MDDrawReport.h"
#include <QFileDialog>
#include <QColorDialog>

CPropertyDrawReport::CPropertyDrawReport(CMDDrawObj *parent) :
    CMDPropertyDrawObj(parent)
{
    m_XMLFileName = "";
    m_FileDialogXMLFileName = "";
    m_reportbackcolor = Qt::gray;

    insertTranslateMap("GUID_Data",tr("数据"));
    insertTranslateMap("FileDialogFileName",tr("文件选择"));
    insertTranslateMap("ReportBackColor",tr("背景色"));
}

CMDDrawReport *CPropertyDrawReport::object()
{
    return dynamic_cast<CMDDrawReport*>(m_object);
}

//void CPropertyDrawReport::setFileName(const QString &value)
//{
//    m_XMLFileName = value;
//    object()->setXMLfileName(value);
//    setModify(true);
//}

//QString CPropertyDrawReport::getFileName()
//{
////    return m_XMLFileName;
//    return object()->getXMLfileName();
//}

void CPropertyDrawReport::setFileDialogFileName(const QString &value)
{
    Q_UNUSED(value);
    QString XMLFilePath = object()->environment()->projectPath()+"/Report/";
    QString file_fullpath = QFileDialog::getOpenFileName(dynamic_cast<QMainWindow*>(object()->parent()),tr("文件对话框"),XMLFilePath,tr("xml文件(*xml *ncr)"));
    QFileInfo fileinfo = QFileInfo(file_fullpath);
    m_FileDialogXMLFileName = fileinfo.fileName();
    object()->setXMLfileName(m_FileDialogXMLFileName);
    setModify(true);
}

QString CPropertyDrawReport::getFileDialogFileName()
{
    return object()->getXMLfileName();
}

/*!
\section 设置报表文件背景色
*/
void CPropertyDrawReport::setReportBackColor(const QString &value)
{
    Q_UNUSED(value);
    QColor tempcolor = QColorDialog::getColor(m_reportbackcolor,dynamic_cast<QMainWindow*>(object()->parent()),tr("颜色对话框"));
    if(!tempcolor.isValid())
    {
        return;
    }
    m_reportbackcolor = tempcolor;
    object()->setReportBackColor(m_reportbackcolor);
    setModify(true);
}

/*!
\section 查询报表文件背景色
*/
QString CPropertyDrawReport::getReportBackColor()
{
    return "";
}
