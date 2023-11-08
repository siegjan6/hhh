#ifndef DATAARCHIVES_H
#define DATAARCHIVES_H

/*!

\author bHVvY2hhbw==

\section 功能
1.CMDDataArchivesUI类的内部实现类。
2.加载数据归档配置界面即对数据组与数据源的配置。
3.处理相应树结点事件的处理。
4.创建相应结点的右键菜单以及处理菜单事件。
5.加载数据源与数据组对应的结点进行树构建。

*/

#include <QObject>
#include <list>
#include <iostream>
#include <qmutex.h>
#include <QMenu>
#include <QAction>
#include <QtWidgets>
#include <QApplication>
#include <QtGlobal>
#include <QSettings>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDir>
#include <QFont>
#include <QMenu>
#include <QLabel>
#include <QAction>
#include <QComboBox>
#include <QToolBar>
#include <QDockWidget>
#include <QTreeWidget>
#include <QLocale>
#include <QTranslator>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenuBar>

//#include "IProjectDirector.h"

#include "DBstudioFramework.h"
#include "NewDBsourceDialog.h"
#include "DBTableDialog.h"

#include "IMDDataField.h"
#include "IMDDataGroup.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"

#include "IStudioInterface.h"

#include "MDSysLogExpert.h"
#define LOG_SOURCE "DataArchives"

const QString STRING_DATASOURCE = QObject::tr("数据源");
const QString STRING_DATAGROUP = QObject::tr("表模板");
const QString STRING_HISGRP = QObject::tr("自定义");

const QString STRING_NEW = QObject::tr("新建");
const QString STRING_DELETE = QObject::tr("删除");
const QString STRING_PROPERTY = QObject::tr("属性");

const QString STRING_NEW_DATASRC = QObject::tr("新建数据源");
const QString STRING_DEL_DATASRC = QObject::tr("删除数据源");
const QString STRING_DATASRC_PROPERTY = QObject::tr("数据源属性");

const QString STRING_NEW_TABLE = QObject::tr("新建表");
const QString STRING_DEL_TABLE = QObject::tr("删除表");
const QString STRING_TAB_PROPERTY = QObject::tr("表属性");

using namespace std;

class QTreeWidget;
class QTreeWidgetItem;

class CDataGroupFramework;
class IMDDataArchives;
class IMDRTDBManagerUI;
class ITranslateWidgetTool;

Q_DECLARE_METATYPE(IMDDataSource*)
Q_DECLARE_METATYPE(IMDDataTable*)
Q_DECLARE_METATYPE(IMDDataField*)
Q_DECLARE_METATYPE(IMDDataGroup*)

class CDataArchivesUI: public QObject
{
    Q_OBJECT
public:
    CDataArchivesUI();
    virtual ~CDataArchivesUI();

    /*!
    \section 设置属性
    */
public: 
    QTreeWidgetItem *m_rootInStudio ;// NULL;
    IStudioInterface* m_studioInterface;
    //! 语言切换
    void changeLanguage();

    /*!
    \section 结点事件处理
    */
public:
    void onMouseRClick(const QPoint &pos); //! 鼠标点击菜单执行事件
    void onMouseLClick(QTreeWidgetItem *pItem,int column); //! 鼠标单击事件
    void onMouseDbClick(QTreeWidgetItem *pItem,int column); //! 鼠标双击事件

    /*!
    \section MDI窗体关闭清理工作
    */
public:
    void closeEvent(CBaseForm *pForm);

    /*!
    \section 加载卸载数据归档界面
    */
public:
    void load();
    void unLoad();

    /*!
    \section 相应菜单响应处理函数
    */
private slots:
    /*!
    \section 历史组菜单响应
    */
    void slotitemActiveNewHisGroup();
    void slotitemActiveDeleteHisGroup();
    void slotitemActiveDataGroup();
    /*!
    \section 数据源菜单响应
    */
    void slotitemActiveNewDBSource();
    void slotitemActiveDeleteDBSource();
    void slotItemActiveDataSource();
    /*!
    \section 数据源表菜单响应
    */
    void slotitemActiveNewDBSourceTable();
    void slotitemActiveDeleteDBSourceTable();

