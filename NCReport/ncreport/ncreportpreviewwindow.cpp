/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. / NociSoft Software Solutions
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

#include "ncreportpreviewwindow.h"
#include "ncreportpreviewwindow_p.h"
#include "ncreportpreviewoutput.h"
#include "ncreportpreviewpage.h"
#include "ncreportsection.h"
#include "ncreport.h"
#include "info.h"

#include <QTextEdit>
#include <QTextStream>
#include <QCloseEvent>
#include <QFileDialog>
#include <QGraphicsView>
#include <QProgressDialog>
#include <QPrintDialog>
#include <QVBoxLayout>
#include <QInputDialog>

#include <QWidget>
#include <QAction>
#include <QActionGroup>
#include <QComboBox>
#include <QMenu>
#include <QTextEdit>
#include <QSpinBox>
#include <QProgressDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSettings>
#include <QScrollBar>
#include <QApplication>
#include <QPicture>
#include <QDateTime>

#define SCALE_BY_COMBOBOX
//#define SCALE_BY_SPINBOX
#define PREVIEW_SAVE_MODE
#define PREVIEW_PRINT_ACTION	1
#define PREVIEW_PDF_ACTION		1
#define PREVIEW_SVG_ACTION		1

NCReportPreviewWindow::NCReportPreviewWindow(QWidget *parent, Qt::WindowFlags flags, const QString &orgName, const QString &appName)
    : QMainWindow(parent,flags),
      d_ptr( new NCReportPreviewWindowPrivate(this) ),
      m_scrollArea( new NCReportPreviewScrollArea( this ) ),
      m_pageContainer(new NCReportPreviewPageContainer(m_scrollArea)),
      m_output(0),
      m_report(0),

//      mnFile(0),
//      mnView(0),
//      mnNav(0),
//      mnAbout(0),

      tbFile(0),
      tbView(0),
      tbNav(0),

      m_actGrpView(0),
      m_actionOpen(0),
      m_actionPrint(0),
      m_actionPdf(0),
      m_actionSvg(0),
      m_actionFrst(0),
      m_actionNext(0),
      m_actionPrev(0),
      m_actionLast(0),
      m_actionGo(0),
      //    m_actionExit(0),
      m_actionZP(0),
      m_actionZM(0),
      m_actionZ1(0),
      m_actNextReport(0),
      m_actPrevReport(0),
      m_actViewSingle(0),
      m_actViewDouble(0),
      m_actViewContinous(0),
      m_actionAbout(0),
      m_actionAboutQt(0),
//      m_actionShow(0),
      m_cbZoom(0),
      m_spZoom(0),
      m_spPage(0),

      m_currentPage(1),
      m_currentReport(1),

      m_initialZoomLevel(1.0),
      m_usePrintDialog(true),
      m_donePrint(false),
      m_donePDF(false),
      m_doneSVG(false),

      m_orgName(orgName),
      m_appName(appName),

      m_previewStyleSheetQss(
          "QScrollBar{ background: #E6E6E6; width:30px ;height:10px;margin-top:0px;margin-bottom:0px ;margin-left:0px;margin-right:0px}"
          "QScrollBar::handle:vertical{ background: #F8F8F8; min-height: 80px ;width:30px }"
          "QScrollBar::sub-line:vertical{height:0px;subcontrol-position:top;subcontrol-origin:margin;}"
          "QScrollBar::add-line:vertical{height:0px;subcontrol-position:bottom;subcontrol-origin:margin;}"
          "QScrollBar::handle:horizontal{background: #F8F8F8; height: 10px ;min-width:40px }"
          "QScrollBar::sub-line:horizontal{width:0px;subcontrol-position:top;subcontrol-origin:margin;}"
          "QScrollBar::add-line:horizontal{width:0px;subcontrol-position:bottom;subcontrol-origin:margin;}"
          )
{

    //m_winTitle = QString("%1 %2 %3 preview").arg(NCREPORTAPP_NAME).arg(NCREPORTAPP_VERSION).arg(NCREPORTAPP_RELEASENAME);
    m_winTitle = QString("Report preview");
    m_defaultOutputFileName = "report";

    setWindowTitle( m_winTitle );
    //setWindowIcon( QIcon(":/images/app.png")  );

    //scrollArea->setBackgroundRole(QPalette::Dark);
    m_scrollArea->setBackgroundRole(QPalette::Light);
    m_scrollArea->setAlignment( Qt::AlignCenter );

    m_scrollArea->setWidget(m_pageContainer);
    setCentralWidget(m_scrollArea);

    m_pageContainer->setShowType( NCReportPreviewPageContainer::Continous );

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    readSettings();

    m_scrollArea->ensureVisible (0, 0, 0, 0);
    connect( m_scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(sliderMoved(int)));

    m_scrollArea->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);//去除scrollArea垂直滚动条的右击菜单
    m_scrollArea->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);//去除scrollArea水平滚动条的右击菜单

    m_scrollArea->setStyleSheet(m_previewStyleSheetQss);

    refreshActions();
}

NCReportPreviewWindow::~NCReportPreviewWindow()
{
    if (m_output)
        delete m_output;
}

void NCReportPreviewWindow::setOutput(NCReportPreviewOutput *o )
{
    if (m_output)
        delete m_output;
    m_output = o;

    m_pageContainer->setOutput(o);

    m_spPage->setMaximum( pageCount() );
    m_spPage->setSuffix( QString("/%1").arg(pageCount()) );
    loadPage(1, Top);
    if (m_initialZoomLevel != 1.0 )
        zoom(m_initialZoomLevel);
}

