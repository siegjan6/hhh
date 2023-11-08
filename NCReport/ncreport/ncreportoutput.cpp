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
#include "ncreportoutput.h"
#include "ncreportpageoption.h"
#include "ncreport.h"

#include <QPainter>

NCReportOutput::NCReportOutput( QObject* parent ) : QObject( parent ),
    m_outputType(Preview),
    m_genMode(DirectPainter),
    m_painter(0),
    m_parentWidget(0),

    m_numCopies(1),
    m_fromPage(1),
    m_toPage(9999),
    m_reportno(0),
    m_rotation(0),

    m_showPrintDialog(false),
    m_autoDelete(true),
    m_isDocumentTemplateBased(false),
    m_isEndlessMedia(false),
    m_batchMode(false),
    m_flagFirst(false),
    m_flagLast(false),
    m_flagPrinting(false)
{
}

NCReportOutput::~ NCReportOutput()
{
    deletePainter();
}

void NCReportOutput::setBatchMode( bool set )
{
    m_batchMode = set;
}

bool NCReportOutput::batchMode()
{
    return m_batchMode;
}

void NCReportOutput::setOutput(OutputType o )
{
    m_outputType = o;
}

NCReportOutput::OutputType NCReportOutput::output() const
{
    return m_outputType;
}

void NCReportOutput::setPageOption( const NCReportPageOption& o )
{
    m_pageOption =o;
}

NCReportPageOption& NCReportOutput::option()
{
    return m_pageOption;
}

bool NCReportOutput::isAutoDelete() const
{
    return m_autoDelete;
}

void NCReportOutput::setAutoDelete(bool set )
{
    m_autoDelete = set;
}

void NCReportOutput::setFileName(const QString & fn)
{
    m_fileName = fn;
}

QString NCReportOutput::fileName() const
{
    return m_fileName;
}

void NCReportOutput::setShowPrintDialog( bool set )
{
    m_showPrintDialog = set;
}

bool NCReportOutput::showPrintDialog()
{
    return m_showPrintDialog;
}

void NCReportOutput::setCopies( int num )
{
    m_numCopies = num;
}

int NCReportOutput::copies() const
{
    return m_numCopies;
}

QPainter * NCReportOutput::painter()
{
    return m_painter;
}

void NCReportOutput::deletePainter()
{
    if ( m_painter ) {
        delete m_painter;
        m_painter =0;
    }
}

bool NCReportOutput::hasValidRotationAngle()
{
    switch (m_rotation) {
    case 90:
    case -90:
    case 270:
    case 180:
        return true;
    default:
        break;
    }
    return false;
}


void NCReportOutput::setDocumentTemplateBased(bool set)
{
    m_isDocumentTemplateBased = set;
}

void NCReportOutput::setGeneratingMode(GeneratingMode mode)
{
    m_genMode = mode;
}

NCReportScale & NCReportOutput::scale()
{
    return m_scale;
}

void NCReportOutput::setPainter(QPainter *painter)
{
    m_painter = painter;
}

QWidget* NCReportOutput::parentWidget()
{
    return m_parentWidget;
}

void NCReportOutput::setParentWidget( QWidget* w )
{
    m_parentWidget = w;
}

/*!
 * \brief NCReportOutput::setPageRange
 * \param from
 * \param to
 * Sets the page range of printable pages. The pages out of this range will not be rendered or printed.
 */
void NCReportOutput::setPageRange( int from, int to )
{
    m_fromPage = from;
    m_toPage = to;
}

/*!
 * \brief NCReportOutput::setPageRanges
 * \param pages
 * Parses and sets the pange ranges by the string expression. The string can contain comma separated page numbers and/or page ranges.
 * The following examples are acceptable:
 * 1 or 1,2,3 or 1-3, or 1,4,7,8 or 1,2-5,7
 */
void NCReportOutput::setPageRanges(const QString &pageRanges)
{
    if (pageRanges.isEmpty()) {
        m_pageList.clear();
        return;
    }
    // parse page range expression
    if (pageRanges.contains(',')) {
        QStringList l = pageRanges.split(',',QString::SkipEmptyParts);
        for (int i=0;i<l.count();i++)
            setPageRanges(l.at(i));  // recursive call

    } else if (pageRanges.contains('-')) {
        int from = pageRanges.section('-',0,0).toInt();
        int to = pageRanges.section('-',1,1).toInt();
        for (int i=from;i<=to; i++)
            m_pageList.append(i);
    } else {
        m_pageList.append(pageRanges.toInt());
    }
}

/*!
 * \brief NCReportOutput::setPageRangeList
 * \param pageList
 * If a non empty list is set, enables support for rendering only the pages specified in the list.
 * If the list is empty it disables the page range limit. This does not affects the range set by setPageRange( int from, int to ) function.
 */
void NCReportOutput::setPageRangeList(const QList<int> &pageList)
{
    m_pageList = pageList;
}

/*!
 * \brief NCReportOutput::addToPageRangeList
 * \param page
 * Adds page to the page range list. Enables the mode when the stored list of pages will be rendered only.
 */
void NCReportOutput::addToPageRangeList(int page)
{
    m_pageList.append(page);
}

QList<int> NCReportOutput::pageRangeList() const
{
    return m_pageList;
}

int NCReportOutput::pageRangeFrom() const
{
    return m_fromPage;
}

int NCReportOutput::pageRangeTo() const
{
    return m_toPage;
}

