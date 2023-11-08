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
#include "ncrdmainwindow.h"
#include "ncreport.h"
#include "ncreportsource.h"
#include "ncrd_global.h"
#include "ncrdgraphitemtestdialog.h"
#include "ncreportutils.h"
#include "ncrdtestfielddemo.h"

#include <QApplication>
#include <QSplashScreen>
#include <QFont>
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>
#include <QAction>
#include <QDirIterator>

#include "ncrddemodata.h"

#include <QFile>

/*!
Designer's main fuction
*/
int main(int argc, char *argv[] )
{
    QApplication app(argc, argv);

    QString m_locale = QLocale::system().name();
    QString m_openDoc;
    QString m_settingsOrg(NCREPORTAPP_ORG);
    QString m_settingsName(NCRDSETTINGS_APP);
    bool m_printOn=false;

    QStringList arguments = app.arguments();
    arguments.takeFirst();      // remove the first argument, which is the program's name
    if (!arguments.isEmpty() ) {

        QStringList::const_iterator i;
        for (i = arguments.constBegin(); i != arguments.constEnd();  ++i) {
               if (!(*i).startsWith('-') )
                   m_openDoc = arguments.at(0);

               if ( (*i).startsWith("--tr=") )
                   m_locale = (*i).mid(5);
               else if ( (*i).startsWith("--appname=") )
                   m_settingsName = (*i).mid(10);
               else if ( (*i).startsWith("--orgname=") )
                   m_settingsOrg = (*i).mid(10);
               else if ( (*i) == "-print" || (*i)=="--print" )
                   m_printOn = true;
        }
    }

    app.setApplicationName( REPORTDESIGNER_NAME );
#if (QT_VERSION >= 0x040400)
    app.setApplicationVersion( REPORTDESIGNER_VERSION );
#endif
    app.setOrganizationName( m_settingsOrg );
    app.setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    app.setApplicationName( m_settingsName );

    QTranslator translator;
    translator.load(QString("ncreport_") + m_locale, "i18n" );
    app.installTranslator(&translator);

    // Qt translator
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + m_locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

#ifdef DESIGNER_SPLASH
//    QPixmap pixmap(":/designer/images/splash.png");
//    QSplashScreen splash(pixmap);
//    splash.show();
//    splash.showMessage( QObject::tr("v%1 / Qt%2").arg(REPORTDESIGNER_VERSION).arg(QT_VERSION_STR), Qt::AlignLeft | Qt::AlignBottom );
//    app.processEvents();
#endif

#ifdef Q_WS_X11
    app.setWindowIcon( QIcon(":/designer/images/ncreportdesigner48.png") );
#endif

    //NCReport *report = new NCReport(&app);
//    NCRDMainWindow window;
    NCRDMainWindow::getInstance();
    //window.setReport( report );

#ifdef NCRD_CUSTOM_DATASOURCE_DEMO
    window.addCustomDataSource(createCustomTestData(&window));
#endif
#ifdef NCRD_ITEMMODEL_DEMO
    window.addItemModel( "model1", NCRDDemoData::createDemoItemModel() );
#endif

#ifdef NCRD_DATABASE_MODE_DEMO
    //-------------------------------------------
    // LOAD FROM/SAVE TO DATABASE IN DESIGNER
    //-------------------------------------------
    window.setDefaultConnectionID( "northwind" );
    window.setDefaultDBTable("reports");
    window.setDefaultDBColumn("reportdef");
    window.setDefaultDBKeyColumn("id");
    window.setDefaultDBKeyValue("R0");
#endif

#ifdef NCRD_CUSTOM_REPORTSOURCE_DEMO
    NCReportSource rSource;
    rSource.setFileName("<CUSTOM REPORT SOURCE>");
    rSource.setSourceType(NCReportSource::String);
    QString reportDef;
    NCReportUtils::fileToString( "TextDocumentReport1.xml", reportDef );
    rSource.setReportDefXML(reportDef);

    window.open(rSource);
#endif

#ifdef NCRD_GRAPHITEMDIALOG_DEMO
    // CUSTOMIZE GRAPH ITEM
    NCRDTestDialogFactory dfactory;
    NCRDTestGraphRenderer renderer750;
    renderer750.setId(750);

    NCRDTestGraphRenderer renderer751;
    renderer751.setId(751);

    window.actionGraph()->setIcon( QIcon(":/designer/images/shape_group.png") );
    window.setGraphItemDialogFactory( &dfactory );

    window.addGraphItemRenderer(&renderer750);
    window.addGraphItemRenderer(&renderer751);

    // CREATE ADDITIONAL CUSTOM ITEM WITH CUSTOM ACTION
    window.addCustomToolAction( 750, new QAction(QIcon(":/designer/images/categories.png"),"My button 1", &window), window.toolBarTools(), window.menuTools() );
    window.addCustomToolAction( 751, new QAction(QIcon(":/designer/images/category.png"),"My button 2", &window), window.toolBarTools(), window.menuTools() );

    // USER FUNCTION TEST
    NCRDTestFunction f;
    window.addUserFunction("F1", &f);

#endif

#ifdef NCRD_CUSTOMFIELDITEMDIALOG_DEMO
    NCRDTestFieldDemoWidgetFactory factory;
    window.setFieldItemDialogWidgetFactory(&factory);

    // CREATE ADDITIONAL CUSTOM ITEM WITH CUSTOM ACTION
    window.addCustomToolAction( 1024, new QAction(QIcon(":/designer/images/comment.png"),"My 1024 field", &window), window.toolBarTools(), window.menuTools() );
    window.addCustomToolAction( 1025, new QAction(QIcon(":/designer/images/comment_twitter.png"),"My 1025 field", &window), window.toolBarTools(), window.menuTools() );

#endif

//    if (!m_openDoc.isEmpty())
//        window.open(m_openDoc);

//    window.show();

//    if ( m_printOn )
//        window.runReportPrint();

    if (!m_openDoc.isEmpty())
        NCRDMainWindow::getInstance()->open(m_openDoc);

//    NCRDMainWindow::getInstance()->setcurLayout(1);
    NCRDMainWindow::getInstance()->show();

    if ( m_printOn )
        NCRDMainWindow::getInstance()->runReportPrint();

#ifdef QT_DEBUG
    QDirIterator it(":/designer/images", QDirIterator::Subdirectories);
    while (it.hasNext())
        qDebug("%s",qPrintable(it.next()));
#endif

#ifdef DESIGNER_SPLASH
//    splash.finish(&window);
#endif
    return app.exec();
}



