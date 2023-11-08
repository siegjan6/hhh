/****************************************************************************
*
*  Copyright (C) 2002-2009 Helta Kft. / NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: nszabo"helta.hu, info"nocisoft.com
*  Web: www.nocisoft.com
*
*  This file is part of the NCReport reporting software
*
*  Licensees holding a valid NCReport License Agreement may use this
*  file in accordance + the rights, responsibilities, and obligations
*  contained therein. Please consult your licensing agreement or contact
*  nszabo"helta.hu if any conditions of this licensing are not clear
*  to you.
*
*  This file is provided AS IS + NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/
#include "ncreportbarcodeitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportbarcoderenderer.h"
#include "ncreportevaluator.h"
#include "ncreportdef.h"
#include "ncreportutils.h"

#include <QPainter>
#ifndef NCREPORT_NO_SVG
#include <QSvgRenderer>
#endif
#include <QProcess>
#include <QDir>
#include <math.h>

NCReportBarcodeItem::NCReportBarcodeItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem( parent )
{
    NCReportBarcodeData *bd = new NCReportBarcodeData;
    d = bd;
    d->reportDef = rdef;
    bd->objectType = Barcode;
    bd->tagname = "barcode";
}

NCReportBarcodeItem::~ NCReportBarcodeItem()
{
}

void NCReportBarcodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rect( QPointF(0, 0), d->currentSize );
    render( painter, rect );	// render as svg
    //painter->drawImage( boundingRect(), data()->image ); // RENDER AS IMAGE

    paintSelection( painter );
}

QPixmap NCReportBarcodeItem::toPixmap(const QSize &size)
{
    QPixmap pm(size);
    pm.fill(Qt::white);
    QPainter painter(&pm);
    render(&painter, QRectF(0,0,size.width(),size.height()) );
    return pm;
}

QImage NCReportBarcodeItem::toImage(const QSize &size)
{
    QImage image(size, QImage::Format_RGB32);
    image.fill(Qt::white);
    QPainter painter(&image);
    render(&painter, QRectF(0,0,size.width(),size.height()) );
    return image;
}


void NCReportBarcodeItem::paint( NCReportOutput* o, const QPointF& mPos )
{
    //NCReportBarcodeData* bd = (NCReportBarcodeData*)d;
    QRect rect( o->scale().mmToPixel( mPos.x() + d->mPos.x() ),
             o->scale().mmToPixel( mPos.y() + d->mPos.y() ),
             o->scale().mmToPixel( d->mSize.width() ),
             o->scale().mmToPixel( d->mSize.height() ) );

    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
            render( o->painter(), rect );
            break;
        default:
            break;
    }
    o->renderItem( this, rect );
}

bool NCReportBarcodeItem::read(NCReportXMLReader *r )
{
    NCReportItem::read( r );
    NCReportBarcodeData* bd = (NCReportBarcodeData*)d;

    if ( r->isAttrib("barcodeType") ) {
        QString bct = r->attrib("barcodeType").toString();
        if (bct == "EAN" || bct=="EAN13" || bct=="EANX")	// for backward compatibility
            bd->barcodeType = 13;
        else
            bd->barcodeType = bct.toInt();
    }
    if ( r->isAttrib("code") )
        bd->setValue( r->attribToString("code") );
    if ( r->isAttrib("heightFactor") )
        bd->heightRatio = r->attribToInt("heightFactor");
    if ( r->isAttrib("heightRatio") )
        bd->heightRatio = r->attribToInt("heightRatio");
    if ( r->isAttrib("whiteSpace") )
        bd->whiteSpace = r->attribToInt("whiteSpace");

    if ( r->isAttrib("borderType") )
        bd->borderType = r->attribToInt("borderType");
    if ( r->isAttrib("borderWidth") )
        bd->borderWidth = r->attribToInt("borderWidth");

    if ( r->isAttrib("foreColor") )
        bd->foreColor = QColor(r->attribToString("foreColor"));
    if ( r->isAttrib("backColor") )
        bd->backColor = QColor(r->attribToString("backColor"));

    if ( r->isAttrib("scale") )
        bd->scale = r->attribToReal("scale");
    if ( r->isAttrib("rotation") )
        bd->rotation = r->attribToInt("rotation");
    if ( r->isAttrib("cols") )
        bd->columns = r->attribToInt("cols");
    if ( r->isAttrib("vers") )
        bd->version = r->attribToInt("vers");
    if ( r->isAttrib("secure") )
        bd->secure = r->attribToInt("secure");
    if ( r->isAttrib("primary") )
        bd->primaryText = r->attribToString("primary");
    if ( r->isAttrib("mode") )
        bd->mode = r->attribToInt("mode");
    if ( r->isAttrib("showText") )
        bd->showCode = r->attribToBool("showText");

    bd->text = r->readElementText();
    update_barcode();

    return true;
}


bool NCReportBarcodeItem::write(NCReportXMLWriter *w)
{
    NCReportBarcodeData* bd = (NCReportBarcodeData*)d;
    w->writeStartElement( bd->tagname );
    NCReportItem::write(w);

    w->writeAttribute("barcodeType", QString::number(barcodeType()) );
    w->writeAttribute("code", value().toString() );

    if ( heightRatio() != BARCODE_DEFAULT_HEIGHT )
        w->writeAttribute("heightRatio", QString::number(heightRatio()) );
    if ( whiteSpace() != BARCODE_DEFAULT_WSPACE )
        w->writeAttribute("whiteSpace", QString::number(whiteSpace()) );

    if ( borderType() != BARCODE_DEFAULT_BORDER ) {
        w->writeAttribute("borderType", QString::number(borderType()) );
        w->writeAttribute("borderWidth", QString::number(borderWidth()) );
    }

    if ( foreColor() != BARCODE_DEFAULT_FC )
        w->writeAttribute("foreColor", foreColor().name() );
    if ( backColor() != BARCODE_DEFAULT_BC )
        w->writeAttribute("backColor", backColor().name() );
    if ( scale() != BARCODE_DEFAULT_SCALE )
        w->writeAttribute("scale", QString::number(scale(),'f',2) );
    if ( rotation() != BARCODE_DEFAULT_ROTATE )
        w->writeAttribute("rotation", QString::number(rotation()) );
    if ( columns() != BARCODE_DEFAULT_COLS )
        w->writeAttribute("cols", QString::number(columns()) );
    if ( version() != BARCODE_DEFAULT_VERS )
        w->writeAttribute("vers", QString::number(version()) );
    if ( secure() != BARCODE_DEFAULT_SECURE )
        w->writeAttribute("secure", QString::number(secure()) );
    if ( !primaryText().isEmpty() )
        w->writeAttribute("primary", bd->primaryText );
    if ( mode() != BARCODE_DEFAULT_MODE )
        w->writeAttribute("mode", QString::number(mode()) );
    if ( !isShowCode() )
        w->writeAttribute("showText", "false" );

    w->writeCharacters( bd->text );
    w->writeEndElement();

    return true;
}

void NCReportBarcodeItem::setDefaultForEditor()
{
    setValue("503102187159");
    resize( QSizeF(110.0,50.0) );
    update_barcode();
}

bool NCReportBarcodeItem::update_barcode()
{
    NCReportBarcodeData* bd = (NCReportBarcodeData*)d;

    if (isEmpty()) {
        bd->data = QByteArray();
        return false;
    }

    QStringList arguments;
    arguments << QString("--barcode=%1").arg(barcodeType());
    arguments << "--directsvg";
    // HEIGHT
    if ( heightRatio() != BARCODE_DEFAULT_HEIGHT )
        arguments << QString("--height=%1").arg(heightRatio());
    // WHITESPACE
    if ( whiteSpace() != BARCODE_DEFAULT_WSPACE )
        arguments << QString("--whitesp=%1").arg(whiteSpace());

    // BORDER
    if ( borderType() == 1 )
        arguments << "--bind";
    if ( borderType() == 2 )
        arguments << "--box";
    if ( borderType()>0 )
        arguments << QString("--border=%1").arg(borderWidth());

    // REVERSE
    if ( false )
        arguments << "-r";

    // COLORS
    if ( foreColor() != BARCODE_DEFAULT_FC )
        arguments << QString("--fg=%1").arg(foreColor().name().right(6));
    if ( backColor() != BARCODE_DEFAULT_BC )
        arguments << QString("--bg=%1").arg(backColor().name().right(6));

    // SCALE
//	if ( scale() == BARCODE_DEFAULT_SCALE )
//		arguments << "--scale=10";
//	else
//		arguments << QString("--scale=%1").arg(scale());

    if ( barcodeType()==57)
        arguments << "--scale=10";

    // ROTATE
    if ( rotation() != BARCODE_DEFAULT_ROTATE )
        arguments << QString("--rotate=%1").arg(rotation());

    // COLS
    if ( columns() != BARCODE_DEFAULT_COLS )
        arguments << QString("--cols=%1").arg(columns());
    // VERS
    if ( version() != BARCODE_DEFAULT_VERS )
        arguments << QString("--vers=%1").arg(version());
    // SECURE
    if ( secure() != BARCODE_DEFAULT_SECURE )
        arguments << QString("--secure=%1").arg(secure());
    // PRIMARY
    if ( !primaryText().isEmpty() )
        arguments << QString("--primary=%1").arg(primaryText());
    // MODE
    if ( mode() != BARCODE_DEFAULT_MODE )
        arguments << QString("--mode=%1").arg(mode());

#if defined(Q_OS_WIN) || defined(Q_WS_WIN)

    //------------------------------------------
    // QR CODE: UNICODE PROBLEM UNDER WINDOWS
    //------------------------------------------
    if (barcodeType()==58)
    {
        QString tmpFile = QDir::tempPath()+QString("/barcodetext_%1.txt").arg(id());
        NCReportUtils::stringToFile( tmpFile, value().toString() );
        QString arg = QString("--input=%1").arg(tmpFile);
        arguments << arg;
    }
    else
    {
        arguments << QString("--data=%1").arg( value().toString());
    }
#else
    arguments << QString("--data=%1").arg( value().toString());
#endif

//#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("zint %s",qPrintable(arguments.join(" ")));
//#endif
    QProcess zint;

    if (reportDef()->zintPath().isEmpty())
        zint.start("zint", arguments );
    else
        zint.start(reportDef()->zintPath()+"/zint", arguments );

    if (!zint.waitForStarted()) {
        reportDef()->emitZintNotFound();
        return false;
    }
    if (!zint.waitForFinished()) {
        return false;
    }

    bd->data = zint.readAllStandardOutput();
    if (bd->data.isEmpty())
        bd->data = zint.readAllStandardError();
    //bd->image.loadFromData(zint.readAll());
    return true;
}

//void NCReportBarcodeItem::renderSvg( QPainter* painter, const QRectF& rect, bool hires )
//{
//	// RENDER AS SVG
//	QSvgRenderer svgrenderer;
//	svgrenderer.load( data()->data );	// barcode as svg xml string
//
//	QRectF r(rect);
//	QSizeF size = svgrenderer.defaultSize();
//	size.scale( r.size(), Qt::KeepAspectRatio );
//	r.setSize(size);
//
//	if ( hires ) {
//		QImage im( size.width(), size.height(), QImage::Format_RGB16 );
//		QPainter p;
//		p.setRenderHint(QPainter::Antialiasing,false);
//		p.setRenderHint(QPainter::TextAntialiasing,true);
//		p.begin(&im);
//		svgrenderer.render( &p, QRectF(0,0,size.width(), size.height()) );
//		p.end();
//
//		painter->drawImage( r, im );
//	} else {
//		svgrenderer.render( painter, r );
//	}
//}

void NCReportBarcodeItem::renderSvg( QPainter* painter, const QRectF& rect )
{
#ifndef NCREPORT_NO_SVG
    // RENDER AS SVG
    QSvgRenderer svgrenderer;
    svgrenderer.load( data()->data );	// barcode as svg xml string

    QRectF r(rect);
    QSizeF size = svgrenderer.defaultSize();
    size.scale( r.size(), Qt::KeepAspectRatio );
    r.setSize(size);
    svgrenderer.render( painter, r );
#else
    Q_UNUSED(painter);
    Q_UNUSED(rect);
#endif
}

void NCReportBarcodeItem::render( QPainter* painter, const QRectF& rect )
{
    if ( isEmpty() )		// no value
        return;

    if ( barcodeType() == 57 ) { // maxicode
        renderSvg(painter,rect);

    } else 	{
        if (rotation()!=0) {
            painter->save();
            rotateBarCode(painter,rect);
        }
        NCReportBarcodeRenderer renderer(painter, data()->data, rect );
        renderer.render( data()->showCode );
        if (rotation()!=0)
            painter->restore();

    }

}

QString NCReportBarcodeItem::barcodeName(uint type) const
{
    QString n;
    switch (type) {
        case 1: n="Code 11"; break;
        case 2: n="Standard Code 2 of 5"; break;
        case 3: n="Interleaved 2 of 5"; break;
        case 4: n="Code 2 of 5 IATA"; break;
        case 6: n="Code 2 of 5 Data Logic"; break;
        case 7: n="Code 2 of 5 Industrial"; break;
        case 8: n="Code 3 of 9 (Code 39)"; break;
        case 9: n="Extended Code 3 of 9 (Code 39+)"; break;
        case 13: n="EAN"; break;
        case 16: n="GS1-128 (UCC.EAN-128)"; break;
        case 18: n="Codabar"; break;
        case 20: n="Code 128 (Auto Subset)"; break;
        case 21: n="Deutshe Post Leitcode"; break;
        case 22: n="Deutshe Post Identcode"; break;
        case 23: n="Code 16K"; break;
        case 24: n="Code 49"; break;
        case 25: n="Code 93"; break;
        case 28: n="Flattermarken"; break;
        case 29: n="GS1 DataBar-14"; break;
        case 30: n="GS1 DataBar Limited"; break;
        case 31: n="GS1 DataBar Extended"; break;
        case 32: n="Telepen Alpha"; break;
        case 34: n="UPC A"; break;
        case 37: n="UPC E"; break;
        case 40: n="PostNet"; break;
        case 47: n="MSI Plessey"; break;
        case 49: n="FIM"; break;
        case 50: n="LOGMARS"; break;
        case 51: n="Pharmacode One-Track"; break;
        case 52: n="PZN"; break;
        case 53: n="Pharmacode Two-Track"; break;
        case 55: n="PDF417"; break;
        case 56: n="PDF417 Truncated"; break;
        case 57: n="Maxicode"; break;
        case 58: n="QR Code"; break;
        case 60: n="Code 128 (Subset B)"; break;
        case 63: n="Australia Post Standard Customer"; break;
        case 66: n="Australia Post Reply Paid"; break;
        case 67: n="Australia Post Routing"; break;
        case 68: n="Australia Post Redirection"; break;
        case 69: n="ISBN (EAN-13 + verification stage)"; break;
        case 70: n="Royal Mail 4 State (RM4SCC)"; break;
        case 71: n="Data Matrix"; break;
        case 72: n="EAN-14"; break;
        case 74: n="Codablock-F"; break;
        case 75: n="NVE-18"; break;
        case 76: n="Japanese Postal Code"; break;
        case 77: n="Korea Post"; break;
        case 79: n="GS1 DataBar-14 Stacked"; break;
        case 80: n="GS1 DataBar-14 Stacked Omnidirectional"; break;
        case 81: n="GS1 DataBar Expanded Stacked"; break;
        case 82: n="PLANET"; break;
        case 84: n="MicroPDF417"; break;
        case 85: n="USPS OneCode"; break;
        case 86: n="Plessey Code"; break;
        case 87: n="Telepen Numeric"; break;
        case 89: n="ITF-14"; break;
        case 90: n="Dutch Post KIX Code"; break;
        case 92: n="Aztec Code"; break;
        case 93: n="DAFT Code"; break;
        case 97: n="Micro QR Code"; break;
        case 98: n="HIBC Code 128"; break;
        case 99: n="HIBC Code 39"; break;
        case 102: n="HIBC Data Matrix"; break;
        case 104: n="HIBC QR Code"; break;
        case 106: n="HIBC PDF417"; break;
        case 108: n="HIBC MicroPDF417"; break;
        case 110: n="HIBC Codablock-F"; break;
        case 112: n="HIBC Aztec Code"; break;
        case 128: n="Aztec Runes"; break;
        case 129: n="Code 32"; break;
        case 130: n="Composite Symbol + EAN linear"; break;
        case 131: n="Composite Symbol + GS1-128 linear"; break;
        case 132: n="Composite Symbol + GS1 DataBar-14 linear"; break;
        case 133: n="Composite Symbol + GS1 DataBar Limited"; break;
        case 134: n="Composite Symbol + GS1 DataBar Extended"; break;
        case 135: n="Composite Symbol + UPC A linear"; break;
        case 136: n="Composite Symbol + UPC E linear"; break;
        case 137: n="Composite Symbol + GS1 DataBar-14 Stacked"; break;
        case 138: n="Composite Symbol + GS1 DataBar-14 Stacked Omn."; break;
        case 139: n="Composite Symbol + GS1 DataBar Expanded Stacked"; break;
        case 140: n="Channel Code"; break;
        case 141: n="Code One"; break;
        case 142: n="Grid Matrix"; break;
    }

    return n;
}

bool NCReportBarcodeItem::isComposite() const
{
    return (barcodeType()>129 && barcodeType()<140);
}

bool NCReportBarcodeItem::isEmpty() const
{
    return value().toString().isEmpty();
}

void NCReportBarcodeItem::updateValue(NCReportOutput*output, NCReportEvaluator *evaluator)
{

    if (sourceType() == Static || sourceType() == File )
        return;

    evaluator->evaluateDynamicBarcode( this );
    NCReportItem::updateValue(output, evaluator);
}

void NCReportBarcodeItem::rotateBarCode(QPainter *painter, const QRectF rect)
{
    const double Pi=3.14159265;
    QPointF offsetPoint( rect.topLeft() );

    const bool method_1 = true;

    if (method_1) {
        offsetPoint.rx()+=rect.height()*sin(rotation()*Pi/180)/2;
        offsetPoint.ry()+=rect.height()*sin(rotation()*Pi/180)/2;

        painter->translate( offsetPoint );
        painter->rotate(rotation());
        painter->translate( -offsetPoint );
        switch (rotation()) {
        case 90:
            painter->translate( 0, rect.height()/2);
            break;
        case 180:
            painter->translate( -rect.width(), -rect.height());
            break;
        case 270:
            painter->translate( -rect.width()*2, 0);
            break;
        }

    } else {
        //offsetPoint.rx()+=rect.height()*sin(rotation()*Pi/180)/2;
        //offsetPoint.ry()+=rect.height()*sin(rotation()*Pi/180)/2;

        qreal t1 = sin(rotation()*Pi/180) * rect.height();
        //qreal t2 = t1*rect.width()/rect.height();
        painter->translate( t1, 0 );
        painter->rotate(rotation());

        //painter->translate( -t1, 0 );
    }
}


QList<int> NCReportBarcodeItem::availableSourceTypes() const
{
    QList<int> types;
    types << NCReportItem::Static << NCReportItem::DataSource << NCReportItem::Parameter << NCReportItem::Template << NCReportItem::ScriptExpression;
    return types;
}
