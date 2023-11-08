/*!

\author bWVpeGw=

\section 功能

管理【报表输出控件的导出】。

*/

#ifndef MDREPORTOUTPUT_H
#define MDREPORTOUTPUT_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QJsonObject>
#include "interface/HMI/IDrawEnvironment.h"

#if defined(MDREPORTOUTPUT_LIBRARY)
#  define MDCNCREPORTOUTPUT_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDCNCREPORTOUTPUT_SHARED_EXPORT Q_DECL_IMPORT
#endif

class CNCReportOutput;
class IMDDBQueryEngine;

class  MDCNCREPORTOUTPUT_SHARED_EXPORT MDCNCReportOutputInc
{
public:
    MDCNCReportOutputInc();
    ~MDCNCReportOutputInc();

/*!
\section 报表输出控件相关属性接口函数管理
*/
public:
    MDCNCReportOutputInc& operator=(const MDCNCReportOutputInc& other); //! 拷贝报表输出控件相关属性

    //! 报表文件名
    void setXMLfileName(const QString &filename);
    QString getXMLfileName();
    //! 报表文件背景色
    void setReportBackColor(const QColor& color);
    QColor getReportBackColor();

    void setReportPath(const QString &strPath); //! 设置报表文件路径
    QString getReportPath();

    void setReportParameter(const QString &key, const QString &value);
    QString getReportParameter(const QString &key);

    void setReportLanguage(const QString &langcode);
    QString getReportLanguage();

    void setQueryEngine(IMDDBQueryEngine * pEngine);//c2pj
    void setIDrawEnvrionment(IDrawEnvrionment *IDrawEnvrionment); //! 设置项目管理参数
    QWidget* getWidget(); //! 获取当前窗体指针
    void query();  //! 查询报表
    //! 需要加密时使用password 和相关允许操作perm
    //! The permissions section can include one or more of the following features:
    //! perm 如下所示
    //    Printing – Top Quality Printing
    //    DegradedPrinting – Lower Quality Printing
    //    ModifyContents – Also allows Assembly
    //    Assembly
    //    CopyContents – Also allows ScreenReaders
    //    ScreenReaders
    //    ModifyAnnotations – Also allows FillIn
    //    FillIn
    //    AllFeatures – Allows the user to perform all of the above, and top quality printing.
    // 为默认值空时表示不处理perm
    void exportTo(const QString &fileName, const QString& password = "", const QString& perm = "");

    //! 序列化
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    //! 多语言
public:
    void changeLanguage();
    void collectWords();

private:
    CNCReportOutput *m_ReportOutput; //! 报表输出控件指针
};

#endif // MDREPORTOUTPUT_H