void NCReportPreviewWindow::showEvent(QShowEvent * /*event*/)
{
#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
    setWindowModality(Qt::NonModal);
#endif
}

void NCReportPreviewWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool NCReportPreviewWindow::save()
{
    if (m_curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(m_curFile);
    }
}

bool NCReportPreviewWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void NCReportPreviewWindow::about()
{
    //    QMessageBox::about(this, tr("About"), QString("Report Preview") + "\n" + NCReport::info() + "\n" + NCREPORTAPP_WEB );
    QMessageBox::about(this, tr("About"), QString("Report Preview") + "\n" + NCReport::info() /*+ "\n" + NCREPORTAPP_WEB */);
}

void NCReportPreviewWindow::documentWasModified()
{
    setWindowModified(true);
}

void NCReportPreviewWindow::createActions()
{
    m_actionOpen= new QAction( this);
    m_actionOpen->setText(tr("Open report file"));
    m_actionOpen->setShortcut(tr("Ctrl+O"));
    m_actionOpen->setStatusTip(tr("Open NCReport Intermediate report file"));
    m_actionOpen->setIcon( QIcon(":/images/fileopen.png") );
    connect(m_actionOpen, SIGNAL(triggered()), this, SLOT(open()));
    m_actionOpen->setEnabled(false);

    m_actionAbout = new QAction( this);
    m_actionAbout->setText(tr("&About"));
    m_actionAbout->setStatusTip(tr("Show the application's About box"));
    connect(m_actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    m_actionAboutQt = new QAction(this);
    m_actionAboutQt->setText(tr("About &Qt"));
    m_actionAboutQt->setStatusTip(tr("Show the Qt library's About box"));
    connect(m_actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    ///////////////////////////
    m_actionPrint = new QAction( this );
    m_actionPrint->setText( tr("Print the report") );
    m_actionPrint->setIconText( tr("Print") );
    m_actionPrint->setIcon( QIcon(":/images/printer.png") );
    m_actionPrint->setToolTip( tr("Print") );
    m_actionPrint->setShortcut( Qt::CTRL + Qt::Key_P );
    connect( m_actionPrint, SIGNAL( triggered() ), this, SLOT( activatePrint() ) );
#if !PREVIEW_PRINT_ACTION
    m_actionPrint->setVisible(false);
#endif

    m_actionPdf = new QAction( this );
    m_actionPdf->setText( tr("Print report to PDF file") );
    m_actionPdf->setIconText( tr("PDF") );
    m_actionPdf->setIcon( QIcon(":/images/pdf.png") );
    m_actionPdf->setToolTip( tr("Print to PDF file") );
    connect( m_actionPdf, SIGNAL( triggered() ), this, SLOT( activatePdf() ) );
#if !PREVIEW_PDF_ACTION
    m_actionPDF->setVisible(false);
#endif

    m_actionSvg = new QAction( this );
    m_actionSvg->setText( tr("Print report to SVG files") );
    m_actionSvg->setIconText( tr("SVG") );
    m_actionSvg->setIcon( QIcon(":/images/svg.png") );
    m_actionSvg->setToolTip( tr("Print to SVG files") );
    connect( m_actionSvg, SIGNAL( triggered() ), this, SLOT( activateSvg()) );
#if !PREVIEW_SVG_ACTION
    m_actionSVG->setVisible(false);
#endif

    m_actionZP = new QAction( this );
    m_actionZP->setText( tr("Zoom in the page") );
    m_actionZP->setIconText( tr("Zoom in") );
    m_actionZP->setIcon( QIcon(":/images/zoom_in.png")  );
    m_actionZP->setToolTip( tr("Zoom in page") );
    m_actionZP->setShortcut( Qt::Key_Plus );
    connect( m_actionZP, SIGNAL( triggered() ), this, SLOT( zoomPlus() ) );

    m_actionZM = new QAction( this );
    m_actionZM->setText( tr("Zoom out page") );
    m_actionZM->setIconText( tr("Zoom out") );
    m_actionZM->setIcon( QIcon(":/images/zoom_out.png") );
    m_actionZM->setToolTip( tr("Zoom out") );
    m_actionZM->setShortcut( Qt::Key_Minus );
    connect( m_actionZM, SIGNAL( triggered() ), this, SLOT( zoomMinus() ) );

    m_actionZ1 = new QAction( this );
    m_actionZ1->setText( tr("Original page size") );
    m_actionZ1->setIconText( tr("1:1") );
    m_actionZ1->setIcon( QIcon(":/images/zoom.png") );
    m_actionZ1->setToolTip( tr("1:1 size") );
    m_actionZ1->setShortcut( Qt::Key_0 );
    connect( m_actionZ1, SIGNAL( triggered() ), this, SLOT( zoom1() ) );

    m_actionFrst = new QAction( this );
    m_actionFrst->setText( tr("First page") );
    m_actionFrst->setIconText( tr("First") );
    m_actionFrst->setIcon( QIcon(":/images/first.png") );
    m_actionFrst->setToolTip( tr("Go to first page") );
    m_actionFrst->setShortcut( Qt::CTRL+Qt::Key_Home );
    connect( m_actionFrst, SIGNAL( triggered() ), this, SLOT( firstPage() ) );

    m_actionPrev = new QAction( this );
    m_actionPrev->setText( tr("Previous page") );
    m_actionPrev->setIconText( tr("Previous") );
    m_actionPrev->setIcon( QIcon(":/images/previous.png") );
    m_actionPrev->setToolTip( tr("Go to previous page") );
    m_actionPrev->setShortcut( Qt::CTRL+Qt::Key_Left );
    connect( m_actionPrev, SIGNAL( triggered() ), this, SLOT( prevPage() ) );

    m_actionNext = new QAction( this );
    m_actionNext->setText( tr("Next page") );
    m_actionNext->setIconText( tr("Next") );
    m_actionNext->setIcon( QIcon(":/images/next.png") );
    m_actionNext->setToolTip( tr("Go to next page") );
    m_actionNext->setShortcut( Qt::CTRL+Qt::Key_Right );
    connect( m_actionNext, SIGNAL( triggered() ), this, SLOT( nextPage() ) );

    m_actionLast = new QAction( this );
    m_actionLast->setText( tr("Last page") );
    m_actionLast->setIconText( tr("Last") );
    m_actionLast->setIcon( QIcon(":/images/last.png") );
    m_actionLast->setToolTip( tr("Go to final page") );
    m_actionLast->setShortcut( Qt::CTRL+Qt::Key_End );
    connect( m_actionLast, SIGNAL( triggered() ), this, SLOT( lastPage() ) );

    m_actionGo = new QAction( this );
    m_actionGo->setText( tr("Go to page...") );
    m_actionGo->setIconText( tr("Go to") );
    m_actionGo->setIcon( QIcon(":/images/pagego.png") );
    m_actionGo->setToolTip( tr("Go to specified page") );
    m_actionGo->setShortcut( Qt::CTRL+Qt::Key_G );
    connect( m_actionGo, SIGNAL( triggered() ), this, SLOT( gotoPage() ) );

    //    m_actionExit = new QAction( this );
    //    m_actionExit->setText( tr("Close") );
    //   // actionExit->setIcon( QIcon(":/images/exit.png") );
    //    m_actionExit->setToolTip( tr("Exit preview") );
    //    m_actionExit->setShortcut( Qt::Key_Escape );
    //    connect( m_actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );

//    m_actionShow= new QAction( this );
//    m_actionShow->setText( tr("RunReport") );
//    m_actionShow->setToolTip( tr("Show preview") );
//    m_actionShow->setShortcut( Qt::Key_R );
//    connect( m_actionShow, SIGNAL( triggered() ), this, SLOT( runReport() ) );

    m_actNextReport = new QAction( this );
    m_actNextReport->setText( tr("Next report") );
    m_actNextReport->setIconText( tr("Next report") );
    m_actNextReport->setIcon( QIcon(":/images/nextreport.png") );
    m_actNextReport->setToolTip( tr("Go to next report") );
    connect( m_actNextReport, SIGNAL( triggered() ), this, SLOT(nextReport()) );

    m_actPrevReport = new QAction( this );
    m_actPrevReport->setText( tr("Previous report") );
    m_actPrevReport->setIconText( tr("Previous report") );
    m_actPrevReport->setIcon( QIcon(":/images/prevreport.png") );
    m_actPrevReport->setToolTip( tr("Go to previous report") );
    connect( m_actPrevReport, SIGNAL( triggered() ), this, SLOT(prevReport()) );


    //------------------ VIEW

    m_actViewSingle = new QAction( this );
    m_actViewSingle->setText( tr("One page") );
    m_actViewSingle->setToolTip( tr("One page mode") );
    m_actViewSingle->setCheckable(true);
    m_actViewSingle->setIcon( QIcon(":/images/page_single.png") );

    m_actViewDouble = new QAction( this );
    m_actViewDouble->setText( tr("Two pages") );
    m_actViewDouble->setToolTip( tr("Two pages mode") );
    m_actViewDouble->setCheckable(true);
    m_actViewDouble->setIcon( QIcon(":/images/page_double.png") );

    m_actViewContinous = new QAction( this );
    m_actViewContinous->setText( tr("Continous") );
    m_actViewContinous->setToolTip( tr("Continous mode") );
    m_actViewContinous->setCheckable(true);
    m_actViewContinous->setIcon( QIcon(":/images/page_continous.png") );

    m_actGrpView = new QActionGroup(this);
    m_actGrpView->addAction(m_actViewSingle);
    m_actGrpView->addAction(m_actViewDouble);
    m_actGrpView->addAction(m_actViewContinous);
    m_actViewSingle->setChecked(true);
    connect(m_actGrpView,SIGNAL(triggered(QAction*)),this,SLOT(viewChange(QAction*)));
}

void NCReportPreviewWindow::createMenus()
{
    //    mnFile = menuBar()->addMenu(tr("&File"));
    //    mnFile->addAction(m_actionOpen);
    //    mnFile->addAction(m_actionPrint);
    //    mnFile->addAction(m_actionPdf);
    //    mnFile->addAction(m_actionSvg);
    //    mnFile->addSeparator();
    //    mnFile->addAction(m_actionExit);

    //    mnView = menuBar()->addMenu(tr("&View"));
    //    mnView->addAction(m_actViewSingle);
    //    mnView->addAction(m_actViewDouble);
    //    mnView->addAction(m_actViewContinous);
    //    mnView->addSeparator();
    //    mnView->addAction(m_actionZ1);
    //    mnView->addAction(m_actionZP);
    //    mnView->addAction(m_actionZM);

    //    mnNav = menuBar()->addMenu(tr("&Navigate"));
    //    mnNav->addAction(m_actionFrst);
    //    mnNav->addAction(m_actionPrev);
    //    mnNav->addAction(m_actionNext);
    //    mnNav->addAction(m_actionLast);
    //    mnNav->addSeparator();
    //    mnNav->addAction(m_actNextReport);
    //    mnNav->addAction(m_actPrevReport);
    //    mnNav->addSeparator();
    //    mnNav->addAction(m_actionGo);

    //    menuBar()->addSeparator();

    //    mnAbout = menuBar()->addMenu(tr("&About"));
    //    mnAbout->addAction(m_actionAbout);
    //    mnAbout->addAction(m_actionAboutQt);

    // ----- TEST ------------
    //	QMenu *mnTest = menuBar()->addMenu(tr("&Tests"));
    //	mnTest->addAction("Test printing 1", this, SLOT(testPrint1()) );
    //	mnTest->addAction("Test printing 2", this, SLOT(testPrint2()) );
    //	mnTest->addAction("Clear", this, SLOT(clearPage()) );
    //mnTest->addAction("Load page 1 content", this, SLOT(clearPage()) );
}

void NCReportPreviewWindow::createToolBars()
{
//    tbFile = addToolBar(tr("File"));
//    tbFile->setObjectName("tbFile");
//    tbFile->addAction(m_actionPrint);
//    tbFile->addAction(m_actionPdf);
//    tbFile->addAction(m_actionSvg);
    //tbFile->addSeparator();

    tbView = addToolBar( tr("View") );
    tbView->setObjectName("tbView");
    tbView->addAction(m_actionZP);
    tbView->addAction(m_actionZM);
    tbView->addAction(m_actionZ1);
    tbView->addSeparator();

#ifdef SCALE_BY_COMBOBOX
    // Zoom combobox
    m_cbZoom =  new QComboBox( tbView );
    m_cbZoom->addItem( tr( "10 %" ));
    m_cbZoom->addItem( tr( "20 %" ));
    m_cbZoom->addItem( tr( "30 %" ));
    m_cbZoom->addItem( tr( "40 %" ));
    m_cbZoom->addItem( tr( "50 %" ));
    m_cbZoom->addItem( tr( "60 %" ));
    m_cbZoom->addItem( tr( "70 %" ));
    m_cbZoom->addItem( tr( "80 %" ));
    m_cbZoom->addItem( tr( "90 %" ));
    m_cbZoom->addItem( tr( "100 %" ));
    m_cbZoom->addItem( tr( "110 %" ));
    m_cbZoom->addItem( tr( "120 %" ));
    m_cbZoom->addItem( tr( "130 %" ));
    m_cbZoom->addItem( tr( "140 %" ));
    m_cbZoom->addItem( tr( "150 %" ));
    m_cbZoom->addItem( tr( "160 %" ));
    m_cbZoom->addItem( tr( "170 %" ));
    m_cbZoom->addItem( tr( "180 %" ));
    m_cbZoom->addItem( tr( "190 %" ));
    m_cbZoom->addItem( tr( "200 %" ));
    m_cbZoom->addItem( tr( "300 %" ));
    m_cbZoom->addItem( tr( "400 %" ));
    m_cbZoom->addItem( tr( "500 %" ));
    m_cbZoom->setFocusPolicy( Qt::NoFocus );
    m_cbZoom->setCurrentIndex( 9 );
    connect( m_cbZoom, SIGNAL(activated(QString)), SLOT( zoomChanged(QString)) );
    tbView->addWidget( m_cbZoom );
#endif
#ifdef SCALE_BY_SPINBOX
    spZoom = new QSpinBox(tbView);
    spZoom->setSuffix( tr(" %") );
    spZoom->setMinimum(10);
    spZoom->setMaximum(700);
    spZoom->setValue(100);
    spZoom->setSingleStep(5);
    spZoom->setFocusPolicy( Qt::ClickFocus );
    connect( spZoom, SIGNAL(valueChanged(int)), SLOT(zoomChanged(int)));
    //connect( spZoom, SIGNAL(editingFinished()), SLOT(zoomChanged()));
    tbView->addWidget( spZoom );
#endif

    tbNav = addToolBar( tr("Navigate") );
    tbNav->setObjectName("tbNav");
    tbNav->addAction(m_actionFrst);
    tbNav->addAction(m_actionPrev);

    m_spPage = new QSpinBox( tbNav );
    //m_spPage->setPrefix( tr("Page: ") );
    m_spPage->setPrefix( tr("") );
    m_spPage->setMinimum(1);
    m_spPage->setMinimumWidth(80);
    m_spPage->setFocusPolicy( Qt::ClickFocus );
    connect( m_spPage, SIGNAL(valueChanged(int)), SLOT(loadPage(int)) );

    tbNav->addWidget( m_spPage );
    tbNav->addAction(m_actionNext);
    tbNav->addAction(m_actionLast);
    tbNav->addSeparator();
//    tbNav->addAction(m_actionGo);
//    tbNav->addAction(m_actPrevReport);
//    tbNav->addAction(m_actNextReport);
//    tbNav->addSeparator();
    tbNav->addAction(m_actViewSingle);
//    tbNav->addAction(m_actViewDouble);
    tbNav->addAction(m_actViewContinous);
    m_actViewContinous->setChecked(true);
    tbNav->addSeparator();
    //    tbNav->addAction( m_actionExit );
//    tbNav->addAction( m_actionShow );

    //tbFile->setIconSize(QSize(24,24));
    tbView->setIconSize(QSize(24,24));
    tbView->setMovable(false);
    tbNav->setIconSize(QSize(24,24));
    tbNav->setMovable(false);

}

void NCReportPreviewWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
    statusBar()->hide();
}

void NCReportPreviewWindow::readSettings()
{
    if (m_orgName.isEmpty() || m_appName.isEmpty()) {
        QSettings settings;
        readSettings( &settings );
    } else {
        QSettings settings(m_orgName, m_appName);
        readSettings( &settings );
    }
}

void NCReportPreviewWindow::readSettings(QSettings *settings)
{
    QByteArray state = settings->value("preview/state", QByteArray() ).toByteArray();
    QByteArray geometry = settings->value( "preview/geometry" ).toByteArray();
    m_lastSelectedDir = settings->value("preview/lastdir",".").toString();
    QString _qss = settings->value("preview/qss","").toString();
    if(!_qss.isEmpty())
        m_previewStyleSheetQss = _qss;
#ifdef PREVIEW_SAVE_MODE
    int viewMode = settings->value( "preview/viewmode", -1 ).toInt();
    m_initialZoomLevel = settings->value( "preview/zoomlevel", 1.0 ).toDouble();
#else
    int viewMode =0;
    initialZoomLevel = 1.0;
#endif

    if ( geometry.isEmpty() ) {
        showMaximized();
    } else {
        restoreGeometry( geometry );
        restoreState( state );
    }
    switch (viewMode) {
    case -1:
    case 0:	break;
    case 1: setViewDouble(); break;
    case 2: setViewContinous(); break;
    }

}

void NCReportPreviewWindow::writeSettings()
{
    if (m_orgName.isEmpty() || m_appName.isEmpty()) {
        QSettings settings;
        writeSettings( &settings );
    } else {
        QSettings settings(m_orgName, m_appName);
        writeSettings( &settings );
    }
}

void NCReportPreviewWindow::writeSettings(QSettings *settings)
{
    settings->setValue( "preview/geometry", saveGeometry());
    settings->setValue( "preview/state", saveState());
    settings->setValue( "preview/lastdir", m_lastSelectedDir);
    settings->setValue( "preview/qss", m_previewStyleSheetQss);
#ifdef PREVIEW_SAVE_MODE
    settings->setValue( "preview/viewmode", (int)showType() );
    settings->setValue( "preview/zoomlevel", m_pageContainer->currentScaleFactor() );
#endif
}


void NCReportPreviewWindow::zoomChanged(const QString & s)
{
    bool ok=false;
    qreal scale = s.left(3).toDouble(&ok);
    if ( !ok )
        return;
    zoom( scale/100 );
}


void NCReportPreviewWindow::zoomChanged()
{
    zoom(((qreal)m_spZoom->value())/100);
}

void NCReportPreviewWindow::zoomChanged(int scaleFactor)
{
    zoom(((qreal)scaleFactor)/100);
}


void NCReportPreviewWindow::zoom(qreal z)
{
    if (  z>7 || z <0.1 )
        return;
    m_pageContainer->scale(z);
    updateZoomInfo();
}

void NCReportPreviewWindow::zoomPlus()
{
    m_pageContainer->scale(m_pageContainer->currentScaleFactor()+0.1);
    updateZoomInfo();
}

void NCReportPreviewWindow::zoomMinus()
{
    m_pageContainer->scale(m_pageContainer->currentScaleFactor()-0.1);
    updateZoomInfo();
}

void NCReportPreviewWindow::zoom1()
{
    m_pageContainer->scale(1.0);
    updateZoomInfo();
}

void NCReportPreviewWindow::firstPage(const PagePosition pp )
{
    loadPage( 1, pp );
}
void NCReportPreviewWindow::nextPage(const PagePosition pp)
{
    if ( m_currentPage < pageCount() )
        loadPage( m_currentPage+1, pp );
}
void NCReportPreviewWindow::prevPage(const PagePosition pp)
{
    if ( m_currentPage > 1 )
        loadPage( m_currentPage-1, pp );
}
void NCReportPreviewWindow::lastPage(const PagePosition pp)
{
    loadPage( pageCount(), pp );
}

void NCReportPreviewWindow::loadPage(int pageno )
{
    //if ( currentPage == pageno )
    //	return;

    loadPage( pageno, Top );
}

void NCReportPreviewWindow::loadPage(int pageno, const PagePosition pp )
{
    if ( !m_output )
        return;

    switch( m_pageContainer->showType()) {
    case NCReportPreviewPageContainer::Single:
    case NCReportPreviewPageContainer::Double: {
        //			if (output->batchMode())
        //                m_pageContainer->setAllPageSize( output->pageSizeMM(pageno) );
        m_pageContainer->loadPage( pageno );
        switch ( pp ) {
        case Top:
            m_scrollArea->ensureVisible(0, 0, 0, 0);
            break;
        case Bottom:
            m_scrollArea->verticalScrollBar()->setValue( m_scrollArea->verticalScrollBar()->maximum() );
            break;
        case Off:
            break;
        }
        break;
    }
    case NCReportPreviewPageContainer::Continous: {
        m_pageContainer->loadPage( pageno );
        if (pageno==1)
            m_scrollArea->verticalScrollBar()->setValue( m_scrollArea->verticalScrollBar()->minimum() );
        else
            m_scrollArea->ensureWidgetVisible(m_pageContainer->page(pageno-1));
        break;
    }
    }

    m_currentPage = pageno;
    refreshActions();
}

void NCReportPreviewWindow::gotoPage()
{
    bool ok = false;
#if QT_VERSION>=0x050000
    int pg = QInputDialog::getInt( this, tr( "Go to page" ), tr( "Page number:" ), 1, 1, pageCount(), 1, &ok );
#else
    int pg = QInputDialog::getInteger( this, tr( "Go to page" ), tr( "Page number:" ), 1, 1, pageCount(), 1, &ok );
#endif

    if ( ok ) {
        loadPage( pg, Top );
    }
}

void NCReportPreviewWindow::gotoPage(int pageno)
{
    if (pageno>0 && pageno <= pageCount())
        loadPage( pageno, Top );
}


bool NCReportPreviewWindow::maybeSave()
{
    return true;
}

void NCReportPreviewWindow::loadFile(const QString &)
{
}

bool NCReportPreviewWindow::saveFile(const QString &fileName)
{
    Q_UNUSED(fileName);
    return true;
}

bool NCReportPreviewWindow::exportTo(const QString &fileName, const QString& password/* = ""*/, const QString& perm/* = ""*/)
{
    QFileInfo fi(fileName);
    QString ext = fi.suffix();
    if(ext == "pdf")
    {
        saveAsPdf(fileName, password/* = ""*/, perm/* = ""*/);
    }
    else if(ext == "svg")
    {
        saveAsSvg(fileName);
    }
    else if(ext == "print")
    {
        QString printName = fi.baseName();
        setPrinterName(printName);
        print();
    }
    return true;
}

void NCReportPreviewWindow::setCurrentFile(const QString &fileName)
{
    //static int sequenceNumber = 1;
    setWindowModified(false);
    setWindowTitle( tr("%1 - %2").arg(strippedName(fileName)).arg(m_winTitle) );
}


QString NCReportPreviewWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString NCReportPreviewWindow::genOutputFileName( const QString& extension ) const
{
    QString fn(m_defaultOutputFileName);
    if (fn.isEmpty())
        fn = "report";

    if (fn.contains("{DT}"))
        fn.replace("{DT}", QDateTime::currentDateTime().toString());

    return m_lastSelectedDir+(m_lastSelectedDir.isEmpty() ? "" : "/")+fn+"." +extension;
}

void NCReportPreviewWindow::closeEvent(QCloseEvent *event)
{
    Q_D(NCReportPreviewWindow);

    if (!isEnabled())
    {
        event->ignore();
        return;
    }

    writeSettings();
    QMainWindow::closeEvent(event);
    if (d->eventLoop)
        d->eventLoop->exit();
}

void NCReportPreviewWindow::clearPage()
{

}

void NCReportPreviewWindow::sliderMoved(int value)
{
    if (m_pageContainer->showType() == NCReportPreviewPageContainer::Continous ) {
        int sliderMax = m_scrollArea->verticalScrollBar()->maximum();
        if (sliderMax==0)
            m_currentPage = 1;
        else
            m_currentPage = value*pageCount()/sliderMax + 1;

        if (m_currentPage>pageCount())
            m_currentPage = pageCount();

        m_spPage->blockSignals(true);
        m_spPage->setValue(m_currentPage);
        m_spPage->blockSignals(false);

        refreshActions();
    }

}

void NCReportPreviewWindow::runReport()
{
    emit sigrunReport();
}

int NCReportPreviewWindow::pageCount() const
{
    if ( !m_output )
        return 0;
    return m_output->pages().count();
}

int NCReportPreviewWindow::reportCount() const
{
    if ( !m_output )
        return 0;
    return m_output->startPagesOfReports().count();
}


void NCReportPreviewWindow::refreshActions()
{
    m_actionFrst->setEnabled( m_output && m_currentPage >1 );
    m_actionPrev->setEnabled( m_output && m_currentPage >1 );
    m_actionNext->setEnabled( m_output && m_currentPage < pageCount() );
    m_actionLast->setEnabled( m_output && m_currentPage < pageCount() );

    m_actNextReport->setEnabled( m_output && m_currentReport < reportCount() );
    m_actPrevReport->setEnabled( m_output && m_currentReport > 1 );

    statusBar()->showMessage(tr("Page: %1/%2").arg(m_currentPage).arg(pageCount()) );
    if ( m_spPage->value() != m_currentPage ) {
        m_spPage->blockSignals(true);
        m_spPage->setValue( m_currentPage );
        m_spPage->blockSignals(false);
    }
}

void NCReportPreviewWindow::initProgress( QProgressDialog* pdia )
{
    pdia->setLabelText( tr("Printing report...") );
    pdia->setWindowModality(Qt::WindowModal);
    connect( m_report, SIGNAL(dataRowCount(int)), pdia, SLOT(setMaximum(int)) );
    connect( m_report, SIGNAL(dataRowProgress(int)), pdia, SLOT(setValue(int)) );
    connect( m_report, SIGNAL(pageProgress(QString)), pdia, SLOT(setLabelText(QString)) );
    connect( pdia, SIGNAL(canceled()), m_report, SLOT(cancel()) );
}

void NCReportPreviewWindow::activatePrint()
{
    if ( m_report ) {
        //QProgressDialog progress( this );
        //initProgress( &progress );
        m_report->reset(false);
        m_report->runReportToPrinter( 1, m_usePrintDialog, this, m_printerName );
        m_donePrint = m_report->reportDone();
        if (m_donePrint)
            emit runPrinterDone();
    } else {
        QPrinter printer( QPrinter::HighResolution );
        if (!m_printerName.isEmpty())
            printer.setPrinterName(m_printerName);
        initPrinter( &printer );

        if (m_usePrintDialog) {
            QPrintDialog printDialog(&printer, this);
            if (printDialog.exec() != QDialog::Accepted) {
                return;
            }
        }

        printPicturePages( &printer );
        m_donePrint = true;
    }
}

void NCReportPreviewWindow::print()
{
    activatePrint();
}

void NCReportPreviewWindow::activatePdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save PDF File"), genOutputFileName("pdf"), tr("Pdf files (*.pdf)"));
    if ( fileName.isEmpty() )
        return;

    setDisabled(true);

    saveAsPdf(fileName);
    setDisabled(false);
}

