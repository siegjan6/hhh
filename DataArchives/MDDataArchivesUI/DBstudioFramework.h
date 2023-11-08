#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "BaseForm.h"
#include "MDDBConnect.h"
#include "Utility.h"
#include "DBTableDialog.h"
#include "DbTableFieldDialog.h"

/*!

\author bHVvY2hhbw==

\section 功能
1.负责数据源与数据源参数窗体呈现的MDI主窗体。

*/

namespace Ui {
class DBstudioFramework;
}

QT_BEGIN_NAMESPACE
class QListWidget;
class QListWidgetItem;
class QStackedWidget;
QT_END_NAMESPACE

class CDataArchivesUI;
class IMDDataSourceManager;
class IMDDataSource;
class IMDDataTable;
class CMDDataSrcTabWidget;
class CDBSourceParamWidget;
class IMDDataArchives;

class CDBstudioFramework : public CBaseForm
{
    Q_OBJECT

public:
    explicit CDBstudioFramework(QWidget *parent = 0);
    ~CDBstudioFramework();

    /*!
    \section 派生实现保存功能
    */
public:
    virtual bool save();

    /*!
    \section 配置属性，初始化属性
    */
public:
    inline void setDataArchivesUI(CDataArchivesUI *pDataArchivesUI){m_pDataArchivesUI = pDataArchivesUI;}
    inline void setDataSourceManager(IMDDataSourceManager *pManager){m_pDataSourceManager = pManager;}
    void initUI(IMDDataArchives *pDataArchives, IMDDataSource *pDataSource);
    void initUI(IMDDataArchives *pDataArchives, IMDDataTable *pDataTable, IMDDataSource *pDataSource);

    void changeLanguage(); //! 语言切换

    /*!
    \section 检查存储
    */
private:
    void GotoError();
    bool CheckError(QString &strError);

    /*!
    \section 派生实现窗体关闭
    */
public:
    void closeEvent(QCloseEvent *e);

private:
    Ui::DBstudioFramework *ui                        ; //! 数据源主窗体
    CDataArchivesUI *m_pDataArchivesUI               ; //! 数据归档窗体
    IMDDataSourceManager *m_pDataSourceManager       ; //! 数据源管理器
    CDBSourceParamWidget *m_pDataSourceParamWidget   ; //! 数据源参数配置窗体
    CMDDataSrcTabWidget *m_pDataSourceTableWidget    ; //! 数据源窗体
    int m_iParamType                                 ;    //! 负责参数窗体与数据源窗体切换标识
};

#endif // MAINWINDOW_H
