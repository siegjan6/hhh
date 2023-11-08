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

#include <QMdiArea>
#include <QAction>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QUndoGroup>
#include <QSignalMapper>
#include <QToolButton>
#include <QApplication>
#include <QStatusBar>


#include "ncrdbasewindow.h"

NCRDBaseWindow::NCRDBaseWindow( QWidget * parent, Qt::WindowFlags flags )
    : CBaseForm( parent )
{
    Q_UNUSED(flags);
    mdiArea = new QMdiArea(this);
    setCentralWidget(mdiArea);
    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(updateMenus()));

    windowMapper = new QSignalMapper(this);
    connect(windowMapper, SIGNAL(mapped(QWidget *)), this, SLOT(setActiveSubWindow(QWidget *)));

    m_actionMapper = new QSignalMapper(this);
    connect(m_actionMapper, SIGNAL(mapped(int)), this, SLOT(activateCustomAction(int)));

    toolGroup = new QActionGroup(this);
    toolGroup->setExclusive(true);
    m_pProjectConfiger = NULL;

    m_transTool = CMDMultiLanguage::instance()->createWidgetTool();
    m_transTool->init(this);
    m_transTool->changeLanguage();
}

NCRDBaseWindow::~NCRDBaseWindow()
{
}


void NCRDBaseWindow::createActions()
{
    //--------------
    // FILE
    //--------------
    //act_new = new QAction(QIcon(":/designer/images/filenew.png"), tr("&New"), this);
    //act_new->setShortcut(tr("Ctrl+N"));
    //act_new->setStatusTip(tr("Create a new report file"));
    //connect(act_new, SIGNAL(triggered()), this, SLOT(newFile()));

    //act_open = new QAction(QIcon(":/designer/images/fileopen.png"), tr("&Open..."), this);
    //act_open->setShortcut(tr("Ctrl+O"));
    //act_open->setStatusTip(tr("Open an existing report file"));
    //connect(act_open, SIGNAL(triggered()), this, SLOT(open()));

    act_opendb = new QAction(QIcon(":/designer/images/fileopen.png"), tr("&Open from database..."), this);
    //act_opendb->setShortcut(tr("Ctrl+O"));
    act_opendb->setStatusTip(tr("Open an existing report file from database"));
    connect(act_opendb, SIGNAL(triggered()), this, SLOT(openDB()));

    act_save = new QAction(QIcon(":/designer/images/disk.png"), tr("&Save"), this);
    act_save->setShortcut(tr("Ctrl+S"));
    act_save->setStatusTip(tr("Save the report to disk"));
    connect(act_save, SIGNAL(triggered()), this, SLOT(save()));

    act_saveas = new QAction(tr("Save &As..."), this);
    act_saveas->setStatusTip(tr("Save the report under a new name"));
    connect(act_saveas, SIGNAL(triggered()), this, SLOT(saveAs()));

    act_exit = new QAction(tr("E&xit"), this);
    act_exit->setShortcut(tr("Ctrl+Q"));
    act_exit->setStatusTip(tr("Exit the application"));
    //connect(act_exit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
    connect(act_exit, SIGNAL(triggered()), this, SLOT(close()));

    act_options = new QAction(tr("Preferences"), this);
    act_options->setText(tr("Preferences..."));
    act_options->setStatusTip(tr("Program preferences"));
    connect(act_options, SIGNAL(triggered()), this, SLOT(optionsDialog()));

    act_saveClip = new QAction(tr("Copy source to clipboard"), this);
    act_saveClip->setStatusTip(tr("Copy XML source to clipboard"));
    connect(act_saveClip, SIGNAL(triggered()), this, SLOT(saveClipboard()));

    act_showSource = new QAction(tr("Show source"), this);
    act_showSource->setStatusTip(tr("Show XML source code"));
    connect(act_showSource, SIGNAL(triggered()), this, SLOT(showSource()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActs[i] = new QAction(this);
        recentFileActs[i]->setVisible(false);
        connect(recentFileActs[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    //--------------
    // REPORT
    //--------------
    act_details = new QAction(tr("Details and grouping"), this);
    act_details->setIcon( QIcon(":/designer/images/details.png") );
    act_details->setText(tr("Details and grouping..."));
    connect(act_details, SIGNAL(triggered()), this, SLOT(detailsDialog()));

    act_pagesettings = new QAction(tr("Report and page settings"), this);
    act_pagesettings->setIcon( QIcon(":/designer/images/pageoptions.png") );
    act_pagesettings->setText(tr("Report and page settings..."));
    act_pagesettings->setStatusTip(tr("Report and page settings"));
    act_pagesettings->setShortcut(tr("Ctrl+Shift+D"));
    connect(act_pagesettings, SIGNAL(triggered()), this, SLOT(pageSettingsDialog()));

    act_variables = new QAction(tr("Variables"), this);
    act_variables->setIcon( QIcon(":/designer/images/variable.png") );
    act_variables->setText(tr("Variables..."));
    act_variables->setStatusTip(tr("Set report variables"));
    connect(act_variables, SIGNAL(triggered()), this, SLOT(variablesDialog()));

    act_scripts = new QAction(tr("Scripts"), this);
    act_scripts->setIcon( QIcon(":/designer/images/script_code.png") );
    act_scripts->setText(tr("Scripts..."));
    act_scripts->setStatusTip(tr("Edit user defined scripts"));
    connect(act_scripts, SIGNAL(triggered()), this, SLOT(editScripts()));

    act_datasources = new QAction(tr("Data sources"), this);
    act_datasources->setIcon( QIcon(":/designer/images/datasource.png") );
    act_datasources->setText(tr("Data sources..."));
    act_datasources->setStatusTip(tr("Report data source settings"));
    connect(act_datasources, SIGNAL(triggered()), this, SLOT(dataSourcesDialog()));

    //act_mindscadadatasources = new QAction(tr("MindSCADA Data sources"), this);
    //act_mindscadadatasources->setIcon( QIcon(":/designer/images/datasource.png") );
    //act_mindscadadatasources->setText(tr("数据源..."));
    //act_mindscadadatasources->setStatusTip(tr("Report MindSCADA data source settings"));
    //connect(act_mindscadadatasources, SIGNAL(triggered()), this, SLOT(MindSCADAdataSourcesDialog()));

    act_edititem = new QAction(tr("Edit selected item..."), this);
    act_edititem->setShortcut(tr("Ctrl+1"));
    connect(act_edititem, SIGNAL(triggered()), this, SLOT(editItem()));

    act_editsection = new QAction(tr("Edit selected section..."), this);
    act_editsection->setShortcut(tr("Ctrl+2"));
    connect(act_editsection, SIGNAL(triggered()), this, SLOT(editSection()));

    act_connectdb = new QAction(tr("Database connection manager..."), this);
    act_connectdb->setIcon( QIcon(":/designer/images/dbconnect.png") );
    connect(act_connectdb, SIGNAL(triggered()), this, SLOT(connectDB()));

/*	act_disconnectdb = new QAction(tr("Disconnect SQL database..."), this);
    connect(act_disconnectdb, SIGNAL(triggered()), this, SLOT(disconnectDB()));*/

    act_parameters = new QAction(tr("Parameters..."), this);
    act_parameters->setIcon( QIcon(":/designer/images/tag.png") );
    connect(act_parameters, SIGNAL(triggered()), this, SLOT(editParameters()));

    act_run = new QAction(tr("Run report..."), this);
    act_run->setIcon( QIcon(":/designer/images/control_play_blue.png") );
    act_run->setShortcut(tr("Ctrl+R"));
    connect(act_run, SIGNAL(triggered()), this, SLOT(runReportDialog()));

    act_runPrint = new QAction(QIcon(":/designer/images/printer.png"), tr("Print"), this);
    act_runPrint->setText(tr("Printer..."));
    act_runPrint->setStatusTip(tr("Run report to printer"));
    connect(act_runPrint, SIGNAL(triggered()), this, SLOT(runReportPrint()));

    act_runPreview = new QAction(tr("Preview"), this);
    act_runPreview->setIcon( QIcon(":/designer/images/preview.png") );
    act_runPreview->setShortcut(tr("Ctrl+P"));
    connect(act_runPreview, SIGNAL(triggered()), this, SLOT(runReportPreview()) );

    act_runQtPreview = new QAction(tr("Qt preview"), this);
    act_runQtPreview->setIcon( QIcon(":/designer/images/qpreview.png") );
    connect(act_runQtPreview, SIGNAL(triggered()), this, SLOT(runReportQtPreview()) );

    act_runPdf = new QAction(tr("PDF file..."), this);
    act_runPdf->setIcon( QIcon(":/designer/images/pdf.png") );
    connect(act_runPdf, SIGNAL(triggered()), this, SLOT(runReportPdf()) );

    act_runSvg = new QAction(tr("SVG file..."), this);
    act_runSvg->setIcon( QIcon(":/designer/images/svg.png") );
    connect(act_runSvg, SIGNAL(triggered()), this, SLOT(runReportSvg()) );

    act_runText = new QAction(tr("Text file..."), this);
    connect(act_runText, SIGNAL(triggered()), this, SLOT(runReportText()));

    act_runHtml = new QAction(tr("HTML file..."), this);
    connect(act_runHtml, SIGNAL(triggered()), this, SLOT(runReportHtml()));

    act_runExcelHtml = new QAction(tr("Excel file..."), this);
    connect(act_runExcelHtml, SIGNAL(triggered()), this, SLOT(runReportExcelHtml()));

    act_runImage = new QAction(tr("Image file..."), this);
    connect(act_runImage, SIGNAL(triggered()), this, SLOT(runReportImage()));

    act_testdata = new QAction(tr("Test data editor..."), this);
    act_testdata->setIcon( QIcon(":/designer/images/testdata.png") );
    act_testdata->setShortcut(tr("Ctrl+T"));
    connect(act_testdata, SIGNAL(triggered()), this, SLOT(testDataEditor()));

    //--------------
    // EDIT
    //--------------
    m_undoGroup = new QUndoGroup(this);
    act_undo = m_undoGroup->createUndoAction(this);
    act_redo = m_undoGroup->createRedoAction(this);

    act_undo->setText( tr("Undo") );
    act_undo->setIcon(QIcon(":/designer/images/undo.png"));
    act_undo->setShortcut(tr("Ctrl+Z"));
    act_undo->setStatusTip(tr("Undo the last action"));
    //connect(act_undo, SIGNAL(triggered()), this, SLOT(undo()));

    act_redo->setText( tr("Redo") );
    act_redo->setIcon(QIcon(":/designer/images/redo.png"));
    act_redo->setShortcut(tr("Ctrl+Y"));
    act_redo->setStatusTip(tr("Redo the last undo action"));
    //connect(act_redo, SIGNAL(triggered()), this, SLOT(redo()));

    act_cut = new QAction(QIcon(":/designer/images/editcut.png"), tr("Cu&t"), this);
    act_cut->setShortcut(tr("Ctrl+X"));
    act_cut->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
    connect(act_cut, SIGNAL(triggered()), this, SLOT(cut()));

    act_copy = new QAction(QIcon(":/designer/images/editcopy.png"), tr("&Copy"), this);
    act_copy->setShortcut(tr("Ctrl+C"));
    act_copy->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(act_copy, SIGNAL(triggered()), this, SLOT(copy()));

    act_paste = new QAction(QIcon(":/designer/images/editpaste.png"), tr("&Paste"), this);
    act_paste->setShortcut(tr("Ctrl+V"));
    act_paste->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
    connect(act_paste, SIGNAL(triggered()), this, SLOT(paste()));

    act_lock = new QAction(QIcon(":/designer/images/lock.png"), tr("&Lock items"), this);
    act_lock->setShortcut(tr("Ctrl+L"));
    act_lock->setStatusTip(tr("Lock selected items in all sections"));
    connect(act_lock, SIGNAL(triggered()), this, SLOT(lock()));

    act_unlock = new QAction(QIcon(":/designer/images/lock_open.png"), tr("&Unlock items"), this);
    act_unlock->setShortcut( Qt::CTRL + Qt::SHIFT + Qt::Key_L );
    act_unlock->setStatusTip(tr("Unlock selected items in all sections"));
    connect(act_unlock, SIGNAL(triggered()), this, SLOT(unlock()));

    act_setzones = new QAction(tr("Set zone ID..."), this);
    act_setzones->setStatusTip(tr("Sets the same zone ID to all selected items"));
    connect(act_setzones, SIGNAL(triggered()), this, SLOT(setZones()));

    //--------------
    // VIEW
    //--------------
    act_showgrid = new QAction(QIcon(":/designer/images/show-grid.png"), tr("Show grid"), this);
    act_showgrid->setStatusTip(tr("Show the grid points"));
    act_showgrid->setCheckable(true);
    connect(act_showgrid, SIGNAL(triggered()), this, SLOT(showGrid()));

    act_snapgrid = new QAction(QIcon(":/designer/images/snap-grid.png"), tr("Snap to grid"), this);
    act_snapgrid->setStatusTip(tr("Snap to the grid points"));
    act_snapgrid->setCheckable(true);
    connect(act_snapgrid, SIGNAL(triggered()), this, SLOT(snapToGrid()));

    act_zoomin = new QAction(QIcon(":/designer/images/zoom_in.png"), tr("Zoom in"), this);
    act_zoomin->setStatusTip(tr("Zoom in the current view"));
    act_zoomin->setShortcut(tr("Ctrl++"));
    connect(act_zoomin, SIGNAL(triggered()), this, SLOT(zoomIn()));

    act_zoomout = new QAction(QIcon(":/designer/images/zoom_out.png"), tr("Zoom out"), this);
    act_zoomout->setStatusTip(tr("Zoom out the current view"));
    act_zoomout->setShortcut(tr("Ctrl+-"));
    connect(act_zoomout, SIGNAL(triggered()), this, SLOT(zoomOut()));

    act_zoom1 = new QAction(QIcon(":/designer/images/zoom.png"), tr("Zoom one"), this);
    act_zoom1->setStatusTip(tr("Reset zoom in the current view"));
    act_zoom1->setShortcut(tr("Ctrl+0"));
    connect(act_zoom1, SIGNAL(triggered()), this, SLOT(zoomOne()));

    //--------------
    // TOOLS
    //--------------
    act_arrow = new QAction(QIcon(":/designer/images/cursor.png"), tr("Selection"), this);
    act_arrow->setStatusTip(tr("Set to selection mode"));
    act_arrow->setCheckable(true);
    connect(act_arrow, SIGNAL(triggered()), this, SLOT(activate_Arrow()));

    act_label = new QAction(QIcon(":/designer/images/label.png"), tr("Label"), this);
    act_label->setStatusTip(tr("Insert label object"));
    act_label->setCheckable(true);
    connect(act_label, SIGNAL(triggered()), this, SLOT(activate_Label()));

    act_text = new QAction(QIcon(":/designer/images/html.png"), tr("Text"), this);
    act_text->setStatusTip(tr("Insert text object"));
    act_text->setCheckable(true);
    connect(act_text, SIGNAL(triggered()), this, SLOT(activate_Text()));

    act_field = new QAction(QIcon(":/designer/images/database_link.png"), tr("Field"), this);
    act_field->setStatusTip(tr("Insert field object"));
    act_field->setCheckable(true);
    connect(act_field, SIGNAL(triggered()), this, SLOT(activate_Field()));

    act_line = new QAction(QIcon(":/designer/images/shape_line.png"), tr("Line"), this);
    act_line->setStatusTip(tr("Insert line object"));
    act_line->setCheckable(true);
    connect(act_line, SIGNAL(triggered()), this, SLOT(activate_Line() ));

    act_rect = new QAction(QIcon(":/designer/images/shape_square.png"), tr("Rectangle"), this);
    act_rect->setStatusTip(tr("Insert rectangle object"));
    act_rect->setCheckable(true);
    connect(act_rect, SIGNAL(triggered()), this, SLOT(activate_Rect() ));

    act_ellipse = new QAction(QIcon(":/designer/images/shape_circ.png"), tr("Ellipse"), this);
    act_ellipse->setStatusTip(tr("Insert ellipse object"));
    act_ellipse->setCheckable(true);
    connect(act_ellipse, SIGNAL(triggered()), this, SLOT(activate_Ellipse() ));

    act_img = new QAction(QIcon(":/designer/images/image.png"), tr("Image"), this);
    act_img->setStatusTip(tr("Insert image object"));
    act_img->setCheckable(true);
    connect(act_img, SIGNAL(triggered()), this, SLOT(activate_Img() ));

    act_barcode = new QAction(QIcon(":/designer/images/barcode.png"), tr("Barcode"), this);
    act_barcode->setStatusTip(tr("Insert barcode object"));
    act_barcode->setCheckable(true);
    connect(act_barcode, SIGNAL(triggered()), this, SLOT(activate_Barcode() ));

    act_graph = new QAction(QIcon(":/designer/images/application_view_tile.png"), tr("Graph"), this);
    act_graph->setStatusTip(tr("Insert graph/custom drawing object"));
    act_graph->setCheckable(true);
    connect(act_graph, SIGNAL(triggered()), this, SLOT(activate_Graph() ));

    act_crosstab = new QAction(QIcon(":/designer/images/crosstab.png"), tr("Cross tab"), this);
    act_crosstab->setStatusTip(tr("Insert cross table object"));
    act_crosstab->setCheckable(true);
    connect(act_crosstab, SIGNAL(triggered()), this, SLOT(activate_CrossTab() ));

    act_chart = new QAction(QIcon(":/designer/images/chart_bar.png"), tr("Chart"), this);
    act_chart->setStatusTip(tr("Insert Chart object"));
    act_chart->setCheckable(true);
    connect(act_chart, SIGNAL(triggered()), this, SLOT(activate_Chart()));

    act_table = new QAction(QIcon(":/designer/images/table.png"), tr("Table View"), this);
    act_table->setStatusTip(tr("Insert Qt Table View object"));
    act_table->setCheckable(true);
    connect(act_table, SIGNAL(triggered()), this, SLOT(activate_Table()));

    toolGroup->addAction(act_arrow);
    toolGroup->addAction(act_label);
    toolGroup->addAction(act_text);
    toolGroup->addAction(act_field);
    toolGroup->addAction(act_rect);
    toolGroup->addAction(act_line);
    toolGroup->addAction(act_ellipse);
    toolGroup->addAction(act_img);
    toolGroup->addAction(act_barcode);
    toolGroup->addAction(act_graph);
    toolGroup->addAction(act_chart);
    toolGroup->addAction(act_crosstab);
    toolGroup->addAction(act_table);

    //--------------
    // ALIGN
    //--------------
    act_back = new QAction(QIcon(":/designer/images/shape_move_back.png"), tr("Move to Back"), this);
    act_back->setStatusTip(tr("Move to back"));
    connect(act_back, SIGNAL(triggered()), this, SLOT(alignBack() ));

    act_front = new QAction(QIcon(":/designer/images/shape_move_front.png"), tr("Bring to Front"), this);
    act_front->setStatusTip(tr("Move to front"));
    connect(act_front, SIGNAL(triggered()), this, SLOT(alignFront() ));

    act_backward = new QAction(QIcon(":/designer/images/shape_move_backwards.png"), tr("Move backwards"), this);
    act_backward->setStatusTip(tr("Move backwards"));
    connect(act_backward, SIGNAL(triggered()), this, SLOT(alignBackward()));

    act_forward = new QAction(QIcon(":/designer/images/shape_move_forwards.png"), tr("Move forwards"), this);
    act_forward->setStatusTip(tr("Move forwards"));
    connect(act_forward, SIGNAL(triggered()), this, SLOT(alignForward() ));

    act_left = new QAction(QIcon(":/designer/images/shape_align_left.png"), tr("Align left"), this);
    act_left->setStatusTip(tr("Align left"));
    connect(act_left, SIGNAL(triggered()), this, SLOT(alignLeft() ));

    act_right = new QAction(QIcon(":/designer/images/shape_align_right.png"), tr("Align right"), this);
    act_right->setStatusTip(tr("Align right"));
    connect(act_right, SIGNAL(triggered()), this, SLOT(alignRight() ));

    act_hcenter = new QAction(QIcon(":/designer/images/shape_align_middle.png"), tr("Center horizontally"), this);
    act_hcenter->setStatusTip(tr("Center horizontally"));
    connect(act_hcenter, SIGNAL(triggered()), this, SLOT(alignHCenter() ));

    act_vcenter = new QAction(QIcon(":/designer/images/shape_align_center.png"), tr("Center vertically"), this);
    act_vcenter->setStatusTip(tr("Center vertically"));
    connect(act_vcenter, SIGNAL(triggered()), this, SLOT(alignVCenter() ));

    act_top = new QAction(QIcon(":/designer/images/shape_align_top.png"), tr("Align top"), this);
    act_top->setStatusTip(tr("Align to top"));
    connect(act_top, SIGNAL(triggered()), this, SLOT(alignTop() ));

    act_bottom = new QAction(QIcon(":/designer/images/shape_align_bottom.png"), tr("Align bottom"), this);
    act_bottom->setStatusTip(tr("Align to bottom"));
    connect(act_bottom, SIGNAL(triggered()), this, SLOT(alignBottom() ));

    act_samewidth = new QAction(QIcon(":/designer/images/shape_same_width.png"), tr("Set widths the same"), this);
    act_samewidth->setStatusTip(tr("Set widths the same"));
    connect(act_samewidth, SIGNAL(triggered()), this, SLOT( alignSameWidth() ));

    act_sameheight = new QAction(QIcon(":/designer/images/shape_same_height.png"), tr("Set heights the same"), this);
    act_sameheight->setStatusTip(tr("Set heights the same"));
    connect(act_sameheight, SIGNAL(triggered()), this, SLOT( alignSameHeight() ));


    //--------------
    // WINDOW
    //--------------
    act_close = new QAction(tr("Cl&ose"), this);
    act_close->setShortcut(tr("Ctrl+F4"));
    act_close->setStatusTip(tr("Close the active window"));
    connect(act_close, SIGNAL(triggered()), mdiArea, SLOT(closeActiveSubWindow()));

    act_closeall = new QAction(tr("Close &All"), this);
    act_closeall->setStatusTip(tr("Close all the windows"));
    connect(act_closeall, SIGNAL(triggered()), mdiArea, SLOT(closeAllSubWindows()));

    act_tile = new QAction(tr("&Tile"), this);
    act_tile->setStatusTip(tr("Tile the windows"));
    connect(act_tile, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

    act_cascade = new QAction(tr("&Cascade"), this);
    act_cascade->setStatusTip(tr("Cascade the windows"));
    connect(act_cascade, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

    act_next = new QAction(tr("Ne&xt"), this);
    act_next->setStatusTip(tr("Move the focus to the next window"));
    connect(act_next, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

    act_previous = new QAction(tr("Pre&vious"), this);
    act_previous->setStatusTip(tr("Move the focus to the previous window"));
    connect(act_previous, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));

    //--------------
    // TEXT PROPS
    //--------------

    //--------------
    // HELP
    //--------------
    act_about = new QAction(tr("&About"), this);
    act_about->setStatusTip(tr("Show the application's About box"));
    connect(act_about, SIGNAL(triggered()), this, SLOT(about()));

    act_aboutqt = new QAction(tr("About &Qt"), this);
    act_aboutqt->setStatusTip(tr("Show the Qt library's About box"));
    connect(act_aboutqt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    //--------------
    // OTHER
    //--------------
    act_separator = new QAction(this);
    act_separator->setSeparator(true);

    act_test = new QAction(tr("Execute test function"), this);
    connect(act_test, SIGNAL(triggered()), this, SLOT(testFunction()));

}

void NCRDBaseWindow::createMenus()
{
    //--------------------------------------------------------
    mnFile = menuBar()->addMenu(tr("&File"));
    //mnFile->addAction(act_new);
    //mnFile->addAction(act_open);
    mnFile->addAction(act_opendb);
    mnFile->addAction(act_save);
    mnFile->addAction(act_saveas);
    mnRecent = mnFile->addMenu( tr("Recent files") );
    mnFile->addSeparator();

    mnRun = mnFile->addMenu( tr("Run report to") );
    mnRun->addAction(act_runPreview);
    mnRun->addAction(act_runQtPreview);
    mnRun->addSeparator();
    mnRun->addAction(act_runPrint);
    mnRun->addSeparator();
    mnRun->addAction(act_runPdf);
    mnRun->addAction(act_runSvg);
    mnRun->addAction(act_runText);
    mnRun->addAction(act_runHtml);
    mnRun->addAction(act_runExcelHtml);
    mnRun->addAction(act_runImage);

    mnFile->addAction(act_run);

    //separatorAct = mnFile->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
        mnRecent->addAction(recentFileActs[i]);
    mnFile->addSeparator();
    //mnFile->addAction(act_showSource);
    mnFile->addAction(act_saveClip);
    mnFile->addAction(act_options);
    mnFile->addSeparator();
    mnFile->addAction(act_exit);

    updateRecentFileActions();

    //--------------------------------------------------------
    mnReport = menuBar()->addMenu(tr("&Report"));
    mnReport->addAction(act_pagesettings);
    mnReport->addAction(act_datasources);
    mnReport->addAction(act_details);
    mnReport->addAction(act_variables);
    mnReport->addAction(act_scripts);
    mnReport->addSeparator();
    mnReport->addAction(act_edititem);
    mnReport->addAction(act_editsection);
    mnReport->addSeparator();
    mnReport->addAction(act_connectdb);

    mnConn = mnReport->addMenu( tr("Database connections") );
    mnConn->setIcon( QIcon(":/designer/images/connect.png") );

    mnLang = mnReport->addMenu( tr("Report language") );
    mnLang->setIcon( QIcon(":/designer/images/world.png") );

    mnReport->addSeparator();
    mnReport->addAction(act_parameters);
    mnReport->addAction(act_testdata);
#ifdef NCRD_TESTDATAMENU
    mnReport->addAction(tr("Add test datasources"),this,SLOT(addTestDataSources()));
#endif

//    //mnReport->addAction(act_sqleditor);

    //--------------------------------------------------------
    mnEdit = menuBar()->addMenu(tr("&Edit"));
    mnEdit->addAction(act_undo);
    mnEdit->addAction(act_redo);
    mnEdit->addAction(act_lock);
    mnEdit->addAction(act_unlock);
    mnEdit->addAction(act_setzones);
    mnEdit->addSeparator();
    mnEdit->addAction(act_cut);
    mnEdit->addAction(act_copy);
    mnEdit->addAction(act_paste);

    //--------------------------------------------------------
    mnView = menuBar()->addMenu(tr("&View"));
    //QAction *action = mnView->addAction(tr("Switch layout direction"));
    //connect(action, SIGNAL(triggered()), this, SLOT(switchLayoutDirection()));
    mnView->addAction(act_zoomin);
    mnView->addAction(act_zoomout);
    mnView->addAction(act_zoom1);
    mnView->addSeparator();
    mnView->addAction(act_showgrid);
    mnView->addAction(act_snapgrid);
    mnView->addSeparator();
    mnView->addAction(act_showpropedit);
    mnView->addAction(act_showdstree);

    //--------------------------------------------------------
    mnTools = menuBar()->addMenu(tr("&Insert"));
    mnTools->addAction(act_arrow);
    mnTools->addSeparator();
    mnTools->addAction(act_label);
    mnTools->addAction(act_text);
    mnTools->addAction(act_field);
    mnTools->addAction(act_line);
    mnTools->addAction(act_rect);
    mnTools->addAction(act_ellipse);
    mnTools->addAction(act_img);
    mnTools->addAction(act_barcode);
    mnTools->addAction(act_graph);
#ifdef HIGHCHARTS_INTEGRATION
    mnTools->addAction(act_chart);
#endif
    mnTools->addAction(act_crosstab);
    mnTools->addAction(act_table);

    //--------------------------------------------------------
    mnFormat = menuBar()->addMenu(tr("F&ormat"));

    //--------------------------------------------------------
    mnAlign = menuBar()->addMenu(tr("&Align"));
    mnAlign->addAction(act_front);
    mnAlign->addAction(act_back);
    mnAlign->addSeparator();
    mnAlign->addAction(act_left);
    mnAlign->addAction(act_hcenter);
    mnAlign->addAction(act_right);
    mnAlign->addAction(act_top);
    mnAlign->addAction(act_vcenter);
    mnAlign->addAction(act_bottom);
    mnAlign->addSeparator();
    mnAlign->addAction(act_samewidth);
    mnAlign->addAction(act_sameheight);

    //--------------------------------------------------------
    mnWindow = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(mnWindow, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    //mnHelp = menuBar()->addMenu(tr("&Help"));
    //mnHelp->addAction(act_about);
    //mnHelp->addAction(act_aboutqt);

    menuBar()->setVisible(false);
}

void NCRDBaseWindow::createToolBars()
{
    tbFile = addToolBar(tr("File"));
    tbFile->setObjectName("tbFile");
    //tbFile->addAction(act_new);
    //tbFile->addAction(act_open);
    tbFile->addAction(act_save);
    tbFile->addSeparator();
    tbFile->addAction(act_runPreview);
    tbFile->addAction(act_runPrint);
    tbFile->addAction(act_runPdf);
    tbFile->addAction(act_runSvg);
    tbFile->addAction(act_run);
    tbFile->addSeparator();
    tbFile->addAction(act_testdata);
    tbFile->addAction(act_parameters);

    //tbFile->addAction(act_close);

    tbEdit = addToolBar(tr("Edit"));
    tbEdit->setObjectName("tbEdit");
    tbEdit->addAction(act_undo);
    tbEdit->addAction(act_redo);
    tbEdit->addAction(act_lock);
    tbEdit->addAction(act_unlock);
    tbEdit->addSeparator();
    tbEdit->addAction(act_cut);
    tbEdit->addAction(act_copy);
    tbEdit->addAction(act_paste);

    tbConnection = addToolBar(tr("Database Connections"));
    tbConnection->setObjectName("tbConn");
    tbConnection->addAction(act_connectdb);
    QAction *actConn = new QAction(tr("Database connections"), this);
    actConn->setIcon( QIcon(":/designer/images/connect.png") );
    actConn->setMenu( mnConn );
    tbConnection->addAction( actConn );

    QAction *actLang = new QAction(tr("Languages"), this);
    actLang->setIcon( QIcon(":/designer/images/world.png") );
    actLang->setMenu( mnLang );

    tbReport = addToolBar(tr("Report"));
    tbReport->setObjectName("tbReport");
    tbReport->addAction(act_pagesettings);
    tbReport->addAction(act_datasources);
    tbReport->addAction(act_details);
    tbReport->addAction(act_variables);
    tbReport->addAction(act_scripts);
    tbReport->addAction(actLang);

    addToolBarBreak();

    tbTools = addToolBar(tr("Tools"));
    tbTools->setObjectName("tbTools");
    tbTools->addAction(act_arrow);
    tbTools->addAction(act_label);
    tbTools->addAction(act_text);
    tbTools->addAction(act_field);
    tbTools->addAction(act_line);
    tbTools->addAction(act_rect);
    tbTools->addAction(act_ellipse);
    tbTools->addAction(act_img);
    tbTools->addAction(act_barcode);
    tbTools->addAction(act_graph);
#ifdef HIGHCHARTS_INTEGRATION
    tbTools->addAction(act_chart);
#endif
    tbTools->addAction(act_crosstab);
    tbTools->addAction(act_table);

    tbAlign = addToolBar(tr("Align"));
    tbAlign->setObjectName("tbAlign");
    tbAlign->addAction(act_front);
    tbAlign->addAction(act_back);
    tbAlign->addSeparator();
    tbAlign->addAction(act_left);
    tbAlign->addAction(act_hcenter);
    tbAlign->addAction(act_right);
    tbAlign->addAction(act_top);
    tbAlign->addAction(act_vcenter);
    tbAlign->addAction(act_bottom);
    tbAlign->addSeparator();
    tbAlign->addAction(act_samewidth);
    tbAlign->addAction(act_sameheight);
    tbAlign->addAction(act_showgrid);
    tbAlign->addAction(act_snapgrid);

    addToolBarBreak();

    tbTextProps = addToolBar(tr("Text properties"));
    tbTextProps->setObjectName("tbTextProps");

    tbShapeProps = addToolBar(tr("Shape properties"));
    tbShapeProps->setObjectName("tbShapeProps");

    // SIZE
    tbFile->setIconSize(QSize(16,16));
    tbReport->setIconSize(QSize(16,16));
    tbConnection->setIconSize(QSize(16,16));
    tbEdit->setIconSize(QSize(16,16));
    tbTools->setIconSize(QSize(16,16));
    tbAlign->setIconSize(QSize(16,16));
    tbTextProps->setIconSize(QSize(16,16));
    tbShapeProps->setIconSize(QSize(16,16));
}

void NCRDBaseWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void NCRDBaseWindow::changeLanguage()
{
    if(m_transTool)
    {
        m_transTool->changeLanguage();
    }
}

void NCRDBaseWindow::switchLayoutDirection()
{
    if (layoutDirection() == Qt::LeftToRight)
        qApp->setLayoutDirection(Qt::RightToLeft);
    else
        qApp->setLayoutDirection(Qt::LeftToRight);
}

void NCRDBaseWindow::setActiveSubWindow(QWidget *window)
{
    if (!window)
        return;
    mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void NCRDBaseWindow::dockWindowVisible(bool set)
{
    QAction *action = qobject_cast<QAction*>(sender());
    if ( action == act_showpropedit ) {
        qDebug("dockWindowVisible(%i)",set);
        act_showpropedit->setChecked( set );
    }
    else if ( action == act_showdstree ) {
        act_showdstree->setChecked( set );
    }
}


QUndoGroup * NCRDBaseWindow::undoGroup() const
{
    return m_undoGroup;
}

/*!
 * \brief NCRDBaseWindow::addGraphItemAction
 * \param typeID
 * \param action
 * \param toolBar
 * \param menu
 * Obsolete. Use addCustomItemAction method instead.
 */

void NCRDBaseWindow::addGraphItemAction(int typeID, QAction *action, QToolBar* toolBar , QMenu *menu)
{
    addCustomToolAction(typeID, action, toolBar , menu);
}

/*!
 * \brief NCRDBaseWindow::addCustomToolAction
 * \param typeID function id for identification.
 * \param action
 * \param toolBar
 * \param menu
 * Adds a custom action to the designer main window. The action will appear in the item tools toolbar and the menu.
 * The function you can use to call custom setting dialogs of graph items and customized field items.
 * Function is identified by the typeID
 */

void NCRDBaseWindow::addCustomToolAction(int typeID, QAction *action, QToolBar* toolBar , QMenu *menu)
{
    m_customActions.append(action);
    connect(action,SIGNAL(triggered()),m_actionMapper,SLOT(map()));
    m_actionMapper->setMapping(action,typeID);
    action->setCheckable(true);
    toolGroup->addAction(action);

    if (toolBar)
        toolBar->addAction(action);
    if (menu)
        menu->addAction(action);
}


void NCRDBaseWindow::activateCustomAction(int typeID)
{
    Q_UNUSED(typeID);
}