void NCReportPreviewWindow::saveAsPdf(QString fileName, const QString& password/* = ""*/, const QString& perm/* = ""*/)
{
    QFileInfo fi(fileName);
    m_lastSelectedDir = fi.absolutePath();

    if ( m_report ) {
        //QProgressDialog progress( this );
        //initProgress( &progress );
#ifndef NCREPORT_NO_CURSOR
        QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
#endif
        m_report->reset();
        if(password.isEmpty() && perm.isEmpty())
            m_report->runReportToPDF( fileName );
        else
            m_report->runReportToPDFWithPassword(fileName, password, perm);
#ifndef NCREPORT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        m_donePDF = m_report->reportDone();
        if (m_donePDF)
            emit runPdfDone();
    } else {
        QPrinter printer( QPrinter::ScreenResolution );
        initPrinter( &printer );
        printer.setOutputFormat ( QPrinter::PdfFormat );
        printer.setOutputFileName( fileName )	;
        printPicturePages( &printer );
        m_donePDF = true;
    }
}


void NCReportPreviewWindow::activateSvg()
{
#ifndef NCREPORT_NO_SVG
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save to SVG File(s)"), genOutputFileName("svg"), tr("Svg files (*.svg)"));
    if ( fileName.isEmpty() )
        return;

    saveAsSvg(fileName);
}

