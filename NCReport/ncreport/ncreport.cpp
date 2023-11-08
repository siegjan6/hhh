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
#include "ncreport.h"
#include "ncreportdef.h"
#include "ncreportoutput.h"
#include "ncreportprinteroutput.h"
#include "ncreportpdfoutput.h"
#include "ncreporthtmloutput.h"
#include "ncreportexcelhtmloutput.h"
#include "ncreportimageoutput.h"
#include "ncreporttextoutput.h"
#include "ncreportdirector.h"
#include "info.h"
#include "ncreportabstractitemrendering.h"
#include "ncreportxmldefreader.h"
#include "ncreportusercustomfunction.h"
//#include <QtPdf>
//#include <QPdfDocument>
//#include <QPdfWriter>
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QTemporaryFile>

#ifndef NCREPORT_NO_SVG
#include "ncreportsvgoutput.h"
#endif

#ifndef NCREPORT_NO_EMAIL
#include "email/mailsender.h"
#endif

#ifndef NCREPORT_NO_PREVIEW_WINDOW
#include "ncreportpreviewwindow.h"
#include "ncreportpreviewoutput.h"
#include "ncreportqtpreviewoutput.h"
#include "ncreportdataviewdatasource.h"

#if (QT_VERSION >= 0x040400)
#include <QPrintPreviewDialog>
#endif

#endif

#include <QAbstractItemModel>
#include <QApplication>
#include <QTime>
#include <QInputDialog>
#include <QFile>

/*! \mainpage NCReport Report Generator Library
 *
 * \section intro_sec Introduction
 *
 * NCReport is a lightweight, fast, multi-platform and easy to use report engine library, report generator written in C++ based on Qt toolkit.
 * This documentation covers the report engine API.
 *
  */

NCReport::NCReport( QObject * parent ) : QObject( parent ),
    m_reportDef(new NCReportDef()),
    m_director(0),
    m_output(0),
    m_forcedCopy(1),
    m_useWaitCursor(true),
    m_reportType(Report),
    m_document(0),
    m_cancelled(false),
    m_elapsed(0),
    m_pageProgressText(tr("Report is running (page %1)...")),
    m_dpi(-1),
    m_pageCount(0),
    m_rotation(0),
    m_reportDone(false),
    m_showWaitCursor(false),
    m_pEvents(false),
    m_quality(QPrinter::HighResolution),
    m_evaluator(0),
    m_parentWidget(0),
    m_altBackColor(QColor(220,220,220)),
    m_pDBQueryEngine(0)
{
}

NCReport::~NCReport()
{
    if ( m_reportDef )
        delete m_reportDef;
    if ( m_output && m_output->isAutoDelete() )
        delete m_output;
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("NCReport object deleted.");
#endif
}

bool NCReport::hasError()
{
    return m_reportDef->error()->error();
}

QString NCReport::lastErrorMsg()
{
    return m_reportDef->error()->errorMsg();
}

void NCReport::setReportSource(const NCReportSource& reportSource )
{
    m_source = reportSource;
}

void NCReport::setReportSource(NCReportSource::ReportSourceType rst )
{
    m_source.setSourceType( rst );
}

void NCReport::setOutput( NCReportOutput* o, bool deleteLast )
{
    if ( m_output && m_output->isAutoDelete() && deleteLast )
        delete m_output;
    m_output = o;
}

//void NCReport::addParameter( NCReportParameter* par )
//{
//	rdef->addParameter( par );
//}

void NCReport::addParameter(const QString & id, const QVariant & value)
{
    m_reportDef->addParameter( id, value );
    m_parameters2change[id] = value;
    //m_source.addParameter(id, value.toString());
}

void NCReport::addParameters(const QMap<QString, QVariant> &parameters, bool clear)
{
    m_reportDef->addParameters( parameters, clear );
}

void NCReport::addCustomDataSource(NCReportDataSource *ds)
{
    addDataSource(ds);
}

void NCReport::addDataSource(NCReportDataSource *ds)
{
    m_reportDef->addDataSource( ds );
}

//------------------------------------------ SLOTS
void NCReport::runReportToPrinter( int copies, bool showdialog, QWidget* parent, const QString& printerName )
{
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportPrinterOutput( m_quality, m_dpi );	// guarded QPointer
    NCReportPrinterOutput* output = qobject_cast<NCReportPrinterOutput*>(m_output);
    output->setCopies(copies);
    output->setShowPrintDialog(showdialog);
    output->setParentWidget( parent );
    output->setRotationAngle(m_rotation);
    if ( !printerName.isEmpty() )
        ((NCReportPrinterOutput*)output)->printer()->setPrinterName(printerName);
    runReport();
    setOutput( 0 );	// delete output
}

void NCReport::runReportToPrinterNoDialog( int copies, const QString& printerName )
{
    runReportToPrinter( copies, false, 0, printerName );
}

