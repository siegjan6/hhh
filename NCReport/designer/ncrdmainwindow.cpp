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
#include <QUndoGroup>
#include <QUndoStack>
#include <QSettings>
#include <QSignalMapper>
#include <QFileInfo>
#include <QProgressDialog>
#include <QBoxLayout>
#include <QProgressBar>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QUndoGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMimeData>
#include <QClipboard>
#include <QDebug>

#include "ncrdmainwindow.h"
#include "ncrd_global.h"
#include "ncrdtextpropertymanager.h"
#include "ncrdshapepropertymanager.h"
#include "ncrddetaildialog.h"
#include "ncrdpagedialog.h"
#include "ncrdvariabledialog.h"
#include "ncrddatasourcedialog.h"
#include "ncrdpropertyeditor.h"
#include "ncrdconnectiondialog.h"
#include "ncrdrunnerdialog.h"
#include "ncrdopenfromdbdialog.h"
#include "ncrdtestdatawindow.h"
#include "ncrdpreferencesdialog.h"
#include "ncrdlanguage.h"
#include "ncrdparametereditor.h"
#include "ui_parameterdialog.h"
#include "ncrdconnectiondialog.h"
#include "ncrdgroupdialog.h"
#include "ncrdsetzonedialog.h"
#include "ncrddef.h"
#include "ncrdtextoutputtemplateselect.h"
#include "ncrddatasourcetreedockwidget.h"
#include "ncrddatasourcetree.h"
#include "ncrddemodata.h"
#include "ncrdscriptdialog.h"

#include "ncreport.h"
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewwindow.h"
#include "ncreportgroup.h"
#include "ncreportevaluator.h"
#include "ncreportgraphrenderer.h"



#include "ncrddocument.h"
#include "ncrdpreferences.h"
#include "ncrdconnection.h"
#include "ncreportitem.h"
#include "ncreportoutput.h"
#include "ncreportsource.h"

#include "Report.h"



#define NCRD_PROGRESS_BAR
#define NCRD_PROGRESS_STATUSBAR

NCRDMainWindow *NCRDMainWindow::m_pWinInstance = NULL;

NCRDMainWindow::NCRDMainWindow( QWidget * parent, Qt::WindowFlags flags ) : NCRDBaseWindow( parent, flags ),
    m_currentTool(NCReportItem::NoObject),
    m_lastTool(NCReportItem::NoObject),
    m_currentState(Arrow),
    m_lastState(Arrow),
    m_progress(0),
    m_btnCancelReport(0),
    m_propEdit(0),
    m_dataSourceTreeDockWidget(0),
    m_graphItemDialogFactory(0),
    m_fieldItemDialogWidgetFactory(0),
    m_currentItemTypeID(0),
    m_report(0),
    m_dummyReportDef(new NCReportDef(this))
{
    m_pReport = NULL;
    // PROPERTY MANAGERS
    pManagers.insert("pmtxt", new NCRDTextPropertyManager( this ));
    pManagers.insert("pmshape", new NCRDShapePropertyManager( this ));
    langGroup =0;   //protected variable
    //    setWindowTitle(QString("%1 %2 %3").arg(REPORTDESIGNER_NAME).arg(REPORTDESIGNER_VERSION).arg(NCREPORTAPP_RELEASENAME));
    //    setWindowTitle(QString("%1 %2").arg(NCRDSETTINGS_PROJECT).arg(NCRDSETTINGS_DESIGNER));
    setWindowTitle(QString(tr("报表设计器")));
    createEditors();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();
    readSettings();
    updateConnectionMenu();

    //    m_dialog = new NCRDMDDataSourceDialog(doc,this);

    dataSourceTree()->updateParameters(m_dummyReportDef);

    activate_Arrow();



    m_transTool = CMDMultiLanguage::instance()->createWidgetTool();
    m_transTool->init(this);
    m_transTool->changeLanguage();
}

NCRDMainWindow *NCRDMainWindow::getInstance()
{
    if (NULL == m_pWinInstance)
    {
        m_pWinInstance = new NCRDMainWindow;
    }

    return m_pWinInstance;
}

void NCRDMainWindow::setWindowAtrribute(QWidget *parent, Qt::WindowFlags flags)
{
    setParent(parent);
    setWindowFlags(flags);
}

NCRDMainWindow::~NCRDMainWindow()
{
    //this->deleteLater();
}
void NCRDMainWindow::CloseAllSubWindow()
{
    mdiArea->closeAllSubWindows();
}

void NCRDMainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();

    if (activeDocument()) {
        event->ignore();
    } else {
        writeSettings();
        event->accept();
        emit aboutToClose();
    }

    CBaseForm::closeEvent(event);

    CReport *pReport = getReport();
    if(pReport)
    {
        //emit sigCloseNCReportWindow();
        pReport->closeEvent(this);
    }
    //    m_pWinInstance = NULL;
}

/*
void NCRDMainWindow::newFile()
{
    NCRDDocument *child = createDocument();
    afterCreateDocument( child );

    child->newFile( m_preferences.reportDir+"/"+tr("report%1.ncr") );
    if ( m_preferences.showMaximized )
        child->showMaximized();
    else
        child->show();
    zoomOne();

    dataSourceTree()->setErrorHandler(child->reportDef()->error());
    child->updateDataSourceTree(dataSourceTree(), true,true,false,false);
}*/

/*
void NCRDMainWindow::open( const QString& fileName )
{
    NCReportSource rs;

    if ( m_lastDir.isEmpty() ) {
        NCRDDocument *doc = activeDocument();
        if ( doc ) {
            QFileInfo fi( doc->reportSource().fileName() );
            if (fi.exists())
                m_lastDir = fi.path();
            else
                m_lastDir = m_preferences.reportDir;
        } else {
            m_lastDir = m_preferences.reportDir;
        }
    }
    if (fileName.isEmpty())
        rs.setFileName( QFileDialog::getOpenFileName(this, tr("Open report file"), m_lastDir, NCREPORTAPP_DIALOG_EXTENSIONS) );
    else
        rs.setFileName(fileName);

    if ( !rs.fileName().isEmpty() ) {
        load( rs );
        QFileInfo fi( rs.fileName() );
        m_lastDir = fi.path();
    }
}
*/

void NCRDMainWindow::openDB()
{
    NCRDOpenFromDBDialog dialog(this);
    customizeDialog(&dialog);
    switch (dialog.exec()) {
    case QDialog::Accepted: {
        NCReportSource rs = dialog.reportSource();
        connectDatabase( rs.connectionID(), true );
        load( rs );
        //SAVE
        m_defaultConnectionID = rs.connectionID();
        mDBColumn = rs.columnName();
        mDBKeyColumn = rs.keyColumnName();
        mDBKeyValue = rs.keyValue();
        mDBTable = rs.tableName();
        break;
    }
    case QDialog::Rejected:
        break;
    }
}

void NCRDMainWindow::load(const NCReportSource &rs )
{
    if ( rs.fileName().isEmpty() )
        return;

    QMdiSubWindow *existing = findDocument( rs );
    if (existing) {
        mdiArea->setActiveSubWindow(existing);
        return;
    }

    NCRDDocument *child = createDocument();

    if ( child->load( rs )) {

        afterCreateDocument( child );
        statusBar()->showMessage(tr("File loaded"), 2000);
        setCurrentFile( rs.fileName() );
        if ( m_preferences.showMaximized )
            child->showMaximized();
        else
            child->show();
        child->showMaximized();
        zoomOne();
    } else {
        //child->close();
        deleteDocument( child );
    }
}

void NCRDMainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        if ( action->data().toString().startsWith("DB:") )
            statusBar()->showMessage(tr("Cannot open report recently from database"), 2000);
        else {
            NCReportSource rs;
            rs.setFileName(action->data().toString());
            load( rs );
        }
    }
}

void NCRDMainWindow::setCurrentFile(const QString &fileName)
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();

    settings.setValue("recentFileList", files);
    updateRecentFileActions();
}

void NCRDMainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg((files[i]));
        recentFileActs[i]->setText(text);
        recentFileActs[i]->setData(files[i]);
        recentFileActs[i]->setVisible(true);
    }
    for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
        recentFileActs[j]->setVisible(false);

    //separatorAct->setVisible(numRecentFiles > 0);
}

bool NCRDMainWindow::save()
{
    NCRDDocument *doc = activeDocument();
    if (doc && doc->save()) {
        setCurrentFile( doc->reportSource().fileName() );
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }
    return false;
}

bool NCRDMainWindow::saveAs()
{
    NCRDDocument *doc = activeDocument();
    if (doc && doc->saveAs( QString::null )) {
        setCurrentFile( doc->reportSource().fileName() );
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }
    return false;
}

bool NCRDMainWindow::saveAs(const QString &fileName)
{
    NCRDDocument *doc = activeDocument();
    if (doc && doc->saveAs( fileName )) {
        setCurrentFile( fileName );
        statusBar()->showMessage(tr("File saved"), 2000);
        return true;
    }
    return false;
}

void NCRDMainWindow::saveClipboard()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        QApplication::clipboard()->setText(doc->saveToString());
}

void NCRDMainWindow::showSource()
{

}

void NCRDMainWindow::cut()
{
    if (activeDocument())
        activeDocument()->cut();
}

void NCRDMainWindow::copy()
{
    if (activeDocument())
        activeDocument()->copy();
}

void NCRDMainWindow::paste()
{
    if (activeDocument())
        activeDocument()->paste();
}

void NCRDMainWindow::lock()
{
    if (activeDocument()) activeDocument()->lock_unlock(true);
}

void NCRDMainWindow::unlock()
{
    if (activeDocument()) activeDocument()->lock_unlock(false);
}

void NCRDMainWindow::setZones()
{
    NCRDDocument *doc = activeDocument();
    if ( doc && doc->currentScene() && !doc->currentScene()->selectedItems().isEmpty() ) {
        NCRDSetZoneDialog dialog(this);
        customizeDialog(&dialog);
        if (dialog.exec()==QDialog::Accepted)
            doc->setZoneValue( dialog.zoneID());
    }
}

void NCRDMainWindow::about()
{
    QMessageBox::about(this, tr("About"),
        QString("<h3>")+ REPORTDESIGNER_NAME +" "+ REPORTDESIGNER_VERSION + " " + NCREPORTAPP_RELEASENAME + "</h3>"
                    "<p>Application for designing NCReport files</p>"
                    "<h3>" NCREPORTAPP_ORG "</h3>"
                    "<a href=\"" NCREPORTAPP_WEB "\">" NCREPORTAPP_WEB "</a>"
                    "<p>"+ REPORTDESIGNER_COPYRIGHT +"</p>"
                       );
}

void NCRDMainWindow::updateMenus()
{
    NCRDDocument *doc = activeDocument();
    bool hasDocument = (doc != 0);
    bool hasSelection = false; //(activeDocument() && activeDocument()->textCursor().hasSelection());

    if ( hasDocument )
        act_save->setEnabled(doc->isModified());
    else
        act_save->setEnabled(hasDocument);

    act_saveas->setEnabled(hasDocument);

    act_pagesettings->setEnabled(hasDocument);
    act_datasources->setEnabled(hasDocument);
    act_details->setEnabled(hasDocument);
    act_variables->setEnabled(hasDocument);
    act_scripts->setEnabled(hasDocument);
    act_run->setEnabled(hasDocument);
    act_runPreview->setEnabled(hasDocument);
    act_runQtPreview->setEnabled(hasDocument);
    act_runPdf->setEnabled(hasDocument);
    act_runSvg->setEnabled(hasDocument);
    act_runText->setEnabled(hasDocument);
    act_runPrint->setEnabled(hasDocument);
    act_edititem->setEnabled(hasDocument);
    act_editsection->setEnabled(hasDocument);

    act_paste->setEnabled(hasDocument);
    act_cut->setEnabled(hasSelection);
    act_copy->setEnabled(hasSelection);

    act_close->setEnabled(hasDocument);
    act_closeall->setEnabled(hasDocument);
    act_tile->setEnabled(hasDocument);
    act_cascade->setEnabled(hasDocument);
    act_next->setEnabled(hasDocument);
    act_previous->setEnabled(hasDocument);
    act_separator->setVisible(hasDocument);

    updateLanguageMenu();
    mnLang->setEnabled(hasDocument);
    mnRun->setEnabled(hasDocument);

    refreshTools();
}

void NCRDMainWindow::updateMenus(NCRDDocument * doc)
{
    // save button
    act_save->setEnabled(doc->isModified());
}

void NCRDMainWindow::updateWindowMenu()
{
    mnWindow->clear();
    mnWindow->addAction(act_close);
    mnWindow->addAction(act_closeall);
    mnWindow->addSeparator();
    mnWindow->addAction(act_tile);
    mnWindow->addAction(act_cascade);
    mnWindow->addSeparator();
    mnWindow->addAction(act_next);
    mnWindow->addAction(act_previous);
    mnWindow->addAction(act_separator);

    QList<QMdiSubWindow *> windowsList = mdiArea->subWindowList();
    act_separator->setVisible(!windowsList.isEmpty());

    for (int i = 0; i < windowsList.size(); ++i) {
        NCRDDocument *child = qobject_cast<NCRDDocument *>(windowsList.at(i)->widget());

        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
        } else {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = mnWindow->addAction(text);
        action->setCheckable(true);
        action ->setChecked(child == activeDocument());
        connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
        windowMapper->setMapping(action, windowsList.at(i));
    }
}

void NCRDMainWindow::updateConnectionMenu()
{
    mnConn->clear();
    QHash<QString,NCRDConnection>::const_iterator i = m_connections.constBegin();
    while (i != m_connections.constEnd()) {
        QAction *action  = mnConn->addAction(i.key());
        action->setCheckable(true);
        action->setChecked( i.value().connected );
        connect(action, SIGNAL(triggered(bool)), this, SLOT(connectDatabase(bool)));
        ++i;
    }
}

void NCRDMainWindow::updateLanguageMenu()
{
    if ( activeDocument() )
        activeDocument()->updateLanguageMenu( mnLang, langGroup, this );
}


NCRDDocument *NCRDMainWindow::createDocument()
{
    NCRDDocument *child = new NCRDDocument( this );
    child->setViewPath(m_ViewPath);
    child->reportDef()->setTemplateDir(m_preferences.templateDir);
    //connect(child, SIGNAL(copyAvailable(bool)), act_cut, SLOT(setEnabled(bool)));
    //connect(child, SIGNAL(copyAvailable(bool)), act_copy, SLOT(setEnabled(bool)));

    return child;
}

void NCRDMainWindow::afterCreateDocument( NCRDDocument *doc )
{
    m_undoGroup->addStack(doc->undoStack());
    QMdiSubWindow * subwin = mdiArea->addSubWindow(doc);
    doc->setSubWindow( subwin );
    doc->setModified( false );
    subwin->resize(700,500);
    refreshTools();
}


