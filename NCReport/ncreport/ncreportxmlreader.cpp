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
#include "ncreportxmlreader.h"
#include "ncreportsection.h"
#include "ncreportpageoption.h"

#include "ncreportlabelitem.h"
#include "ncreportfielditem.h"
#include "ncreportlineitem.h"
#include "ncreportrectitem.h"
#include "ncreportbarcodeitem.h"
#include "ncreportimageitem.h"
#include "ncreportgraphitem.h"
#include "ncreporttextitem.h"
#include "ncreportcrosstabitem.h"
#include "ncreportellipseitem.h"
#include "ncreportchartitem.h"
#ifndef NCREPORT_NO_TABLE_VIEW
#include "ncreporttableitem.h"
#endif
#include "ncreportscale.h"

#include <QObject>
#include <QDate>
#include <QDateTime>

NCReportXMLReader::NCReportXMLReader()
{
    basetag = "NCReport";
    rdef = 0;
    m_langcode = "en";
}

NCReportXMLReader::~ NCReportXMLReader()
{
}

NCReportDef * NCReportXMLReader::reportDef() const
{
    return rdef;
}

void NCReportXMLReader::setReportDef(NCReportDef *rd)
{
    rdef = rd;
}

void NCReportXMLReader::setPageOption(NCReportPageOption *o)
{
    pageoption = o;
}

bool NCReportXMLReader::read(QIODevice *device, const QString& def )
{
    if ( device )
        setDevice(device);
    else if ( !def.isNull() )
        addData( def );

    while (!atEnd()) {
        readNext();

        if (isStartElement()) {
            if (name() == basetag && attributes().value("version").toString() >= "2.0")
                readReport();
            else
                raiseError(QObject::tr("Uncompatible %1 file version.").arg(basetag) );
        }
    }

    return !error();
}



void NCReportXMLReader::readReport()
{
    Q_ASSERT(isStartElement() && name() == basetag);

    pageoption->setReportName(attribToString("name"));
    pageoption->setReportType( attribToString("type") );

    while (!atEnd()) {
        readNext();

        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "options")
                readOptions();
            else if (name() == "datasources")
                readDataSources();
            else if (name() == "parameters")
                readParameters();
            else if (name() == "variables")
                readVariables();
            else if (name() == "scripts")
                readScripts();
            else if (name() == "pageheader")
                readPageHeader();
            else if (name() == "pagefooter")
                readPageFooter();
            else if (name() == "reportheader")
                readReportHeader();
            else if (name() == "reportfooter")
                readReportFooter();
            else if (name() == "details")
                readDetails();
            else if (name() == "htmltext")
                readHtmlText();
            else if (name() == "page")
                readPage();
            else
                readUnknownElement();
        }
    }
}

void NCReportXMLReader::readOptions()
{
    pageoption->read( this );
}

void NCReportXMLReader::readItems( NCReportSection* section )
{
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug( "ReadItems(%p)",section);
#endif

    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;

        if (isStartElement()) {
#ifdef NCREPORT_DEBUG_OUTPUT
            qDebug( "Read item: %s",qPrintable(name().toString()) );
#endif
            NCReportItem *i=0;

            if (name() == "label") {
                i = new NCReportLabelItem( NCReportItem::Label, reportDef() );
            }
            else if (name() == "field") {
                NCReportFieldItem *item = new NCReportFieldItem(reportDef());
                i = item;
                readField( item );	// virtual
            }
            else if (name() == "text") {
                i = new NCReportTextItem(reportDef());
            }
            else if (name() == "line")  {
                i = new NCReportLineItem(reportDef());
            }
            else if (name() == "rectangle") {
                i = new NCReportRectItem(reportDef());
            }
            else if (name() == "ellipse") {
                i = new NCReportEllipseItem(reportDef());
            }
            else if (name() == "image") {
                i = new NCReportImageItem(reportDef());
            }
            else if (name() == "barcode") {
                i = new NCReportBarcodeItem(reportDef());
            }
            else if (name() == "graph") {
                i = new NCReportGraphItem(reportDef());
            }
            else if (name() == "crosstab") {
                i = new NCReportCrossTabItem(reportDef());
            }
            else if (name() == "table") {
#ifndef NCREPORT_NO_TABLE_VIEW
                i = new NCReportTableItem(reportDef());
#endif
            }
            else if (name() == "chart") {
#ifdef HIGHCHARTS_INTEGRATION
                i = new NCReportChartItem(reportDef());
#endif
            }
            else {
                readUnknownElement();
            }

            // READ AND ADD ITEM
            if (i) {
                beforeReadItem(i);
                i->read( this );
                afterReadItem( i );
                addItem(i,section);
                afterAddItem( i );
            }
        }

    }
}

