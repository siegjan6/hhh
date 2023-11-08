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
#include "ncreportdirector.h"
#include "ncreportdef.h"
#include "ncreportgroup.h"
#include "ncreportsection.h"
#include "ncreportevaluator.h"
#include "ncreportoutput.h"
#include "ncreportitem.h"
#include "ncreporttextitem.h"
#include "ncreportimageitem.h"
#include "ncreportsectiondirector.h"
#include "info.h"
#include "ncreportutils.h"
#include "ncreportdatasourcerelation.h"

#include <QHash>
#include <QStringList>
#include <QTextDocument>
#include <QTextDocumentWriter>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QApplication>
#include <QUrl>

#define NCRDIRECTOR_PROCESS_INDICATOR
//#define NCRDIRECTOR_PROCESSEVENTS

NCReportDirector::NCReportDirector(NCReport * parent) : QObject( parent ),
    m_report(parent),
    m_reportDef(0),
    m_evaluator( parent->evaluator() ? parent->evaluator() : new NCReportEvaluator()),
    m_state(Initial),
    m_pageStatus( Printable),
    m_passState(Off),
    m_paintPosMM( QPointF(0,0) ),
    m_pageNo(0),
    m_globalPageNo(0),
    m_pageCount(0),
    m_globalPageCount(0),
    m_reportNo(0),
    m_reportCount(0),
    m_currentDetailId(-1),
    m_forceCopies(0),
    m_currentForceCopy(0),
    m_currentRow(0),
    m_lastRenderedSection(0),
    m_runtime( QTime() ),
    m_pageBreakStop(false),
    m_pageBreakIsOnProcess(false),
    m_variableCorrectionCallerID(-1),
    m_subReportDS(0),
    m_dataSourceRelation(0),
    m_sectionCounter(0),
    m_processEvents(false),
    m_allowNewPage(false)
{
    m_evaluator->setDirector( this );

    setFlag(FlagFinished,false);
    setFlag(FlagRHeader,false);
    setFlag(FlagRFooter,false);
    setFlag(FlagNoSpaceInPage,false);
    setFlag(FlagBeginReport,false);
    setFlag(FlagNoUpdate,false);
    setFlag(FlagLastRecord,false);
}

NCReportDirector::~ NCReportDirector()
{
    if (!m_report->evaluator())
        delete m_evaluator;
    delete m_dataSourceRelation;
}

void NCReportDirector::setReportDef(NCReportDef *d )
{
    m_reportDef = d;
    m_reportDef->option();
    m_evaluator->setReportDef( d );
    //m_dataSourceRelation = new NCReportDataSourceRelation(this, 0, 0);
}

NCReportEvaluator *NCReportDirector::evaluator()
{
    return m_evaluator;
}

//------------------------------------------
// CENTRAL PROCESSING METHOD
//------------------------------------------
bool NCReportDirector::reportProcess()
{
    if (m_report->output()->isSuccessive())
        m_reportDef->option().setDoublePassMode(false);

    int saveGlobalPageNum = globalPageNum();

//    if (m_report->isBatchMode()) {
//        if ( !m_report->output()->isPageInRange( saveGlobalPageNum+1 ))
//             return true;    // batch mode - limited pages check to avoid empty pages
//    }

    if ( m_reportDef->option().isDoublePassMode() && m_passState == Off ) {
        //--------------------------------
        // double pass mode, first pass
        //--------------------------------
        NCReportOutput *real_output = m_report->output();
        NCReportNullOutput *null_output = new NCReportNullOutput( real_output );
        m_report->setOutput( null_output, false );
        m_passState = TestPass;
        bool done = reportProcess(); //recursive call;
        m_report->setOutput( real_output, false );
        delete null_output;

        if (done)
            done = m_reportDef->error()->isOK();

        if ( done ) {
            m_passState = RealPass;
            m_pageCount = pageNum();
        } else {
            m_passState = Off;
            return false;
        }
    }
    m_sectionCounter = 0;
    m_runtime.start();
    m_pageStatus = Printable;
    m_state = Initial;
    m_lastRenderedSection = 0;
    m_allowNewPage = false;
    bool ok = true;
    setPaintPosX(0);
    setPaintPosY(0);
    m_currentRow =0;
    m_currentDetailId =0;
    m_pageNo = 0;
    m_globalPageNo = saveGlobalPageNum;

    if ( m_passState != RealPass ) // reset pagecount if not the second pass if double pass mode
            m_pageCount =0;

    setFlag(FlagFinished,false);
    setFlag(FlagRHeader,true);
    setFlag(FlagRFooter,true);
    setFlag(FlagNoSpaceInPage,false);
    setFlag(FlagBeginReport,true);
    setFlag(FlagNoUpdate,false);
    setFlag(FlagLastRecord,false);

    // OPEN the subreport data sources first
    if ( !openDataSources( NCReportDataSource::SubReport ) ) {
        closeDataSources( true );
        return false;
    }
    if ( !openDataSources() ) {
        closeDataSources( true );
        return false;
    }
    benchmark("Opening databases");
//    if ( currentDataSource() && currentDataSource()->rowCount() == 0 ) // SUBQUERY ISSUE: inititaly go to 1st record if subquery is empty
//        nextRecord( currentDataSource() );

#ifdef NCRDIRECTOR_PROCESS_INDICATOR
    if ( currentDataSource() )
        emit dataRowCount( progressRowCount(currentDataSource()) );	// emit current rowCount signal
    if (m_processEvents)
        QApplication::processEvents();
#endif
    resetVariables(All);
    initGroupStacks();
    updateGroups( currentDetail() );	// init 1st detail group
    updateVariables();
    currentDetail()->setFinished(false);

    m_report->output()->setReportNum( m_reportNo );
    m_report->output()->begin();

    if (m_subReportDS) {
        bool firstReport=true;
        do {
            if (!firstReport) {
                m_currentDetailId = 0;

                setFlag(FlagFinished,false);
                setFlag(FlagRHeader,true);
                setFlag(FlagRFooter,true);
                setFlag(FlagNoSpaceInPage,false);
                setFlag(FlagBeginReport,true);
                setFlag(FlagNoUpdate,false);
                setFlag(FlagLastRecord,false);

                // REQUERY NON-SUBQUERY CHILD DATA SOURCES
                QHash<QString, NCReportDataSource*>::const_iterator iterator = m_reportDef->dataSources().constBegin();
                while (iterator != m_reportDef->dataSources().constEnd()) {
                    NCReportDataSource *ds = iterator.value();
                    ds->setAvailable(true);
                    if (ds->openRole() != NCReportDataSource::SubReport && ds->openRole() != NCReportDataSource::GroupChange)
                        updateDataSource(ds);
                    ++iterator;
                }

                resetVariables(All);
                initGroupStacks();
                updateGroups( currentDetail() );
                updateVariables();
                currentDetail()->setFinished(false);
            }

            if (m_reportDef->option().subReportOnNewPage())
                ok = reportSectionProcess(true,true);
            else
                ok = reportSectionProcess(firstReport,false);

            firstReport = false;

        } while (m_subReportDS->next());
        pageEnd();

    } else {
        ok = reportSectionProcess(true,true);
    }

    m_report->output()->end();
    inside.clear();
    outside.clear();

    closeDataSources( true );

    delete m_dataSourceRelation;
    m_dataSourceRelation = 0;

    if ( m_passState != Off ) {
        if ( m_passState == RealPass ) {
            m_passState = Off;
            m_sectionInfoList.clear();
        }
    }
    benchmark("Finished");
    return ok;
}