void NCRDMainWindow::deleteDocument( NCRDDocument* doc )
{
    m_undoGroup->removeStack( doc->undoStack() );
    mdiArea->removeSubWindow(doc);
    delete doc;
}

void NCRDMainWindow::createActions()
{
    NCRDBaseWindow::createActions();
    act_showgrid->setChecked( m_preferences.gridVisible );

    act_showpropedit = m_propEdit->toggleViewAction();	// propEdit has to be existed already
    act_showpropedit->setText(tr("Geometry editor"));
    act_showpropedit->setStatusTip(tr("Show geometry editor"));

    act_showdstree = m_dataSourceTreeDockWidget->toggleViewAction();
    act_showdstree->setText(tr("Datasource manager"));
    act_showdstree->setStatusTip(tr("Enable/Disable Datasource manager"));
}

void NCRDMainWindow::createMenus()
{
    NCRDBaseWindow::createMenus();
    pManagers["pmtxt"]->buildMenu( mnFormat );

}

void NCRDMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/plain"))
        event->acceptProposedAction();
}

void NCRDMainWindow::dropEvent(QDropEvent *event)
{
    //open(event->mimeData()->text());
   // event->acceptProposedAction();
}

void NCRDMainWindow::customizeDialog(QDialog *dialog)
{
    Q_UNUSED(dialog)
}

NCRDDataSourceTree *NCRDMainWindow::dataSourceTree() const
{
    return m_dataSourceTreeDockWidget->tree();
}

void NCRDMainWindow::setGraphItemDialogFactory(NCRDGraphItemDialogFactory *factory)
{
    m_graphItemDialogFactory = factory;
}

NCRDGraphItemDialogFactory *NCRDMainWindow::graphItemDialogFactory() const
{
    return m_graphItemDialogFactory;
}

void NCRDMainWindow::setFieldItemDialogWidgetFactory(NCRDItemWidgetFactory *factory)
{
    m_fieldItemDialogWidgetFactory = factory;
}

NCRDItemWidgetFactory *NCRDMainWindow::fieldItemDialogWidgetFactory() const
{
    return m_fieldItemDialogWidgetFactory;
}

/*!
 * \brief NCRDMainWindow::dummyReportDef
 * \return
 * Report definition object for storage purposes only.
 */
NCReportDef *NCRDMainWindow::dummyReportDef()
{
    return m_dummyReportDef;
}

const NCRDPreferences &NCRDMainWindow::preferences() const
{
    return m_preferences;
}

const NCReport *NCRDMainWindow::report()
{
    return m_report;
}

void NCRDMainWindow::createToolBars()
{
    NCRDBaseWindow::createToolBars();
    pManagers["pmtxt"]->buildToolBar(tbTextProps);
    pManagers["pmshape"]->buildToolBar(tbShapeProps);
}

void NCRDMainWindow::readSettings()
{
    QSettings settings;

    m_preferences.read( settings );
    if (!m_preferences.zintDir.isEmpty())
        setZintPath(m_preferences.zintDir);

    //QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    //QSize size = settings.value("size", QSize(400, 400)).toSize();

    QByteArray state = settings.value("state", QByteArray() ).toByteArray();
    QByteArray geometry = settings.value("geometry", QByteArray() ).toByteArray();
    if ( geometry.isEmpty() ) {
        showMaximized();
    } else {
        restoreGeometry( geometry );
        restoreState( state );
    }

    int arraysize = settings.beginReadArray("connections");
    m_connections.clear();

    for (int i = 0; i < arraysize; ++i) {
        settings.setArrayIndex(i);
        NCRDConnection con;

        con.name = settings.value("name").toString();
        con.driver = settings.value("driver").toInt();
        con.host = settings.value("host").toString();
        con.database = settings.value("db").toString();
        con.user = settings.value("user").toString();
        con.password = settings.value("pwd").toString();
        con.options = settings.value("options").toString();

        if (m_preferences.encodePwd)
            con.password = QByteArray::fromBase64(con.password.toLatin1());

        con.port = settings.value("port").toInt();
        con.connected = false;

        m_connections.insert( con.name, con );
    }
    settings.endArray();

    settings.beginGroup("parameters");
    QStringList keys = settings.childKeys();
    dataSourceTree()->clearParameters();

    QStringList::const_iterator it;
    for (it = keys.constBegin(); it != keys.constEnd(); ++it) {
        m_dummyReportDef->addParameter(*it, settings.value(*it));
    }
    settings.endGroup();

    mDBColumn = settings.value("mDBColumn").toString();
    mDBKeyColumn = settings.value("mDBKeyColumn").toString();
    mDBKeyValue = settings.value("mDBKeyValue").toString();
    mDBTable = settings.value("mDBTable").toString();
    m_defaultConnectionID = settings.value("mDBConnectionID").toString();

    applyPreferences( m_preferences );
}

void NCRDMainWindow::writeSettings()
{
    QSettings settings;

    settings.setValue("geometry", saveGeometry() );
    settings.setValue("state", saveState() );

    //settings.setValue("pos", pos());
    //settings.setValue("size", size());
    settings.beginWriteArray("connections");

    QHash<QString,NCRDConnection>::const_iterator i = m_connections.constBegin();
    int index=0;
    while (i != m_connections.constEnd()) {
        settings.setArrayIndex(index);
        settings.setValue("name", i.value().name);
        settings.setValue("driver", i.value().driver);
        settings.setValue("host", i.value().host);
        settings.setValue("db", i.value().database);
        settings.setValue("user", i.value().user);
        settings.setValue("options", i.value().options);

        if (m_preferences.encodePwd) {
            QByteArray pwd(i.value().password.toLatin1());
            settings.setValue("pwd", pwd.toBase64());
        }
        else
            settings.setValue("pwd", i.value().password);

        settings.setValue("port", i.value().port);
        ++i;
        ++index;
    }
    settings.endArray();

    QHash<QString, QVariant>::const_iterator mi = m_dummyReportDef->parameters().constBegin();
    while (mi != m_dummyReportDef->parameters().constEnd()) {
        settings.setValue( "parameters/"+mi.key(), mi.value().toString() );
        ++mi;
    }
    for (int i = 0; i < m_removedParameters.size(); ++i)
        settings.remove( "parameters/"+m_removedParameters.at(i) );

    settings.setValue("mDBColumn",mDBColumn);
    settings.setValue("mDBKeyColumn",mDBKeyColumn);
    settings.setValue("mDBKeyValue",mDBKeyValue);
    settings.setValue("mDBTable",mDBTable);
    settings.setValue("mDBConnectionID",m_defaultConnectionID);
    settings.setValue("filecounter", m_preferences.fileCounter );

}

NCRDDocument *NCRDMainWindow::activeDocument()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<NCRDDocument *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *NCRDMainWindow::findDocument(const NCReportSource & rs )
{
    QString canonicalFilePath;

    if ( rs.sourceType() == NCReportSource::File ) {
        canonicalFilePath = QFileInfo(rs.fileName()).canonicalFilePath();
    } else if ( rs.sourceType() == NCReportSource::Database ) {
        canonicalFilePath = rs.fileName();
    }

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        NCRDDocument *mdiChild = qobject_cast<NCRDDocument *>(window->widget());
        if ( mdiChild ) {
            if (mdiChild->reportSource().fileName() == canonicalFilePath)
                return window;
        }
    }
    return 0;
}

