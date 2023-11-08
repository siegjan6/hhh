/*!

\author luyj

\section 功能

管理【图形窗体，特殊控件pdf viewer靠】。

*/

#ifndef MDDRAWPDFVIEWER_H
#define MDDRAWPDFVIEWER_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include "MDDrawWidget.h"
#include "IDrawEnvironment.h"

class CDrawPdfViewerData;

//! 报表控件
class MDDRAWSPECIALSHARED_EXPORT CMDDrawPdfViewer : public CMDDrawWidget
{
public:
    CMDDrawPdfViewer();
    virtual ~CMDDrawPdfViewer();

/*!
\section pdf viewer控件相关属性接口
*/
public:
    //! 显示pdf文档
    void open(const QString& filePath);

    void setFilePath(const QString& filePath);
    QString getFilePath();

    // mode: 0 - SinglePage, 1 - MultiPage
    void setPageMode(int mode);

    //! 多语言
public:
    void changeLanguage();
    void collectWords();

private:
    CDrawPdfViewerData* m_pdfViewerData;

/*!
\section DrawWidget
*/
public:
    DrawType type() ;
    QString className(){return "CMDDrawPdfViewer";}

/*!
\section virtual function
*/
public:
    CMDPropertyDrawObj *createPropertyDrawObj() ; //! 返回属性框对象
    void onInitialization() ; //! 初始化

protected:

    CMDDrawObj *createSelf() ; //! 创建自身

/*!
\section 序列化
*/
public:
    CMDDrawObj* clone() ; //! 克隆函数，复制报表控件

    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);
};

#endif // MDDRAWPDFVIEWER_H