bool NCReportDirector::reportSectionProcess( bool doPageBegin, bool doPageEnd )
{
    bool ok=true;

    switch (m_reportDef->option().reportType()) {
    case NCReport::Report: {
        variableCorrections(true,100);
        if (doPageBegin)
            pageBegin();
        renderReportHeader();
        variableCorrections(false,100);
        //-----------------------------
        // DETAILS, GROUPS MANAGEMENT
        //-----------------------------
        while ( renderDetailsAndGroups() ) {}

        renderReportFooter();
        m_report->output()->setFinalPosMM(paintPosMM());
        if (doPageEnd)
            pageEnd();
        if ( flag(FlagRFooter) && m_reportDef->error()->isOK() ) { // If Report footer could not be printed.
            pageBegin();
            renderReportFooter();
            if (flag(FlagRFooter)) // Report footer could not be printed.
                m_reportDef->error()->setError( tr("Report footer is too large." ) );
            if (doPageEnd)
                pageEnd();
        }
        break;
    }
    case NCReport::TextDocumentPrint: {
        ok = documentReportProcess();
        break;
    }

    }

    return ok;
}

//------------------------------------------
// EOF CENTRAL PROCESSING METHOD
//------------------------------------------

bool NCReportDirector::documentReportProcess()
{
    NCReportSection *d = m_reportDef->details().at(0);
    if (!d)
        return false;

    setPaintPosX(m_reportDef->option().leftMarginMM());
    setPaintPosY(m_reportDef->option().topMarginMM());

    qreal mHeaderHeightMM = 0.0;
    qreal mFooterHeightMM = 0.0;
    if ( m_reportDef->pageHeader() )
        mHeaderHeightMM= m_reportDef->pageHeader()->metricHeight();
    if ( m_reportDef->pageFooter() )
        mFooterHeightMM = m_reportDef->pageFooter()->metricHeight();

    qreal mTopY = m_reportDef->option().topMarginMM() + mHeaderHeightMM;
    qreal mBottomY = m_reportDef->option().pageHeightMM() - m_reportDef->option().bottomMarginMM() - mFooterHeightMM;

    NCReportOutput *output = m_report->output();
    if ( !output->painter() ) {
        m_reportDef->error()->setError( tr("The current report does not support this output type." ) );
        return false;
    }

    // Document properties
    QTextDocument *document = new QTextDocument(this);
    //QTextDocument *document =0;		// It's probably NULL, otherwise Document is already existed
    qreal documentLeftMarginMM = 0;
    QSizeF documentPageSizeMM( m_reportDef->option().pageWidthMM() - m_reportDef->option().leftMarginMM() - m_reportDef->option().rightMarginMM(), mBottomY - mTopY );
    int detailCount = m_reportDef->details().count();
    QString unitedHtml;
    QTextStream inputHtml(&unitedHtml);
    bool sizeAndPosIsModified = false;

    // REPORT DOCUMENTS are being printed
    for ( int d=0; d<detailCount; ++d ) { // DETAILS
        m_currentDetailId = d;
        NCReportSection *detail = currentDetail();
        NCReportDataSource *dataSource = currentDataSource();
#ifdef NCRDIRECTOR_PROCESS_INDICATOR
        if (dataSource)
            emit dataRowCount( progressRowCount(dataSource) );	// emit current rowCount signal
        emit sectionProgress( detail->id() );
        if (m_processEvents)
            QApplication::processEvents();
#endif
        if (dataSource) {
            firstRecord(dataSource);
            do {
                documentReportProcess_Items(detail, inputHtml, documentLeftMarginMM, documentPageSizeMM, sizeAndPosIsModified, document, dataSource );
            } while (nextRecord(dataSource));
        } else {
            documentReportProcess_Items(detail,inputHtml, documentLeftMarginMM, documentPageSizeMM, sizeAndPosIsModified, document, dataSource );
        }

    }
    // init the document

    document->setHtml(unitedHtml);
    QSizeF documentPageSize( output->scale().mmToSize(documentPageSizeMM) );
    document->documentLayout()->setPaintDevice( output->painter()->device() );
    document->setPageSize( documentPageSize );

    m_pageCount = document->pageCount();
    QPointF pos( output->scale().mmToPoint( QPointF(m_paintPosMM.x()+documentLeftMarginMM, mTopY) ) );
    QRectF rect( QPointF(0,0), documentPageSize ); // Internal rect of painted text


    //--------------------------
    // RENDERING TEXT DOCUMENT
    //--------------------------
    for ( int i=0; i<m_pageCount; ++i ) {

#ifdef NCRDIRECTOR_PROCESS_INDICATOR
        emit pageProgress(i+1);
        if (m_processEvents)
            QApplication::processEvents();
#endif
        pageBegin();

        output->painter()->save();
        output->painter()->translate( pos );
        document->drawContents( output->painter(), rect );
        output->painter()->restore();

        pageEnd();

        rect.translate( 0, documentPageSize.height() );
        pos.setY( pos.y()-documentPageSize.height() );

    }


    //!TEST!//
    /*
    QTextDocumentWriter w;
    w.setFormat("odf");
    w.setFileName("output.odt");
    w.write(document);
    w.setFormat("html");
    w.setFileName("output.html");
    w.write(document);
    w.setFormat("plaintext");
    w.setFileName("output.txt");
    w.write(document);
    */

    //!TEST!//
    //NCReportUtils::stringToFile( "raw_output.html", unitedHtml );
    delete document;
    return true;


//	Page break:
//		<P STYLE="page-break-before: always"></P>

//	Table reprat header/footer (doesn't work)
//		<STYLE type="text/css">
//		THEAD { display: table-header-group; }
//		TFOOT { display: table-footer-group; }
//		</STYLE>

//	?????
//		thead {display: table-header-group;}

//	?????
//		table {page-break-inside: avoid;}
}