void NCRDMainWindow::changeLanguage()
{
    if(m_transTool)
    {
        m_transTool->changeLanguage();
    }
}
CReport *NCRDMainWindow::getReport() const
{
    return m_pReport;
}

void NCRDMainWindow::setReport(CReport *pReport)
{
    m_pReport = pReport;
}

void NCRDMainWindow::setViewPath(const QString & viewPath)
{
    m_ViewPath = viewPath;
}

void NCRDMainWindow::undo()
{
}

void NCRDMainWindow::redo()
{
}

void NCRDMainWindow::deleteObject()
{
}

void NCRDMainWindow::zoomIn()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;

    doc->zoom( NCRDDocument::ZoomIn );
}

void NCRDMainWindow::zoomOut()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;

    doc->zoom( NCRDDocument::ZoomOut );
}

void NCRDMainWindow::zoomOne()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;

    doc->zoom( NCRDDocument::ZoomOne );
}

void NCRDMainWindow::alignFront()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->bringToFront();
}

void NCRDMainWindow::alignBack()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->moveToBack();
}

void NCRDMainWindow::alignForward()
{
}

void NCRDMainWindow::alignBackward()
{
}

void NCRDMainWindow::alignLeft()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignLeft();
}

void NCRDMainWindow::alignHCenter()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignHCenter();
}

void NCRDMainWindow::alignVCenter()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignVCenter();
}

void NCRDMainWindow::alignRight()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignRight();
}

void NCRDMainWindow::alignTop()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignTop();
}

void NCRDMainWindow::alignBottom()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignBottom();
}

void NCRDMainWindow::alignSameWidth()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignSameWidth();
}

void NCRDMainWindow::alignSameHeight()
{
    NCRDDocument *doc = activeDocument();
    if (doc)
        doc->alignSameHeight();
}


void NCRDMainWindow::showPropEdit()
{
    if ( m_propEdit->isVisible() ) {
        m_propEdit->hide();
    } else {
        m_propEdit->show();
        NCRDDocument *doc = activeDocument();
        if (doc)
            doc->initPropertyEditor();
    }
}

void NCRDMainWindow::showGrid()
{
    if ( m_preferences.gridVisible != act_showgrid->isChecked() ) {
        m_preferences.gridVisible = act_showgrid->isChecked();
        act_showgrid->setChecked( m_preferences.gridVisible );
        applyGridSettings();
    }
}

void NCRDMainWindow::snapToGrid()
{
    if ( m_preferences.snapToGrid != act_snapgrid->isChecked() ) {
        m_preferences.snapToGrid = act_snapgrid->isChecked();
        act_snapgrid->setChecked( m_preferences.snapToGrid );
        applyGridSettings();
    }
}

void NCRDMainWindow::activate_Arrow()
{
    activateTool(Arrow,NCReportItem::NoObject);
}

void NCRDMainWindow::activate_Label()
{
    activateTool(Tool, NCReportItem::Label);
}

void NCRDMainWindow::activate_Field()
{
    activateTool(Tool, NCReportItem::Field);
}

void NCRDMainWindow::activate_Rect()
{
    activateTool(Tool, NCReportItem::Rectangle);
}

void NCRDMainWindow::activate_Line()
{
    activateTool(Tool, NCReportItem::Line);
}

void NCRDMainWindow::activate_Img()
{
    activateTool(Tool, NCReportItem::Image);
}

void NCRDMainWindow::activate_Ellipse()
{
    activateTool(Tool, NCReportItem::Ellipse);
}

void NCRDMainWindow::activate_Barcode()
{
    activateTool(Tool, NCReportItem::Barcode);
}

void NCRDMainWindow::activate_Graph()
{
    activateTool(Tool, NCReportItem::Graph);
}

void NCRDMainWindow::activate_Chart()
{
    activateTool(Tool, NCReportItem::Chart);
}

void NCRDMainWindow::activate_CrossTab()
{
    activateTool(Tool,NCReportItem::CrossTab);
}

void NCRDMainWindow::activate_Table()
{
    activateTool(Tool,NCReportItem::Table);
}

void NCRDMainWindow::activateCustomAction(int typeID)
{
    //--------------------------
    // CUSTOM ACTION ID RANGES
    //--------------------------
    // 0-999      GRAPH
    // 1000-1999  FIELD

    if (typeID < 1000 ) // RANGE FOR CUSTOM GRAPHS
        activateTool(Tool, NCReportItem::Graph, typeID);
    else if(typeID < 2000 ) // RANGE FOR CUSTOM FIELDS
        activateTool(Tool, NCReportItem::Field, typeID);
}

void NCRDMainWindow::activate_Text()
{
    activateTool(Tool, NCReportItem::Text);
}

void NCRDMainWindow::activateTool(ToolState state, NCReportItem::ObjectType type, int typeID )
{
    m_currentState = state;
    m_currentTool = type;
    m_currentItemTypeID = typeID;

    if (state == Arrow && !act_arrow->isChecked())
        act_arrow->setChecked(true);

    refreshTools();
    refreshActiveDocument();

    if ( m_currentTool != m_lastTool )
        emit currentToolChanged();

    m_lastTool = m_currentTool;
    m_lastState = m_currentState;
}

void NCRDMainWindow::refreshTools()
{
    bool isActive = (activeDocument() != 0);

    m_undoGroup->setActiveStack( isActive ? activeDocument()->undoStack() : 0 );

    act_zoomin->setEnabled(isActive);
    act_zoomout->setEnabled(isActive);
    act_zoom1->setEnabled(isActive);

    act_front->setEnabled(isActive);
    act_back->setEnabled(isActive);
    act_forward->setEnabled(isActive);
    act_backward->setEnabled(isActive);

    act_left->setEnabled(isActive);
    act_right->setEnabled(isActive);
    act_hcenter->setEnabled(isActive);
    act_vcenter->setEnabled(isActive);
    act_top->setEnabled(isActive);
    act_bottom->setEnabled(isActive);
    act_samewidth->setEnabled(isActive);
    act_sameheight->setEnabled(isActive);

    act_copy->setEnabled(isActive);
    act_paste->setEnabled(isActive);
    act_cut->setEnabled(isActive);

    // TOOLS
    toolGroup->setEnabled(isActive);

    pManagers["pmtxt"]->setEnabled(isActive);
    disableUnfinishedFunctions();
}

NCRDMainWindow::ToolState NCRDMainWindow::currentToolState() const
{
    return m_currentState;
}
NCReportItem::ObjectType NCRDMainWindow::currentTool() const
{
    return m_currentTool;
}

int NCRDMainWindow::currentItemTypeID() const
{
    return m_currentItemTypeID;
}


// void NCRDMainWindow::setGrid( const QPoint &p )
// {
// 	if ( p == grd )
// 		return;
// 	grd = p;
// }

void NCRDMainWindow::testFunction()
{
    if ( !activeDocument() )
        return;
    activeDocument()->testFunction();
}

void NCRDMainWindow::refreshActiveDocument()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;

    doc->setToolMode();
}

NCRDPropertyManager * NCRDMainWindow::propertyManager(const QString & name)
{
    return pManagers[name];
}

void NCRDMainWindow::setManagersEnabled(bool set)
{
    QHash<QString,NCRDPropertyManager*>::const_iterator i = pManagers.constBegin();
    while (i != pManagers.constEnd()) {
        i.value()->setEnabled(set);
        ++i;
    }
}