#ifndef NCREPORT_NO_PREVIEW_WINDOW
void NCReport::runReportToPreview()
{
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportPreviewOutput();
    m_output->setAutoDelete( false );	// restrict delete when ncreport object destroys
    m_output->setRotationAngle(m_rotation);
    runReport();
    // output object will not be deleted after running report! It's needed for preview form. Commented out
    //setOutput( 0 );	// delete output
}

void NCReport::runReportToQtPreview()
{
#if (QT_VERSION >= 0x040400)
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportQtPreviewOutput( m_quality, m_dpi );
    NCReportQtPreviewOutput* output = qobject_cast<NCReportQtPreviewOutput*>(m_output);
    output->setAutoDelete( true );
    output->setReport( this );
    output->setRotationAngle(m_rotation);
    emit showQtPreviewDialog();
    output->dialog()->exec();
    setOutput( 0 );	// delete output
#endif
}
#endif

void NCReport::runReportToPDF(const QString& filename)
{
    runReportToPdf(filename);
}

void NCReport::runReportToPdf(const QString &filename)
{
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportPdfOutput( m_quality, m_dpi );
    NCReportPdfOutput* output = qobject_cast<NCReportPdfOutput*>(m_output);
    output->setFileName( filename );
    output->setRotationAngle(m_rotation);
    runReport();
    setOutput( 0 );	// delete output
}

int NCReport::runReportToPDFWithPassword(const QString &filename, const QString& password/* = ""*/, const QString& perm/* = ""*/)
{
    // 设置模板名称
    QString strFileName = QDir::tempPath() + "/" +
                QCoreApplication::applicationName() + "_XXXXXX." + "pdf";
    QTemporaryFile tmpFile(strFileName);

    // 设置为不自动删除
    tmpFile.setAutoRemove(false);

    qDebug() << "tempPath : " << QDir::tempPath();

    if (tmpFile.open())
    {
        tmpFile.close();

        QString strFileTemplate = tmpFile.fileTemplate();
        QString strFileTempName = tmpFile.fileName();

        qDebug() << "fileTemplate : " << strFileTemplate;
        qDebug() << "strFileTempName : " << strFileTempName;

        runReportToPDF(strFileTempName);
        if (!hasError()) {
            //QPdfDocument pdfDoc;
            //QPdfDocument::DocumentError err = pdfDoc.load(filename);
            //if(err == QPdfDocument::DocumentError::NoError){
                //const auto documentTitle = m_document->metaData(QPdfDocument::Title).toString();
                //pdfDoc.setPassword(psw);
                //pdfDoc.setEncryption(QPdfDocument::EncryptionAES, "mypassword"); // 设置密码
                //QPainter painter(&pdfDoc);
            //}
            //pdfDoc.close();

            QStringList arguments;
            ////////////////////////////////////////////////////////////////////////////////
            //        The permissions section can include one or more of the following features:

            //        Printing – Top Quality Printing
            //        DegradedPrinting – Lower Quality Printing
            //        ModifyContents – Also allows Assembly
            //        Assembly
            //        CopyContents – Also allows ScreenReaders
            //        ScreenReaders
            //        ModifyAnnotations – Also allows FillIn
            //        FillIn
            //        AllFeatures – Allows the user to perform all of the above, and top quality printing.
            /////////////////////////////////////////////////////////////////////////////////////
            arguments << strFileTempName << "output" << filename;
            if(!perm.isEmpty())
                arguments << "owner_pw" << "bosmutus@pdf" << "user_pw" <<  password;
            if(!perm.isEmpty())
                arguments << "allow" << perm;
            //qDebug() << QApplication::applicationDirPath()+"/dtools/pdftk.exe "+ arguments.join(" ");
            QProcess pdftk;

    #if defined(Q_OS_WIN)
            qDebug() << "starting pdftkCmd";
            // case 1: only cmd
//            pdftk.start(QApplication::applicationDirPath()+"/dtools/pdftk.exe "+arguments.join(" ") );
            // case 2: string concat
//            QString pdftkCmd = QString("\"" + QApplication::applicationDirPath()+"/dtools/pdftk.exe\"  \"%1\" output \"%2\" owner_pw bosmutus@pdf user_pw %3 allow Printing")
//                    .arg(strFileTempName).arg(filename).arg(password);
//            pdftk.start(pdftkCmd);
            // case 3: arguments
            pdftk.start(QApplication::applicationDirPath()+"/dtools/pdftk.exe", arguments);
            qDebug() << QApplication::applicationDirPath()+"/dtools/pdftk.exe" << arguments;
    #elif defined(Q_OS_UNIX)
            qDebug() << "starting pdftkCmd";
            pdftk.start("pdftk", arguments );
            qDebug() << "pdftk" << arguments;
    #endif

            if(!pdftk.isOpen()) {
                qDebug() << "return -1 " << pdftk.exitStatus();
                qDebug() << "pdftk not open";
                return -1;
            }
            if (!pdftk.waitForStarted()) {
                qDebug() << "return -2 " << pdftk.exitStatus();
                QString str_output = QString(pdftk.readAllStandardOutput());
                qDebug() << "pdftk console output" << str_output;
                return -2;
            }

            if (!pdftk.waitForFinished()) {
                qDebug() << "return -3" << pdftk.exitStatus();
                QString str_output = QString(pdftk.readAllStandardOutput());
                qDebug() << "pdftk console output" << str_output;
                return -3;
            }

            QString str_output = QString(pdftk.readAllStandardError());
            qDebug() << "pdftk console output" << str_output;

            if(str_output.indexOf("Error",0,Qt::CaseInsensitive) >=0){
                //Error: The user and owner passwords are the same.
                //   PDF Viewers interpret this to mean your PDF has
                //   no owner password, so they must be different.
                //   Or, supply no owner password to pdftk if this is
                //   what you desire.
                //Exiting.
                //Errors encountered.  No output created.
                //Done.  Input errors, so no output created.
                qDebug() << "return -4";
                return -4;
            }
            tmpFile.remove();
            qDebug() << "return 0" ;
            qDebug() << "pdf file export succeed:" << filename;
            return 0;
        }

    }
    else
    {
        qCritical() << "failed to write temporary file";
        return -1;
    }
    return -1;
}