bool NCReportOutput::isPageInRange(int page) const
{
    int p = page;
    if (p==0)
        p=1;

    bool inRange = false;
    if (!m_pageList.isEmpty()) {
        inRange = m_pageList.contains(p) && (p >= pageRangeFrom() && p <= pageRangeTo());
    } else {
        inRange = (p >= pageRangeFrom() && p <= pageRangeTo());
    }
    return inRange;
}

void NCReportOutput::renderItem( NCReportItem* item, const QRectF& rect )
{
    // virtual function
    Q_UNUSED(item);
    Q_UNUSED(rect);
}

/*!
  Sets the flag if this is the first report in batch mode
  */
void NCReportOutput::setFlagFirst( bool set )
{
    m_flagFirst = set;
}

/*!
  Sets the flag if this is the last report in batch mode
  */
void NCReportOutput::setFlagLast( bool set )
{
    m_flagLast = set;
}

/*!
 * \brief NCReportOutput::setFlagPrintStarted
 * \param set
 * Sets the print started flag. This flag is responsible for knowing if the real printing/rendering is already on progress.
 */
void NCReportOutput::setFlagPrintStarted(bool set)
{
    m_flagPrinting = set;
}

/*!
  Returns true if this is the first report in batch mode
  */
bool NCReportOutput::flagFirst() const
{
    return m_flagFirst;
}

/*!
  Returns true if this is the last report in batch mode
  */
bool NCReportOutput::flagLast() const
{
    return m_flagLast;
}

/*!
 * \brief NCReportOutput::flagPrinting
 * \return
 * Returns the flag if the real printing has already been started.
 */
bool NCReportOutput::flagPrintStarted() const
{
    return m_flagPrinting;
}

/*!
  Sets the report number. The numbering starts from 1. For batch mode
  */
void NCReportOutput::setReportNum( int reportno )
{
    m_reportno = reportno;
}

/*!
  Returns the number of the report. For batch mode.
  */
int NCReportOutput::reportNum() const
{
    return m_reportno;
}

int	NCReportOutput::resolution() const
{
    return (int)m_scale.dpi();
}

void NCReportOutput::setResolution( int dpi )
{
    m_scale.setDpi( dpi );
}

QPaintDevice* NCReportOutput::device() const
{
    return 0;
}

/*!
    Returns if the output is based on a (text) document template.
    This is true for HTML/XML/TEXT..etc type of templates.
  */
bool NCReportOutput::isDocumentTemplateBased() const
{
    return m_isDocumentTemplateBased;
}

/*!
  Virtual method for rendering section contents if the output is document based.
  */
bool NCReportOutput::renderSection(NCReportSection *, NCReportEvaluator *)
{
    return false;
}

void NCReportOutput::sectionStarts(NCReportSection *section, NCReportEvaluator *evaluator)
{
    Q_UNUSED(section)
    Q_UNUSED(evaluator)
}

void NCReportOutput::sectionEnds(NCReportSection *section, NCReportEvaluator *evaluator)
{
    Q_UNUSED(section)
    Q_UNUSED(evaluator)
}

void NCReportOutput::setSuccessive(bool set)
{
    m_isEndlessMedia = set;
}

bool NCReportOutput::isSuccessive() const
{
    return m_isEndlessMedia;
}

void NCReportOutput::setFinalPosMM(const QPointF &point)
{
    m_finalPosMM = point;
}

QPointF NCReportOutput::finalPosMM() const
{
    return m_finalPosMM;
}

NCReportOutput::GeneratingMode NCReportOutput::generatingMode() const
{
    return m_genMode;
}

void NCReportOutput::rotatePainter(QPainter *painter)
{
    if (hasValidRotationAngle()) {
        qreal v_trans = 0;
        qreal h_trans = 0;

        if (option().pageOrientation() == QPrinter::Portrait) {
            v_trans = scale().mmToPixel(option().pageWidthMM());
            h_trans = scale().mmToPixel(option().pageHeightMM());
        } else {
            v_trans = scale().mmToPixel(option().pageHeightMM());
            h_trans = scale().mmToPixel(option().pageWidthMM());
        }

        QTransform transform;
        switch (m_rotation) {
        case -90:
        case 270:
            transform.translate( 0, v_trans );
            break;
        case 90:
            transform.translate( h_trans, 0 );
            break;
        case 180:
            transform.translate( v_trans, h_trans );
            break;
        default:
            break;
        }

        transform.rotate(m_rotation);
        painter->setTransform(transform);
    }
}

void NCReportOutput::customizePainter(QPainter *painter)
{
    Q_UNUSED(painter)
}

int NCReportOutput::rotationAngle() const
{
    return m_rotation;
}

void NCReportOutput::setRotationAngle(int angle)
{
    m_rotation = angle;
}


//--------------------------------------------------------------------------

NCReportNullOutput::NCReportNullOutput(NCReportOutput * realOutput, QObject* parent) : NCReportOutput( parent ), m_realOutput(realOutput)
{
    setOutput( Null );
    setPageRange(0,0);
    scale().setDpi( realOutput->scale().dpi() );
}

NCReportNullOutput::~ NCReportNullOutput()
{
}

bool NCReportNullOutput::setup()
{
    return true;
}

void NCReportNullOutput::begin()
{
}

void NCReportNullOutput::end()
{
}

void NCReportNullOutput::newPage()
{
}

void NCReportNullOutput::cleanup()
{
}

QPaintDevice* NCReportNullOutput::device() const
{
    return m_realOutput->device();
}

