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
#include "ncreportlabelitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportsection.h"
#include "ncreportdef.h"
#include "ncreportvariablemodifier.h"
#include "ncreportconditionalformat.h"
#include "ncreportutils.h"

#include <QPainter>
#include <QApplication>

#include <math.h>

#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
//#define LABEL_FONT_TRIM
#define LABEL_FONT_TRIM_FACTOR	0.3527
#endif

//#define LABEL_QT_TRANSLATOR_INTEGRATION

const float Pi = 3.14159f;
#define LABEL_HEIGHT_TRIM_MARGIN_MM	0.7		// trim/margin size (mm) for text height calculation

NCReportLabelItem::NCReportLabelItem( int type, NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem( parent )
{
    if ( type == Label ) {
        d = new NCReportLabelData;
        d->objectType = Label;
        d->tagname = "label";
        setReportDef(rdef);
    }
}

NCReportLabelItem::~ NCReportLabelItem()
{
}

bool NCReportLabelItem::read(NCReportXMLReader *reader )
{
    NCReportItem::read( reader );

    reader->readFont(fontRef());

    setAlignment(reader->readAlignment());
    if ( reader->isAttrib("rotation") )
        setRotation( reader->attribToInt("rotation") );
    if ( reader->isAttrib("wordbreak") )
        setWordBreak(reader->attribToBool("wordbreak"));
    if ( reader->isAttrib("forecolor") )
        setForeColor( QColor(reader->attribToString("forecolor")) );
    if ( reader->isAttrib("backcolor") )
        setBackColor( QColor(reader->attribToString("backcolor")) );
    if ( reader->isAttrib("bgMode") )
        setBackgroundMode(reader->attrib("bgMode")=="tr" ? Transparent : FilledBackground );
    if ( reader->isAttrib("bgDesign") )
        setBackgroundToDesign(reader->attribToBool("bgDesign"));
    if ( reader->isAttrib("hideRepeated") )
        setHideIfRepeated( reader->attribToBool("hideRepeated") );
    if ( reader->isAttrib("useLastNoneNull") )
        setUseLastNoneNull( reader->attribToBool("useLastNoneNull") );
    if ( reader->isAttrib("tr") )
        setHasTranslation( reader->attribToBool("tr") );
    if ( reader->isAttrib("cf") )
        setHasConditionalFormat( reader->attribToBool("cf") );


    if ( reader->isAttrib("borderVisible")  ) {
        setBorderVisible( reader->attribToBool("borderVisible") );
    }

    QPen pen;
    if ( reader->isAttrib("borderPenlineColor") )
        pen.setColor(QColor(reader->attribToString("borderPenlineColor"))) ;
    if ( reader->isAttrib("borderPenlineStyle") ) {
        Qt::PenStyle ps = Qt::SolidLine;
        QStringRef ls = reader->attrib("borderPenlineStyle");

        if (ls == "solid")
            ps = Qt::SolidLine;
        else if (ls == "dash")
            ps = Qt::DashLine;
        else if (ls == "dot")
            ps = Qt::DotLine;
        else if (ls == "dashdotdot")
            ps = Qt::DashDotDotLine;
        else if (ls == "dashdot")
            ps = Qt::DashDotLine;
        else if (ls == "nopen" || ls=="no" || ls=="none" )
            ps = Qt::NoPen;

        pen.setStyle( ps );
    }
    NCReportScale s;
    if ( reader->isAttrib("borderPenlineWidth") )
        pen.setWidthF( s.mmToRealPixel(reader->attribToReal("borderPenlineWidth")) );

    setBorderPen(pen);


    readProps( reader );		// more properties for field

    if ( reader->isAttrib("hasParameters") && reader->attribToBool("hasParameters") ) {
        // NEW WAY
        reader->readParameters(parametersRef());
    }
    if ( hasTranslation() || hasConditionalFormat() )
        readSubElements( reader );
    else
        setText(reader->readElementText());  // for backward compatibility

    saveTextStyle();	// backup static style

    return true;
}

void NCReportLabelItem::readSubElements(NCReportXMLReader *reader )
{
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isEndElement())
            break;

        if (reader->isStartElement()) {
            if (reader->name() == "tr") {
                QString langId = "00";
                if (reader->isAttrib("lang"))
                    langId = reader->attribToString("lang");
                setText(reader->readElementText(), langId );
            } else if (reader->name() == "cf") {
                setConditionalFormatCode(reader->readElementText());
            }
        }
    }
}