#ifndef NCREPORT_NO_EMAIL
void NCReport::runReportToPDFSendMail(const QString &filename, LMailSender *mailSender)
{
    if (!mailSender)
        return;

    runReportToPDF(filename);
    if (!hasError()) {
        QFile file(filename);
        if (file.open(QFile::ReadOnly)) {
            QByteArray ba = file.readAll();
            mailSender->setAttachment(filename, ba);
            mailSender->send();
        }
    }
}
#endif

void NCReport::runReportToPostScript(const QString &filename)
{
#if QT_VERSION < 0x050000
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportPdfOutput( m_quality, m_dpi, QPrinter::PostScriptFormat );
    NCReportPdfOutput* output = qobject_cast<NCReportPdfOutput*>(m_output);
    output->setFileName( filename );
    runReport();
    setOutput( 0 );	// delete output
#else
    Q_UNUSED(filename)
#endif
}

#ifndef NCREPORT_NO_SVG
void NCReport::runReportToSVG( const QString& filename )
{
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportSvgOutput();
    NCReportSvgOutput* output = qobject_cast<NCReportSvgOutput*>(m_output);
    output->setFileName( filename );
    output->setRotationAngle(m_rotation);
    runReport();
    setOutput( 0 );	// delete output
}
#endif

void NCReport::runReportToImage( const QString& filename )
{
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportImageOutput();
    NCReportImageOutput* output = qobject_cast<NCReportImageOutput*>(m_output);
    output->setFileName( filename );
    output->setRotationAngle(m_rotation);
    runReport();
    setOutput( 0 );	// delete output
}

void NCReport::runReportToHTML(const QString &filename, int strategy, const QString &cssFile)
{
    runReportToHtml(filename,strategy,cssFile);
}

void NCReport::runReportToHtml(const QString &filename, int strategy, const QString &cssFile)
{
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportHtmlOutput();
    NCReportHtmlOutput* output = qobject_cast<NCReportHtmlOutput*>(m_output);
    output->setStrategy( strategy==0 ? NCReportHtmlOutput::SectionOneTable : NCReportHtmlOutput::SectionMultiTables );
    output->setStyleSheetFile( cssFile );
    output->setFileName( filename );
    runReport();
    setOutput( 0 );	// delete output
}

void NCReport::runReportToExcelHtml(const QString &filename)
{
    setOutput( 0 );	// delete last output if exists
    m_output = new NCReportExcelHtmlOutput();
    m_output->setFileName( filename );
    runReport();
    setOutput( 0 );	// delete output
}

void NCReport::runReportToText(const QString &filename, const QString &templateFileName)
{
    setOutput( 0 );	// delete last output if exists
    NCReportTextOutput *output = new NCReportTextOutput();
    m_output = output;
    output->setFileName( filename );
    output->setTemplateFile(templateFileName);
    if ( runReport() ) {

    } else {
        m_reportDef->error()->setError(tr("Invalid text output template."));
    }
    setOutput( 0 );	// delete output
}


