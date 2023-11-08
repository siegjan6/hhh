#ifndef DATASELECTUI_H
#define DATASELECTUI_H

/*!

\author bHVvY2hhbw==

\section 功能
1.CMDDataSelectUI类内部实现类。
2.用于UI界面选择，选择当前项目中配置的所有数据源以及表。

*/

#include <QWidget>

class IMDDataArchives;
class IMDDataSource;
class IMDDataTable;
class QPushButton;
class QLabel;
class QComboBox;
class QGridLayout;

class CDataSelectUI : public QWidget
{
    Q_OBJECT
public:
    explicit CDataSelectUI(QWidget *parent = 0);
    virtual ~CDataSelectUI();

    /*!
    \section 设置/获取属性
    */
public:
    void setDataArchives(IMDDataArchives *pDataArchives); //! 设置数据归档
    void setComboxText(const QString &strSrc, const QString &strTab); //! 设置当前选择的数据源名与表名，用于从配置读取后设置

    QString dataSourceName(); //! 获取当前选择数据源名
    QString dataTableName(); //! 获取当前选择的数据表名

    IMDDataSource *dataSource(); //! 获取当前选择数据源指针
    IMDDataTable *dataTable(); //! 获取当前选择的数据指针

    /*!
    \section 通知选择的数据源或者表名改变的信号
    */
signals:
    void currentTableChange(const QString &);
    void currentSourceChange(const QString &);

private:
    void initUI(); //! 初始化界面

    /*!
    \section 数据源选择改变的处理函数
    */
private slots:
    void on_comDataSrc_indexChange(const QString &text);

private:
    IMDDataArchives *m_pDataArchives ; //! 数据归档

    QLabel *m_pLbDataSrc             ; //! 数据源标签
    QLabel *m_pLbDataTab             ; //! 数据表标签
    QComboBox *m_pComDataSrc         ; //! 数据源下拉框
    QComboBox *m_pComDataTab         ; //! 数据表下拉框
    QPushButton *m_pBtnOk            ; //! 确定按钮
    QPushButton *m_pBtnExit          ; //! 退出按钮

    QGridLayout *m_pMainLayout       ; //! 网络布局
};

#endif // DATASELECTUI_H