void NCReportXMLReader::addItem(NCReportItem *item, NCReportSection* section )
{
    section->addItemZ( item );
    section->addItemForEngine( item );
}

void NCReportXMLReader::readUnknownElement()
{
    //Q_ASSERT(isStartElement());
#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug( "Unknown element: %s", qPrintable(name().toString()) ) ;
#endif
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement())
            readUnknownElement();
    }
}

bool NCReportXMLReader::isAttrib(const QString & name )
{
    return !attributes().value(name).isEmpty();
}

QStringRef NCReportXMLReader::attrib(const QString & name )
{
    return attributes().value(name);
}

QString NCReportXMLReader::attribToString(const QString & name)
{
    return attributes().value(name).toString();
}

int NCReportXMLReader::attribToInt(const QString & name)
{
    return attributes().value(name).toString().toInt();
}

qreal NCReportXMLReader::attribToReal(const QString & name)
{
    return attributes().value(name).toString().toDouble();
}

bool NCReportXMLReader::attribToBool(const QString & name)
{
    return (attributes().value(name) == "true" || attributes().value(name)=="1");
}

void NCReportXMLReader::readFont( QFont& f )
{
    if ( isAttrib("fontName") )
        f.setFamily(attrib("fontName").toString());
    else
        setDefaultFont( f );	// default font settings virtual func

    if ( isAttrib("fontSize") )
        f.setPointSize(attrib("fontSize").toString().toInt());
        //f.setPixelSize( sc.fontPointSizeToPixel(attrib("fontSize").toString().toDouble()) );

    if ( isAttrib("fontWeight") ) {
        QFont::Weight w = QFont::Normal;
        QStringRef att = attrib("fontWeight");
        if (att == "light")
            w = QFont::Light;
        else if (att == "normal")
            w = QFont::Normal;
        else if (att == "demibold")
            w = QFont::DemiBold;
        else if (att == "bold")
            w = QFont::Bold;
        else if (att == "black")
            w = QFont::Black;

        f.setWeight(w);
    }
    if ( isAttrib("fontItalic") )
        f.setItalic( attribToBool("fontItalic") );
    if ( isAttrib("fontUnderline") )
        f.setUnderline( attribToBool("fontUnderline") );
    if ( isAttrib("fontStrikeOut") )
        f.setStrikeOut( attribToBool("fontStrikeOut") );

    //f.setLetterSpacing(QFont::AbsoluteSpacing,100);

    if ( isAttrib("letterSpacing") )
        f.setLetterSpacing( QFont::AbsoluteSpacing, attribToReal("letterSpacing") );
    if ( isAttrib("capital") )
        f.setCapitalization( (QFont::Capitalization)attribToInt("capital") );

}