    /*!
    \section 构建相应的树结点
    */
private:
    void initData();
    void initLanguage();
    void buildAlmTree(QTreeWidgetItem *pTreeRoot); //! 构建报警配置结点
    void buildHisTree(QTreeWidgetItem *pTreeRoot); //! 构建历史配置结点
    void buildLogTree(QTreeWidgetItem *pTreeRoot); //! 构建日志配置结点
    void buildDBTree(); //! 构建数据源结点
    void buildDBDataGroupTree(); //! 构建数据组结点

    //! hItem 節點是否为htrDestParent子结点?(包括自己本身)
    bool isItemInChildren(QTreeWidgetItem *hItem, QTreeWidgetItem *htrDestParent);

    /*!
    \section 显示菜单
    */
private:
    void ShowHisGroupMenu(const QPoint &pos);
    void ShowDBMenu(const QPoint &pos);

    //! 当创建数据组时，且数据源下的表不可见时，创建相应的表
    void createDataGrpCheckDataSrc(IMDDataGroup *pDataGrp);

    //! 切换语言刷新
    void refreshText();

    /*!
    \section 加载对应的树结点
    */
private:
    void LoadHisGroup(QTreeWidgetItem *pRoot);
    void LoadAlmNotfy(QTreeWidgetItem *pRoot);
    void LoadLogTable(QTreeWidgetItem *pRoot);
    void LoadAlmOrLogNote(QTreeWidgetItem *pRoot, bool isAlm);
    void LoadDBTree(QTreeWidgetItem *pRoot);
    void LoadDBDataGroupTree(QTreeWidgetItem *pRoot);

private:
    QTreeWidgetItem *m_AlmFieldRoot                     ;  //! 报警组根结点
    QTreeWidgetItem *m_HisGroupRoot                     ;  //! 历史组根结点
    QTreeWidgetItem *m_LogFieldItem                     ;  //! 日志根结点
    QTreeWidgetItem *m_DBStudioRoot                     ;  //! 数据源根结点
    QTreeWidgetItem *m_DBDataGroupRoot                  ;  //! 数据组根结点

    /*!
    \section 历史组菜单
    */
    QMenu *m_pHisGroupPopupMenu                         ;  //! 历史组菜单
    QAction *m_pHisGroup_NewAction                      ;  //! 历史组新建选项
    QAction *m_pHisGroup_DeleteAction                   ;  //! 历史组删除选项
    QAction *m_pHisGroup_PropertiesAction               ;  //! 历史组属性选项

    /*!
    \section 数据源菜单
    */
    QMenu *m_pDBSourcePopupMenu                         ;  //! 数据源菜单
    QAction *m_pDBSource_NewAction                      ;  //! 数据源新建选项
    QAction *m_pDBSource_DeleteAction                   ;  //! 数据源删除选项
    QAction *m_pDBSource_PropertiesAction               ;  //! 数据源属性选项

    /*!
    \section 数据表菜单
    */
    QAction *m_pDBSourceTable_NewAction                 ;  //! 数据表新建选项
    QAction *m_pDBSourceTable_DeleteAction              ;  //! 数据表删除选项
    QAction *m_pDBSourceTable_PropertiesAction          ;  //! 数据表属性选项

    CDBstudioFramework *m_pDBstudioFramework            ;  //! 数据源MDI主窗体
    CDataGroupFramework *m_pDataGrpFramework            ;  //! 数据组MDI主窗体
    IMDDataArchives *m_pDataArchives                    ;  //! 数据归档

    ITranslateWidgetTool *m_pMulLnaguageTool; //! 多语言翻译工具

    bool m_isOpenDataSrcFramwork                        ; //! 数据源MDI主窗体打开标识
    bool m_isOpenDataGrpFramwork                        ; //! 数据组MDI主窗体打开标识
};

#endif // DATAARCHIVES_H