bool NCReportLabelItem::write(NCReportXMLWriter *writer)
{
    NCReportLabelData* ld = (NCReportLabelData*)d;

    writer->writeStartElement( tagname() );

    NCReportItem::write(writer);
    writer->writeFont( font() );
    writer->writeAlignment( alignment() );

    if ( rotation() != 0 )
        writer->writeAttribute("rotation", QString::number(rotation()) );
    if ( ld->wordbreak )
        writer->writeAttribute("wordbreak","true");

    if (foreColor()!=Qt::black)
        writer->writeAttribute("forecolor", foreColor().name() );

    if ( backgroundMode() == FilledBackground )
        writer->writeAttribute("bgMode", "fb" );

    if ( backgroundMode() == FilledBackground || isBackgroundToDesign() )
        writer->writeAttribute("backcolor", backColor().name() );

    if ( isBackgroundToDesign() )
        writer->writeAttribute("bgDesign","true");

    if ( hideIfRepeated() )
        writer->writeAttribute("hideRepeated","true");

    if ( useLastNoneNull() )
        writer->writeAttribute("useLastNoneNull","true");

    // do we have translations?
    bool noTranslation = ( translations().count() == 1 && translations().contains("00"));
    setHasTranslation( !noTranslation );
    if ( hasTranslation() )
        writer->writeBool("tr",true);

    if (borderVisible()) {
        writer->writeAttribute("borderVisible","1");
    }
    else {
        writer->writeAttribute("borderVisible","0");
    }
    writer->writeAttribute("borderPenlineColor", borderPen().color().name());

    QString ls("solid");
    switch ( borderPen().style() ) {
        case Qt::SolidLine: ls="solid"; break;
        case Qt::DashLine: ls="dash"; break;
        case Qt::DotLine: ls="dot"; break;
        case Qt::DashDotDotLine: ls="dashdotdot"; break;
        case Qt::DashDotLine: ls="dashdot"; break;
        case Qt::NoPen: ls="nopen"; break;
        default: break;
    }
    writer->writeAttribute("borderPenlineStyle", ls );
    //writer->writeAttribute("borderPenlineWidth", QString("%1").arg(lineWidth()));
    //NCReportScale s;
    writer->writeAttribute("borderPenlineWidth", writer->qrealToString(lineWidth() ));

    // is there conditional formatting?
    setHasConditionalFormat( !conditionalFormatCode().isEmpty() );
    if ( hasConditionalFormat() )
        writer->writeBool("cf",true);

    writeProps( writer );		// more properties for field

    if (hasParameter()) {
        writer->writeAttribute("hasParameters","true");
        writer->writeParameters(parametersRef());
    }
    if ( hasTranslation() || hasConditionalFormat() )
        writeSubElements(writer);
    else
        writer->writeCharacters( text() );

    writer->writeEndElement();

    return true;
}

void NCReportLabelItem::writeSubElements(NCReportXMLWriter *w)
{
    NCReportLabelData* ld = (NCReportLabelData*)d;

    QHash<QString,QString>::const_iterator i = ld->translations.constBegin();
    while (i != ld->translations.constEnd()) {
        if ( !i.value().isEmpty() ) {
            w->writeStartElement("tr");
            if (i.key()!="00")
                w->writeAttribute("lang",i.key());
            w->writeCharacters( i.value() );
            w->writeEndElement();
        }
        ++i;
    }
    if ( hasConditionalFormat() ) {
        w->writeStartElement("cf");
        w->writeCharacters( conditionalFormatCode() );
        w->writeEndElement();
    }

}

void NCReportLabelItem::setDefaultForEditor()
{
    resize( QSizeF(20*SELECTION_BOXSIZE,3*SELECTION_BOXSIZE) );
    if ( d->objectType == Field )
        setText( QObject::tr("Field") );
    else
        setText( QObject::tr("Text label") );
    //ld->font = qApp->font();
}

void NCReportLabelItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QFont m_font(font());
    adjustFont( m_font );
    painter->setFont( m_font );

    int aflag = alignment(); //H | ld->alignmentV;
    if ( isWordBreak() )
        aflag |= Qt::TextWordWrap;

    // BACKGROUND IN DESIGNER
    QPen m_borderpen(Qt::darkGray);
    m_borderpen.setStyle(Qt::DotLine);

    if(borderVisible()){
        m_borderpen = borderPen();
        NCReportScale sc;
        m_borderpen.setWidthF( sc.mmToRealPixel(lineWidth()) );
    }
    painter->setPen( m_borderpen );

    if ( backgroundMode() == FilledBackground || isBackgroundToDesign() )
        painter->setBrush( QBrush(backColor()) );

    QRectF rect( 0, 0, currentSize().width(), currentSize().height() );
    painter->drawRect( rect );
    rect.setLeft( rect.left()+1 );

    painter->setPen( QPen(foreColor(),1) );

    if (rotation()!=0) {
        painter->save();
        painter->setClipRect(rect);
        rotateText( painter, QPointF(0,0), screenRect().size() );
    }

    painter->drawText( rect, aflag, text() );

    if (rotation()!=0)
        painter->restore();

    paintSelection(painter);
    //print( painter, sc );
}

void NCReportLabelItem::paint( NCReportOutput* output, const QPointF& painterPosMM )
{
    QPointF startPoint( output->scale().mmToPoint( painterPosMM + metricPos() ) );
    QRectF rect( startPoint, output->scale().mmToSize( realMetricSize() ) );

    switch ( output->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
        {
            // Conversion from point into DTP-Millimeter and then into pixels
            QFont m_font(font());
            adjustFont( m_font, output );
            //qDebug( "Capitalization: %i", m_font.capitalization());

            output->painter()->setFont( m_font );

            if ( rotation() != 0) {
                output->painter()->save();
                rotateText( output->painter(), startPoint, outputRect(output).size() );
            }

            int aflag = alignment();
            if ( isWordBreak() )
                aflag |= Qt::TextWordWrap;





            if ( backgroundMode() == FilledBackground ) {
                output->painter()->setPen( Qt::NoPen );
                output->painter()->setBrush( QBrush( backColor() ) );
                output->painter()->drawRect( rect );
            }

            if(borderVisible()){
                output->painter()->save();
                QPen m_borderpen(Qt::NoPen);
                m_borderpen = borderPen();
                NCReportScale sc;
                m_borderpen.setWidthF( sc.mmToRealPixel(lineWidth()) );
                output->painter()->setPen( m_borderpen );
                output->painter()->setBrush(Qt::NoBrush);
                output->painter()->drawRect( rect );
                output->painter()->restore();
            }



            //QRectF rect( 0, 0, currentSize().width(), currentSize().height() );
            //output->painter()->drawRect( rect );
            //rect.setLeft( rect.left()+1 );

            output->painter()->setPen( QPen(foreColor(),1) );

//            output->painter()->setPen( QPen(foreColor()) );

#if defined(Q_WS_MAC) || defined(Q_OS_MAC)
            rect.setHeight(rect.height()*1.5);
#endif
            //if(!printableText().isEmpty() && !printableText().isNull())
            output->painter()->drawText( rect, aflag, printableText());

            if ( rotation() != 0)
                output->painter()->restore();

            break;
        }
        default:
            break;
    }
    output->renderItem( this, rect );
}


void NCReportLabelItem::rotateText( QPainter* p, const QPointF& point, const QSizeF& textSize )
{
    NCReportLabelData* ld = (NCReportLabelData*)d;
    QPointF offsetPoint( point );
    qreal h = textSize.height();
    qreal w = textSize.width();
//    if (ld->rotation==90 || ld->rotation==270)
//        height = textSize.width();

    offsetPoint.rx()+=h*sin(ld->rotation*Pi/180)/2;
    offsetPoint.ry()+=h*sin(ld->rotation*Pi/180)/2;

    p->translate( offsetPoint );
    p->rotate(ld->rotation);
    p->translate( -offsetPoint );

    switch (rotation()) {
    case 90:
        p->translate( 0, 0);
        break;
    case 180:
        p->translate( -w, -h);
        break;
    case 270:
        p->translate( -w, 0);
        break;
    case 315:
        p->translate( -w/2, h*2 );
        break;
    }
}

void NCReportLabelItem::readProps(NCReportXMLReader *)
{
}