void NCRDMainWindow::resetManagers(NCRDDocument* doc)
{
    QHash<QString,NCRDPropertyManager*>::const_iterator i = pManagers.constBegin();
    while (i != pManagers.constEnd()) {
        i.value()->setDocument( doc );
        i.value()->reset();
        ++i;
    }
}

void NCRDMainWindow::detailsDialog()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;
    detailsDialog(doc,0);
}

void NCRDMainWindow::detailsDialog(NCRDDocument* doc, NCRDSectionScene *default_detail )
{
    NCRDDetailDialog dialog(doc,default_detail,this);
    customizeDialog(&dialog);
    switch (dialog.exec()) {
        case QDialog::Accepted:
            dialog.applyChanges();
            break;
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}

/*!
  Calls Group setting dialog. This comes from context menu
  */
void NCRDMainWindow::groupSettings( NCRDDocument *doc, NCRDSectionScene* section )
{
    if ( !section->relatedGroup() )
        return;

    NCRDGroupDialog dialog( doc, section->relatedGroup()->detail(), this);
    dialog.setCurrentGroup(section->relatedGroup());
    customizeDialog(&dialog);
    switch (dialog.exec()) {
        case QDialog::Accepted:
            dialog.applyChanges();
            break;
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}


void NCRDMainWindow::pageSettingsDialog()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;
    NCRDPageDialog dialog(doc,this);
    customizeDialog(&dialog);
    switch (dialog.exec()) {
        case QDialog::Accepted: {
            dialog.applyChanges();
            break;
        }
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}

void NCRDMainWindow::variablesDialog()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;
    NCRDVariableDialog dialog(doc,this);
    customizeDialog(&dialog);
    dialog.setCurrentVariable(m_defaultVariableID);
    switch (dialog.exec()) {
        case QDialog::Accepted:
            dialog.applyChanges();
            break;
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}

void NCRDMainWindow::dataSourcesDialog()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;
    NCRDDataSourceDialog dialog(doc,this);
    dialog.setCurrentDataSource(m_defaultDataSourceID);
    customizeDialog(&dialog);
    switch (dialog.exec()) {
        case QDialog::Accepted:
            dialog.applyChanges();
            break;
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}


void NCRDMainWindow::optionsDialog()
{
    QSettings settings;

    NCRDPreferencesDialog dialog( this );
    dialog.load( m_preferences );
    customizeDialog(&dialog);

    switch (dialog.exec()) {
        case QDialog::Accepted:
        {
            dialog.save( m_preferences );

            applyPreferences( m_preferences );
            m_preferences.write( settings );

            break;
        }
        case QDialog::Rejected:
            break;
    }

}

void NCRDMainWindow::applyGridSettings()
{
/*	NCRDDocument *doc = activeDocument();
    if (doc)
        doc->updateGrid();*/

    QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
    for (int i = 0; i < windows.size(); ++i) {
        NCRDDocument *doc = qobject_cast<NCRDDocument *>(windows.at(i)->widget());
        if (doc)
            doc->updateGrid();
    }
}

NCRDPropertyEditor * NCRDMainWindow::propertyEditor()
{
    return m_propEdit;
}

void NCRDMainWindow::disableUnfinishedFunctions()
{
    //act_showdstree->setEnabled(false);
}

void NCRDMainWindow::connectDB()
{
    NCRDConnectionDialog dialog(this, this);
    customizeDialog(&dialog);
    switch (dialog.exec()) {
        case QDialog::Accepted:
            dialog.applyChanges();
            break;
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}

void NCRDMainWindow::disconnectDB()
{
}

void NCRDMainWindow::runReportDialog()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;
    if ( doc->isModified() ) {
        if (!save())
            return;
    }

    NCRDRunnerDialog dialog(this,doc,this);
    customizeDialog(&dialog);
    switch (dialog.exec()) {
        case QDialog::Accepted: {
            //dialog.save();
            dialog.runReport();
            break;
        }
        case QDialog::Rejected:
            //dialog.save();
            break;
    }
}

void NCRDMainWindow::runReportPreview()
{
    runReport( NCReportOutput::Preview );
}

void NCRDMainWindow::runReportQtPreview()
{
    runReport( NCReportOutput::QtPreview );
}

void NCRDMainWindow::runReportPrint()
{
    runReport( NCReportOutput::Printer );
}

void NCRDMainWindow::runReportPdf()
{
    runReport( NCReportOutput::Pdf );
}

void NCRDMainWindow::runReportSvg()
{
    runReport( NCReportOutput::Svg );
}

void NCRDMainWindow::runReportText()
{
    runReport( NCReportOutput::Text );
}

void NCRDMainWindow::runReportHtml()
{
    runReport( NCReportOutput::Html );
}

void NCRDMainWindow::runReportExcelHtml()
{
    runReport( NCReportOutput::ExcelHtml );
}

void NCRDMainWindow::runReportImage()
{
    runReport( NCReportOutput::Image );
}

QHash< QString, NCRDConnection > & NCRDMainWindow::sqlConnections()
{
    return m_connections;
}

const QHash<QString, QVariant> &NCRDMainWindow::reportParameters() const
{
    return m_dummyReportDef->parameters();
}

//void NCRDMainWindow::connectDatabase(const QString & cname )
void NCRDMainWindow::connectDatabase( bool on )
{
    QAction *action = qobject_cast<QAction*>(sender());
    if (!action)
        return;

    if ( !connectDatabase(action->text(), on ) )
        action->setChecked(false);
}

bool NCRDMainWindow::connectDatabase(const QString & cname, bool on )
{
    if ( !m_connections.contains(cname) ) {
        qDebug("Conn.name %s not found", qPrintable(cname) );
        return false;
    }

    NCRDConnection con = m_connections[cname];
    bool ok = true;
    QString msg;
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));

    if ( on ) {
        if ( con.connected ) {
            QApplication::restoreOverrideCursor();
            return true;
        }
        qDebug("Connect to database. Connection name = %s", qPrintable(con.name) );
        ok = NCRDConnectionDialog::connectDatabase( con, msg );
        m_connections[cname].connected = ok;

    } else {
        if ( !con.connected ) {
            QApplication::restoreOverrideCursor();
            return true;
        }
        NCRDConnectionDialog::disconnectDatabase( con.name );
        m_connections[cname].connected = false;
    }

    QApplication::restoreOverrideCursor();
    if ( !ok )
        QMessageBox::warning( this, tr("Connection error"), msg );

    return ok;
}


