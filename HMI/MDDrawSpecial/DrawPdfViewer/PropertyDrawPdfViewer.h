/*!

\author luyj

\section 功能

管理【图形窗体，pdf viewer停靠控件属性类】。

*/

#ifndef PROPERTYDRAWPDFVIEWER_H
#define PROPERTYDRAWPDFVIEWER_H

#include "MDPropertyDrawObj.h"
#include <QColor>

class CMDDrawPdfViewer;


class CPropertyDrawPdfViewer : public CMDPropertyDrawObj
{
    Q_OBJECT

    Q_PROPERTY(QString GUID_Data)
    //    Q_PROPERTY(QString FileName READ getFileName WRITE setFileName)
    Q_PROPERTY(QString filePath READ getFilePath WRITE setFilePath)
    //Q_PROPERTY(QString DILReportBackColor READ getReportBackColor WRITE setReportBackColor)

public:
    CPropertyDrawPdfViewer(CMDDrawObj *parent);
    virtual ~CPropertyDrawPdfViewer(){}

    /*!
\section pdf viewer 控件相关属性接口
*/

protected:
    CMDDrawPdfViewer* object();

private:
    void setFilePath(const QString& filePath);
    QString getFilePath();

private:
    QString m_filePath;
};

#endif // PROPERTYDRAWPDFVIEWER_H
