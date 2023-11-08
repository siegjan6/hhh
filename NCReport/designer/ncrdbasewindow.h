/****************************************************************************
*
*  Copyright (C) 2002-2008 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo@helta.hu, info@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo@helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QMainWindow>
#include "ncrd_global.h"
#include "BaseForm.h"
#include "MDProjectConfiger.h"
#include "MDMultiLanguage.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QUndoGroup;
class QSignalMapper;
class QToolButton;
QT_END_NAMESPACE

/*!
NCReport Designer base main window class
*/

/*!

\section 功能

报表设计器主界面基类
【管理报表设计器的菜单栏／工具栏／主工作区域／状态栏等，
　继承自ＮetSCADA项目CBaseForm基类，目的是将报表设计器界面嵌入到MindSCADA编辑时主界面工作区域的中】。

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDBaseWindow : public CBaseForm
{
    Q_OBJECT
public:
    NCRDBaseWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    ~NCRDBaseWindow();

    QUndoGroup* undoGroup() const;
    QAction* actionGraph() { return act_graph; }

    QToolBar *toolBarFile() {return tbFile; }
    QToolBar *toolBarReport() {return tbReport; }
    QToolBar *toolBarConnection() {return tbConnection; }
    QToolBar *toolBarEdit() {return tbEdit; }
    QToolBar *toolBarTools() {return tbTools; }
    QToolBar *toolBarAlign() {return tbAlign; }
    QToolBar *toolBarTextProperties() {return tbTextProps; }
    QToolBar *toolBarShapeProperties() {return tbShapeProps; }
    QToolBar *toolBarDimProperties() {return tbDimProps; }

    QMenu* menuFile() {return mnFile;}
    QMenu* menuReport() {return mnReport;}
    QMenu* menuEdit() {return mnEdit;}
    QMenu* menuView() {return mnView;}
    QMenu* menuTools() {return mnTools;}
    QMenu* menuFormat() {return mnFormat;}
    QMenu* menuAlign() {return mnAlign;}
    QMenu* menuHelp() {return mnHelp;}
    QMenu* menuWindow() {return mnWindow;}
    QMenu* menuConn() {return mnConn;}
    QMenu* menuRecent() {return mnRecent;}
    QMenu* menuLanguage() {return mnLang;}
    QMenu* menuRun() {return mnRun;}

    void addCustomToolAction(int typeID , QAction* action, QToolBar *toolBar=0, QMenu* menu=0);
    void addGraphItemAction(int typeID , QAction* action, QToolBar *toolBar=0, QMenu* menu=0);
public:
    virtual CMDProjectConfiger *projectConfiger(){return m_pProjectConfiger;}
    virtual void setProjectConfiger(CMDProjectConfiger *pProjectConfiger){m_pProjectConfiger = pProjectConfiger;}
signals:
    void currentToolChanged();
    void aboutToClose();

public slots:
    virtual void activate_Arrow() {}
    virtual void activate_Label() {}
    virtual void activate_Text() {}
    virtual void activate_Field() {}
    virtual void activate_Line() {}
    virtual void activate_Rect() {}
    virtual void activate_Ellipse() {}
    virtual void activate_Img() {}
    virtual void activate_Barcode() {}
    virtual void activate_Graph() {}
    virtual void activate_CrossTab() {}
    virtual void activate_Chart() {}
    virtual void activate_Table() {}
    virtual void activateCustomAction(int typeID);

    //virtual void newFile() {}
    //virtual void open(const QString& = QString()) {}
    virtual void openDB() {}
    virtual bool save() { return false; }
    virtual bool saveAs() { return false; }
    virtual bool saveAs( const QString& ) { return false; }
    virtual void saveClipboard() {}
    virtual void showSource() {}
    virtual void about() {}

    virtual void detailsDialog() {}
    virtual void pageSettingsDialog() {}
    virtual void variablesDialog() {}
    virtual void dataSourcesDialog() {}


    virtual void optionsDialog() {}
    virtual void editItem() {}
    virtual void editSection() {}
    virtual void languageChangeByAction( QAction* ) {}

    virtual void cut() {}
    virtual void copy() {}
    virtual void paste() {}
    virtual void undo() {}
    virtual void redo() {}
    virtual void deleteObject() {}
    virtual void zoomIn() {}
    virtual void zoomOut() {}
    virtual void zoomOne() {}
    virtual void lock() {}
    virtual void unlock() {}

    virtual void runReportDialog() {}
    virtual void runReportPrint() {}
    virtual void runReportPreview() {}
    virtual void runReportQtPreview() {}
    virtual void runReportPdf() {}
    virtual void runReportSvg() {}
    virtual void runReportText() {}
    virtual void runReportHtml() {}
    virtual void runReportExcelHtml() {}
    virtual void runReportImage() {}

    virtual void addTestDataSources() {}

protected slots:
    void setActiveSubWindow(QWidget *window);
    void switchLayoutDirection();
    void dockWindowVisible(bool);

protected slots:
    virtual void updateMenus() {}
    virtual void updateWindowMenu() {}

    virtual void alignFront() {}
    virtual void alignBack() {}
    virtual void alignForward() {}
    virtual void alignBackward() {}
    virtual void alignLeft() {}
    virtual void alignRight() {}
    virtual void alignHCenter() {}
    virtual void alignTop() {}
    virtual void alignBottom() {}
    virtual void alignVCenter() {}
    virtual void alignSameWidth() {}
    virtual void alignSameHeight() {}

    virtual void showGrid() {}
    virtual void snapToGrid() {}
    virtual void testFunction() {}
    virtual void showPropEdit() {}
    virtual void showDataSourceTree() {}
    virtual void connectDB() {}
    virtual void disconnectDB() {}
    virtual void testDataEditor() {}
    //void connectDatabase(const QString &);
    virtual void connectDatabase(bool) {}
    virtual void openRecentFile() {}
    virtual void editParameters() {}
    virtual void editScripts() {}
    virtual void setZones() {}

protected:
    virtual void createActions();
    virtual void createMenus();
    virtual void createToolBars();
    virtual void createStatusBar();

protected:
    virtual void createEditors() {}
    virtual void readSettings() {}
    virtual void writeSettings() {}
    virtual void updateRecentFileActions() {}

protected:
    QMdiArea *mdiArea;
    QSignalMapper *windowMapper;
    QSignalMapper *m_actionMapper;
    QUndoGroup *m_undoGroup;

    QMenu* mnFile;
    QMenu* mnReport;
    QMenu* mnEdit;
    QMenu* mnView;
    QMenu* mnTools;
    QMenu* mnFormat;
    QMenu* mnAlign;
    QMenu* mnHelp;
    QMenu* mnWindow;
    QMenu* mnConn;
    QMenu* mnRecent;
    QMenu* mnLang;
    QMenu* mnRun;

    QToolBar *tbFile;
    QToolBar *tbReport;
    QToolBar *tbConnection;
    QToolBar *tbEdit;
    QToolBar *tbTools;
    QToolBar *tbAlign;
    QToolBar *tbTextProps;
    QToolBar *tbShapeProps;
    QToolBar *tbDimProps;

    //QAction *act_new;
    //QAction *act_open;
    QAction *act_opendb;
    QAction *act_save;
    QAction *act_saveas;
    QAction *act_options;
    QAction *act_exit;
    QAction *act_saveClip;
    QAction *act_showSource;

    QAction *act_details;
    QAction *act_pagesettings;
    QAction *act_variables;
    QAction *act_datasources;

    //QAction *act_mindscadadatasources;

    QAction *act_sqleditor;
    QAction *act_connectdb;
    QAction *act_disconnectdb;
    QAction *act_testdata;
    QAction *act_run;
    QAction *act_runPreview;
    QAction *act_runQtPreview;
    QAction *act_runPrint;
    QAction *act_runPdf;
    QAction *act_runSvg;
    QAction *act_runText;
    QAction *act_runHtml;
    QAction *act_runExcelHtml;
    QAction *act_runImage;

    QAction *act_edititem;
    QAction *act_editsection;
    QAction *act_parameters;
    QAction *act_scripts;

    QAction *act_showgrid;
    QAction *act_snapgrid;
    QAction *act_zoomin;
    QAction *act_zoomout;
    QAction *act_zoom1;
    QAction *act_showpropedit;
    QAction *act_showdstree;

    QAction *act_copy;
    QAction *act_paste;
    QAction *act_cut;
    QAction *act_delete;
    QAction *act_undo;
    QAction *act_redo;
    QAction *act_lock;
    QAction *act_unlock;
    QAction *act_setzones;

    QAction *act_close;
    QAction *act_closeall;
    QAction *act_tile;
    QAction *act_cascade;
    QAction *act_next;
    QAction *act_previous;

    QAction *act_about;
    QAction *act_aboutqt;

    QAction *act_arrow;
    QAction *act_label;
    QAction *act_text;
    QAction *act_field;
    QAction *act_rect;
    QAction *act_line;
    QAction *act_ellipse;
    QAction *act_img;
    QAction *act_barcode;
    QAction *act_graph;
    QAction *act_chart;
    QAction *act_crosstab;
    QAction *act_table;

    QAction *act_front;
    QAction *act_back;
    QAction *act_forward;
    QAction *act_backward;
    QAction *act_left;
    QAction *act_right;
    QAction *act_hcenter;
    QAction *act_vcenter;
    QAction *act_top;
    QAction *act_bottom;
    QAction *act_sameheight;
    QAction *act_samewidth;

    QAction *act_separator;
    QAction *act_test;

    QAction *separatorAct;
    enum { MaxRecentFiles = 5 };
    QAction *recentFileActs[MaxRecentFiles];
    QActionGroup *langGroup;
    QActionGroup *toolGroup;

    QList<QAction*> m_customActions;
private:
    CMDProjectConfiger *m_pProjectConfiger;

public:
    void changeLanguage();
    ITranslateWidgetTool* m_transTool;
};

#endif
