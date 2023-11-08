/*!

\author luyj

\section 功能

管理【pdf viewer控件数据类】。

*/

#ifndef DRAWPDFVIEWERDATA_H
#define DRAWPDFVIEWERDATA_H

#include <QJsonObject>
#include "pdfviewerwidget.h"

//! 进程主窗体停靠控件,数据类
class CDrawPdfViewerData
{
public:
    QString m_filePath;

    PdfViewerWidget m_PdfViewer;

/*!
\section pdf viewer控件拷贝和序列化相关接口
*/
public:
    CDrawPdfViewerData();
    ~CDrawPdfViewerData();
    CDrawPdfViewerData& operator=(const CDrawPdfViewerData& other); //! 拷贝

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // DRAWPDFVIEWERDATA_H