void NCReportPreviewWindow::saveAsSvg(QString fileName)
{
    QFileInfo fi(fileName);
    m_lastSelectedDir = fi.absolutePath();

    if ( m_report ) {
#ifndef NCREPORT_NO_CURSOR
        QApplication::setOverrideCursor( QCursor( Qt::WaitCursor ) );
#endif
        m_report->reset();
        m_report->runReportToSVG( fileName );
#ifndef NCREPORT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        m_doneSVG = m_report->reportDone();
        if (m_doneSVG)
            emit runSvgDone();
    }
#endif
}

void NCReportPreviewWindow::initPrinter( QPrinter* printer)
{
    printer->setOrientation( m_output->option().pageOrientation() );
    printer->setFullPage( true );
    printer->setPaperSize( m_output->option().paperSize() );
}

void NCReportPreviewWindow::printPicturePages( QPrinter* printer )
{
    NCReportScale sc;
    sc.setDpi(printer->resolution());
    QPainter p(printer);
    int counter=0;
    for ( int i=0; i<m_output->pages().count(); ++i ) {
        bool outOfRange = (i+1 < printer->fromPage() || i+1 > printer->toPage());
        if ( printer->printRange()==QPrinter::PageRange && outOfRange )
            continue;

        if (counter>0)
            printer->newPage();

        QPicture picture;
        QByteArray content = m_output->pages().at(i);
        NCReportPreviewPage::loadPicture(picture, content);
        p.drawPicture( 0,0, picture );
        counter++;
    }
}

