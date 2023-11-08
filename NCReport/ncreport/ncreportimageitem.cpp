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
#include "ncreportimageitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportevaluator.h"
#include "ncreporthttpclient.h"
#include "ncreportutils.h"
#include "ncreportdef.h"

#include <QPainter>
#include <QBuffer>
#include <QByteArray>
#include <QTextStream>
#include <QFile>
#ifndef NCREPORT_NO_SVG
#include <QSvgRenderer>
#endif
#include <QFlags>
#include <QMessageBox>
// IMAGE ITEM

NCReportImageItem::NCReportImageItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem( parent )
{
    d = new NCReportImageData;
    d->reportDef = rdef;
    NCReportImageData* data = (NCReportImageData*)d;
    data->tagname = "image";
    data->objectType = Image;
    data->format = Binary;
}

NCReportImageItem::~ NCReportImageItem()
{
}


// QRectF NCReportImageItem::boundingRect() const
// {
// 	NCReportScale sc;
// 	return QRectF( 0, 0, sc.mmToSize( d->mSize ).width(), sc.mmToSize( d->mSize ).height() );
// }


bool NCReportImageItem::read(NCReportXMLReader *r )
{
    NCReportItem::read( r );
    NCReportImageData* id = (NCReportImageData*)d;

    id->scaling = true; //r->attribToBool("scaling");
    if ( r->isAttrib("aspectRatio") ) {
        QStringRef asp = r->attrib("aspectRatio");
        if ( asp=="keep" ) id->aspRatMode = Qt::KeepAspectRatio;
        else if ( asp=="expand" ) id->aspRatMode = Qt::KeepAspectRatioByExpanding;
        else if ( asp=="ignore" ) id->aspRatMode = Qt::IgnoreAspectRatio;
    }
    if ( r->isAttrib("transformMode") ) {
        QStringRef asp = r->attrib("transformMode");
        if ( asp=="fast" ) id->transformMode = Qt::FastTransformation;
        else if ( asp=="smooth" ) id->transformMode = Qt::SmoothTransformation;
        else id->transformMode = Qt::FastTransformation;
    }
    if ( r->isAttrib("format") ) {
        if ( r->attrib("format")=="bin" || r->attrib("format")=="0")
            id->format = Binary;
        else if ( r->attrib("format")=="b64" || r->attrib("format")=="1")
            id->format = Base64Encoded;
        else if ( r->attrib("format")=="svg" || r->attrib("format")=="2")
            id->format = Svg;
    }

    if (r->isAttrib("htmlAlign"))
        setAlignment( NCReportUtils::indexToHorizontalAlignment(r->attribToInt("htmlAlign")));

    if (r->isAttrib("htmlWidth"))
        setHtmlWidth(r->attrib("htmlWidth").toString().toLatin1());
    if (r->isAttrib("htmlHeight"))
        setHtmlHeight(r->attrib("htmlHeight").toString().toLatin1());

    setAlignment(r->readAlignment());

    d->text = r->readElementText();

    switch ( d->sourceType ) {
        case Static:
        case File:
        case Url:
        case Parameter:
        {
            load(0);
            break;
        }
        case DataSource: //dynamic image from dsource
        case FileNameFromDataSource: //dynamic image - filename from dsource
        case UrlFromDataSource: //dynamic image - filename from dsource
        case InternalDocument:
            break;
    }
    return true;
}