void NCReportLabelItem::writeProps(NCReportXMLWriter *)
{
}

void NCReportLabelItem::adjustSize()
{
    if ( !sectionScene() )
        return;

    QRectF rect(screenRect());
    resize( QSizeF( qMax(rect.width(),currentSize().width()) ,qMax(rect.height(),currentSize().height() ) ));

    //QSizeF sizehint = QSizeF( sectionScene()->snapToX(fm.width(text())) , sectionScene()->snapToY(fm.height()*lines) );
    //resize( QSizeF( qMax(sizehint.width(),currentSize().width()) ,qMax(sizehint.height(),currentSize().height() ) ));
}

QRectF NCReportLabelItem::screenRect() const
{
    QFontMetricsF fm(font());
    int aflag = alignment();
    if ( isWordBreak() )
        aflag |= Qt::TextWordWrap;
    return fm.boundingRect( QRectF(QPoint(0,0), currentSize() ), aflag, printableText() );
}

QRectF NCReportLabelItem::outputRect(NCReportOutput* output) const
{
    QFontMetricsF fm(font(), output->device() );

    int aflag = alignment();
    if ( isWordBreak() )
        aflag |= Qt::TextWordWrap;

    QRectF originalRect(QRectF( output->scale().mmToPoint( metricPos() ), output->scale().mmToSize( metricSize() )));
    if (reportDef()) {
        if (pinToLeft())
            originalRect.setLeft( output->scale().mmToRealPixel(reportDef()->option().leftMarginMM()) );
        if (pinToRight())
            originalRect.setWidth( output->scale().mmToRealPixel(
                reportDef()->option().pageWidthMM()
                - reportDef()->option().rightMarginMM()
                - reportDef()->option().leftMarginMM()
                - metricPos().x() ) );
    }
    QRectF rect(fm.boundingRect( originalRect, aflag, printableText() ));
    rect.setWidth( originalRect.width() );
    rect.setHeight( rect.height() + output->scale().mmToPixel(LABEL_HEIGHT_TRIM_MARGIN_MM));		//TRIM

    return rect;
}

void NCReportLabelItem::adjustSize(NCReportOutput* output)
{
    if ( !isAutoHeight() && !pinToLeft() && !pinToRight() )
        return;

    if ( isAdjusted() )
        return;

    QRectF pixelRect( outputRect(output) );

    //setRealMetricSize( QSizeF( metricSize().width(), o->scale().pixelToMM( outputRect(o).height()) ) );
    setRealMetricSize( output->scale().sizeToMM( pixelRect.size() ) );

    if ( output->output() != NCReportOutput::Null )
        setAdjusted(true);
}


bool NCReportLabelItem::isEmpty() const
{
    return text().isEmpty();
}

void NCReportLabelItem::adjustFont( QFont & font, NCReportOutput *o )
{
    //font.setPointSizeF( NCReportUtils::fontSizeConvert(font.pointSizeF()) );
#ifdef LABEL_FONT_TRIM
    // Conversion from point into DTP-Millimeter and then into pixels
    NCReportScale scale;
    if ( o )
        scale = o->scale();
    font.setPixelSize( scale.mmToPixel( font.pointSizeF() * LABEL_FONT_TRIM_FACTOR ));
#else
    Q_UNUSED(font);
    Q_UNUSED(o);
#endif
    /*
    if (o) {
        font.setPixelSize( o->scale().fontPointSizeToPixel(font.pointSize()) );
    } else {
        NCReportScale scale;
        font.setPixelSize( scale.fontPointSizeToPixel(font.pointSize()) );
    }
    */
}

