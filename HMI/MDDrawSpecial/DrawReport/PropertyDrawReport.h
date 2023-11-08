/*!

\author bWVpeGw=

\section 功能

管理【图形窗体，报表控件属性类】。

*/

#ifndef PROPERTYDRAWREPORT_H
#define PROPERTYDRAWREPORT_H

#include "MDPropertyDrawObj.h"
#include <QColor>

class CMDDrawReport;


class CPropertyDrawReport : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Data)
    //    Q_PROPERTY(QString FileName READ getFileName WRITE setFileName)
    Q_PROPERTY(QString DILFileDialogFileName READ getFileDialogFileName WRITE setFileDialogFileName)
    Q_PROPERTY(QString DILReportBackColor READ getReportBackColor WRITE setReportBackColor)

public:
    CPropertyDrawReport(CMDDrawObj *parent);
    virtual ~CPropertyDrawReport(){}

    /*!
\section 报表输出控件相关属性接口
*/

protected:
    CMDDrawReport* object();

private:
    //    void setFileName(const QString& value);
    //    QString getFileName();
    void setFileDialogFileName(const QString& value);
    QString getFileDialogFileName();

    //! 报表文件背景色
    void setReportBackColor(const QString& value);
    QString getReportBackColor();

private:
    QString m_XMLFileName;
    QString m_FileDialogXMLFileName;
    QColor m_reportbackcolor;
};

#endif // PROPERTYDRAWREPORT_H