bool NCReportImageItem::write(NCReportXMLWriter *w)
{
    NCReportImageData* id = (NCReportImageData*)d;

    w->writeStartElement( d->tagname );
    NCReportItem::write(w);

    QString asp;
    //w->writeAttribute( "scaling", id->scaling ? "true" : "false");
    switch ( id->aspRatMode ) {
        case Qt::KeepAspectRatio: w->writeAttribute("aspectRatio","keep"); break;
        case Qt::KeepAspectRatioByExpanding: w->writeAttribute("aspectRatio","expand"); break;
        case Qt::IgnoreAspectRatio: w->writeAttribute("aspectRatio","ignore"); break;
    }
    switch ( id->transformMode ) {
        case Qt::FastTransformation: break;
        case Qt::SmoothTransformation: w->writeAttribute("transformMode","smooth"); break;
    }

    w->writeInt("format", (int)id->format );
    w->writeAlignment(alignment());

//	if (htmlAlign()>0)
//		w->writeInt("htmlAlign",htmlAlign());

    if (!htmlWidth().isEmpty())
        w->writeAttribute("htmlWidth",htmlWidth());
    if (!htmlHeight().isEmpty())
        w->writeAttribute("htmlHeight",htmlHeight());

    switch ( d->sourceType ) {
        case Static:
        {
                if ( id->format==Binary || id->format==Base64Encoded ) {
                    w->writeCharacters( toBase64() );
                } else if (id->format==Svg) {
                    w->writeCharacters( id->svgXml );
                }
            break;
        }
        case DataSource: //dynamic image from datasource
        case File:
        case FileNameFromDataSource: //dynamic image - filename from datasource
        case Url:
        case UrlFromDataSource:
        case Parameter:
        {
            w->writeCharacters( id->text );
            break;
        }
        default:
            break;
    }
    w->writeEndElement();
    return true;
}

void NCReportImageItem::paint(NCReportOutput* output, const QPointF& mPos )
{
    NCReportImageData* id = (NCReportImageData*)d;
    //QSize imageSize(id->image.size());
    QRectF itemRect( output->scale().mmToPoint( mPos + metricPos()), output->scale().mmToSize( realMetricSize() ) );
    QRectF source( QPointF(0,0), id->image.size() );
    QSizeF imageTargetSize( id->image.size() );
    imageTargetSize.scale( output->scale().mmToSize( d->mSize ), id->aspRatMode );

    //QRectF target( o->scale().mmToPoint( mPos + d->mPos ), imageTargetSize );
    QRectF target( imageTargetPoint(itemRect,imageTargetSize), imageTargetSize );
    if (pinToRight())
    {
        target.moveRight( output->scale().mmToRealPixel(
            reportDef()->option().pageWidthMM() -
            reportDef()->option().rightMarginMM()
        ));
    }

    switch ( output->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
        {
            if ( id->format == Binary || id->format == Base64Encoded ) {
                if ( id->image.isNull() ) {
                    return;
                }

#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
                if ( id->transformMode == Qt::SmoothTransformation ) {
                    QImage im = id->image.scaled( imageTargetSize.toSize(), id->aspRatMode, Qt::SmoothTransformation );
                    output->painter()->drawImage( target.topLeft(), im );
                } else {
                    output->painter()->drawImage( target, id->image, source );
                }
#else
                if ( id->transformMode == Qt::SmoothTransformation ) {
                    QPixmap im = id->image.scaled( imageTargetSize.toSize(), id->aspRatMode, Qt::SmoothTransformation );
                    output->painter()->drawPixmap( target.topLeft(), im );
                } else {
                    output->painter()->drawPixmap( target, id->image, source );
                }
#endif
                break;
            } else if ( id->format == Svg ) {
                // SVG
                //QRectF rect( o->scale().mmToPoint( mPos + d->mPos ), o->scale().mmToSize( d->mSize ) );
#ifndef NCREPORT_NO_SVG
                QSvgRenderer renderer;
                if ( renderer.load(id->svgXml) ) {
                    QSizeF imageSize(renderer.defaultSize());
                    imageSize.scale( output->scale().mmToSize( d->mSize ), id->aspRatMode );
                    QRectF rect( output->scale().mmToPoint( mPos + d->mPos ), imageSize );
                    renderer.render( output->painter(), rect );
                }
#endif
            }
        }
        default:
            break;
    }
    output->renderItem( this, target );
}

void NCReportImageItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    NCReportScale sc;
    Q_UNUSED(option);
    Q_UNUSED(widget);

    NCReportImageData* id = (NCReportImageData*)d;
    QRectF rect( QPointF(0, 0), d->currentSize );
    painter->setClipRect( boundingRect().adjusted(0,0,1,1) );
    painter->setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform, true);
    QSizeF imageSize(id->image.size());
    imageSize.scale( rect.size(), id->aspRatMode );

    QRectF source( QPointF(0,0), id->image.size() );
    //QRectF target( QPointF(0,0), imageSize );
    QRectF target( imageTargetPoint(rect,imageSize) ,imageSize);


    if ( id->format == Svg && (sourceType()==Static || sourceType()==File) ) {
#ifndef NCREPORT_NO_SVG
        QSvgRenderer renderer;
        renderer.setViewBox( rect );
        if ( renderer.load(id->svgXml) ) {
            QSizeF imageSize(renderer.defaultSize());
            imageSize.scale( rect.size(), id->aspRatMode );
            QRectF target( QPointF(0,0), imageSize );
            renderer.render( painter, target );
        }
#endif
    } else {
        if ( id->image.isNull() ) {
            painter->setPen( QPen(Qt::black) );
            painter->setBrush( QBrush(Qt::gray,Qt::BDiagPattern) );
            painter->drawRect( rect );
            painter->setFont( QFont("Arial",8) );
            painter->drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter, QString("IMAGE (%1)").arg( id->text ) );
        } else {
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
//            if ( id->transformMode == Qt::SmoothTransformation ) {
//                QImage im;
//                im = id->image.scaled( rect.size().toSize(), id->aspRatMode, Qt::SmoothTransformation );
//                painter->drawImage( QPointF(0,0), im );
//            } else {
//                painter->drawImage( target, id->image, source );
//            }
            painter->drawImage( target, id->image, source );
#else
            painter->drawPixmap( target, id->image, source );
#endif
        }
    }
    paintSelection( painter );
}

void NCReportImageItem::setDefaultForEditor()
{
    resize( QSizeF( 60.0, 60.0 ) );
}

void NCReportImageItem::adjustSize()
{
}

bool NCReportImageItem::load(NCReportEvaluator *evaluator)
{
    //Static=0, DataSource, File, FileNameFromDataSource, CustomAdded
    NCReportImageData* id = (NCReportImageData*)d;
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
    id->image = QImage();
#else
    id->image = QPixmap();
#endif

    switch (d->sourceType) {
        case Static:
        {
            if ( id->format==Binary || id->format==Base64Encoded ) {
                QString pmBase64String = id->text;
                id->image.loadFromData( QByteArray::fromBase64( pmBase64String.toLatin1() ) );
                id->text.clear();  // we don't waste memory
                break;
            } else if ( id->format==Svg ) {
                // text = SVG xml text, nothing to do
                id->svgXml = id->text.toUtf8();
                id->text.clear(); // we don't waste memory
                break;
            }
        }
        case File:
        case FileNameFromDataSource:
        {
            QString fileName;

            if ( id->sourceType == FileNameFromDataSource )
                fileName = id->valueRef().toString();
            else if ( id->sourceType == File )
                fileName = id->text;

            NCReportUtils::evaluateFileName(fileName);
            if (evaluator && fileName.contains('$'))
                evaluator->evaluate(fileName);

            if (id->format==Base64Encoded) {
                QString b64;
                if ( !NCReportUtils::fileToString(fileName,b64) )
                    return false;
                id->image.loadFromData( QByteArray::fromBase64( b64.toLatin1() ) );
            } else if (id->format==Binary) {
                id->image.load( fileName );
            } else if ( id->format==Svg ) {
                QString svgXmlText;
                if ( NCReportUtils::fileToString(fileName, svgXmlText) )
                    id->svgXml = svgXmlText.toUtf8();
                else
                    return false;
            }
            break;
        }
        case DataSource:
        {
            if (id->format==Base64Encoded)
                id->setValue( QByteArray::fromBase64( id->valueRef().toByteArray()) );
            // HRaba
            if (id->format==Binary || id->format==Base64Encoded ) {
                if (id->valueRef().type() == QVariant::ByteArray)
                    id->image.loadFromData(id->value().toByteArray());
                else {
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
                    id->image = id->value().value<QImage>();
#else
                    id->image = id->value().value<QPixmap>();
#endif
                }
                break;
            } else if ( id->format==Svg ) {
                // text is the svg xml, nothing to do.
                id->svgXml = id->value().toByteArray();
                id->setValue( QVariant() );
            }
        }
        case Url:
        case UrlFromDataSource:
        {
            QString url(id->text);
            if ( id->sourceType == UrlFromDataSource || id->sourceType == Url)
                url = id->valueRef().toString();

            //QMessageBox::information(0,"url",url);
            NCReportHttpClient http;
            http.waitForDownloadContent( url );	//download by url
            id->image.loadFromData( http.content() );
            http.clear();
            break;
        }
        case Parameter:
        {
            QVariant par = reportDef()->parameters().value(id->text);
            if (par.isValid()) {
                switch (par.type()) {
#ifdef USE_QIMAGE_IMDTEAD_OF_QPIXMAP
                case QVariant::Image:
                    id->image = par.value<QImage>(); break;
#else
                case QVariant::Pixmap:
                    id->image = par.value<QPixmap>(); break;
#endif
                case QVariant::String:
                    id->image.load(par.value<QString>()); break;
                default:
                    break;
                }
            }
            break;
        }
    }
    return true;
}