void NCReportPreviewWindow::updateZoomInfo()
{
#ifdef SCALE_BY_COMBOBOX
    QString txt = QString::number( m_pageContainer->currentScaleFactor()* 100, 'f',0 )+ " %";
    m_cbZoom->setCurrentIndex( m_cbZoom->findText(txt) );
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug( "m_pageContainer->currentScaleFactor() text = %s", qPrintable(txt) );
#endif
#endif
#ifdef SCALE_BY_SPINBOX
    spZoom->blockSignals(true);
    spZoom->setValue( qRound(m_pageContainer->currentScaleFactor()* 100) );
    spZoom->blockSignals(false);
#endif
}

void NCReportPreviewWindow::setReport( NCReport* r )
{
    m_report = r;
}

NCReport *NCReportPreviewWindow::report() const
{
    return m_report;
}

void NCReportPreviewWindow::nextReport()
{
    if ( m_currentReport < reportCount() )
        loadPage( m_output->startPagesOfReports().at(++m_currentReport-1), Top );
}

void NCReportPreviewWindow::prevReport()
{
    if ( m_currentReport > 1 )
        loadPage( m_output->startPagesOfReports().at(--m_currentReport-1), Top );
}


void NCReportPreviewWindow::viewChange(QAction *action)
{
    if (action==m_actViewSingle)
        setViewSingle();
    else if (action==m_actViewDouble)
        setViewDouble();
    else if (action==m_actViewContinous)
        setViewContinous();
}


