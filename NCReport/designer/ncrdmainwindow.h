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
#ifndef NCRDMAINWINDOW_H
#define NCRDMAINWINDOW_H

#include "ncrdbasewindow.h"
#include "ncrddocument.h"
#include "ncrdpreferences.h"
#include "ncrdconnection.h"
#include "ncreportitem.h"
#include "ncreportoutput.h"
#include "ncreportsource.h"
#include "MDProjectConfiger.h"
#include "MDMultiLanguage.h"

#include <QFont>
#include <QHash>
#include <QMap>

#define NCRDSETTINGS_PROJECT	"MindSCADA"
#define NCRDSETTINGS_DESIGNER	"ReportDesigner"

class NCRDDocument;
class NCRDPropertyManager;
class NCRDTextPropertyManager;
class NCRDShapePropertyManager;
class NCRDPropertyEditor;
class NCReport;
class NCRDDataSourceTreeDockWidget;
class NCRDDataSourceTree;
class NCRDGraphItemDialogFactory;
class NCRDItemWidgetFactory;

QT_BEGIN_NAMESPACE
class QProgressBar;
class QAction;
class QActionGroup;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QUndoGroup;
class QSignalMapper;
class QTreeWidgetItem;
class CReport;
class NCRDMDDataSourceDialog;
QT_END_NAMESPACE

/*!
NCReport Designer main window class
*/

/*!

\section 功能

报表设计器主界面管理类
【公有继承自NCRDBaseWindow基类，在NCRDMainWindow派生类中实现了NCRDBaseWindow基类相关虚函数，如工具栏和菜单栏相关action的操作等】

*/
class NCREPORTDESIGNER_SHARED_EXPORT NCRDMainWindow : public NCRDBaseWindow
{
    Q_OBJECT   

public:
    static NCRDMainWindow *getInstance();
    NCRDMainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
    void setWindowAtrribute(QWidget * parent = 0, Qt::WindowFlags flags = 0 );
//    void setcurLayout(int m_index);
    ~NCRDMainWindow();

    enum ToolState { Arrow=0, Tool };
    enum DataSourceTreeUpdateMode { All=0, DataSources, Parameters, SystemParameters };

    ToolState currentToolState() const;
    NCReportItem::ObjectType currentTool() const;
    int currentItemTypeID() const;


    //void setGrid( const QPoint &p );
    QPoint grid() const;
    bool isShowGrid() const;
    bool isSnapGrid() const;
    bool isPropEditor() const;
    NCRDPropertyManager* propertyManager( const QString& name );
    NCRDPropertyEditor* propertyEditor( );
    void setManagersEnabled( bool );
    void resetManagers( NCRDDocument* );
    QHash<QString,NCRDConnection>& sqlConnections();
    const QHash<QString,QVariant>& reportParameters() const;
    QStringList parameterIDList() const;
    QStringList& removedReportParameters();

    /*! Sets the defauld load query for DB sourced reports */
    //void setLoadQuery( const QString& );
    /*! Sets the defauld save query for DB sourced reports */
    //void setSaveQuery( const QString& );
    /*! Sets the default database connection ID for DB sourced reports */
    void setDefaultConnectionID( const QString& );
    QString defaultConnectionID() const;

    /*! Sets the default SQL table name for DB sourced reports */
    void setDefaultDBTable( const QString& );
    QString defaultDBTable() const;
    /*! Sets the default SQL column name for DB sourced reports */
    void setDefaultDBColumn( const QString& );
    QString defaultDBColumn() const;
    /*! Sets the default SQL key column name for DB sourced reports */
    void setDefaultDBKeyColumn( const QString& );
    QString defaultDBKeyColumn() const;
    /*! Sets the default SQL key value for DB sourced reports */
    void setDefaultDBKeyValue( const QString& );
    QString defaultDBKeyValue() const;

    QHash<QString,NCRDConnection>& dbConnections();

    bool connectDatabase( const QString&, bool );
    /*! Updates the toolbar/menu states by document. Often called by an active document */
    void updateMenus( NCRDDocument* doc );
    QFont defaultFont() const;
    void setCopyXML( const QString& );
    QString copyXML() const;
    QMenu* menuTools();
    void updateConnectionMenu();
    void updateLanguageMenu();
    QMenu* languageMenu();
    QActionGroup* languageActionGroup();
    virtual void runReport(NCReportOutput::OutputType, NCRDDocument* document =0 ,
                           const QStringList &batchReports = QStringList() ,
                           const QString &pageRanges = QString());
    void groupSettings( NCRDDocument *doc, NCRDSectionScene* section );
    void detailsDialog( NCRDDocument* doc, NCRDSectionScene *default_detail );

    void addCustomDataSource(NCReportDataSource *ds);
    const QHash<QString, NCReportDataSource *> customDataSources() const;
    //void addCustomDataSourcesToDocument(NCRDDocument *doc);

    void addItemModel(const QString &id, QAbstractItemModel *model);
    const QHash<QString,QAbstractItemModel*> itemModels() const;

    void addGraphItemRenderer(NCReportGraphRenderer *renderer);
    const QHash<QString,NCReportGraphRenderer*> graphItemRenderers() const;

    void addUserFunction( const QString &id, NCReportUserFunction *function );

    /*! Programatically open report by report source object */
    void open(const NCReportSource& source);

    NCRDDataSourceTree* dataSourceTree() const;

