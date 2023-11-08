/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Ltd. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: office@nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*  Created: 2014.03.24. (nocisoft)
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance with the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  office@nocisoft.com if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncreportmarkuplanguageoutput.h"
#include "ncreportlabelitem.h"
#include "ncreportlineitem.h"
#include "ncreportrectitem.h"
#include "ncreporttextitem.h"
#include "ncreportimageitem.h"
#include "ncreportbarcodeitem.h"
#include "ncreportsection.h"
#include "ncreportdirector.h"
#include "ncreportchartitem.h"
#include "ncreportutils.h"

#include <QFile>
#include <QFileInfo>
#include <QBuffer>
#include <QTextDocument>
#include <QTextDocumentWriter>

//#define HTMLOUTPUT_DEBUGMODE

/*!
 * \brief NCReportMarkupLanguageOutput::NCReportMarkupLanguageOutput
 * \param parent
 */

NCReportMarkupLanguageOutput::NCReportMarkupLanguageOutput(QObject *parent) : NCReportOutput(parent), m_file(0), m_crlf("\n"),
    m_styleSheetMarker("<!-- ##CSS## -->"), m_sectionID("#####"), m_initalState(true), m_rowCounter(0), m_fileCounter(0), m_strategy(SectionOneTable), m_cssFile(QString())
{
    setOutput( Null );
    setDocumentTemplateBased(false);
    setGeneratingMode( DocumentFile );
}

NCReportMarkupLanguageOutput::~ NCReportMarkupLanguageOutput()
{
    qDebug("NCReportMarkupLanguageOutput deleted.");
}

bool NCReportMarkupLanguageOutput::setup()
{
    m_initalState = true;
    m_rowCounter = 0;
    m_fileCounter = 0;
    return true;
}

void NCReportMarkupLanguageOutput::begin()
{
//    m_file = new QFile(fileName());
//    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text))
//        return;
//    m_tsHtml.setDevice(m_file);
    m_styleClasses.clear();
    m_tsContent.setString(&m_content);
    m_tsStyle.setString(&m_style);
    m_initalState = true;

    writeHeader();
}

void NCReportMarkupLanguageOutput::end()
{
    if (!m_initalState)
        writeSectionEnd(0);

    writeFooter();

    // write style items
    QHash<QString, QString>::const_iterator i = m_styleClasses.constBegin();
    while (i != m_styleClasses.constEnd()) {
        m_tsStyle << i.key() << "{" << i.value() << "}" << m_crlf;
        ++i;
    }

    m_tsStyle.flush();
    m_tsContent.flush();

    if (m_cssFile.isEmpty())
        m_content.replace(m_styleSheetMarker,m_style);
    else {
        QFileInfo fi_html(fileName());

        if (m_cssFile=="@")
            m_cssFile = fi_html.path()+"/"+fi_html.baseName()+".css";

        QFileInfo fi_css(m_cssFile);

        QString url;
        if ( fi_css.isAbsolute() ) {
            url = "file:///";
        }

        m_content.replace(m_styleSheetMarker, styleSheetReference( url + m_cssFile ) );
        if ( fi_css.isRelative() )
            NCReportUtils::stringToFile( fi_html.path()+"/"+m_cssFile, m_style );
        else
            NCReportUtils::stringToFile( m_cssFile, m_style );
    }

    m_style.clear();
    NCReportUtils::stringToFile(fileName(), m_content, option().encoding().toLocal8Bit() );
    // ODF TEST:
    //writeDocumentFormats();
}

void NCReportMarkupLanguageOutput::newPage()
{
    // nothing to do
}

void NCReportMarkupLanguageOutput::cleanup()
{
    // we don't delete any pages
}

void NCReportMarkupLanguageOutput::renderItem( NCReportItem* item, const QRectF& rect )
{
    switch ( item->objectType() ) {
    case NCReportItem::Line:
    case NCReportItem::Rectangle:
    case NCReportItem::Ellipse:
    case NCReportItem::Chart:
#ifdef HIGHCHARTS_INTEGRATION
        break;
#endif
    case NCReportItem::CrossTab:
    case NCReportItem::Table:
    case NCReportItem::NoObject:
        return;
    default:
        break;
    }

    item->setPaintRect(rect);
    short x = item->metricPos().x()*100;
    short y = item->metricPos().y()*100;

    int itemKey = NCReportUtils::id2D::id(x,y);   // 2D identifier. Converts 2D short coordinates to int;
    qDebug("Insert item: %s x:%i y:%i itemKey:%i",qPrintable(item->text()),x,y,itemKey );

    m_sectionItems.insert(itemKey,item);
}