void NCReportDirector::documentReportProcess_Items(NCReportSection *detail, QTextStream &inputHtml, qreal& documentLeftMarginMM, QSizeF& documentPageSizeMM, bool &sizeAndPosIsModified,
                                                   QTextDocument *document, NCReportDataSource* ds )
{
    Q_UNUSED(document)
#ifdef NCRDIRECTOR_PROCESS_INDICATOR
    emit dataRowProgress( ds->id(), progressCurrentRow(ds) );
    emit dataRowProgress( progressCurrentRow(ds) );
    m_currentRow++;
#endif

    // SECTION PRINT WHEN
    if ( !detail->printWhenExp().isEmpty() ) {
        if (!m_evaluator->evaluatePrintWhen(detail->printWhenExp()))
            return;
    }

    QListIterator<NCReportItem*> i(detail->reportItems());
    while ( i.hasNext() ) {
        // DETAIL CONTENTS ITERATION: MULTIPLE TEXTS WITHIN A DETAIL IS ALLOWED.
        NCReportItem *item = i.next();

#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug( "Detail: %s Item: %s Z-value: %f", qPrintable(detail->id()), qPrintable(item->tagname()+"("+item->id()+")"), item->zValue());
#endif

        if ( !item->printWhen().isEmpty() ) {
            if (!m_evaluator->evaluatePrintWhen(item->printWhen()))
                continue;
        }

        switch (item->objectType()) {
        case NCReportItem::Text: {
            NCReportTextItem *textItem = (NCReportTextItem*)item;

            if ( item->sourceType() == NCReportItem::InternalDocument ) {
                inputHtml << m_report->document()->toHtml() << "\n";
                return;
            }
            if ( !sizeAndPosIsModified && !textItem->isDefaultPosAndSize() ) { // custom size by the 1st text object
                documentLeftMarginMM = item->metricPos().x();
                documentPageSizeMM.setWidth( item->metricSize().width());
                sizeAndPosIsModified = true;
            }
            QString itemHtml;
            m_evaluator->evaluateDynamicText( textItem, &itemHtml );
            if (itemHtml.isEmpty())
                itemHtml = textItem->documentTemplate();

            if (textItem->isPlainText())
                itemHtml = textItem->convertPlainTextToHtml(itemHtml);

            inputHtml << itemHtml << "\n";
            break;
        }
        case NCReportItem::Image: {
            NCReportImageItem *imageItem = (NCReportImageItem*)item;
            if (imageItem->isDynamic())
                m_evaluator->evaluateDynamicImage(imageItem);
            inputHtml << imageItem->toHtml();
            //document->addResource(QTextDocument::ImageResource, QUrl( QString("imgdata://image%1.png").arg(imageItem->id()) ), QVariant(imageItem->image()));
        }
        default:
            break;
        }

    }
}

bool NCReportDirector::openDataSources()
{
    if ( m_reportDef->dataSources().empty() ) {
        m_reportDef->error()->setError( tr("No datasource is defined." ) );
        return false;
    }
    if (!m_dataSourceRelation)
        m_dataSourceRelation = new NCReportDataSourceRelation(this, 0, 0);

    if (m_dataSourceRelation->childCount()==0)
        m_dataSourceRelation->insertDataSources( m_reportDef->dataSources() );

    return m_dataSourceRelation->open();
}


/*!
 * \brief NCReportDirector::openDataSources opens data sources by role
 * \param role open role of data source
 * \return
 */
bool NCReportDirector::openDataSources( NCReportDataSource::OpenRoles role )
{
    if ( m_reportDef->dataSources().empty() ) {
        m_reportDef->error()->setError( tr("No datasource is defined." ) );
        return false;
    }
    bool ok = true;
    QHash<QString, NCReportDataSource*>::const_iterator iterator = m_reportDef->dataSources().constBegin();
    while (iterator != m_reportDef->dataSources().constEnd()) {
        NCReportDataSource *ds = iterator.value();
        ds->setErrorHandler( m_reportDef->error() );
        if (ds->openRole()==role) {
            if ( !ds->isOpened() )
                ok = openDataSource( ds );
            if (!ok)
                break;
        }
        ++iterator;
    }
    return ok;
}

bool NCReportDirector::openDataSource( NCReportDataSource* ds )
{
    if ( ds->isOpened() )
        return true;

    ds->evaluate( m_evaluator );
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Open datasource \"%s\",%i", qPrintable(ds->id()), ds->dataSourceType() );
#endif
#ifdef NCRDIRECTOR_PROCESS_INDICATOR
    emit dataSourceOpen( ds->id() );
    if (m_processEvents)
        QApplication::processEvents();
#endif
    if ( ds->open(NCReportDataSource::OPEN_MODEL_QUERY) ) {
        if (ds->openRole()==NCReportDataSource::SubReport) {
            // inititalize master data source
            m_subReportDS = ds;
            ds->first();
        }
    } else {
        QString errormsg = m_reportDef->error()->errorMsg();
        m_reportDef->error()->setError( QString("Cannot open datasource \"%1\"\nError: %2").arg(ds->id()).arg(errormsg) );
        return false;
    }
    //------------------ IF DATA SOURCE CAN CONTAIN PARAMETERS ---------------------------
    if (ds->hasParameters()) {
        m_reportDef->addParameters( ds->parameters() );
        for ( int i=0; i<m_reportDef->dynamicItems().count(); i++) {
            NCReportItem *item = m_reportDef->dynamicItems().at(i);
            if (item->sourceType()==NCReportItem::Parameter) {
                item->updateContent();
            }
        }
    }
    // ----------------- CHECK NUM ROWS. If = -1 then not applicable
#ifdef NCRDIRECTOR_PROCESS_INDICATOR
    emit dataSourceOpened( ds->id(), ds->size() );
#endif
    if ( ds->openRole()==NCReportDataSource::BeginReport && !ds->allowEmpty() && ds->rowCount() == 0 ) {
        m_reportDef->error()->setError( tr("Data not found in datasource: %1").arg( ds->id() ) );
        return false;
    }
    if ( ds->rowCount() != 0 ) {
        ds->first();	// init datasource
        ds->setAvailable(true);
    }

    benchmark("Open datasource");
    return true;
}

bool NCReportDirector::closeDataSources( bool all, NCReportDataSource::OpenRoles role )
{
    QHash<QString, NCReportDataSource*>::const_iterator i = m_reportDef->dataSources().constBegin();
    while (i != m_reportDef->dataSources().constEnd()) {
        NCReportDataSource* ds = i.value();

        if ( all || (!all && ds->openRole() == role) ) {
            ds->close();
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Close datasource \"%s\",%i", qPrintable(ds->id()), ds->dataSourceType() );
#endif
        }
        ++i;
    }
    return true;
}

void NCReportDirector::cancel()
{
    setFlag(FlagFinished, true);
    m_state = Finished;
    m_reportDef->error()->setError( tr("Report process cancelled." ) );
}

