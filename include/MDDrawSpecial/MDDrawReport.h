/*!

\author bWVpeGw=

\section 功能

管理【图形窗体，特殊控件报表导出】。

*/

#ifndef MDDRAWREPORT_H
#define MDDRAWREPORT_H

#include <QtCore/qglobal.h>

#if defined(MDDRAWSPECIAL_LIBRARY)
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MDDRAWSPECIALSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QJsonObject>
#include "MDDrawWidget.h"
#include "IDrawEnvironment.h"

class IMDDBQueryEngine;
class CDrawReportData;

//! 报表控件
class MDDRAWSPECIALSHARED_EXPORT CMDDrawReport : public CMDDrawWidget
{
public:
    CMDDrawReport();
    virtual ~CMDDrawReport();

/*!
\section 报表输出控件相关属性接口
*/
public:
    //! 报表路径
    void setReportPath(const QString &value);
    QString getReportPath();
    //! 报表文件名
    void setXMLfileName(const QString &value);
    QString getXMLfileName();
    //! 报表文件背景色
    void setReportBackColor(const QColor& color);
    QColor getReportBackColor();
    //! 报表参数
    void setReportParameter(const QString &key, const QString &value);
    QString getReportParameter(const QString &key);
    //! 报表国际化语言2位langcode
    void setReportLanguage(const QString &langcode);
    QString getReportLanguage();

    void setDataSourceParameter(const QString &dataSourceName,int type,const QString &parameter); //! 设置报表xml文件数据库参数

    void setQueryEngine(IMDDBQueryEngine * pEngine);// mxl 2016.12.02
    void query(); //! 查询报表

    void exportTo(const QString &fileName, const QString& password = "", const QString& perm = "");

    //! 多语言
public:
    void changeLanguage();
    void collectWords();

private:
    CDrawReportData* m_report;

/*!
\section DrawWidget
*/
public:
    DrawType type() ;
    QString className(){return "CMDDrawReport";}

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

#endif // MDDRAWREPORT_H