void NCRDMainWindow::runReport( NCReportOutput::OutputType out, NCRDDocument* document, const QStringList& batchReports, const QString& pageRanges )
{
    if ( !document ) {
        // CURRENT DOCUMENT
        document = activeDocument();
        if (!document)
            return;
    }
    if ( document->isModified() ) {
        if (!save())
            return;
    }

    //-----------------------------
    // GET TEXT OUTPUT TEMPLATE
    //-----------------------------
    QString tplFile;
    if (out==NCReportOutput::Text) {
        NCRDTextOutputTemplateSelect dia(this);
        if (dia.exec()==QDialog::Accepted && !dia.fileName().isEmpty() )
            tplFile = dia.fileName();
        else
            return;
    }

    //-------------------------------------
    // PRE CONDITIONS OUTPUT FILE DIALOGS
    //-------------------------------------
    bool toFile = (out == NCReportOutput::Pdf || out == NCReportOutput::Svg || out == NCReportOutput::Html || out == NCReportOutput::Text || out==NCReportOutput::Image );

    QString fileName;
    switch ( out ) {
    case NCReportOutput::Pdf:
        fileName = QFileDialog::getSaveFileName(this, tr("Save to PDF file"), genOutputFile("pdf"), tr("Pdf file (*.pdf)"));
        break;
    case NCReportOutput::Svg:
        fileName = QFileDialog::getSaveFileName(this, tr("Save to SVG file(s)"), genOutputFile("svg"), tr("Svg files (*.svg)"));
        break;
    case NCReportOutput::Image:
        fileName = QFileDialog::getSaveFileName(this, tr("Save to Image file(s)"), genOutputFile("jpg"), tr("Images (*.png *.jpg *.jpeg *.bmp *.ppm *.tiff)"));
        break;
    case NCReportOutput::Html:
        fileName = QFileDialog::getSaveFileName(this, tr("Save to HTML file"), genOutputFile("html"), tr("Html file (*.html)"));
        break;
    case NCReportOutput::ExcelHtml:
        fileName = QFileDialog::getSaveFileName(this, tr("Save to XLS file"), genOutputFile("xls"), tr("Xls file (*.xls *.html)"));
        break;
    case NCReportOutput::Text:
        fileName = QFileDialog::getSaveFileName(this, tr("Save to Text file"), genOutputFile("txt"), tr("Text file (*.txt *.html *.htm *.xml *.xls *.csv)"));
        break;
    default:
        break;
    }

    if (toFile) {
        if ( fileName.isEmpty() ) {
            return;
        } else {
            QFileInfo fi(fileName);
            m_lastOutputDir = fi.path();
        }
    }

    m_report = createReportObject();
    if ( !m_report )
        return;

    //-----------------------------
    // ADD PARAMETERS
    //-----------------------------
    QHash<QString, QVariant>::const_iterator i = m_dummyReportDef->parameters().constBegin();
    while (i != m_dummyReportDef->parameters().constEnd()) {
        bool ok = true;
        if ( i.value().toString() == "?" ) {
            m_report->getParameter( i.key(), QVariant(), this, tr("Get parameter"), i.key(), &ok );
            if (!ok) {
                delete m_report;
                m_report = 0;
                return;
            }
        } else {
            m_report->addParameter( i.key(), i.value() );
        }
        ++i;
    }
    //m_report->reportDef()->addParameters(m_dummyReportDef->parameters(),true);

    // CONNECT SIGNALS
    connect( m_report, SIGNAL(reportStarts()), this, SLOT(showReportProgress()) );
    connect( m_report, SIGNAL(reportEnds()), this, SLOT(hideReportProgress()) );
    //connect( m_report->reportDef(), SIGNAL(zintNotFound()), this, SLOT(errorMessageZintNotFound()) );
#ifdef NCRD_PROGRESS_STATUSBAR
    connect( m_report, SIGNAL(pageProgress(QString)), statusBar(), SLOT(showMessage(QString)) );
#endif

    m_report->setProcessEvents(true);
    //----------------------------
    // Add custom datasources
    //----------------------------
    m_report->reportDef()->addDataSources(m_dummyReportDef->dataSources());
    //----------------------------
    // ADD RENDERERS
    //----------------------------
    m_report->reportDef()->addGraphItemRenderers(m_dummyReportDef->itemRenderers());
    //----------------------------
    // ADD USER FUNCTIONS
    //----------------------------
    m_report->reportDef()->addUserFunctions(m_dummyReportDef->userFunctions());
    //----------------------------
    // SET ZINT PATH
    //----------------------------
    m_report->setZintPath(m_dummyReportDef->zintPath());

    //-----------------------------
    // SOME SETTINGS
    //-----------------------------
    m_report->setReportSource( document->reportSource() );
    m_report->setCurrentLanguage( document->currentLanguage() );

    if (m_preferences.printerResolution == 1)
        m_report->setPrintQuality( QPrinter::ScreenResolution );
    if (m_preferences.printerDPI > 0 )
        m_report->setPrintResolution( m_preferences.printerDPI );

    m_report->reportDef()->setTemplateDir(m_preferences.templateDir);
    m_report->setPageRanges( pageRanges );
    //-----------------------------
    // ADD ITEM MODELS
    //-----------------------------
    m_report->reportDef()->setItemModels(m_dummyReportDef->itemModels());

    //-----------------------------
    // ADD TEST DATA
    //-----------------------------
    NCRDTestDataWindow::generateTestData( NCRDTestDataWindow::defaultTestDataFile(), m_report->reportDef() );

#ifdef NCRD_STRINGLIST_DEMO
    m_report->addStringList( NCRDDemoData::createStringList1(), "sl0" );
    m_report->addStringList( NCRDDemoData::createStringList2(), "sl1" );
    m_report->addStringList( NCRDDemoData::createStringListPB(), "datenliste" );
#endif


    //-----------------------------
    // RUN REPORT
    //-----------------------------
    if (!batchReports.isEmpty()) {
        m_report->setReportBatch(batchReports);
    } else {
        m_report->setReportSource( document->reportSource() );
    }
    //-----------------------------
    // RUN REPORT
    //-----------------------------
    bool ok = true;

    if (ok) {
        switch ( out ) {
        case NCReportOutput::Preview:
            m_report->runReportToPreview();
            break;
        case NCReportOutput::QtPreview:
            m_report->runReportToQtPreview();
            break;
        case NCReportOutput::Printer: {
                if ( m_preferences.noPrinterDialog )
                    m_report->runReportToPrinterNoDialog(1,m_preferences.defaultPrinterName);
                else
                    m_report->runReportToPrinter( 1, true, this );
                break;
            }
        case NCReportOutput::Pdf:
            m_report->runReportToPDF( fileName );
            break;
        case NCReportOutput::Svg:
            m_report->runReportToSVG( fileName );
            break;
        case NCReportOutput::Image:
            m_report->runReportToImage( fileName );
            break;
        case NCReportOutput::Html:
            m_report->runReportToHTML( fileName, m_preferences.htmlOutputStrategy, m_preferences.htmlOutputCssFile );
            break;
        case NCReportOutput::ExcelHtml:
            m_report->runReportToExcelHtml( fileName );
            break;
        case NCReportOutput::Text:
            m_report->runReportToText( fileName, tplFile );
            break;
        default:
            break;
        }

        //-----------------------------
        // ERROR HANDLING
        //-----------------------------
        bool error = m_report->hasError();
        QString err = m_report->lastErrorMsg();

        if ( error )
            QMessageBox::information( this, tr("Report error"), err );
        else {
            if ( out == NCReportOutput::Preview ) {
                NCReportPreviewWindow *pv = new NCReportPreviewWindow();
                pv->setOutput( (NCReportPreviewOutput*)m_report->output() ); // report->output() exists (not deleted) when preview
                pv->setReport( m_report );
                if (pv->defaultOutputDir()==".")
                    pv->setDefaultOutputDir( m_preferences.outputDir );

                pv->setWindowModality( Qt::ApplicationModal );
                pv->setAttribute( Qt::WA_DeleteOnClose );
                pv->exec();
            }
        }
    }

    m_report->setCurrentLanguage("00");   // reset default locale
    delete m_report;
    m_report = 0;
}

