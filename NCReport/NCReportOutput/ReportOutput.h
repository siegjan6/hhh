/*!

\author bWVpeGw=

\section 功能

管理【报表输出控件】。

*/

#ifndef CNCREPORTOUTPUT_H
#define CNCREPORTOUTPUT_H

#include <qobject.h>
#include <QWidget>
#include <qcolor.h>
#include <QMouseEvent>
#include <QLayout>
#include <QJsonObject>
#include <QLabel>
#include "interface/HMI/IDrawEnvironment.h"

#include "ITranslateWidgetTool.h"
#include "MDMultiLanguage.h"

class NCReport;
class NCReportPreviewWindow;
class IMDDBQueryEngine;
class QMenu;
class QAction;

enum class ReturnType{
    DataSource = 0,
    Filter,
    Rejected
};

class  CNCReportOutput : public QWidget
{
    Q_OBJECT

public:   
    CNCReportOutput(QWidget *parent = 0, Qt::WindowFlags f = Qt::Window);
    ~CNCReportOutput();

    /*!
\section 报表输出控件相关接口函数，主要用于设置报表输出控件相关属性设置和序列化处理
*/
public:
    //! 报表文件名
    void setXMLfileName(const QString &XMLfileName);
    QString getXMLfileName() const;
public:
    QString m_XMLfileName;  //! 报表文件名称,默认为空

public:
    void setQueryEngine(IMDDBQueryEngine * pEngine);
private:
    IMDDBQueryEngine * m_pDBQueryEngine;
public:
    //! 报表文件背景色
    void setReportBackColor(const QColor& color);
    QColor getReportBackColor();
public:
    QColor m_reportbackcolor;  //! 报表文件背景色,默认为灰色

public:
    //! 报表文件路径
    void setReportPath(const QString &strPath);
    QString getReportPath() const;
private:
    QString m_XMLfilePath; //! 报表文件路径

public:
    void setReportParameter(const QString &key, const QString &value);
    QString getReportParameter(const QString &key);

    void setReportLanguage(const QString &langcode);
    QString getReportLanguage();
public:
    //! 项目管理参数
    void setIDrawEnvrionment(IDrawEnvrionment *IDrawEnvrionment);
    IDrawEnvrionment *getIDrawEnvrionment() const;
private:
    IDrawEnvrionment *m_IDrawEnvrionment; //! 图形控件所需要的框架接口指针
public:
    /*!
     运行报表输出文件

     \param    1. QString filename 需要输出的报表文件名
     \returns   无
    */
    void runNCReport(QString filename);

    /*!
     保存报表至输出文件

     \param    1. QString filename 需要输出的报表文件名
               * fileName.pdf  导出pdf
               * fileName.svg  导出svg
               * printerName.print   打印至打印机名为printName到指定打印机
               * 当导出为pdf时可使用password及perm选项
     \returns   无
    */
    void exportTo(const QString& fileName, const QString& password = "", const QString& perm = "");

    //! 序列化
    void serialize(QJsonObject& json);
    void deserialize(const QJsonObject& json);

    /*!
\section 事件处理
*/
protected:
    //! 显示事件处理
    void showEvent(QShowEvent *event);
    //! 绘制报表输出控件背景
    void paintEvent(QPaintEvent *event);
    // !鼠标双击事件
    //    void mouseDoubleClickEvent(QMouseEvent *event);
    //    // !事件过滤
    //    bool eventFilter(QObject *watched, QEvent *event);

private:
    //! 获取报表文件目录
    QString getNCReportFilePath();
    //! 序列化过滤条件
    void loadFilter(const QString &fileName);

/*!
\section 右键菜单
*/
private:
    QMenu * m_pPopupMenu;                                                                        //! 右键菜单
    QAction * m_pSetQueryParameterAction;                                                  //! 设置运行时查询参数Action
    QAction * m_pQueryAction;                                                                      //! 查询Action
private slots:
    void slotEditMenu(const QPoint &pos);
    void slot_SetQueryParameter();
    void slot_Query();

private:
    QVBoxLayout *m_mainLayout ;                                                                //! 窗体垂直布局
    NCReport *m_report;                                                                                 //! 报表指针
    NCReportPreviewWindow *m_priviewWindow;                                          //! 报表预览窗体指针
    QPalette palette;                                                                                         //! 调色板
    QString m_NCReportFilePath;                                                                    //! 报表文件路径全局变量，,默认为空
    int m_version;                                                                                             //! 版本保护
    QMap<QString, QStringList> dataSourceInfoCopy;

    //! 多语言
public:
    void changeLanguage();
    void collectWords();
private:
    ITranslateWidgetTool* m_transTool;
    QString strText;
    QLabel* m_mvLabel;
    QMovie *movie;
};

#endif // CNCREPORTOUTPUT_H