void NCReportDirector::pageBegin()
{
    resetVariablesByScope( NCReportVariable::Page );
    if ( m_reportDef->error()->error() )
        return;
    m_state = PageBegin;
    nextPageNum();
    m_pageStatus = pageStatus( globalPageNum() );

//    PageInfo pageInfo;
//    pageInfo.pageno = globalPageNum();
//    pageInfo.status = m_pageStatus;
//    if (globalPageNum()==1)
//        m_pageInfoList.clear();
//    m_pageInfoList.append(pageInfo);

    if ( globalPageNum() > 1 && m_pageStatus == Printable && m_allowNewPage /*&& m_previousPageStatus == Printable && m_passState != TestPass*/ ) {
        m_report->output()->newPage(); // because of page range filtering
    }
    if (m_pageStatus==Printable)
        m_allowNewPage = true;

#ifdef NCRDIRECTOR_PROCESS_INDICATOR
    emit pageProgress(globalPageNum());
    if (m_processEvents)
        QApplication::processEvents();
#endif
    setPaintPosX(m_reportDef->option().leftMarginMM());
    setPaintPosY(m_reportDef->option().topMarginMM());
    if ( !renderSection( m_reportDef->pageHeader() ) )
        m_reportDef->error()->setError( tr("Page header is too large." ) );
}

void NCReportDirector::pageEnd()
{
    m_report->setLastPagePositionMM( paintPosMM() ); // save last position:

    if (m_report->output()->isSuccessive()) {
        paintUnregText();
        return;
    }
    if (m_reportDef->error()->error())
        return;
    m_state = PageEnd;
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("--------- PAGE FOOTER ---------");
#endif
    NCReportSection *section = m_reportDef->pageFooter();
    if ( !section )
        return;
    if ( !section->isShown() )
        return;

    setPaintPosX(m_reportDef->option().leftMarginMM());
    setPaintPosY(m_reportDef->option().pageHeightMM() - m_reportDef->option().bottomMarginMM() - section->metricHeight());

    NCReportSectionDirector sDirector(this);
    if ( !sDirector.simpleRenderSection(section) )
        m_reportDef->error()->setError( tr("Page footer is too large." ) );
    paintUnregText();

    if ( pageNum() == m_report->output()->pageRangeTo() && m_passState != TestPass ) {
        // out of page range
        setFlag(FlagFinished, true);
        m_state = Finished;
        return;
    }
}

void NCReportDirector::renderReportHeader()
{
    if ( !m_reportDef->reportHeader() ) {
        setFlag(FlagRHeader, false);
        return;
    }

    if ( !flag(FlagRHeader) || m_reportDef->error()->error() )
        return;

    if ( renderSection( m_reportDef->reportHeader() ) )
        setFlag(FlagRHeader, false);
    else // error, the report header too large
        m_reportDef->error()->setError( tr("Report header is too large." ) );
}

void NCReportDirector::renderReportFooter()
{
    if ( !m_reportDef->reportFooter() ) {
        setFlag(FlagRFooter, false);
        return;
    }

    if ( !flag(FlagFinished) || !flag(FlagRFooter) || m_reportDef->error()->error() )
        return;

    if (renderSection( m_reportDef->reportFooter() ) )
        setFlag(FlagRFooter, false);
    else
        m_reportDef->error()->setError( tr("Report footer is too large." ) );
}

bool NCReportDirector::renderDetailsAndGroups()
{
    if (flag(FlagFinished))
        return false;

    bool renderingDone = true;
    NCReportSection *detail = currentDetail();
    NCReportDataSource *cDataSource = currentDataSource();
    if ( !detail || !cDataSource)
        return false;

    NCReportDataSource *cDataSource2 = 0;
    if (!detail->secondaryDatasourceID().isEmpty()) {
        cDataSource2 = m_reportDef->dataSource(detail->secondaryDatasourceID());
    }

    if ( cDataSource->rowCount() == 0 ) {	// is there a valid datasource connected to detail

        if (activateNextDetail()) {
            // OK
            detail = currentDetail();
            cDataSource = currentDataSource();
        } else {
            return false;
        }
    }
    //-------------------------------------------
    // Page break evaluation
    //-------------------------------------------
    if ( !detail->pageBreakExp().isEmpty() ) {
        if (m_evaluator->evaluatePrintWhen(detail->pageBreakExp())) {
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("pageBreakExp - pagebreak");
#endif
            pageBreak(false,true);
        }
    }

    if (flag(FlagFinished))
        return false;

    //-------------------------------------------
    // group OUT
    //-------------------------------------------
    if ( !groupOut() )
        return false;

    if ( !flag(FlagBeginReport) ) {
        //-----------------------------------------------------------------------------------------
        // update after groupOut, before groupIn. This is important for variable synchronization.
        //-----------------------------------------------------------------------------------------
        if ( flag(FlagNoUpdate) ) {
            // flag was set ON for avoidance the unneccessary update of variables, fields...
            // This occures when a groupIn could not run (i.e. no space for group header)
            setFlag(FlagNoUpdate, false);
        } else {
            updateVariables();
        }
    }
    //-------------------------------------------
    // group IN
    //-------------------------------------------
    if ( !groupIn() ) {
        setFlag(FlagNoUpdate, true);
        return false;
    }
    //-------------------------------------------
    // RENDER DETAIL SECTION
    //-------------------------------------------
    renderingDone = renderSection( detail );
    if (flag(FlagFinished))
        return false;

    if ( renderingDone  ) {
        setFlag(FlagBeginReport, false);
        m_state = PageOnProcess;
        //--------------------------------------
        // Activate the NEXT row in datasource
        //--------------------------------------
        if ( nextRecord( cDataSource ) ) {
            //--------------------------------------
            // NEXT row is activated in datasource
            //--------------------------------------
            ++m_currentRow;
            if (cDataSource2) // Detail has a secondary data source, which is synchronized to the primary data source.
                cDataSource2->setAvailable( nextRecord( cDataSource2 ) );

#ifdef NCRDIRECTOR_PROCESS_INDICATOR
            emit dataRowProgress( cDataSource->id(), progressCurrentRow(cDataSource) );
            emit dataRowProgress( progressCurrentRow(cDataSource) );
#endif
            updateGroups( detail );
        } else {
            //-----------------------------------
            // no more records in the datasource
            //-----------------------------------
            setFlag(FlagLastRecord,true);
            if ( !detail->isFinished() ) {
                if ( !groupOut( true ) )
                    return false;  //there was not enough place for final group out

                detail->setFinished( true );	// finished only when groupOut(true) (final) succesfully done
            }
            activateNextDetail();
        }

    } else {
        // Could not render detail -> page break;
        setFlag(FlagNoUpdate, true);
        renderingDone = false;
        if ( m_state == PageBegin )
            m_reportDef->error()->setError( tr("Detail section is too large." ) );
    }

    return renderingDone;
}

bool NCReportDirector::activateNextDetail()
{
    while ( isNextDetail() ) { // have next detail?
        m_currentRow = 0;
        m_currentDetailId++;

        NCReportDataSource *ds = currentDataSource();
        if ( !ds || ds->rowCount() == 0 )
            continue;

#ifdef NCRDIRECTOR_PROCESS_INDICATOR
        emit dataRowCount(  progressRowCount(ds) );	// emit current rowCount signal
#endif
        NCReportSection *detail = currentDetail();

        detail->setFinished(false);
        firstRecord(ds);

        resetVariables(CurrentDataSourceOnly);

        initGroupStacks();
        updateGroups( detail );	// init 1st detail group

        // VERIFY PAGE BREAK CONDITION AND IF DETAIL IS SUPPOSED TO BE PRINTED
        if ( detail->startsOnNewPage() && m_evaluator->evaluatePrintWhen(detail->printWhenExp()))
            pageBreak(false,false);

        setFlag(FlagLastRecord,false);
        setFlag(FlagBeginReport,false);
        return true;
    }
    setFlag(FlagFinished, true);

    if ( currentDataSource() )
        lastRecord(currentDataSource()); // This is necessary because of the fields at the last page footer
    return false;
}