void NCRDMainWindow::showReportProgress()
{
//#ifdef NCRD_PROGRESS_DIALOG
//	progress = new QProgressDialog( this );
//	progress->setLabelText( tr("Running report...") );
//	progress->setWindowModality(Qt::WindowModal);
//	connect( report, SIGNAL(dataRowCount(int)), progress, SLOT(setMaximum(int)) );
//	connect( report, SIGNAL(dataRowProgress(int)), progress, SLOT(setValue(int)) );
//	connect( report, SIGNAL(pageProgress(QString)), progress, SLOT(setLabelText(QString)) );
//	connect( progress, SIGNAL(canceled()), report, SLOT(cancel()) );
//#endif

#ifdef NCRD_PROGRESS_BAR
    if ( m_progress ) {
        m_progress->show();
    } else {
        m_progress = new QProgressBar;
        m_progress->setMaximumWidth(200);
        connect( m_report, SIGNAL(dataSourceOpen(QString)), statusBar(), SLOT(showMessage(QString)));
        connect( m_report, SIGNAL(dataRowCount(int)), m_progress, SLOT(setMaximum(int)) );
        connect( m_report, SIGNAL(dataRowProgress(int)), m_progress, SLOT(setValue(int)) );
        //connect( report, SIGNAL(pageProgress(QString)), progress, SLOT(setLabelText(QString)) );
        statusBar()->addPermanentWidget(m_progress);
    }
    m_progress->reset();

    if ( m_btnCancelReport ) {
        m_btnCancelReport->show();
    } else {
        m_btnCancelReport = new QToolButton;
        m_btnCancelReport->setText( tr("Cancel") );
        connect( m_btnCancelReport, SIGNAL(clicked()), m_report, SLOT(cancel()) );
        statusBar()->addPermanentWidget(m_btnCancelReport);
    }
#endif
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
}

void NCRDMainWindow::hideReportProgress()
{
//#ifdef NCRD_PROGRESS_DIALOG
//	delete progress;
//#endif
#ifdef NCRD_PROGRESS_BAR
    if ( m_progress ) {
        m_progress->setValue( m_progress->maximum() );
        m_progress->hide();
    }
    if ( m_btnCancelReport ) {
        m_btnCancelReport->hide();
    }
#endif
#ifdef NCRD_PROGRESS_STATUSBAR
    statusBar()->showMessage( tr("Report runtime: %1 ms").arg( m_report->runTimeElapsed() ) );
#endif
    QApplication::restoreOverrideCursor();
}

/*!
 * \brief NCRDMainWindow::createReportObject
 * \return
 * VIRTUAL METHOD FOR CREATE REPORT OBJECT
 */
NCReport *NCRDMainWindow::createReportObject()
{
    // Use a timer singleshot to make it possible to re-implement the
    // createReportObject member function in a sub-class.
    // QTimer::singleShot( 0, this, SLOT( createReportObject()) );
    return new NCReport(this);
}

void NCRDMainWindow::dataSourceTreeDoubleClick(QTreeWidgetItem *item, int column)
{
    if (column>0)
        return;

    switch (item->type()) {
    case NCRDDataSourceTree::DataSource:
    {
        m_defaultDataSourceID = item->text(0);
        dataSourcesDialog();
        break;
    }
    case NCRDDataSourceTree::Variable:
    {
        m_defaultVariableID = item->text(0);
        variablesDialog();
        break;
    }
    case NCRDDataSourceTree::Parameter:
    {
        m_defaultParameterID = item->text(0);
        editParameters();
    }

    }
}

void NCRDMainWindow::errorMessageZintNotFound()
{
    QMessageBox::warning(this, tr("Barcode error"), tr("Zint executable is not found."));
}


QStringList NCRDMainWindow::parameterIDList() const
{
    return m_dummyReportDef->parameters().keys();
}


QStringList & NCRDMainWindow::removedReportParameters()
{
    return m_removedParameters;
}

QString NCRDMainWindow::strippedName(const QString & fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void NCRDMainWindow::setDefaultConnectionID(const QString & id )
{
    m_defaultConnectionID = id;
}

QString NCRDMainWindow::defaultConnectionID() const
{
    return m_defaultConnectionID;
}

QHash< QString, NCRDConnection > & NCRDMainWindow::dbConnections()
{
    return m_connections;
}

void NCRDMainWindow::setDefaultDBTable(const QString & name)
{
    mDBTable = name;
}

void NCRDMainWindow::setDefaultDBColumn(const QString & name)
{
    mDBColumn = name;
}

void NCRDMainWindow::setDefaultDBKeyColumn(const QString & name)
{
    mDBKeyColumn = name;
}

void NCRDMainWindow::setDefaultDBKeyValue(const QString & value)
{
    mDBKeyValue = value;
}

QString NCRDMainWindow::defaultDBTable() const
{
    return mDBTable;
}

QString NCRDMainWindow::defaultDBColumn() const
{
    return mDBColumn;
}

QString NCRDMainWindow::defaultDBKeyColumn() const
{
    return mDBKeyColumn;
}

QString NCRDMainWindow::defaultDBKeyValue() const
{
    return mDBKeyValue;
}

void NCRDMainWindow::testDataEditor()
{
    NCRDTestDataWindow *w = new NCRDTestDataWindow();
    w->setAttribute( Qt::WA_DeleteOnClose );
    w->show();
}

void NCRDMainWindow::applyPreferences(const NCRDPreferences & pref )
{
    //setGrid( QPoint(pref.gridX,pref.gridY) );
    act_showgrid->setChecked( pref.gridVisible );
    act_snapgrid->setChecked( pref.snapToGrid );
    applyGridSettings();
}


QFont NCRDMainWindow::defaultFont() const
{
    return QFont(m_preferences.fontFamily, m_preferences.fontPointSize);
}

void NCRDMainWindow::createEditors()
{
    m_propEdit = new NCRDPropertyEditor( this );
    addDockWidget(Qt::RightDockWidgetArea, m_propEdit );
    m_dataSourceTreeDockWidget = new NCRDDataSourceTreeDockWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, m_dataSourceTreeDockWidget);
    //m_propEdit->hide();
    //m_dataSourceTreeDockWidget->hide();
    QStringList svlist = NCReportEvaluator::availableSystemVariables();
    for (int i=0;i<svlist.count();i++)
        dataSourceTree()->addSystemVariable(svlist.at(i));

    connect(m_dataSourceTreeDockWidget->tree(),SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(dataSourceTreeDoubleClick(QTreeWidgetItem*,int)));
}

QPoint NCRDMainWindow::grid() const
{
    return m_preferences.grid;
}

bool NCRDMainWindow::isShowGrid() const
{
    return m_preferences.gridVisible;
}

bool NCRDMainWindow::isSnapGrid() const
{
    return m_preferences.snapToGrid;
}

bool NCRDMainWindow::isPropEditor() const
{
    return m_propEdit->isVisible();
}

void NCRDMainWindow::setCopyXML(const QString & xml)
{
    m_copyXML = xml;
}

QString NCRDMainWindow::copyXML() const
{
    return m_copyXML;
}

QMenu * NCRDMainWindow::menuTools()
{
    return mnTools;
}

void NCRDMainWindow::editItem()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;

    doc->itemEdit( doc->firstSelectedItem() );
}

void NCRDMainWindow::editSection()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;

    doc->sectionPropDialog( doc->currentScene() );
}

void NCRDMainWindow::languageChangeByAction( QAction* action )
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;
    //QAction *action = qobject_cast<QAction *>(sender());
    //if (action) {
        QString langcode = action->data().toString();
        doc->setCurrentLanguage( langcode );
    //}

}

QMenu* NCRDMainWindow::languageMenu()
{
    return mnLang;
}

QActionGroup* NCRDMainWindow::languageActionGroup()
{
    return langGroup;
}