QByteArray NCReportImageItem::toBase64() const
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    ((NCReportImageData*)d)->image.save(&buffer,"PNG");
    return bytes.toBase64();
}

QByteArray NCReportImageItem::toHtml() const
{
    if ( image().isNull() )
        return QString("<!-- Invalid image. Item ID: %1 -->").arg(d->id()).toLatin1();

    const char* img_html_template = "<div style=\"margin-top: 10px; margin-bottom: 10px;\" align=\"{IMG_ALIGN}\"><img src=\"data:image/png;base64,{IMG_BASE64}\" {IMG_WIDTH} {IMG_HEIGHT}/></div>";
    //const char* img_html_template = "<div style=\"margin-top: 10px; margin-bottom: 10px;\" align=\"{IMG_ALIGN}\"><img src=\"{IMG_SOURCE}\" {IMG_WIDTH} {IMG_HEIGHT}/></div>";

    QByteArray align;
    switch (NCReportUtils::horizontalAlignmentToIndex(alignment())) {
    case 0: align="left"; break;
    case 1: align="center"; break;
    case 2: align="right"; break;
    }

    QByteArray bytes(img_html_template);
    bytes.replace("{IMG_ALIGN}",align);
    bytes.replace("{IMG_BASE64}",toBase64());
    //bytes.replace("{IMG_SOURCE}",QString("imgdata://image%1.png").arg(id()).toLatin1());

    if (htmlWidth().isEmpty())
        bytes.replace("{IMG_WIDTH}","");
    else
        bytes.replace("{IMG_WIDTH}","width=\""+htmlWidth()+"\"");

    if (htmlHeight().isEmpty())
        bytes.replace("{IMG_HEIGHT}","");
    else
        bytes.replace("{IMG_HEIGHT}","height=\""+htmlHeight()+"\"");

    return bytes;
}

QPointF NCReportImageItem::imageTargetPoint(const QRectF &itemRect, const QSizeF &imageSize) const
{
    qreal targetX = itemRect.left();
    qreal targetY = itemRect.top();
    QFlags<Qt::AlignmentFlag> f(alignment());

    if (imageSize.width()<itemRect.width()) {

        if (f.testFlag(Qt::AlignHCenter))
            targetX = itemRect.left()+(itemRect.width()-imageSize.width())/2;
        else if (f.testFlag(Qt::AlignRight))
            targetX = itemRect.left()+itemRect.width()-imageSize.width();
    }

    if (imageSize.height()<itemRect.height()) {

        if (f.testFlag(Qt::AlignVCenter))
            targetY = itemRect.top()+(itemRect.height()-imageSize.height())/2;
        else if (f.testFlag(Qt::AlignBottom))
            targetY = itemRect.top()+itemRect.height()-imageSize.height();
    }

    return QPointF(targetX,targetY);
}

void NCReportImageItem::updateValue(NCReportOutput*output, NCReportEvaluator *evaluator)
{
    if (d->sourceType == Static )
        return;

    if ( d->sourceType == File ) {
        if (d->text.contains('$')) {

        } else
            return;
    }

    evaluator->evaluateDynamicImage( this );
    NCReportItem::updateValue(output, evaluator);
}

void NCReportImageItem::updateContent()
{
    load(0);
}

QList<int> NCReportImageItem::availableSourceTypes() const
{
    QList<int> types;
    types << Static << DataSource << FileNameFromDataSource << File << UrlFromDataSource << Url << Parameter;
    return types;
}

void NCReportImageItem::setAspectRatioMode(Qt::AspectRatioMode am)
{
    ((NCReportImageData*)d)->aspRatMode = am;
    setAutoHeight(am==Qt::KeepAspectRatioByExpanding);
}