bool NCReport::runReport()
{
    m_reportDef->error()->setError("");

    if (!getParameters())
        return false;

    emit reportStarts();

    bool defaultLanguage = (m_reportDef->option().currentLanguage().isEmpty() || m_reportDef->option().currentLanguage()=="00");
    if (!defaultLanguage)
        QLocale::setDefault( QLocale(m_reportDef->option().currentLanguage()) );

    m_cancelled = false;
    m_reportDone = false;

    bool ok = true;
    m_director = new NCReportDirector( this );
    m_director->setProcessEvents(m_pEvents);
    connect( m_director, SIGNAL(dataSourceOpen(QString)), this, SLOT(slotDataSourceOpen(QString)) );
    connect( m_director, SIGNAL(dataSourceOpened(QString,int)), this, SLOT(slotDataSourceOpened(QString,int)) );
    connect( m_director, SIGNAL(dataRowProgress(QString,int)), this, SLOT(slotDataRowProgress(QString,int)) );
    connect( m_director, SIGNAL(dataRowProgress(int)), this, SLOT(slotDataRowProgress(int)) );
    connect( m_director, SIGNAL(dataRowCount(int)), this, SLOT(slotDataRowCount(int)) );
    connect( m_director, SIGNAL(pageProgress(int)), this, SLOT(slotPageProgress(int)) );
    connect( m_director, SIGNAL(sectionProgress(QString)), this, SLOT(slotSectionProgress(QString)) );

    m_director->setForceCopies( m_forcedCopy );
    m_director->setReportDef( m_reportDef );
    m_director->resetPageNum();
    m_director->resetGlobalPageNum();

    if (m_reportObjectBatch.isEmpty())
    {
        if ( m_reportStringBatch.isEmpty() ) {
            m_director->setReportCount( 1 );
            m_director->setReportNumber( 1 );
            ok = runReport(false,false,false); // RUN REPORT IN NORMAL MODE
        } else {
            m_director->setReportCount( m_reportStringBatch.size() );
            ok = runReportBatch(); // RUN REPORT IN BATCH MODE
        }
    }
    else
    {
        m_director->setReportCount( m_reportObjectBatch.length() );
        ok = runReportBatch(); // RUN REPORT IN BATCH MODE
    }

    m_pageCount = m_director->pageCount();
    m_director->paintPosMM().y();
    delete m_director;
    m_director =0;
    m_reportDone = ok && !m_cancelled;
    emit reportEnds();

    if (!defaultLanguage)
        QLocale::setDefault( QLocale::system() );

    return ok;
}

bool NCReport::runReportBatchV1()
{
    bool ok = true;
    int counter=0;
    for (int i = 0; i < m_reportStringBatch.size(); ++i) {

        if (m_reportStringBatch.at(i).endsWith(".xml") || m_reportStringBatch.at(i).endsWith(".ncr")) {
            // it's a file name
            m_source.setSourceType( NCReportSource::File );
            m_source.setFileName(m_reportStringBatch.at(i));
        } else {
            // it's an XML string
            m_source.setSourceType( NCReportSource::String );
            m_source.setReportDefXML( m_reportStringBatch.at(i) );
        }

        if (m_reportBatchParameters.count()>i) {
            QHash< QString, QVariant > parametersForReport = m_reportBatchParameters.at( i );
            m_reportDef->addParameters( parametersForReport );
        }
        m_director->setReportNumber( counter+1 );
        ok = runReport( true, counter==0, counter==m_reportStringBatch.size()-1 );
        if ( !ok )
            break;
        counter++;
    }
    return ok;
}

bool NCReport::runReportBatch()
{
    if (m_reportObjectBatch.isEmpty())
        return runReportBatchV1();

    bool ok = true;
    int counter=0;
    for (int i = 0; i < m_reportObjectBatch.size(); ++i) {
        NCReport *r = m_reportObjectBatch.at(i);
        setReportSource( r->reportSource() );
        //m_reportDef->addParameters( r->reportDef()->parameters() );
        m_reportDef->setParameters( r->reportDef()->parameters() );
        m_reportDef->setItemModels( r->reportDef()->itemModels() );
        m_reportDef->setStringLists( r->reportDef()->stringLists() );
        m_reportDef->setDataSources( r->reportDef()->dataSources() );

        m_director->setReportNumber( counter+1 );
        ok = runReport( true, counter==0, counter==m_reportObjectBatch.size()-1 );
        if ( !ok )
            break;
        counter++;
    }
    return ok;
}

void NCReport::connectDataSourceSignals()
{
    QHash<QString, NCReportDataSource*>::const_iterator i = m_reportDef->dataSources().constBegin();
    while (i != m_reportDef->dataSources().constEnd()) {
        NCReportDataSource* ds = i.value();
        connect(ds,SIGNAL(updateRequestFKey(QString,QString)),this,SIGNAL(dataSourceUpdateRequestFKey(QString,QString)));
        connect(ds,SIGNAL(updateRequest(QString,QString)),this,SIGNAL(dataSourceUpdateRequest(QString,QString)));
        ++i;
    }
}