void NCReportPreviewWindow::setViewSingle()
{
    m_pageContainer->setShowType( NCReportPreviewPageContainer::Single );
    m_actViewSingle->setChecked(true);
    loadPage(m_currentPage);
}

void NCReportPreviewWindow::setViewDouble()
{
    m_pageContainer->setShowType( NCReportPreviewPageContainer::Double );
    m_actViewDouble->setChecked(true);
    loadPage(m_currentPage);
}

void NCReportPreviewWindow::setViewContinous()
{
    m_pageContainer->setShowType( NCReportPreviewPageContainer::Continous );
    m_actViewContinous->setChecked(true);
    loadPage(m_currentPage);
}

void NCReportPreviewWindow::setDefaultOutputDir(const QString &dir)
{
    m_lastSelectedDir = dir;
}

QString NCReportPreviewWindow::defaultOutputDir() const
{
    return m_lastSelectedDir;
}

void NCReportPreviewWindow::setPrinterName(const QString &printer)
{
    m_printerName = printer;
}

QString NCReportPreviewWindow::printerName() const
{
    return m_printerName;
}

void NCReportPreviewWindow::setUsePrintDialog(bool set)
{
    m_usePrintDialog = set;
}

bool NCReportPreviewWindow::usePrintDialog() const
{
    return m_usePrintDialog;
}