bool NCReportDirector::isNextDetail() const
{
    return m_currentDetailId < m_reportDef->details().count()-1;
}

/*!
 * \brief NCReportDirector::nextRecord
 * \param ds
 * \return true if succeeded
 * Steps to next record in the ds datasource
 */
bool NCReportDirector::nextRecord( NCReportDataSource* ds , bool isSamplingPageBreak)
{
    if (m_currentRow>1000000) {
        // endless loop
        fatalError(tr("Too much processed data records. Endless loop is probably happening."));
        return false;
    }

//    if ( !ds ) ds = currentDataSource();
//    if ( !ds ) return false;

//    NCReportDataSourceRelation *rel = m_dataSourceRelation->find(ds);
//    return rel->nextRecord(m_evaluator);

    //return m_dataSourceRelation->child(0)->nextRecord(m_evaluator);
    return  m_dataSourceRelation->nextRecord( ds ? ds : currentDataSource(), isSamplingPageBreak);
}

/*!
 * \brief NCReportDirector::previousRecord
 * \param ds
 * \return true if succeded
 * Steps to previous record in the datasource
 */
bool NCReportDirector::previousRecord( NCReportDataSource* ds)
{
    return  m_dataSourceRelation->previousRecord( ds ? ds : currentDataSource());
}

bool NCReportDirector::firstRecord(NCReportDataSource *ds)
{
    return  m_dataSourceRelation->firstRecord( ds ? ds : currentDataSource());
}

bool NCReportDirector::lastRecord(NCReportDataSource *ds)
{
    return  m_dataSourceRelation->lastRecord( ds ? ds : currentDataSource());
}

/*!
 * \brief NCReportDirector::updateGroupDataSources
 * \param groupID
 * \return true if success
 * Updates all datasources are assigned to a group by group ID
 */
bool NCReportDirector::updateGroupDataSources( const QString& groupID ) const
{
    bool ok = true;
    QHash<QString, NCReportDataSource*>::const_iterator i = m_reportDef->dataSources().constBegin();
    while (i != m_reportDef->dataSources().constEnd()) {
        NCReportDataSource *ds = i.value();
        if ( ds->openRole() == NCReportDataSource::GroupChange && ds->parentId() == groupID ) {
            ds->evaluate( m_evaluator );
            ok = ds->update();
            if ( ok ) ds->first();
        }
        if (!ok)
            break;
        ++i;
    }
    return ok;
}

bool NCReportDirector::updateDataSource( NCReportDataSource* ds ) const
{
    if ( !ds )
        return false;
    ds->evaluate( m_evaluator );
    bool ok = ds->update();
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("ReQuery: %s", qPrintable(ds->specText()));
#endif
    if ( ok ) {
        ds->first();
    } else {
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("ReQuery error.");
#endif
    }

    return ok;
}

bool NCReportDirector::renderSection(NCReportSection *section )
{
    if ( !section )
        return true;

    if (section->isPrintLock())
        return true;

    benchmark("Render section begin");

#ifdef NCRDIRECTOR_PROCESS_INDICATOR
    emit sectionProgress( section->id() );
#endif
    //ProcessState old_state = state;
    switch ( section->type() ) {
    case NCReportSection::Detail:
    case NCReportSection::GroupFooter:
    case NCReportSection::GroupHeader:
    case NCReportSection::PageFooter:
    case NCReportSection::PageHeader:
    case NCReportSection::ReportHeader:
    case NCReportSection::Custom:
        break;
    case NCReportSection::ReportFooter:
        {
        //state = ReportFooter;
        if (section->startsOnNewPage())
            pageBreak(false,true);
        break;
        }
    }
    bool ok = true;
    bool trimCurrentRecord= false;

    if (m_reportDef->option().reportType() == NCReport::Report && section->type() == NCReportSection::GroupFooter ) {
        //---------------------------------------------------
        // trim current record because of correct printWhen
        //---------------------------------------------------
        trimCurrentRecord = !flag(FlagLastRecord) && m_reportDef->option().isDataTrimming();
        if ( trimCurrentRecord )
            trimCurrentRecord = previousRecord( currentDataSource() );

    }

    if (section->isAnchorToPageFooter())
        anchorBottomPaintPosY(section);

    //---------------------------------------------------
    // 1st try
    //---------------------------------------------------
    {
        NCReportSectionDirector sDirector(this);
        ok = sDirector.renderSection(section);
    }
    if ( ok ) {
        if ( section->type() != NCReportSection::GroupHeader )
            setGroupHeaderPrintLocks(false); // Turn off group header print lock

    } else {
        // 2009.04.29: probably there is not enough room.
        if ( (section->type() == NCReportSection::PageHeader || section->type() == NCReportSection::PageFooter) ||
            section->metricHeight() > (m_reportDef->option().pageHeightMM() - m_reportDef->option().bottomMarginMM() - m_reportDef->option().topMarginMM() ) ) {
            // FATAL ERROR, Page header/footer must have enough space or invalid section size
            fatalError( tr("Invalid section size: %1").arg(section->tagname()) );
        } else {
            // Now we do the page break here because there is no room enough.

            //bool protection = (state == NCReportDirector::GroupHeader && section->checkRole() == NCReportSection::SectionPlusOrphanedTolerance );
            //if (protection)
            //	reprintGroupProtection(true, section);	// avoid bad reprint group

            bool trimRec = !trimCurrentRecord && (section->type() == NCReportSection::GroupHeader ||
                 (section->type() == NCReportSection::Detail && (m_lastRenderedSection && m_lastRenderedSection->type() == NCReportSection::Detail)));
            pageBreak( trimRec, true);
            //if (protection)
            //	reprintGroupProtection(false,section);	// restore

            if ( section->isPrintLock() ) {
                ok = true;
            } else {
                NCReportSectionDirector sDirector(this);
                ok = sDirector.renderSection(section);
            }
        }
    }
    //state = old_state;
    //---------------------------------------------------
    // restore trim
    //---------------------------------------------------
    if ( trimCurrentRecord )
        nextRecord( currentDataSource() );

    benchmark("Render section end");
    return ok;
}

qreal NCReportDirector::spaceToBottom() const
{
    if (m_report->output()->isSuccessive())
        return 999999.0;

    qreal footerHeightMM = 0.0;
    if ( m_reportDef->pageFooter() )
        footerHeightMM = m_reportDef->pageFooter()->metricHeight();

    qreal mBottomY = m_reportDef->option().pageHeightMM() - m_reportDef->option().bottomMarginMM() - footerHeightMM;
    return mBottomY - paintPosMM().y();
}