QString NCRDMainWindow::genOutputFile( const QString & ext )
{
    QString outputDir;
    if ( m_lastOutputDir.isEmpty() ) {
        outputDir = m_preferences.outputDir;
    } else {
        outputDir = m_lastOutputDir;
    }
    ++m_preferences.fileCounter;
    QString fname = QString("report_result_%1").arg( m_preferences.fileCounter, 4, 10, QChar('0') )+"."+ext;
    //if (m_preferences.outputDir.isEmpty())
    fname = outputDir + (outputDir.isEmpty() ? "" : "/") + fname;

    return fname;
}

void NCRDMainWindow::editParameters()
{
    QDialog dialog(this);
    Ui::NCRDParameterDialogUI ui;
    ui.setupUi(&dialog);
    NCRDDocument * ad = activeDocument();
    if(ad == NULL)
        return;
    NCRDParameterEditor *pEdit = new NCRDParameterEditor( /*m_dummyReportDef*/ad->reportDef(), m_removedParameters, &dialog);
    QBoxLayout *lay = qobject_cast<QBoxLayout *>(dialog.layout());
    lay->insertWidget(0,pEdit);
    pEdit->load();
    pEdit->setCurrentParameter(m_defaultParameterID);

    switch (dialog.exec()) {
        case QDialog::Accepted: {
            pEdit->save();
            dataSourceTree()->updateParameters(/*m_dummyReportDef*/ad->reportDef());

            break;
        }
        case QDialog::Rejected:
            break;
    }
}

void NCRDMainWindow::editScripts()
{
    NCRDDocument *doc = activeDocument();
    if (!doc)
        return;
    NCRDScriptDialog dialog(doc, this);
    switch (dialog.exec()) {
        case QDialog::Accepted: {
            dialog.applyChanges();
            break;
        }
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}

void NCRDMainWindow::addCustomDataSource(NCReportDataSource *ds)
{
    m_dummyReportDef->addDataSource(ds);
}

const QHash<QString, NCReportDataSource *> NCRDMainWindow::customDataSources() const
{
    return m_dummyReportDef->dataSources();
}

/*!
  Sets the custom data sources to the document object. This is necessary for automatic defining of custom data source in a new report if
  the object is already assigned to NCRDMainWindow by window->addCustomDatasource(NCReportDataSource*)
  */
//void NCRDMainWindow::addCustomDataSourcesToDocument(NCRDDocument *doc)
//{
//    for (int i = 0; i < m_customDataSources.size(); ++i)
//        doc->reportDef()->addDataSource(m_customDataSources.at(i));
//}

void NCRDMainWindow::addItemModel(const QString &id, QAbstractItemModel *model)
{
    m_dummyReportDef->addItemModel( id, model );
}

const QHash<QString, QAbstractItemModel *> NCRDMainWindow::itemModels() const
{
    return m_dummyReportDef->itemModels();
}

void NCRDMainWindow::addGraphItemRenderer(NCReportGraphRenderer *renderer)
{
    m_dummyReportDef->addGraphItemRenderer(renderer);
}

const QHash<QString, NCReportGraphRenderer *> NCRDMainWindow::graphItemRenderers() const
{
    return m_dummyReportDef->itemRenderers();
}

void NCRDMainWindow::addUserFunction(const QString &id, NCReportUserFunction *function)
{
    m_dummyReportDef->addUserFunction(id,function);
}

void NCRDMainWindow::addTestDataSources()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QString sql = "SELECT CustomerID, CompanyName, ContactName, ContactTitle, Address FROM customers WHERE city='London'";
    model->setQuery(sql, QSqlDatabase::database("northwind"));
    if (model->lastError().isValid()) {
        statusBar()->showMessage(model->lastError().text(), 5000);
        delete model;
        return;
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CustomerID"), Qt::EditRole);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Customer Identifical Number"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CompanyName"), Qt::EditRole);
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Company's Name"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ContactName"), Qt::EditRole);
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contact's Name"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ContactTitle"), Qt::EditRole);
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Title of contact"), Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Address"), Qt::EditRole);
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Company's Address"), Qt::DisplayRole);
    addItemModel("sqlqm1",model);
}

void NCRDMainWindow::slotnewNCReportFile(QString filename)
{
    NCRDDocument *child = createDocument();
    afterCreateDocument( child );

    child->newFile( filename );

    //    child->newFile( m_preferences.reportDir+"/"+tr("report%1.ncr") );
    if ( m_preferences.showMaximized )
        child->showMaximized();
    else
        child->show();
        child->showMaximized();
    zoomOne();

    dataSourceTree()->setErrorHandler(child->reportDef()->error());
    child->updateDataSourceTree(dataSourceTree(), true,true,false,false);

    NCRDDocument *doc = activeDocument();
    if (doc && doc->StudiosaveAs( filename )) {
        setCurrentFile( filename );
        statusBar()->showMessage(tr("File saved"), 2000);
    }
}

void NCRDMainWindow::slotopenNCReportFile( QString fileName)
{
    NCReportSource rs;

    NCRDDocument *doc = activeDocument();
    if ( doc ) {
        QFileInfo fi( fileName );
        if (fi.exists())
            m_lastDir = fi.path();
        else
            m_lastDir = m_preferences.reportDir;
    } else {
        m_lastDir = m_preferences.reportDir;
    }

    rs.setFileName(fileName);
    if ( !rs.fileName().isEmpty() )
    {
        load( rs );
        QFileInfo fi( rs.fileName() );
        m_lastDir = fi.path();
    }
}

bool NCRDMainWindow::slotdeleteNCReportFile(QString absolutefilePath, QString NCReportFilePath, QString currentitem_name)
{
    QFileInfo fileinfo;
    fileinfo.setFile(NCReportFilePath,currentitem_name);
    if(!fileinfo.exists())
    {
        return false;
    }
    QFile file;
    file.setFileName(absolutefilePath);

    if(file.isOpen())
    {
        QMessageBox::warning(NULL, tr("警告"), tr(" 报表文件已经打开,禁止删除!"), QMessageBox::Yes);
        return false;
    }

    bool isexist = false;

    QList<NCRDDocument*> documents = openedDocuments();

    for (int i=0; i<documents.count(); i++)
    {
        if(documents.at(i)->fileName() == NCReportFilePath + QString("/") + currentitem_name)
        {
            isexist = true;
        }
    }
    if(isexist == true)
    {
        QMessageBox::warning(NULL, tr("警告"), tr(" 报表文件已经打开,禁止删除!"), QMessageBox::Yes);
        return false;
    }
    else
    {




        if(file.remove(absolutefilePath))
        {
            qDebug()<<"remove XMLfile succed!";
        }
        else
        {
            qDebug()<<"remove XMLfile failed!";
        }

        return true;
    }
}

void NCRDMainWindow::open(const NCReportSource &source)
{
    load( source );
}

QString NCRDMainWindow::zintPath() const
{
    return m_dummyReportDef->zintPath();
}

void NCRDMainWindow::setZintPath(const QString &dir)
{
    m_dummyReportDef->setZintPath(dir);
}

QList<NCRDDocument *> NCRDMainWindow::openedDocuments()
{
    QList<NCRDDocument *> documents;
    QList<QMdiSubWindow *> windowsList = mdiArea->subWindowList();
    for (int i = 0; i < windowsList.size(); ++i)
        documents << qobject_cast<NCRDDocument *>(windowsList.at(i)->widget());
    return documents;
}