NCReportPreviewPageContainer::ShowType NCReportPreviewWindow::showType() const
{
    return m_pageContainer->showType();
}

void NCReportPreviewWindow::setCurrentPage(int pageno, NCReportPreviewPageContainer::ShowType type)
{
    m_pageContainer->setShowType( type );
    QAction *action=0;
    switch (type) {
    case NCReportPreviewPageContainer::Single: action=m_actViewSingle; break;
    case NCReportPreviewPageContainer::Double: action=m_actViewDouble; break;
    case NCReportPreviewPageContainer::Continous: action=m_actViewContinous; break;
    }
    action->setChecked(true);
    loadPage(pageno);
}

QToolBar *NCReportPreviewWindow::toolBarFile()
{
    return tbFile;
}

QToolBar *NCReportPreviewWindow::toolBarView()
{
    return tbView;
}

QToolBar *NCReportPreviewWindow::toolBarNavigation()
{
    return tbNav;
}

void NCReportPreviewWindow::exec()
{
    Q_D(NCReportPreviewWindow);
    show();
    QEventLoop eventLoop;
    d->eventLoop = &eventLoop;
    eventLoop.exec();
    d->eventLoop = NULL;
}

void NCReportPreviewWindow::setDefaultOutputFileName(const QString &fileName )
{
    m_defaultOutputFileName = fileName;
}