void NCReportMarkupLanguageOutput::setStrategy(NCReportMarkupLanguageOutput::SpreadStrategy strategy)
{
    m_strategy = strategy;
}

NCReportMarkupLanguageOutput::SpreadStrategy NCReportMarkupLanguageOutput::strategy() const
{
    return m_strategy;
}

void NCReportMarkupLanguageOutput::setStyleSheetFile(const QString &cssFile)
{
    m_cssFile = cssFile;
}

QString NCReportMarkupLanguageOutput::styleSheetFile() const
{
    return m_cssFile;
}

const QString NCReportMarkupLanguageOutput::crlf() const
{
    return m_crlf;
}

const QString NCReportMarkupLanguageOutput::styleSheetMarker() const
{
    return m_styleSheetMarker;
}

int NCReportMarkupLanguageOutput::rowCounter() const
{
    return m_rowCounter;
}

QString NCReportMarkupLanguageOutput::penStyleName( int style ) const
{
    QString name;
    switch ( style ) {
    case Qt::NoPen: break;
    case Qt::SolidLine: name="solid"; break;
    case Qt::DashLine: name="dashed"; break;
    case Qt::DotLine:
    case Qt::DashDotLine:
    case Qt::DashDotDotLine: name="dotted"; break;
    case Qt::CustomDashLine: name="double"; break;
    }
    return name;
}

QString NCReportMarkupLanguageOutput::itemClassId(NCReportItem *item) const
{
    QString itemClass = QString("%1_item_%2").arg(fixedID(item->section()->id()), fixedID(item->id()));
    if (item->objectType() == NCReportItem::Label || item->objectType() == NCReportItem::Field) {
        if (((NCReportLabelItem*)item)->hasConditionalFormat()) {
            itemClass += "_row" + QString::number(item->director()->currentRow());
        }
    }
    return itemClass;
}

QString NCReportMarkupLanguageOutput::styleSheetReference( const QString& css) const
{
    Q_UNUSED(css)
    return QString();
}

void NCReportMarkupLanguageOutput::writeItem(NCReportItem *item)
{
    QTextStream cellStyle;
    cellStarts(item, cellStyle);

    switch ( item->objectType() ) {
    case NCReportItem::Label:
    case NCReportItem::Field:
        writeLabel( (NCReportLabelItem*)item, cellStyle );
        break;
    case NCReportItem::Text:
        writeText((NCReportTextItem*)item );
        break;
    case NCReportItem::Line:
        writeLine( (NCReportLineItem*)item);
        break;
    case NCReportItem::Rectangle:
        writeRectangle( (NCReportRectItem*)item);
        break;
    case NCReportItem::Image:
        writeImage((NCReportImageItem*)item);
        break;
    case NCReportItem::Barcode:
        writeBarcode((NCReportBarcodeItem*)item);
        break;
    case NCReportItem::Chart:
#ifdef HIGHCHARTS_INTEGRATION
        writeChart((NCReportChartItem*)item);
        break;
#endif
    case NCReportItem::Ellipse:
    case NCReportItem::Graph:
    case NCReportItem::CrossTab:
    case NCReportItem::Table:
    case NCReportItem::NoObject:
        break;
    }

    cellEnds(item);
}

//void NCReportMarkupLanguageOutput::writeCellStyle(NCReportItem *item)
//{
//}

void NCReportMarkupLanguageOutput::writeSpace(NCReportSection *section, int row, int width)
{
    Q_UNUSED(section)
    Q_UNUSED(row)
    Q_UNUSED(width)
}



void NCReportMarkupLanguageOutput::writeLabel(NCReportLabelItem *item, QTextStream& cellStyle)
{
    Q_UNUSED(item)
    Q_UNUSED(cellStyle)
}

void NCReportMarkupLanguageOutput::writeLine(NCReportLineItem * item)
{
    Q_UNUSED(item)
}


void NCReportMarkupLanguageOutput::writeRectangle(NCReportRectItem * item)
{
    Q_UNUSED(item)
}


void NCReportMarkupLanguageOutput::initStyleStream(QTextStream &styleStream, const QString &classID)
{
    if ( m_styleClasses.contains(classID)) {
        // style class is already existed
    } else {
        m_styleClasses.insert(classID,QString());
        styleStream.setString( &m_styleClasses[classID] );
    }
}

