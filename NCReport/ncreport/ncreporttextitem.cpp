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
#include "ncreporttextitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportevaluator.h"
#include "ncreportutils.h"
#include "ncreporthttpclient.h"
#include "ncreportdef.h"
#ifdef MARKDOWN_INTEGRATION
    #include "MarkdownProcessor.h"
#endif

#include <QFile>
#include <QTextStream>
#include <QTextDocument>
#include <QPainter>
#include <QAbstractTextDocumentLayout>
#include <QTextBlock>
#include <QTextLine>

#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
//#define TEXT_FONT_TRIM
#define TEXT_FONT_TRIM_FACTOR	0.3527
#endif

#define TEXT_HEIGHT_TRIM_MARGIN_MM	0.7

NCReportTextItem::NCReportTextItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem( parent )
{
    d = new NCReportTextData;
    setReportDef(rdef);
    d->objectType = Text;
    d->tagname = "text";
    NCReportTextData* td = (NCReportTextData*)d;
    td->document = new QTextDocument();
}


NCReportTextItem::~NCReportTextItem()
{
    NCReportTextData* td = (NCReportTextData*)d;
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("Delete td->document");
#endif
    delete td->document;
}

void NCReportTextItem::setHtml(const QString & html )
{
    NCReportTextData* td = (NCReportTextData*)d;
    if ( td->document ) {
        if ( td->useFont ) {
            td->document->setDefaultFont( td->font );
            td->document->setDefaultStyleSheet( QString("body, a, p { color: %1; }").arg(td->forecolor.name()) );
            //td->document->setUseDesignMetrics(true);
        }
        td->document->clear();
        if ( isPlainText() )
            td->document->setHtml( convertPlainTextToHtml(html) );
        else
            td->document->setHtml( html );
        setAdjusted(false);
    }
}

QString NCReportTextItem::toHtml() const
{
    NCReportTextData* td = (NCReportTextData*)d;
    if (td->document)
        return td->document->toHtml();

    return QString();
}

bool NCReportTextItem::read(NCReportXMLReader *r )
{
    NCReportItem::read( r );
    NCReportTextData* td = (NCReportTextData*)d;

    r->readFont(td->font);
    //if ( r->isAttrib("expandDetail") )
    //	td->expandDetail = r->attribToBool("expandDetail");
    //if ( r->isAttrib("multiPage") )
    //	td->multiPage = r->attribToBool("multiPage");

    if ( r->isAttrib("useFont") )
        td->useFont = r->attribToBool("useFont");
    if ( r->isAttrib("sizeFix") )
        td->sizeFix = r->attribToBool("sizeFix");
    if ( r->isAttrib("defPosAndSize") )
        td->defaultPosAndSize = r->attribToBool("defPosAndSize");
    if ( r->isAttrib("plain") )
        td->plainText = r->attribToBool("plain");
    if ( r->isAttrib("markdown") )
        td->markdown = r->attribToBool("markdown");

    if ( r->isAttrib("forecolor") )
        td->forecolor =  QColor(r->attribToString("forecolor"));

    switch ( d->sourceType ) {
        case Static: // static
        case Template:
        {
            td->documentTemplate = r->readElementText();
            setHtml( td->documentTemplate );	// needed for designer only
            break;
        }
        case File: // static file
        {
            td->text = r->readElementText();
            QFile file(td->text);
            if ( file.exists() ) {
                if ( file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream ts(&file);
                    td->documentTemplate = ts.readAll();
                    setHtml( td->documentTemplate ); // needed for designer only
                }
            } else {
                qWarning( "TextItem: File not found: %s", qPrintable(td->text) );
            }
            break;
        }
        case Url: //static text from url
        {
            d->text = r->readElementText();
            NCReportHttpClient http;
            http.waitForDownloadContent( d->text );	//download by url
            td->documentTemplate = http.content();
            setHtml( td->documentTemplate );
            http.clear();
            //id->ist = Static;
            break;
        }
        case DataSource: //dynamic text from dsource
        case Parameter: //dynamic text from dsource
        case FileNameFromDataSource: //dynamic text - filename from dsource
        case UrlFromDataSource: //dynamic text - filename from dsource
        {
            d->text = r->readElementText();
            break;
        }
        case InternalDocument:
            break;
    }

    return true;
}