/*!
 * \brief resetPageNum
 * Resets the report level page number counter.
 */

void NCReportDirector::resetPageNum()
{
    m_pageNo = 0;
}

/*!
 * \brief NCReportDirector::resetGlobalPageNum
 * Resets the global page numbering. Global page number is used in batch mode.
 */
void NCReportDirector::resetGlobalPageNum()
{
    m_globalPageNo = 0;
}

void NCReportDirector::nextPageNum()
{
    m_pageNo++;
    m_globalPageNo++;
}

QList<NCReportDirector::SectionInfo> &NCReportDirector::sectionInfoList()
{
    return m_sectionInfoList;
}

NCReportDirector::PassState NCReportDirector::currentPassState() const
{
    return m_passState;
}

NCReportDirector::ProcessState NCReportDirector::currentProcessState() const
{
    return m_state;
}

int &NCReportDirector::sectionCounter()
{
    return m_sectionCounter;
}

void NCReportDirector::setProcessEvents(bool enable)
{
    m_processEvents = enable;
}

bool NCReportDirector::isProcessEvents() const
{
    return m_processEvents;
}

/*!
 * \brief NCReportDirector::finish forces to finish the report
 */
void NCReportDirector::finish()
{
    setFlag(FlagFinished,true);
}

bool NCReportDirector::isFinished() const
{
    return flag(FlagFinished);
}

void NCReportDirector::updateDynamicItems()
{
    return;
}

void NCReportDirector::updateSectionDynamicItems( NCReportSection * )
{
    return;
}

void NCReportDirector::updateVariables()
{
    QHash<QString,NCReportVariable*>::const_iterator i = m_reportDef->variables().begin();
    while (i != m_reportDef->variables().end()) {
        NCReportVariable *var = i.value();
        bool ok = m_evaluator->evaluateVariable( var );
        if (!ok)
            fatalError( tr("Variable error in %1").arg(var->id()));

#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("Update variable: %s = %f", qPrintable(var->id()), var->value().toDouble());
#endif
        ++i;
    }
}

void NCReportDirector::updateGroups( NCReportSection *d )
{
    if ( !d )
        return;

    bool changed = false;
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("UPDATE GROUPS ...");
#endif
    QList<NCReportGroup*>::iterator i;
    for (i = d->groups().begin(); i != d->groups().end(); ++i) {
        NCReportGroup *g = *i;
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("Group ID: %s Value: %s",qPrintable(g->groupId()), qPrintable(g->value()) );
#endif

        if (m_evaluator->evaluateGroup( g )) {
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Group ID: %s New value: %s Changed? %i",qPrintable(g->groupId()), qPrintable(g->value()), g->isGroupChanged() );
#endif
            //--------------------------------------------------------
            // IMPORTANT! (FIX 2008-06-04)
            // IF A HIGHER LEVEL GROUP CHANGES THEN ALL
            // OTHER LOWER LEVEL GROUPS MUST ALSO BE CHANGED!
            //--------------------------------------------------------
            if ( g->isGroupChanged() )
                changed = true;
            if ( changed )
                g->setGroupChanged( true );

        } else {
            // Invalid group expression
            fatalError(tr("Invalid group column or expression: %1\nGroup ID: %2\nValue: %3").arg(g->expression()).arg(g->groupId()).arg(g->value()));
        }
    }
}

void NCReportDirector::initGroupStacks()
{
    inside.clear();
    outside.clear();

    NCReportSection *d = currentDetail();

    // WE MUST TURN FOR END!
    QStack<NCReportGroup*> stackTemp;
    for ( int i=0; i<d->groups().count(); ++i ) {
        NCReportGroup* g = d->groups().at(i);
        if (g->header())
            g->header()->setPrintLock(false);
        if (g->footer())
            g->footer()->setPrintLock(false);

        g->setInitialPrintDone(false);
        g->reset();
        stackTemp.push( g );
    }
    while (!stackTemp.isEmpty())
        outside.push( stackTemp.pop() );

}

bool NCReportDirector::groupIn()
{
    bool pageBreakDone = false; // only one page break is allowed.

    while ( !outside.isEmpty() ) {	//empty the outside stack
        NCReportGroup *group = outside.top();
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("groupIn (ID: %s Value: %s): isGroupChanged=%i printlock=%i", qPrintable(group->groupId()), qPrintable(group->value()), group->isGroupChanged(), group->printLock() );
#endif
        if ( group->isGroupChanged() ) {
            //------------------------------------------------------------------------
            // group changed, needs to render group header
            //------------------------------------------------------------------------

            updateGroupDataSources( group->groupId() );		// re-query group level datasources

            //-----------------------------------------------------------------------------------------------------
            // Page break evaluation. (Q: If the Detail starts on new page but the group not, it should be yes?)
            //-----------------------------------------------------------------------------------------------------
            bool groupPageBreak = (group->startsOnNewPage() && group->initialPrintDone() && !pageBreakDone)
                    || (group->initialPrintDone() && !pageBreakDone && !group->pageBreakExp().isEmpty() && m_evaluator->evaluatePrintWhen(group->pageBreakExp()));

            if ( groupPageBreak) {
                //------------------------------------------------------------------------
                //pageBreak( currentDataSource()->hasNext(), true );
                //------------------------------------------------------------------------
                pageBreak( true, true );
                pageBreakDone = true;
            }
            variableCorrections(true,200);
            bool ok = renderSection(group->header());
            variableCorrections(false,200);
            if ( ok )  {
                // gr.header succeeded
                inside.push( outside.pop() );	// group goes to inside
                group->setInitialPrintDone( true );
            } else {
                // no space for gr.header
                return false;
            }
        } else {
            // nothing changed
            return true;
        }
    }
    return true;
}

bool NCReportDirector::groupOut( bool final )
{
    while ( !inside.isEmpty() ) {	//empty the outside stack
        NCReportGroup *group = inside.top();
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("groupOut (ID: %s Value: %s): final=%i isGroupChanged=%i printlock=%i", qPrintable(group->groupId()), qPrintable(group->value()), final, group->isGroupChanged(), group->printLock() );
#endif
        if ( group->isGroupChanged() || final ) {
            // group changed, needs to render group footer

            if ( renderSection( group->footer() ) ) {
                // gr.header succeeded
                outside.push( inside.pop() );	// group goes to outside
                resetGroup( outside.top() );
            } else {
                // no space for gr.footer
                return false;
            }
        } else {
            // nothing changed
            return true;
        }
    }
    return true;
}

void NCReportDirector::resetGroup( NCReportGroup* g )
{
    if (!g)
        return;

    if (g->header())
        g->header()->setPrintLock(false);

    if (g->resetVarList().isEmpty() )
        return;
    QStringList list = g->resetVarList().split(',');
    for (int i=0; i< list.size(); ++i) {
        if ( m_reportDef->variables().contains(list.at(i)) ) {
            NCReportVariable *var = m_reportDef->variables().value(list.at(i));
            if ( var )
                var->reset();
        }
    }
}