QString NCReportLabelItem::text() const
{
    NCReportLabelData* ld = (NCReportLabelData*)d;
    QString text;

#ifdef LABEL_QT_TRANSLATOR_INTEGRATION
    if (ld->reportDef->parameters().value("useQTranslator").toBool()) {
        QString translationContext = ld->reportDef->parameters().value("QTranslatorContext").toString();
        QString originalTextReportStyle = ld->translations[ld->reportDef->option().currentLanguage()];

        // extract variables and convert text to Qt format (%1, %2 instead of $D{foo})
        QString originalTextQtStyle = originalTextReportStyle;
        QList<QString> variableExpressions;
        QString comment;
        QRegExp variableRegex(QString("\\$(?:D|V|F|P)\\{([^}]+)\\}"), Qt::CaseSensitive, QRegExp::RegExp2);
        int count = 1;
        int pos = 0;
        while ((pos = variableRegex.indexIn(originalTextReportStyle, pos)) != -1) {
            QString variableExpression = variableRegex.cap(0);
            variableExpressions << variableExpression;
            QString replacement = QString("\%%1").arg(count);
            originalTextQtStyle.replace(variableExpression, replacement);
            QString variableName = variableRegex.cap(1);
            pos += variableRegex.matchedLength();
            comment += QString::fromLatin1("\%%1 is %2 ").arg(QString::number(count), variableName);
            ++count;
        }
        text = QCoreApplication::translate(translationContext.toUtf8().constData(), originalTextQtStyle.toUtf8().constData(), comment.toUtf8().constData());

        // convert variable substitutions from Qt style (%1, %2...) back to NCReport style ($D{foo})
        for (int i = 0; i < variableExpressions.length(); ++i) {
            text.replace(QString("\%%1").arg(i+1), variableExpressions.at(i));
        }
    } else {
        text = ld->translations[ld->reportDef->option().currentLanguage()];
    }
#else
    text = ld->translations[ld->reportDef->option().currentLanguage()];
    if (text.isEmpty()) {
        switch (ld->reportDef->option().languageRole()) {
            case NCReportPageOption::UseDefaultLanguage:
                text = ld->translations[ld->reportDef->option().defaultLanguage()];
                break;
            case NCReportPageOption::LeaveEmpty:
                break;
        }
    }

#endif

    return text;
}

void NCReportLabelItem::setText(const QString &text, const QString& langcode )
{
    NCReportItem::setText(text, langcode);

    NCReportLabelData* ld = (NCReportLabelData*)d;
    if (langcode.isEmpty())
        ld->translations[ld->reportDef->option().currentLanguage()] = text;
    else
        ld->translations[langcode] = text;
}

void NCReportLabelItem::updateValue(NCReportOutput *output, NCReportEvaluator *evaluator)
{
    if (hasConditionalFormat()) {
        NCReportConditionalFormat cFormat;
        cFormat.setFont( font() );

        if (cFormat.evaluate( conditionalFormatCode(), evaluator )) {
            if (cFormat.fontChanged())
                setFont(cFormat.font());

            if (cFormat.colorChanged())
                setForeColor(cFormat.color());

            if (cFormat.backColorChanged())
                setBackColor(cFormat.backColor());
        } else {
            restoreTextStyle();
        }
    }

    NCReportItem::updateValue(output, evaluator);
}

void NCReportLabelItem::saveTextStyle()
{
    NCReportLabelData* ld = (NCReportLabelData*)d;
    ld->textStyle_backup = ld->textStyle;
}

void NCReportLabelItem::restoreTextStyle()
{
    NCReportLabelData* ld = (NCReportLabelData*)d;
    ld->textStyle = ld->textStyle_backup;
}

void NCReportLabelItem::printWhenTrueAction(NCReportEvaluator *evaluator)
{
    if (hasConditionalFormat()) {
        // Conditional format expression can contain a variable manipulation lines.
        // The lines starts with $V{...
        NCReportVariableModifier vm;
        vm.modifyValue(conditionalFormatCode(), evaluator);
    }
}

void NCReportLabelItem::setDisplayValue( const QString& dv )
{
    NCReportLabelData* ld = (NCReportLabelData*)d;
    //ld->lastDisplayValue = ld->displayValue; // save last display value
    ld->displayValue = dv;

}

QString NCReportLabelItem::printableText() const
{
    NCReportLabelData* ld = (NCReportLabelData*)d;
    if ( ld->objectType == Label )
        return text();
    else if ( ld->objectType == Field )
        return ld->displayValue;

    return QString();
}

QFont & NCReportLabelItem::fontRef()
{
    NCReportLabelData* ld = (NCReportLabelData*)d;
    return ld->textStyle.font;
}

//
void NCReportLabelItem::setBorderPen(const QPen &pen)
{
    NCReportLabelData *ld = (NCReportLabelData*)d;
    prepareGeometryChange();
    ld->borderPen = pen;
    update();
}