bool NCReport::runReport( bool batchMode, bool first, bool last  )
{
    if ( !m_output ) {
        m_reportDef->error()->setError( tr("No report output defined.") );
        return false;
    }
    m_source.setReportDef( m_reportDef );
    m_reportDef->reset( false );
    NCReportXMLDefReader reader( m_reportDef );

    QMap<QString, QVariant>::const_iterator i = m_parameters2change.constBegin();
    while (i != m_parameters2change.constEnd()) {
        m_source.addParameter(i.key(), i.value().toString());
        //m_reportDefXML.replace( QString("{{%1}}").arg(i.key()), i.value());
        //m_reportDefXML.replace( QString("$P{%1}").arg(i.key()), i.value());
        ++i;
    }

    bool ok = m_source.load( reader );
    NCRDUserFunction_getValueWithNullOrEmpty func_getValueWithNullOrEmpty;
    if(ok){
        m_reportDef->addParameters(m_parameters2change);
        // add userfunction
        m_reportDef->addUserFunction("getValueWithNullOrEmpty", &func_getValueWithNullOrEmpty);
    }

    //<设置视图数据源>
    const QHash<QString, NCReportDataSource *> dsHashTb = m_reportDef->dataSources();
    foreach(NCReportDataSource * pDS, dsHashTb)
    {
        if(!pDS)
            continue;

        NCReportDataViewDataSource * dvSource = qobject_cast<NCReportDataViewDataSource *>(pDS);
        if(!dvSource)
            continue;
        dvSource->setViewPath(m_viewPath);
        dvSource->setQueryEngine(m_pDBQueryEngine);
    }

    //</设置自定义数据>
    if (!m_customData.isEmpty())
        m_reportDef->option().setCustomData(m_customData);

    emit reportDefChange( m_reportDef );

    if ( ok ) {
        connectDataSourceSignals();
        customizeReportDef( m_reportDef );

        m_output->setPageOption( m_reportDef->option() );
        m_output->setBatchMode( batchMode );
        m_output->setFlagFirst( first );  // set the flag if this is the first report in batch
        m_output->setFlagLast( last ); // set the flag if this is the last report in batch
        m_output->setPageRanges(m_pageRanges);
        ok = m_output->setup();

        QTime time;
        time.start();
        m_elapsed =0;

        if (ok) {
#ifndef NCREPORT_NO_CURSOR
            if (m_showWaitCursor)
                qApp->setOverrideCursor(Qt::WaitCursor);
#endif
            for (int i=0; i<m_forcedCopy; ++i ) {
                m_director->setCurrentForceCopy( i );
                ok = m_director->reportProcess();
                if (!ok )
                    break;
            }
#ifndef NCREPORT_NO_CURSOR
            if (m_showWaitCursor)
                qApp->restoreOverrideCursor();
#endif
        }
        m_output->cleanup();
        m_elapsed = time.elapsed();
    }
    m_reportDef->reset( false );

    return ok;
}

bool NCReport::reportProcess()
{
    return true;
}

NCReportOutput * NCReport::output()
{
    return m_output;
}

/*! Returns the current program version*/
const QString NCReport::version()
{
    return QString(NCREPORTAPP_VERSION);
}

/*! Return short program information */
const QString NCReport::info()
{
    return QString()+ /*NCREPORTAPP_NAME+" "+version()*/+" "+NCREPORTAPP_RELEASENAME+" "+NCREPORTAPP_COPYRIGHT;
}

NCReportSource & NCReport::reportSourceRef()
{
    return m_source;
}

NCReportSource NCReport::reportSource() const
{
    return m_source;
}

void NCReport::setForceCopies(int c )
{
    m_forcedCopy = c;
}

NCReportDataSource * NCReport::dataSource(const QString & dsname )
{
    return m_reportDef->dataSources()[dsname];
}

void NCReport::addItemRenderingClass(NCReportGraphRenderer *renderer )
{
    m_reportDef->addGraphItemRenderer(renderer);
}

void NCReport::addGraphItemRenderer(NCReportGraphRenderer *renderer)
{
    m_reportDef->addGraphItemRenderer(renderer);
}

/*!
 * \brief NCReport::reset
 * \param all If true all objects will be deleted
 * Overloaded function.
 */
void NCReport::reset( bool all )
{
    reset(all,all,all,all,all,true);
}

/*!
 * \brief NCReport::reset
 * \param clearRenderers clears graph renderers
 * \param clearParameters clears parameters
 * \param clearUserFunctions clears user functions
 * \param clearStringLists clears string lists
 * \param clearBatchReports removes all added reports for batch running
 * \param keepCustomDataSources if true the custom data sources will not be deleted
 * Method for resetting the report object.
 * Clears all objects from memory and makes the report object enable to run again a new report.
 */
void NCReport::reset(bool clearRenderers, bool clearParameters, bool clearUserFunctions, bool clearStringLists, bool clearBatchReports, bool keepCustomDataSources)
{
    if (clearBatchReports) {
        m_reportObjectBatch.clear();
        m_reportStringBatch.clear();
    }
    m_reportDef->reset(clearRenderers, clearParameters, clearUserFunctions, clearStringLists, keepCustomDataSources);
    if ( m_output && m_output->isAutoDelete() ) {
        delete m_output;
        m_output = 0;
    }
}