void NCReportMarkupLanguageOutput::addCSSItem(const QString &classID, const QString &value)
{
    m_styleClasses.insert(classID,value);
}

int &NCReportMarkupLanguageOutput::fileCounter()
{
    return m_fileCounter;
}

void NCReportMarkupLanguageOutput::writeImage(NCReportImageItem *item)
{
    Q_UNUSED(item)
}

void NCReportMarkupLanguageOutput::writeText(NCReportTextItem * item)
{
    Q_UNUSED(item)
}

void NCReportMarkupLanguageOutput::writeBarcode(NCReportBarcodeItem *item)
{
    Q_UNUSED(item)
}

void NCReportMarkupLanguageOutput::writeChart(NCReportChartItem *item)
{
    Q_UNUSED(item)
}

void NCReportMarkupLanguageOutput::sectionStarts(NCReportSection *section, NCReportEvaluator *evaluator)
{
    Q_UNUSED(evaluator)

    if ( fixedID(section->id()) == m_sectionID) {
        //the same section is repeated, this is likely a detail
        m_rowCounter++;
    } else {
        //new section
        if (!m_initalState) {
            // close the previous
            writeSectionEnd(section);
        }
        writeSectionStart(section);
        m_rowCounter = 0;
    }

    writeRowStart(section);

    // clear buffer
    m_sectionItems.clear();
    m_sectionID = fixedID(section->id());
    m_initalState = false;
}

void NCReportMarkupLanguageOutput::sectionEnds(NCReportSection *section, NCReportEvaluator *evaluator)
{
    Q_UNUSED(section)
    Q_UNUSED(evaluator)
    // write items in the right order:
    QMultiMap<int,NCReportItem*>::const_iterator i = m_sectionItems.constBegin();

    int rowCounter=0;
    short lastItemY = -32000;
    short rowFirstItemX = -32000;   // the 1st x coordinate in the current row
    bool initial = true;
    while (i != m_sectionItems.constEnd()) {
        short itemY = NCReportUtils::id2D::y(i.key());

        if (initial) {
            rowFirstItemX = NCReportUtils::id2D::x(i.key());
            if (rowFirstItemX > 0)
                writeSpace(section, rowCounter, scale().mmToPixel(rowFirstItemX/100) );  // insert spacing
        } else {
            if ( itemY>lastItemY ) {
                rowCounter++;

                writeRowEnd(section);
                writeRowStart(section); //start next row ????

                rowFirstItemX = NCReportUtils::id2D::x(i.key());    // the 1st x coordinate in the new row
                if (rowFirstItemX > 0)
                    writeSpace(section, rowCounter, scale().mmToPixel(rowFirstItemX/100) );  // insert spacing
            }
        }

#ifdef HTMLOUTPUT_DEBUGMODE
        qDebug("Section: %s Item: %s id:%i",qPrintable(fixedID(i.value()->section()->id())),qPrintable(i.value()->text()), i.key());
        qDebug("Key:%i KeyY:%i KeyX:%i",i.key(),NCReportUtils::id2D::y(i.key()), NCReportUtils::id2D::x(i.key()));
#endif

        writeItem(i.value());

        ++i;
        lastItemY = itemY;
        initial = false;
    }

    writeRowEnd(section);
}

QTextStream &NCReportMarkupLanguageOutput::contentStream()
{
    return m_tsContent;
}

QTextStream &NCReportMarkupLanguageOutput::styleStream()
{
    return m_tsStyle;
}

void NCReportMarkupLanguageOutput::writeDocumentFormats()
{
    QFileInfo fi_html(fileName());

    QTextDocument document;
    document.setHtml(m_content);

    QTextDocumentWriter w;

    // ODF TEST
    w.setFormat("odf");
    w.setFileName(fi_html.path()+"/"+fi_html.baseName()+".odt");
    w.write(&document);

    // PLAIN TEXT TEST
    w.setFormat("plaintext");
    w.setFileName(fi_html.path()+"/"+fi_html.baseName()+".txt");
    w.write(&document);
}

QString NCReportMarkupLanguageOutput::fixedID(const QString &id) const
{
    if (id.contains(' ')) {
        QString id_fixed(id);
        return id_fixed.replace(' ','_');
    }
    return id;
}
