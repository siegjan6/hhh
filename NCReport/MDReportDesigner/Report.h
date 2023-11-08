/*!

\author bWVpeGw=

\section 功能

管理【报表设计器，用来实现新建／打开／关闭／删除报表等相关操作】。

*/

#ifndef REPORT_H
#define REPORT_H
#include <QtCore/qglobal.h>
#include <QObject>
#include <list>
#include <iostream>
#include <qmutex.h>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QAbstractItemDelegate>

#include "MDProjectConfiger.h"
#include "IStudioInterface.h"
#include "ncrdmainwindow.h"
#include "NewNCReportFileDialog.h"
#include "RenameNCReportFileDilaog.h"

#include "ITranslateWidgetTool.h"

class CMDProjectConfiger;
class QTreeWidget;
class QTreeWidgetItem;


class CReport: public QObject
{
    Q_OBJECT
public:
    CReport();
    virtual ~CReport();

/*!
\section 项目管理，报表树节点相关接口函数
*/
public:
    QTreeWidgetItem *m_rootInStudio ;// NULL;
    IStudioInterface* m_studioInterface;

    //! 报表目录
public:
    void setNCReportFilePath(const QString &NCReportFilePath);
    QString NCReportFilePath() const;
private:
    QString m_NCReportFilePath;

public:
    QTreeWidget *treeWidget(); //! 获取树管理器指针
    QTreeWidgetItem *treeRoot(); //! 获取树根节点指针
    void load(); //! 加载报表树节点
    void unLoad(); //! 卸载报表树节点

/*!
\section 事件处理函数
*/
public:
    void closeEvent(CBaseForm *pForm);

private:
    QTreeWidgetItem* selectNode();
    bool isReportNode(QTreeWidgetItem* node);
    void clearNode(QTreeWidgetItem* node);

/*!
\section 信号与槽函数，槽函数主要是响应右键菜单相关功能
*/
signals:

private slots:
    void onMouseRClick(const QPoint &pos);
    void onMouseDbClick();
    void slotitemActiveNewNCReportFile(); //! 新建报表槽函数
    void slotitemActiveOpenNCReportFile(); //! 打开报表槽函数
    void slotitemActiveDeleteNCReportFile(); //! 删除报表槽函数
    void slotitemActiveRenameNCReportFile(); //! 重命名槽函数
    void slotCloseNCReportWindow(); //! 关闭报表设计器窗体槽函数
//    void slotHandleSameXmlName(QTreeWidgetItem *item, int column); //! 处理报表重命名时文件名重名的槽函数

private:
    void buildReportDirTree(QTreeWidgetItem *pTreeRoot); //! 构建报表文件目录
    void LoadReportDirTree(QTreeWidgetItem *pRoot); //! 加载报表树节点
    QFileInfoList AddReportDirallfile(QTreeWidgetItem *root,QString path); //! 遍历查找报表目录下报表文件
    bool isItemInChildren(QTreeWidgetItem *hItem,QTreeWidgetItem *htrDestParent); //! 判断hItem节点是否为htrDestParent子节点?(包括自己本身)

    //!右键菜单相关函数
private:
    void ShowNCReportDirMenu(const QPoint &pos); //! 显示操作报表目录菜单
    void ShowNCReportFileMenu(const QPoint &pos); //! 显示操作报表文件菜单
    void NewNCRreportFile(); //! 报表目录节点下新建报表文件函数
    void OpenCurrentNCRreportFile(); //! 打开当前节点报表文件函数
    void DeleteCurrentNCRreportFile(); //! 删除当前节点报表文件函数
    void RenameCurrentNCRreportFile(); //! 重命名当前节点报表文件函数

private:
    /*!
     判断当前文件目录下是否含有与所要新建的报表文件名相同的文件

     \param    1. QString xmlName 需要新建的报表文件名
                      2. QString Path  报表文件的存储目录
     \returns   bool
     */
    bool hascurrentXMLFile(QString xmlName,QString Path);

private:
    QMenu *m_pNCReportDirPopupMenu;
    QAction *m_NCReportDir_NewAction;
    QMenu *m_pNCReportFilePopupMenu;
    QAction *m_pNCReportFile_OpenAction;
    QAction *m_pNCReportFile_DeleteAction;
    QAction *m_pNCReportFile_RenameAction;

private:
    NCRDMainWindow *m_pNCRDMainWindow;
    QTreeWidget *m_pTreeWidget;
    QTreeWidgetItem *m_ReportDirRoot;
    bool isexists_currentfile;
    QString m_oldname;
    QString m_newname;
    QFileInfoList m_filelist;
    QString sRootName;

//! 多语言
public:
    void changeLanguage();
private:
    ITranslateWidgetTool* m_transTool;
};

#endif // REPORT_H