bool NCReportTextItem::write(NCReportXMLWriter *w )
{
    NCReportTextData* td = (NCReportTextData*)d;

    w->writeStartElement( d->tagname );

    NCReportItem::write(w);

    w->writeAttribute("useFont", td->useFont ? "true" : "false");
    if ( td->useFont ) {
        w->writeFont( td->font );
        w->writeAttribute("forecolor", td->forecolor.name() );
    }
    //if ( td->multiPage )
    //	w->writeAttribute("multiPage","true");
    //if ( td->expandDetail )
    //	w->writeAttribute("expandDetail","true");
    if ( td->sizeFix )
        w->writeAttribute("sizeFix","true");
    if ( td->defaultPosAndSize )
        w->writeAttribute("defPosAndSize","true");
    if ( td->plainText )
        w->writeAttribute("plain","true");
    if ( td->markdown )
        w->writeAttribute("markdown","true");

    switch ( d->sourceType ) {
        case Static:
        case Template:
            w->writeCharacters( td->documentTemplate );
            break;
        case File:
        case DataSource:
        case Parameter:
        case FileNameFromDataSource:
        case Url:
        case UrlFromDataSource:
            w->writeCharacters( td->text );
            break;
        case InternalDocument:
            break;
    }
    w->writeEndElement();

    return true;
}

void NCReportTextItem::setDefaultForEditor()
{
    //NCReportTextData* td = (NCReportTextData*)d;
    resize( QSizeF(120.0,19.0) );
    //td->setHtml( NCRD_DEFAULT_TEXT );
}

void NCReportTextItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    NCReportScale sc;
    Q_UNUSED(option);
    Q_UNUSED(widget);
    NCReportTextData* td = (NCReportTextData*)d;

    adjustFont();

    // BACKGROUND IN DESIGNER
    QPen pen(Qt::darkGray);
    pen.setStyle(Qt::DotLine);
    painter->setPen( pen );

    //painter->setBrush( QBrush( QColor(224,233,200,128) ) );

    QRectF rect( 0, 0, d->currentSize.width(), d->currentSize.height() );
    painter->drawRect( rect );
    // CONTENT
    //td->document->defaultTextOption().setAlignment( td->alignmentH | td->alignmentV ) ;
    //td->document->setTextWidth( boundingRect().width() );
    if ( td->useFont ) {
        painter->setFont( td->font );
        painter->setPen( QPen(td->forecolor) );
    }

    if ( td->document->isEmpty() ) {

        QString label;
        switch ( td->sourceType ) {
            case Static: label = NCRD_DEFAULT_TEXT; break;
            case DataSource: label = QObject::tr("Text data column (%1)").arg(td->text); break;
            case Parameter: label = QObject::tr("Parameter (%1)").arg(td->text); break;
            case File: label = QObject::tr("File (%1)").arg(td->text); break;
            case FileNameFromDataSource: label = QObject::tr("File name column (%1)").arg(td->text); break;
            case Url: label = QObject::tr("URL (%1)").arg(td->text); break;
            case UrlFromDataSource: label = QObject::tr("URL column (%1)").arg(td->text); break;
            case InternalDocument: label = QObject::tr("Custom QTextDocument from report->document()"); break;
        }

        painter->drawText( rect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWrapAnywhere, label );
    } else {
        QSizeF size( sc.mmToSize( td->mSize ) );
        if ( td->sizeFix )
            td->document->setPageSize( size );
        else
            td->document->setTextWidth( size.width() );
        td->document->drawContents( painter, rect );
    }

    paintSelection( painter );
}

void NCReportTextItem::paint(NCReportOutput *o, const QPointF &mPos )
{
    NCReportTextData* td = (NCReportTextData*)d;
    QSizeF size( o->scale().mmToSize( realMetricSize() ) );
    QPointF pos( o->scale().mmToPoint( mPos + td->mPos ) );
    QRectF rect(pos,size);

    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::QtPreview:
        case NCReportOutput::Preview:
        {
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("paint(%p)", o->painter());
#endif
            adjustFont( o );
            if ( td->useFont ) {
                o->painter()->setPen( QPen(td->forecolor) );
                o->painter()->setFont( td->font );
            }
            //if ( td->expandDetail )
            //	size.setHeight( o->scale().mmToPixel( td->requiredHeightMM ) );

            if ( isBroken() ) {
                // partial print
                partialPaint(o, pos, false);
            } else {
                // normal (full) print
                QRectF r2( QPointF(0,0), size ); // Internal rect of painted text

                o->painter()->save();
                o->painter()->translate( pos );
                td->document->drawContents( o->painter(), r2 );
                o->painter()->restore();
            }
            break;
        }
        default:
            break;
    }
    o->renderItem( this, rect );

}

/*!
  Partial paint method for breakable texts
  */