NCReportSection* NCReportDirector::currentDetail()
{
    return m_reportDef->details().at(m_currentDetailId);
}

NCReportDataSource* NCReportDirector::currentDataSource()
{
    NCReportSection * d = currentDetail();
    NCReportDataSource *ds = 0;
    if (d)
        ds = m_reportDef->dataSource(d->dataSourceId());
    return ds;
}

bool NCReportDirector::renderRepeatedGroupHeaders()
{
    if ( pageNum() == 1 || flag(FlagBeginReport) )
        return true;

    NCReportSection *d = currentDetail();

    int groups = d->groups().count();
    for ( int i=0; i<groups; ++i ) {
        NCReportGroup *g = d->groups().at(i);

        if (g->initialPrintDone() && inside.contains(g) ) {
            // repeating must happening only when initial print of a group header
            // is already done and group level is not closed. Otherwise not!

            #ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Repeat group header: group:%s locked:%i", qPrintable(g->groupId()), g->printLock() );
            #endif
            if ( g->repeatHeaderOnNextPage() && g->header() ) {
                if ( renderSection( g->header() ) ) {
                    g->header()->setPrintLock(true);
                } else {
                    return false;
                }
            }
        }
    }
    return true;
}

void NCReportDirector::setGroupHeaderPrintLocks( bool enable )
{
    NCReportSection *d = currentDetail();
    int groups = d->groups().count();
    for ( int i=0; i<groups; ++i ) {
        NCReportGroup *g = d->groups().at(i);
        if (g->header())
            g->header()->setPrintLock(enable);
    }
}


/*!
  Protect group headers unneccessarily reprint when a pageBreak
  If section is set then we force the protection into a section (group header)
  */
void NCReportDirector::reprintGroupProtection( bool enable, NCReportSection* section )
{
    return; // obsolete

    NCReportSection *detail = currentDetail();

    bool groupStartsOnNewPage = false;
    int level = 0;
    int level_of_groupStartsOnNewPage=0;

    QList<NCReportGroup*>::iterator i;
    for (i = detail->groups().begin(); i != detail->groups().end(); ++i) {
        NCReportGroup *g = *i;
        if (enable) {
            // ENABLE
            if ( g->header() == section || g->startsOnNewPage() ) {
                // LOCK THE REPRINT OF NESTED LEVELS!
                level_of_groupStartsOnNewPage = level;
                groupStartsOnNewPage = true;
            }
            if (groupStartsOnNewPage && level > level_of_groupStartsOnNewPage) {
                //g->setPrintLock(true);
            }
        } else {
            // DISABLE LOCKING
            //g->setPrintLock(false);
        }
        ++level;
    }
}

/*!
Checks if a group is allowed to break a page or not.\n
pageBreak is allowed only if the groupheader (which begins to start in a new page)
hasn't got a (lower level) parent that have already done a pagebreak.
*/
bool NCReportDirector::pageBreakIsAllowed( NCReportGroup* )
{
    /*
    QList<NCReportGroup*>::iterator i;
    for (i = group->detail()->groups().begin(); i != group->detail()->groups().end(); ++i) {
        NCReportGroup *g = *i;
        if ( g->startsOnNewPage() && g->nestedLevel() < group->nestedLevel() )
            return false;
    }
    */
    return true;
}

void NCReportDirector::variableCorrections(bool enable, int callerID)
{
    if ( m_variableCorrectionCallerID < 0 && !enable )
        return; // restrict invalid disable

    if ( m_variableCorrectionCallerID >= 0 && callerID!= m_variableCorrectionCallerID)
        return;	// to avoid repeated call by another ID

    if ( m_variableCorrectionCallerID >= 0 && callerID == m_variableCorrectionCallerID && enable )
        return; // restrict double enable

    QHash<QString,NCReportVariable*>::const_iterator i = m_reportDef->variables().begin();
    while (i != m_reportDef->variables().end()) {
        NCReportVariable *var = i.value();
        if (currentDataSource() == var->relatedDataSource() )
            var->correction( enable ? NCReportVariable::Undo : NCReportVariable::Redo );
        ++i;
    }

    if (enable)
        m_variableCorrectionCallerID = callerID;
    else
        m_variableCorrectionCallerID = -1;
}

void NCReportDirector::registerLastPrintedSection()
{
    if ( currentPassState() == TestPass && m_sectionInfoList.count()>0 ) {
        m_sectionInfoList.last().lastSectionOfThePage = true;
    }
}

int NCReportDirector::progressRowCount(NCReportDataSource *ds) const
{
    switch (m_passState) {
    case Off:
        return ds->rowCount();
    case TestPass:
    case RealPass:
        return ds->rowCount()*2;
    }

    return 0;
}

int NCReportDirector::progressCurrentRow(NCReportDataSource *ds) const
{
/*    if ( m_reportDef->option().isDoublePassMode() ) {*/
    switch (m_passState) {
    case Off:
    case TestPass:
        return m_currentRow;
    case RealPass:
        return ds->rowCount()+m_currentRow;
    }
    return 0;
}

int NCReportDirector::pageNum() const
{
    return m_pageNo;
}

int NCReportDirector::globalPageNum() const
{
    return m_globalPageNo;
}

int NCReportDirector::pageCount() const
{
    return m_pageCount;
}

int NCReportDirector::reportNumber() const
{
    return m_reportNo;
}

int NCReportDirector::reportCount() const
{
    return m_reportCount;
}

void NCReportDirector::setReportNumber( int no )
{
    m_reportNo = no;
}

void NCReportDirector::setReportCount( int cnt )
{
    m_reportCount = cnt;
}

int NCReportDirector::numForceCopies() const
{
    return m_forceCopies;
}

int NCReportDirector::currentRow() const
{
    return m_currentRow;
}

int NCReportDirector::currentForceCopy() const
{
    return m_currentForceCopy;
}

void NCReportDirector::setForceCopies(int num)
{
    m_forceCopies = num;
}

void NCReportDirector::setCurrentForceCopy(int num)
{
    m_currentForceCopy = num;
}

void NCReportDirector::resetVariables(VariableResetMode mode)
{
    QHash<QString, NCReportVariable*>::const_iterator i = m_reportDef->variables().constBegin();
    while (i != m_reportDef->variables().constEnd()) {
        NCReportVariable* var = i.value();

        if (mode == All) {
            var->reset();
        } else if ( mode == CurrentDataSourceOnly ) {
            if (currentDataSource() == var->relatedDataSource() )
                var->reset();
        }
        ++i;
    }
}

void NCReportDirector::resetVariablesByScope(NCReportVariable::ResetScope scope)
{
    QHash<QString, NCReportVariable*>::const_iterator i = m_reportDef->variables().constBegin();
    while (i != m_reportDef->variables().constEnd()) {
        NCReportVariable* var = i.value();
        if (var->scope() == scope ) {
            var->reset();
        }
        ++i;
    }
}