void NCReport::addStringList( const QStringList & list, const QString & id)
{
    m_reportDef->addStringList(id,list);
}

/*!
 * \brief NCReport::addStringList
 * \param id
 * \param list
 * Adds a QStringList as a datasource to the report. The id is used for identification for data sources
 */
void NCReport::addStringList(const QString &id, const QStringList &list)
{
    m_reportDef->addStringList(id,list);
}

void NCReport::setReportDefXml(const QString & xml )
{
    m_source.setSourceType( NCReportSource::String );
    m_source.setReportDefXML( xml );
}

QString NCReport::reportDefXml() const
{
    return m_source.reportDefXML();
}

void NCReport::setReportFile(const QString & fileName )
{
    if ( m_source.sourceType() != NCReportSource::File )
        m_source.setSourceType( NCReportSource::File );

    m_source.setFileName( fileName );
}

QString NCReport::reportFile() const
{
    return m_source.fileName();
}

void NCReport::setReportDef(const QString &xml)
{
    setReportDefXml(xml);
}

/*! Adds an item model object to the report. The model is used by item model datasource.
  @param model item model
  @param id used for identification
*/
void NCReport::addItemModel(QAbstractItemModel *model, const QString& id )
{
    m_reportDef->addItemModel( id, model );
}

void NCReport::addItemModel(const QString &id, QAbstractItemModel *model)
{
    m_reportDef->addItemModel( id, model );
}

void NCReport::setCustomData(const QString &data)
{
    m_customData = data;
}

QString NCReport::customData() const
{
    return m_customData;
}

#ifndef NCREPORT_NO_TABLE_VIEW
void NCReport::addTableView(QTableView *tableView, const QString &id)
{
    m_reportDef->addTableView(id, tableView);
}

void NCReport::addTableView(const QString &id, QTableView *tableView)
{
    m_reportDef->addTableView(id, tableView);
}
#endif

void NCReport::addUserFunction(const QString &id, NCReportUserFunction *function)
{
    m_reportDef->addUserFunction(id,function);
}

void NCReport::setUseWaitCursor(bool set )
{
    m_useWaitCursor = set;
}

void NCReport::setTextDocument(QTextDocument *doc )
{
    m_document = doc;
}

void NCReport::customizeReportDef( NCReportDef* )
{
    // virtual
}


void NCReport::slotDataSourceOpen( const QString& dsID )
{
    emit dataSourceOpen( tr("Opening data source: %1").arg(dsID) );
}
void NCReport::slotDataSourceOpened( const QString& dsID, int totalRows )
{
    emit dataSourceOpened( tr("Data source opened: %1").arg(dsID), totalRows );
}
void NCReport::slotDataRowProgress( const QString& dsID, int row )
{
    emit dataRowProgress( dsID, row );
}
void NCReport::slotDataRowProgress( int row )
{
    emit dataRowProgress( row );
}
void NCReport::slotDataRowCount( int row )
{
    emit dataRowCount( row );
}
void NCReport::slotSectionProgress( const QString& sectionID )
{
    sectionProgress( sectionID );
}
void NCReport::slotPageProgress( int page )
{
    emit pageProgress( page );
    emit pageProgress( m_pageProgressText.arg(page) );
}

void NCReport::cancel()
{
    if ( m_director )
        m_director->cancel();
}

/*!
 * \brief NCReport::setLastPagePositionMM
 * \param position
 * Sets the last page position of the printing engine. This is used by report director.
 */
void NCReport::setLastPagePositionMM(const QPointF &position)
{
    m_lastPagePositionMM = position;
}

/*!
 * \brief NCReport::lastPagePositionMM
 * \return position point in millimeter
 * Returns tha last page position of the last printing process. The value is useful for getting the printed page height on endless outputs.
 */
QPointF NCReport::lastPagePositionMM() const
{
    return m_lastPagePositionMM;
}

/*!
 * \brief NCReport::setParentWidget
 * \param parent
 * Sets the parent widget for occurrent input dialogs
 */
void NCReport::setParentWidget(QWidget *parent)
{
    m_parentWidget = parent;
}

QWidget *NCReport::parentWidget() const
{
    return m_parentWidget;
}

/*!
 * \brief NCReport::getParameters
 * \return
 * Gets the parameters via QInputDialog for which value is specified as '{GET}' token.
 * These parameters are prepared as user input parameters.
 */
bool NCReport::getParameters()
{
    bool ok = true;
    QHash<QString, QVariant>::const_iterator i = m_reportDef->parameters().constBegin();
    while (i != m_reportDef->parameters().constEnd()) {
        if ( i.value().toString() == "{GET}" ) {
            // find a defult value:
            QVariant defaultValue = m_reportDef->parameter(i.key()+"_DEFAULT");
            getParameter( i.key(), defaultValue, m_parentWidget, tr("Get parameter"), i.key(), &ok );
            if (!ok) {
                m_reportDef->error()->setError( tr("Parameter %1 input is cancelled by the user.").arg(i.key()) );
                break;
            }
        }
        ++i;
    }
    return ok;
}