void NCReportTextItem::partialPaint(NCReportOutput *o, QPointF &pos, bool dummyPaint )
{
    NCReportTextData* td = (NCReportTextData*)d;
    bool done = false;
    bool isLastBlock = false;
    bool isLastLine = false;
    qreal margin = td->document->documentMargin();
    QPointF originalPos(pos);
    qreal fullHeight=0;
    pos.ry()+=margin;
    fullHeight += margin;

    for (int block=partFrom.blockNo; block<=partTo.blockNo; block++ ) {
        QTextBlock tbl = td->document->findBlockByNumber(block);
        if (!tbl.isValid())
            continue;

        isLastBlock = (block == td->document->blockCount()-1 );

        QTextLayout *layout = tbl.layout();
        if (!layout)
            continue;

        int lineCount = layout->lineCount();

        for ( int line=(block==partFrom.blockNo ? partFrom.lineNo : 0); line <= (block==partTo.blockNo ? partTo.lineNo : lineCount-1); line++ ) {

            isLastLine = (line == lineCount-1);
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Paint line %i Linecount: %i PosY: %f",line,lineCount,pos.y());
#endif
            QTextLine textLine = layout->lineAt(line);
            QPointF linePos( textLine.position().x() ,0 );
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug("Line pos: x:%f y%f:",linePos.x(),linePos.y());
#endif
            textLine.setPosition(linePos);
            if ( !dummyPaint ) {
                o->painter()->translate(margin,0);
                textLine.draw(o->painter(), pos);
                o->painter()->translate(-margin,0);
            }
            done = isLastBlock && isLastLine;
            if (done)
                pos.ry()+=margin;
            else
                pos.ry()+=textLine.height();

            fullHeight += textLine.height();
        }
    }
    if (done) {
        partFrom.blockNo = -1;
        partFrom.lineNo = -1;
        partTo.blockNo = -1;
        partTo.lineNo = -1;
    } else {
        partFrom.blockNo = ( !isLastBlock && isLastLine ? partTo.blockNo +1 : partTo.blockNo );
        partFrom.lineNo = (isLastLine ? 0 : partTo.lineNo +1 );
        partTo.blockNo = -1;
        partTo.lineNo = -1;
    }

    //------------------------------
    // RECT AROUND THE TEXT
    //------------------------------
    setBoundingRectOfLastPart(QRectF(originalPos, QSizeF(td->document->size().width(),fullHeight)));
//	QRectF rectAround(originalPos, QSizeF(td->document->size().width(),fullHeight));
//	o->painter()->drawRect(rectAround);
}


void NCReportTextItem::adjustSize()
{
    NCReportTextData* td = (NCReportTextData*)d;
    td->document->adjustSize();
    resize( td->document->size() );
}

void NCReportTextItem::adjustSize(NCReportOutput* o)
{
    if ( isAdjusted() )
        return;

    NCReportTextData* td = (NCReportTextData*)d;
    //if ( o->painter() )
    //	td->document->documentLayout()->setPaintDevice(o->painter()->device() );

    td->document->documentLayout()->setPaintDevice( o->device()  );
    QSizeF size( o->scale().mmToSize( td->mSize ) );

    if (pinToRight()) {
        size.setWidth( o->scale().mmToRealPixel( reportDef()->option().pageWidthMM()
                - reportDef()->option().rightMarginMM()
                - reportDef()->option().leftMarginMM()
                - metricPos().x() ) );
    }

    if ( td->sizeFix )
        td->document->setPageSize( size );
    else
        td->document->setTextWidth( size.width() );

    //td->document->adjustSize();
    td->mRealSize = o->scale().sizeToMM( td->document->size() );
    td->mRealSize += QSizeF(0,TEXT_HEIGHT_TRIM_MARGIN_MM);

    if ( o->output() != NCReportOutput::Null )
        setAdjusted(true);
    //resize( td->document->size() );
}

void NCReportTextItem::readProps(NCReportXMLReader *)
{
}

void NCReportTextItem::writeProps(NCReportXMLWriter *)
{
}

void NCReportTextItem::resize(const QSizeF & size )
{
    NCReportTextData* td = (NCReportTextData*)d;
    if ( td->document ) {
        td->document->setTextWidth( size.width() );
        if ( td->sizeFix )
            td->document->setPageSize( size );
    }
    NCReportItem::resize( size );
}



bool NCReportTextItem::isEmpty() const
{
    NCReportTextData* td = (NCReportTextData*)d;

    if (isTemplateMode()) {
        bool isempty = false;
        if ( td->document ) {
            isempty = td->document->isEmpty();
        }
        if (isempty)
            isempty = td->valueRef().isNull();
        return isempty;
    } else {
        return td->documentTemplate.isEmpty();
    }
    return true;
}