void NCReportDirector::pageBreak( bool trimRecord, bool doRepeatGroupHeaders )
{
    if (m_report->output()->isSuccessive())
        return;	// never breaks

    if (m_pageBreakIsOnProcess)
        return;	// avoid stack overflow

    if (m_pageBreakStop) {
        // page break stop was enabled
        m_pageBreakStop = false;
        return;
    }
    m_pageBreakIsOnProcess = true;
    beforePageBreak();

    //-------------------------------------------------------------
    // trim record: to prevent the next record status in page footer & header
    //-------------------------------------------------------------
    trimRecord = trimRecord && m_reportDef->option().isDataTrimming();
    if (trimRecord) {
        trimRecord = previousRecord( currentDataSource() );
        variableCorrections(true,300);
    }

    registerLastPrintedSection();
    pageEnd();
//    if (flag(FlagFinished)) {   - commented because of wrong/missing report footer
//        m_pageBreakIsOnProcess = false;
//        return;
//    }
    pageBegin();

    if (trimRecord) {
        variableCorrections(false,300);
        nextRecord( currentDataSource() ,true);
    }

    if ( doRepeatGroupHeaders )
        renderRepeatedGroupHeaders();

    afterPageBreak();
    m_pageBreakIsOnProcess = false;
}

qreal NCReportDirector::sectionWidthMM( NCReportSection* ) const
{
    // UNTIL COLUMN HANDLING IS UNAVAILABLE
    return m_reportDef->option().purePageSize().width();
}

NCReportDirector::PageStatus NCReportDirector::currentPageStatus() const
{
    return m_pageStatus;
}

NCReportDirector::PageStatus NCReportDirector::pageStatus( int page) const
{
    PageStatus ps = Hidden;
    if (m_report->output()->isPageInRange(page))
        ps = Printable;
    return ps;
}

/*!
Sets back or restores the last values of the fields in a specified section
*/
//void NCReportDirector::restoreFields( NCReportSection* section, uint mode )
//{
//    if ( !section )
//        return;
//    QString tmp;
//    QListIterator<NCReportItem*> i(section->reportItems());
//    while ( i.hasNext() ) {
//        NCReportItem *item = i.next(); //mSection->reportItems().at(i);
//        switch ( item->objectType() ) {
//            case NCReportItem::Field: {
//                NCReportFieldItem *field = (NCReportFieldItem*)item;
//                field->swapLastValue( (NCReportFieldItem::SwapMode)mode );
//            }
//            default:
//                break;
//        }
//    }
//}

bool NCReportDirector::flag( Flags flag ) const
{
    return m_flags[flag];
}

void NCReportDirector::setFlag( Flags flag, bool set )
{
    m_flags[(int)flag] = set;
}

void NCReportDirector::benchmark( const QString& msg )
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("*** Runtime (%s) :\t%i", qPrintable(msg), m_runtime.elapsed() );
#else
    Q_UNUSED(msg);
#endif
}

void NCReportDirector::paintUnregText()
{
#ifdef NCREPORT_EVAL
    QPainter *p = m_report->output()->painter();
    if (!p)
        return;

    // Under Page text
    p->setPen( QPen(Qt::black) );
    QFont font;
    font.setPointSize(6);
    p->setFont( font );
    QPointF pointmm( 5.0, m_reportDef->option().pageHeightMM() - 5.0 );
    p->drawText( m_report->output()->scale().mmToPoint( pointmm ), QString("Generated by %1 %2 EVALUATION VERSION %3 %4")
                 .arg(NCREPORTAPP_NAME,NCREPORTAPP_VERSION,NCREPORTAPP_COPYRIGHT,NCREPORTAPP_WEB) );

    // Watermark
    QString wmt("EVALUATION VERSION");
    pointmm.setX( m_reportDef->option().pageWidthMM() / 2 );
    pointmm.setY( m_reportDef->option().pageHeightMM() / 2 );
    QPointF pointPixel = m_report->output()->scale().mmToPoint( pointmm );

    font.setPointSize(32);
    font.setBold(true);

    QFontMetricsF fm(font,p->device());
    pointPixel.ry() -= fm.height()/2;
    pointPixel.rx() -= fm.width(wmt)/2;

    p->setFont(font);
    p->setPen( QPen(QColor(128,32,32,64) ));
    p->drawText( pointPixel, wmt);
#endif
}

void NCReportDirector::fatalError(const QString &msg)
{
    m_reportDef->error()->setError( msg );
    setFlag(FlagFinished, true);
    m_state = Finished;
}

void NCReportDirector::addSlicedItem(NCReportItem *item)
{
    m_slicedItems.append(item);
}

/*!
  Enables one time page break stop to avoid a duplicated page break. This is mostly called by section director.
  */
void NCReportDirector::setPageBreakStop()
{
    m_pageBreakStop = true;
}
/*!
  Trigger for executing needed things before page break action
  */
void NCReportDirector::beforePageBreak()
{
}

/*!
  Trigger for executing needed things after page break action
  */
void NCReportDirector::afterPageBreak()
{
    /*
    for ( int i=0; i<rdef->fields().count(); ++i ) {
        NCReportFieldItem *field = rdef->fields().at(i);
        if (field->hideIfRepeated())
            field->resetHideRepeated();
    }
    */
    NCReportSection * detail = currentDetail();

    QListIterator<NCReportItem*> i(detail->reportItems());
    while ( i.hasNext() ) {
        NCReportItem *item = i.next(); //mSection->reportItems().at(i);
        if (item->objectType() == NCReportItem::Field) {
            NCReportFieldItem *field = (NCReportFieldItem*)item;
            field->resetHideRepeated();
        }
    }

}

QPointF NCReportDirector::paintPosMM() const
{
    return m_paintPosMM;
}

/*
QPointF& NCReportDirector::rPaintPosMM()
{
    return mPaintPos;
}
*/

void NCReportDirector::translatePaintPosY(qreal byMM)
{
    m_paintPosMM.ry() += byMM;
}

void NCReportDirector::setPaintPosY(qreal yMM)
{
    m_paintPosMM.setY(yMM);
}

void NCReportDirector::setPaintPosX(qreal xMM)
{
    m_paintPosMM.setX(xMM);
}

void NCReportDirector::anchorBottomPaintPosY(NCReportSection *section)
{
    if (!section || section->isAutoHeight() || !section->isShown())
        return;

    qreal spaceMM = spaceToBottom();
    if ( spaceToBottom()>section->metricHeight() )
        translatePaintPosY( spaceMM - section->metricHeight() );

//	int footerHeightMM = 0;
//	if ( rdef->pageFooter() && rdef->pageFooter()->isShown())
//		footerHeightMM = rdef->pageFooter()->metricHeight();

//	setPaintPosY(rdef->option().pageHeightMM() - rdef->option().bottomMargin() - footerHeightMM - section->metricHeight() );
}