int NCReport::runTimeElapsed() const
{
    return m_elapsed;
}

void NCReport::setPageProgressText( const QString& txt )
{
    m_pageProgressText = txt;
}
QString NCReport::pageProgressText() const
{
    return m_pageProgressText;
}

void NCReport::setReportBatch( const QStringList& batch )
{
    m_reportStringBatch = batch;
}

void NCReport::addReportToBatch( const QString& reportXML )
{
    addReportToBatch( reportXML, QHash<QString,QVariant>() );
}

void NCReport::addReportToBatch( const QString& reportXML, const QHash<QString,QVariant>& inAssociatedParameters  )
{
    m_reportStringBatch.append(reportXML);
    m_reportBatchParameters.append( inAssociatedParameters );
}

void NCReport::addReportToBatch( NCReport *report)
{
    m_reportObjectBatch.append(report);
}

void NCReport::clearBatch()
{
    m_reportObjectBatch.clear();
    m_reportStringBatch.clear();
    m_reportBatchParameters.clear();
}

/*!
 * \brief NCReport::isBatchMode
 * \return true if batch mode is enabled.
 * Batch mode is enabled when a report is added to the batch.
 */
bool NCReport::isBatchMode() const
{
    return !m_reportObjectBatch.isEmpty() || !m_reportStringBatch.isEmpty();
}

/*!
  Sets the current language by 2 letters language code
  */
void NCReport::setCurrentLanguage( const QString& langcode )
{
    m_reportDef->option().setCurrentLanguage(langcode);
}

QString NCReport::currentLanguage()
{
    return m_reportDef->option().currentLanguage();
}

QPrinter::PrinterMode NCReport::printQuality() const
{
    return m_quality;
}

void NCReport::setPrintQuality( QPrinter::PrinterMode mode )
{
    m_quality = mode;
}

int NCReport::printResolution() const
{
    return m_dpi;
}

void NCReport::setPrintResolution(int dpi)
{
    m_dpi = dpi;
}

#ifndef NCREPORT_NO_PREVIEW_WINDOW
NCReportPreviewWindow* NCReport::createPreviewWindow(QWidget *parent)
{
    NCReportPreviewWindow *pv = new NCReportPreviewWindow(parent);
    pv->setReport( this );
    pv->setWindowModality( Qt::ApplicationModal );
    pv->setAttribute( Qt::WA_DeleteOnClose );
    return pv;
}


QWidget* NCReport::runReportToShowPreview()
{
    runReportToPreview();

    if ( hasError() )
        return 0;

    return execPreview( output() );
}

bool NCReport::runReportToShowPreviewDonePrint()
{
    runReportToPreview();

    if ( hasError() )
        return false;

    NCReportPreviewWindow *pv = createPreviewWindow();
    pv->setOutput( (NCReportPreviewOutput*)output() );
    pv->exec();

    return pv->donePrint();
}

QWidget *NCReport::execPreview(NCReportOutput *output )
{
    NCReportPreviewWindow *pv = createPreviewWindow();
    pv->setOutput( (NCReportPreviewOutput*)output ); // report->output() exists (not deleted) when preview
    pv->exec();
    return pv;
}


QVariant NCReport::getParameter( const QString& id, const QVariant& value, QWidget* parent, const QString & title, const QString & label, bool* ok )
{
    QVariant retval;
    switch ( value.type() )	{
    case QVariant::Int: retval = QInputDialog::getInt( parent, title, label+":", value.toInt(), -2147483647, 2147483647, 1, ok ); break;
    case QVariant::Double: retval = QInputDialog::getDouble( parent, title, label+":", value.toInt(), -2147483647, 2147483647, 1, ok ); break;
    default: retval = QInputDialog::getText( parent, title, label+":", QLineEdit::Normal, value.toString(), ok ); break;
    }
    if (ok && *ok)
        m_reportDef->addParameter(id,retval);

    return retval;
}
#endif

/*!
  Returns the last value of finished report total page number.
  */
int NCReport::pageCount() const
{
    return m_pageCount;
}

bool NCReport::reportDone() const
{
    return m_reportDone;
}

void NCReport::setShowWaitCursor(bool set)
{
    m_showWaitCursor = set;
}

bool NCReport::showWaitCursor() const
{
    return m_showWaitCursor;
}

void NCReport::setRootDir(const QString &rootDir)
{
    qApp->setProperty("NCREPORT_ROOT_DIR", rootDir );
}