void NCReportTextItem::adjustFont( NCReportOutput *o )
{
#ifdef TEXT_FONT_TRIM
    NCReportScale scale;
    if ( o )
        scale = o->scale();

    // Conversion from Point into Pixel
    NCReportTextData* td = (NCReportTextData*)d;
    QString str = td->document->toHtml();
    QRegExp rx("font-size:[0-9]{1,3}pt;");
    int rxpos = 0;

    while ((rxpos = rx.indexIn(str, rxpos)) != -1)
    {
        QString tmp = rx.cap(0); // gefundener String
        tmp = tmp.remove(0,10); // font-size entfernen
        tmp = tmp.remove(tmp.length()-3, 3); // pt; entfernen
        tmp = QString("font-size:%1px;").arg( scale.mmToPixel( tmp.toDouble() * TEXT_FONT_TRIM_FACTOR )); // Umrechnen
        str.remove(rxpos, rx.matchedLength());
        str.insert(rxpos, tmp);
        rxpos += rx.matchedLength();
    }
    td->document->setHtml(str);
#else
    Q_UNUSED(o);
#endif
}

void NCReportTextItem::updateValue(NCReportOutput* output, NCReportEvaluator *evaluator)
{
    if (output->generatingMode() == NCReportOutput::DocumentFile) {
        QString t;
        evaluator->evaluateDynamicText( this, &t );
        if (!t.isEmpty())
            setValue(t);
    } else {
        evaluator->evaluateDynamicText( this );
    }

    NCReportItem::updateValue(output, evaluator);
}

qreal NCReportTextItem::breakTo(qreal heightMM, NCReportOutput* output)
{
    if (heightMM<0)
        return -1;
    NCReportTextData* td = (NCReportTextData*)d;

    bool shrink=false;
    bool break_calculation = false;
    //bool partialMode = !(partFrom.blockNo<0 || partFrom.lineNo<0);

    if (partFrom.blockNo<0 )
        partFrom.blockNo = 0;
    if (partFrom.lineNo<0)
        partFrom.lineNo = 0;

    int blockCount = td->document->blockCount();
    qreal marginMM = output->scale().pixelToMM(td->document->documentMargin()); /* + d->reportDef->option().zoneSpacing();*/
    qreal calculateHeight=0;
    QString contect = td->document->toPlainText();

    for ( int blockNo=partFrom.blockNo; blockNo<blockCount; blockNo++) {
        QTextBlock block = td->document->findBlockByNumber(blockNo);
        QTextLayout *layout = block.layout();
        int lineCount = layout->lineCount();

        //calculateHeight += marginMM;
        for ( int line=(blockNo == partFrom.blockNo ? partFrom.lineNo : 0); line < lineCount; line++ ) {
            QTextLine textLine = layout->lineAt(line);
            qreal lineHeightMM = output->scale().pixelToMM(textLine.height());
            calculateHeight += lineHeightMM;
            if ( calculateHeight + marginMM <= heightMM ) {
                // line has enought space
                shrink = true;
                partTo.blockNo = blockNo;
                partTo.lineNo = line;
            } else {
                calculateHeight -= lineHeightMM;	// undo last increment
                break_calculation = true;
                break;
            }
        }
        if (break_calculation)
            break;
    }

    if (!shrink /*&& !partialMode*/ ) {
        partFrom.blockNo = -1;
        partFrom.lineNo = -1;
    }

    if ( shrink )
        return qMin(calculateHeight+marginMM,heightMM);
    else
        return -1;

}

bool NCReportTextItem::isBroken() const
{
    return !(partFrom.blockNo<0);
}

void NCReportTextItem::dummyPaint(NCReportOutput *o, const QPointF &mPos)
{
    NCReportTextData* td = (NCReportTextData*)d;
    QPointF pos( o->scale().mmToPoint( mPos + td->mPos ) );
    if ( isBroken() )
        partialPaint(o, pos, true);
}

QString NCReportTextItem::convertPlainTextToHtml(const QString &text) const
{
    QString html(text);
#ifdef MARKDOWN_INTEGRATION
    MarkdownProcessor mdp;
    html = mdp.toHtml( text );
#else
    html.replace('\n',"<br>");
    html.prepend("<p>");
    html.append("</p>");
#endif
    return html;
}

QList<int> NCReportTextItem::availableSourceTypes() const
{
    QList<int> types;
    types << Static << DataSource << Parameter << File << FileNameFromDataSource << Url;
    return types;
}

QString NCReportTextItem::htmlContent() const
{
    if (isTemplateMode()) {
        return value().toString();
    } else {
        return documentTemplate();
    }
    return QString();
}