void NCReportXMLReader::readPen( QPen& pen )
{
    if ( isAttrib("lineColor") )
        pen.setColor(QColor(attribToString("lineColor"))) ;
    if ( isAttrib("lineStyle") ) {
        Qt::PenStyle ps = Qt::SolidLine;
        QStringRef ls = attrib("lineStyle");

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
    //NCReportScale s;
    //if ( isAttrib("lineWidth") )
    //	pen.setWidthF( s.mmToRealPixel(attribToReal("lineWidth")) );
}

void NCReportXMLReader::readBrush( QBrush& b )
{
    if ( isAttrib("fillColor") )
        b.setColor(QColor(attribToString("fillColor"))) ;
    if ( isAttrib("fillStyle") ) {
        Qt::BrushStyle bs = Qt::NoBrush;
        QStringRef ls = attrib("fillStyle");

        if (ls == "no")
            bs = Qt::NoBrush;
        if (ls == "solid")
            bs = Qt::SolidPattern;
        if (ls == "dense1")
            bs = Qt::Dense1Pattern;
        if (ls == "dense2")
            bs = Qt::Dense2Pattern;
        if (ls == "dense3")
            bs = Qt::Dense3Pattern;
        if (ls == "dense4")
            bs = Qt::Dense4Pattern;
        if (ls == "dense5")
            bs = Qt::Dense5Pattern;
        if (ls == "dense6")
            bs = Qt::Dense6Pattern;
        if (ls == "dense7")
            bs = Qt::Dense7Pattern;
        if (ls == "hor")
            bs = Qt::HorPattern;
        if (ls == "ver")
            bs = Qt::VerPattern;
        if (ls == "cross")
            bs = Qt::CrossPattern;
        if (ls == "bdiag")
            bs = Qt::BDiagPattern;
        if (ls == "fdiag")
            bs = Qt::FDiagPattern;
        if (ls == "diagcross")
            bs = Qt::DiagCrossPattern;

        b.setStyle( bs );
    }
}

QPointF NCReportXMLReader::readPos() const
{
    qreal x = attributes().value("posX").toString().toDouble();
    qreal y = attributes().value("posY").toString().toDouble();
    return QPointF(x,y);
}

QPointF NCReportXMLReader::readPosFrom() const
{
    qreal x = attributes().value("fromX").toString().toDouble();
    qreal y = attributes().value("fromY").toString().toDouble();
    return QPointF(x,y);
}

QPointF NCReportXMLReader::readPosTo() const
{
    qreal x = attributes().value("toX").toString().toDouble();
    qreal y = attributes().value("toY").toString().toDouble();
    return QPointF(x,y);
}

QSizeF NCReportXMLReader::readSize() const
{
    qreal w = attributes().value("width").toString().toDouble();
    qreal h = attributes().value("height").toString().toDouble();
    return QSizeF( w, h );
}

Qt::Alignment NCReportXMLReader::readAlignment()
{
    Qt::Alignment align=Qt::AlignLeft;
    if ( isAttrib("alignment") )
        align = (Qt::Alignment)attribToInt("alignment");

    if ( isAttrib("alignmentH") ) {		//backward comp
        QStringRef att = attrib("alignmentH");
        if (att == "left")
            align = Qt::AlignLeft;
        else if (att == "right")
            align = Qt::AlignRight;
        else if (att == "center")
            align = Qt::AlignHCenter;
    }

    if ( isAttrib("alignmentV") ) {
        QStringRef att = attrib("alignmentV");
        if (att == "top")
            align |= Qt::AlignTop;
        else if (att == "bottom")
            align |= Qt::AlignBottom;
        else if (att == "center")
            align |= Qt::AlignVCenter;
    }
    return align;
}

/*
Qt::Alignment NCReportXMLReader::readAlignmentH()
{
    Qt::Alignment align=Qt::AlignLeft;

    if ( isAttrib("alignmentH") ) {
        QStringRef att = attrib("alignmentH");
        if (att == "left")
            align = Qt::AlignLeft;
        else if (att == "right")
            align = Qt::AlignRight;
        else if (att == "center")
            align = Qt::AlignHCenter;
    }
    return align;
}
Qt::Alignment NCReportXMLReader::readAlignmentV()
{
    Qt::Alignment align=Qt::AlignTop;
    if ( isAttrib("alignmentV") ) {
        QStringRef att = attrib("alignmentV");
        if (att == "top")
            align = Qt::AlignTop;
        else if (att == "bottom")
            align = Qt::AlignBottom;
        else if (att == "center")
            align = Qt::AlignVCenter;
    }
    return align;
}
*/

void NCReportXMLReader::readDataSources() {}
void NCReportXMLReader::readDataSource() {}
void NCReportXMLReader::readVariables() {}
void NCReportXMLReader::readVariable() {}
void NCReportXMLReader::readScripts() {}
void NCReportXMLReader::readScript() {}

void NCReportXMLReader::readParameters() {}

void NCReportXMLReader::readParameters(QHash<QString, QVariant> &values)
{
    while (!atEnd()) {
        readNext();
        if (isEndElement())
            break;

        if (isStartElement()) {
            if (name() == "par" || name()=="parameter") {
                readParameter( values );
            } else {
                readUnknownElement();
            }
        }
    }
}

void NCReportXMLReader::readPageHeader() {}
void NCReportXMLReader::readPageFooter() {}
void NCReportXMLReader::readReportHeader() {}
void NCReportXMLReader::readReportFooter() {}
void NCReportXMLReader::readDetails() {}
void NCReportXMLReader::readHtmlText() {}
void NCReportXMLReader::readPage() {}

void NCReportXMLReader::readField(NCReportFieldItem *)
{
}

void NCReportXMLReader::beforeReadItem( NCReportItem* )
{
}

void NCReportXMLReader::afterReadItem( NCReportItem* )
{
}

void NCReportXMLReader::afterAddItem(NCReportItem *)
{
}

qreal NCReportXMLReader::readLineWidth()
{
    qreal lw=0;
    if ( isAttrib("lineWidth") )
        lw = attribToReal("lineWidth");

    return lw;
}

void NCReportXMLReader::setDefaultFont(QFont &)
{
}

void NCReportXMLReader::setDefaultPen(QPen &)
{
}

void NCReportXMLReader::setDefaultBrush(QBrush &)
{
}

void NCReportXMLReader::readSection(NCReportSection *section )
{
    section->setTagname( name().toString() );
    section->setMetricHeight( attribToReal("height") );
    if ( isAttrib("id") )
        section->setID(attribToString("id"));
    if ( isAttrib("autoHeight") )
        section->setAutoHeight( attribToBool("autoHeight") );
    if ( isAttrib("hideIfBlank") )
        section->setHideIfBlank( attribToBool("hideIfBlank") );
    if ( isAttrib("breakRole") ) {
        QStringRef att = attrib("breakRole");
        if (att == "spd")
            section->setCheckRole( NCReportSection::SectionPlusDetail );
        else if (att == "so")
            section->setCheckRole( NCReportSection::SectionOnly );
    }
    if ( isAttrib("orphanedTolerance") ) {
        section->setOrphanedTolaranceMM( attribToReal("orphanedTolerance") );
        if ( section->orphanedTolaranceMM()>0 )
            section->setCheckRole( NCReportSection::SectionPlusOrphanedTolerance );
    }

    if ( isAttrib("printWhen") )
        section->setPrintWhenExp( attribToString("printWhen") );

    if ( isAttrib("startsOnNewPage") )
        section->setStartsOnNewPage( attribToBool("startsOnNewPage") );

    if ( isAttrib("anchorToPageFooter") )
        section->setAnchorToPageFooter( attribToBool("anchorToPageFooter") );

    if ( isAttrib("notAlone") )
        section->setNotAlone( attribToBool("notAlone") );

    if ( isAttrib("htmlOpt") )
        section->setHtmlOptions( attribToString("htmlOpt") );

    if ( isAttrib("altBackRowMode") ) {
        QStringRef att = attrib("altBackRowMode");
        if (att == "disabled")
            section->setAlternateRowBackgroundMode( NCReportSection::Disabled );
        else if (att == "odd")
            section->setAlternateRowBackgroundMode( NCReportSection::Odd );
        else if (att == "even")
            section->setAlternateRowBackgroundMode( NCReportSection::Even );
    }
    if ( isAttrib("altBackRowColor") )
        section->setAlternateRowBackgroundColor( attribToString("altBackRowColor") );
    if ( isAttrib("useDefaultAltBackRowColor") )
        section->setUseDefaultAlternateRowColor( attribToBool("useDefaultAltBackRowColor") );
}


void NCReportXMLReader::setLanguage( const QString& langcode )
{
    m_langcode = langcode;
}

QString NCReportXMLReader::language() const
{
    return m_langcode;
}

void NCReportXMLReader::readColor(const QString &property, QColor &color)
{
    if ( isAttrib(property) )
        color = QColor(attribToString(property)) ;
}

/*!
 * \brief NCReportXMLReader::readParameter
 * \param values
 * Reads static report parameters into a parameter hash
 */
void NCReportXMLReader::readParameter( QHash<QString, QVariant> &values )
{
    QString type = attribToString("type");
    QString id = attribToString("id");
    QString stringValue = readElementText();
    QVariant value;
    if (type=="string" || type=="QString")
        value.setValue(stringValue);
    else if (type=="int")
        value.setValue(stringValue.toInt());
    else if (type=="bool")
        value.setValue(stringValue=="1" || stringValue=="true");
    else if (type=="float")
        value.setValue(stringValue.toFloat());
    else if (type=="double" || type=="numeric")
        value.setValue(stringValue.toDouble());
    else if (type=="long")
        value.setValue(stringValue.toLong());
    else if (type=="longlong")
        value.setValue(stringValue.toLongLong());
    else if (type=="date" || type=="QDate")
        //value.setValue(QDate::fromString(stringValue));
        value.setValue(stringValue);
    else if (type=="datetime" || type=="QDateTime")
        //value.setValue(QDateTime::fromString(stringValue));
        value.setValue(stringValue);
    else
        value.setValue(stringValue);

    values.insert( id, value );
}