    void setGraphItemDialogFactory( NCRDGraphItemDialogFactory* factory );
    NCRDGraphItemDialogFactory *graphItemDialogFactory() const;
    void setFieldItemDialogWidgetFactory( NCRDItemWidgetFactory* factory );
    NCRDItemWidgetFactory *fieldItemDialogWidgetFactory() const;

    NCReportDef* dummyReportDef();
    const NCRDPreferences& preferences() const;
    const NCReport* report();

    QString zintPath() const;
    void setZintPath( const QString& dir );

    QList<NCRDDocument*> openedDocuments();


    CReport *getReport() const;
    void setReport(CReport *pReport);

    void setViewPath(const QString & viewPath); //数据视图绝对路径
public:
    void CloseAllSubWindow();
    NCRDDocument *activeDocument();
signals:
    void currentToolChanged();
    void connectClicked(const QString &);
    void aboutToClose();
    void sigCloseNCReportWindow();

public slots:
    void activate_Arrow();
    void activate_Label();
    void activate_Text();
    void activate_Field();
    void activate_Line();
    void activate_Rect();
    void activate_Ellipse();
    void activate_Img();
    void activate_Barcode();
    void activate_Graph();
    void activate_Chart();
    void activate_CrossTab();
    void activate_Table();
    void activateCustomAction(int typeID);

    //void newFile();
    //void open(const QString& fileName = QString());
    void openDB();
    bool save();
    bool saveAs();
    bool saveAs( const QString& fileName);
    void saveClipboard();
    void showSource();
    void about();

    void detailsDialog();
    void pageSettingsDialog();
    void variablesDialog();
    void dataSourcesDialog();
    void optionsDialog();
    void editItem();
    void editSection();
    void languageChangeByAction( QAction* );

    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    void deleteObject();
    void zoomIn();
    void zoomOut();
    void zoomOne();
    void lock();
    void unlock();

    void runReportDialog();
    void runReportPrint();
    void runReportPreview();
    void runReportQtPreview();
    void runReportPdf();
    void runReportSvg();
    void runReportText();
    void runReportHtml();
    void runReportExcelHtml();
    void runReportImage();

    void addTestDataSources();

    void slotnewNCReportFile(QString filename);
    void slotopenNCReportFile(QString fileName);
    bool slotdeleteNCReportFile(QString absolutefilePath,QString NCReportFilePath,QString currentitem_name);

protected slots:

    void updateMenus();
    void updateWindowMenu();

    virtual NCRDDocument *createDocument();
    void deleteDocument( NCRDDocument*);

    void alignFront();
    void alignBack();
    void alignForward();
    void alignBackward();
    void alignLeft();
    void alignRight();
    void alignHCenter();
    void alignTop();
    void alignBottom();
    void alignVCenter();
    void alignSameWidth();
    void alignSameHeight();

    void showGrid();
    void snapToGrid();
    void testFunction();
    void showPropEdit();
    void connectDB();
    void disconnectDB();
    void testDataEditor();
//    void connectDatabase(const QString &);
    void connectDatabase(bool);
    void openRecentFile();
    void editParameters();
    void editScripts();
    void showReportProgress();
    void hideReportProgress();
    void setZones();

    void dataSourceTreeDoubleClick(QTreeWidgetItem* item, int column);
    void errorMessageZintNotFound();

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual NCReport* createReportObject();
    virtual void createMenus();
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent( QDropEvent* event );
    virtual void customizeDialog( QDialog* dialog );

private:   
    void createActions();
    void createToolBars();
    void createEditors();
    void readSettings();
    void writeSettings();
    void activateTool(ToolState state, NCReportItem::ObjectType type, int typeID=0);

    void refreshActiveDocument();
    void refreshTools();
    void applyGridSettings();
    void disableUnfinishedFunctions();
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    void load(const NCReportSource & rs );
    void afterCreateDocument( NCRDDocument*);
    void applyPreferences( const NCRDPreferences & );
    QString genOutputFile( const QString & ext );


    QMdiSubWindow *findDocument( const NCReportSource &rs );

private:
    NCReportItem::ObjectType m_currentTool, m_lastTool;
    ToolState m_currentState, m_lastState;
    QProgressBar *m_progress;
    QToolButton *m_btnCancelReport;
    NCRDPropertyEditor *m_propEdit;
    NCRDDataSourceTreeDockWidget *m_dataSourceTreeDockWidget;
    NCRDGraphItemDialogFactory *m_graphItemDialogFactory;
    NCRDItemWidgetFactory *m_fieldItemDialogWidgetFactory;
    int m_currentItemTypeID;
    NCReport *m_report;
    NCReportDef *m_dummyReportDef;  // object for storage;

    NCRDPreferences m_preferences;
    QHash<QString,NCRDPropertyManager*> pManagers;
    QHash<QString,NCRDConnection> m_connections;

    QStringList m_removedParameters;
    QString m_defaultConnectionID;
    QString mDBTable, mDBColumn, mDBKeyColumn, mDBKeyValue;
    QString m_copyXML;
    QString m_lastDir, m_lastOutputDir;
    QString m_defaultDataSourceID;
    QString m_defaultVariableID;
    QString m_defaultParameterID;

    static NCRDMainWindow *m_pWinInstance;
//    NCRDMDDataSourceDialog *m_dialog;
    CReport *m_pReport;
    QString m_ViewPath; //数据视图绝对路径
public:
    void changeLanguage();
    ITranslateWidgetTool* m_transTool;
};

#endif