QString NCReportPreviewWindow::defaultOutputFileName() const
{
    return m_defaultOutputFileName;
}

void NCReportPreviewWindow::setSettingsData(const QString &orgName, const QString &appName)
{
    m_orgName = orgName;
    m_appName = appName;
}



//------------------------------------------------------------------------
// TESTS
//------------------------------------------------------------------------
void NCReportPreviewWindow::addTestItems()
{
}

void NCReportPreviewWindow::testPrint1()
{
}

void NCReportPreviewWindow::testPrint2()
{
}

//-----------------------------------------------------------------------------------------

NCReportPreviewScrollArea::NCReportPreviewScrollArea( NCReportPreviewWindow * parent)
    : QScrollArea( parent ), pWindow( parent )
{
}

NCReportPreviewScrollArea::~ NCReportPreviewScrollArea()
{
}

void NCReportPreviewScrollArea::keyPressEvent(QKeyEvent * e)
{
    switch (pWindow->showType())
    {
    case NCReportPreviewPageContainer::Single:
    {
        if ( e->key() == Qt::Key_PageDown && verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum() )
            pWindow->nextPage( NCReportPreviewWindow::Top );
        else if ( e->key() == Qt::Key_PageUp && verticalScrollBar()->sliderPosition() == verticalScrollBar()->minimum() )
            pWindow->prevPage( NCReportPreviewWindow::Bottom );
        else if ( e->key() == Qt::Key_Home )
            ensureVisible(0, 0, 0, 0);
        else if ( e->key() == Qt::Key_End )
            verticalScrollBar()->setValue( verticalScrollBar()->maximum() );
        else
            QScrollArea::keyPressEvent(e);

        break;
    }
    case NCReportPreviewPageContainer::Double:
    {
        if ( pWindow->pageCount() % 2 == 1 && pWindow->pageCount() ) {

        }

        if ( e->key() == Qt::Key_PageDown && verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum() )
            pWindow->nextPage( NCReportPreviewWindow::Top );
        else if ( e->key() == Qt::Key_PageUp && verticalScrollBar()->sliderPosition() == verticalScrollBar()->minimum() )
            pWindow->prevPage( NCReportPreviewWindow::Bottom );
        else if ( e->key() == Qt::Key_Home )
            ensureVisible(0, 0, 0, 0);
        else if ( e->key() == Qt::Key_End )
            verticalScrollBar()->setValue( verticalScrollBar()->maximum() );
        else
            QScrollArea::keyPressEvent(e);

        break;
    }
    case NCReportPreviewPageContainer::Continous:
    {
        if ( e->key() == Qt::Key_Home )
            ensureVisible(0, 0, 0, 0);
        else if ( e->key() == Qt::Key_End )
            verticalScrollBar()->setValue( verticalScrollBar()->maximum() );
        else
        {
            QScrollArea::keyPressEvent(e);
            //pWindow->sliderMoved(verticalScrollBar()->value());
        }

        break;
    }
    }
}