NCReportOutput *NCReport::createOutput(int outputType, QObject *parent)
{
    NCReportOutput *output =0;

    NCReportOutput::OutputType type = (NCReportOutput::OutputType)outputType;

    switch (type)
    {
    case NCReportOutput::Preview:
#ifndef NCREPORT_NO_PREVIEW_WINDOW
        output = new NCReportPreviewOutput( parent );
#endif
        break;
    case NCReportOutput::Pdf:
        output = new NCReportPdfOutput( QPrinter::HighResolution, -1, QPrinter::PdfFormat, parent );
        break;
    case NCReportOutput::PostScript:
#if QT_VERSION < 0x050000
        output = new NCReportPdfOutput( QPrinter::HighResolution, -1, QPrinter::PostScriptFormat, parent );
#endif
        break;
    case NCReportOutput::Printer:
        output = new NCReportPrinterOutput( QPrinter::HighResolution, -1, parent );
        break;
    case NCReportOutput::Svg:
#ifndef NCREPORT_NO_SVG
        output = new NCReportSvgOutput(parent);
#endif
        break;
    case NCReportOutput::Image:
        output = new NCReportImageOutput(parent);
        break;
    case NCReportOutput::QtPreview:
#ifndef NCREPORT_NO_PREVIEW_WINDOW
        output = new NCReportQtPreviewOutput( QPrinter::HighResolution, -1, parent );
#endif
        break;
    case NCReportOutput::Text:
        output = new NCReportTextOutput(parent);
        break;
    case NCReportOutput::Html:
        output = new NCReportHtmlOutput(parent);
        break;
    case NCReportOutput::ExcelHtml:
        output = new NCReportExcelHtmlOutput(parent);
        break;
    case NCReportOutput::ExcelXml:
    case NCReportOutput::Xml:
    case NCReportOutput::Odf:
    case NCReportOutput::PlainText:
    case NCReportOutput::Null:
    case NCReportOutput::Custom:
        break;
    }

    return output;
}

void NCReport::setProcessEvents(bool enable)
{
    m_pEvents = enable;
}

bool NCReport::isProcessEvents() const
{
    return m_pEvents;
}

void NCReport::setConnectionID(const QString &connectionID)
{
    m_reportDef->setConnectionID(connectionID);
}

QString NCReport::connectionID() const
{
    return m_reportDef->connectionID();
}

/*!
 * \brief NCReport::setEvaluator
 * \param evaluator
 * Sets a user defined evaluator. If set, the report director will use this evaluator instead of the base class.
 * This object will be deleted after running the report.
 */
void NCReport::setEvaluator(NCReportEvaluator *evaluator)
{
    m_evaluator = evaluator;
}

/*!
 * \brief NCReport::evaluator
 * \return user defined evaluator
 */
NCReportEvaluator *NCReport::evaluator()
{
    return m_evaluator;
}

QString NCReport::zintPath() const
{
    return m_reportDef->zintPath();
}

void NCReport::setZintPath(const QString &dir)
{
    m_reportDef->setZintPath(dir);
}

//NCReport *NCReport::clone(QObject *parent)
//{
//    NCReport *newObject = new NCReport(parent);

//    QHash<QString, NCReportDataSource*>::const_iterator i = rdef->dataSources().constBegin();
//    while (i != rdef->dataSources().constEnd()) {
//        newObject->addDataSource( i.key(), i.value() );
//        ++i;
//    }
//    QHash<QString, QVariant>::const_iterator i = rdef->parameters().constBegin();
//    while (i != rdef->parameters().constEnd()) {
//        newObject->addParameter( i.key(), i.value() );
//        ++i;
//    }
//    QHash<QString, QStringList>::const_iterator i = rdef->stringLists().constBegin();
//    while (i != rdef->stringLists().constEnd()) {
//        newObject->addStringList( i.key(), i.value() );
//        ++i;
//    }
//    return newObject;
//}

void NCReport::setCustomDataSourceFactory(NCReportDataSourceFactory *dsf)
{
    m_reportDef->setDataSourceFactory( dsf );
}

int NCReport::rotationAngle() const
{
    return m_rotation;
}

/*!
 * \brief NCReport::setPageRanges
 * \param ranges
 * Enables the page range expression for all outputs. See more in \class NCReportOutput::setPageRanges()
 */
void NCReport::setPageRanges(const QString &ranges)
{
    m_pageRanges = ranges;
}

QString NCReport::pageRanges() const
{
    return m_pageRanges;
}

void NCReport::setAlternateRowBackgroundColor(const QColor &color)
{
    m_altBackColor = color;
}

QColor NCReport::alternateRowBackgroundColor() const
{
    return m_altBackColor;
}

QString NCReport::viewPath()
{
    return m_viewPath;
}

void NCReport::setViewPath(const QString & viewPath)
{
    m_viewPath = viewPath;
}

IMDDBQueryEngine * NCReport::dbQueryEngine()
{
    return m_pDBQueryEngine;
}

void NCReport::setDBQueryEngine(IMDDBQueryEngine * pDBQueryEngine)
{
    m_pDBQueryEngine = pDBQueryEngine;
}

void NCReport::setRotationAngle(int angle)
{
    m_rotation = angle;
}
