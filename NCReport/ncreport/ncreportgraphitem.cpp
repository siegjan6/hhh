/****************************************************************************
*
*  Copyright (C) 2002-2014 Helta Kft. / NociSoft Software Solutions
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
#include "ncreportgraphitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportoutput.h"
#include "ncreportdef.h"
#include "ncreportabstractitemrendering.h"
#include "ncreportevaluator.h"

#include <QPainter>

//----------------------------------------------------------------------------------------

NCReportGraphItem::NCReportGraphItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem( parent )
{
    NCReportGraphData* gd = new NCReportGraphData;
    d = gd;
    d->reportDef = rdef;
    d->objectType = Graph;
    d->tagname = "graph";
    gd->classID = "none";
}

NCReportGraphItem::~ NCReportGraphItem()
{
}

bool NCReportGraphItem::read(NCReportXMLReader *reader )
{
    NCReportItem::read( reader );
    NCReportGraphData* gd = (NCReportGraphData*)d;

    if ( reader->isAttrib("classID") )
        gd->classID = reader->attribToString("classID");

    if ( reader->isAttrib("hasParameters") && reader->attribToBool("hasParameters") ) {
        // NEW WAY
        reader->readParameters(parametersRef());
    } else {
        // ORIGINAL WAY - for backward compatibility
        switch ( gd->sourceType ) {
            case Static:
                gd->graphDefinition = reader->readElementText();
                break;
            case DataSource: //dynamic pixmap from idsource
                gd->text = reader->readElementText();
                break;
            case File:
            case FileNameFromDataSource:
            default:
                break;
        }
    }

    return true;
}

bool NCReportGraphItem::write(NCReportXMLWriter *writer)
{
    NCReportGraphData* gd = (NCReportGraphData*)d;

    writer->writeStartElement( d->tagname );
    NCReportItem::write(writer);

    writer->writeAttribute("classID",gd->classID);
    if (isParameterMode()) {
        // NEW WAY
        writer->writeAttribute("hasParameters","true");
        writer->writeParameters(parametersRef());
    } else {
        // OLD WAY - for backward compatibility
        switch ( d->sourceType ) {
            case Static:
                writer->writeCharacters( gd->graphDefinition );
                break;
            case DataSource: //dynamic pixmap from datasource
                writer->writeCharacters( gd->text );
                break;
            case File:
            case FileNameFromDataSource:
            default:
                break;
        }
    }

    writer->writeEndElement();

    return true;
}

void NCReportGraphItem::paint( NCReportOutput* o, const QPointF& mPos )
{
    QSizeF graphSize( o->scale().mmToSize( d->mSize ) );
    QRectF rect( o->scale().mmToPoint( mPos + d->mPos ), graphSize );
    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
        case NCReportOutput::Svg:
        case NCReportOutput::Image:
        case NCReportOutput::Preview:
        case NCReportOutput::QtPreview:
        {
            NCReportGraphRenderer *_renderer = renderer();
            if ( _renderer ) {
                _renderer->setItem(this);
                _renderer->setDataSource( reportDef()->dataSource( _renderer->dataSourceID() ) );
                _renderer->paintItem( o->painter(), o, rect, graphDefinition() );
            }
        }
        default:
            break;
    }
    o->renderItem( this, rect );
}

void NCReportGraphItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rect( QPointF(0, 0), currentSize() );

    NCReportGraphRenderer *_renderer = renderer();

    if ( _renderer ) {
        _renderer->setItem(this);
        _renderer->paintEditorItem(painter,rect,text());
    } else {
        paintEditor(painter,rect);
    }

    paintSelection( painter );
}

void NCReportGraphItem::paintEditor(QPainter *painter, const QRectF &rect)
{
    const int numcols = 10;
    const int cw = qRound(rect.width()/numcols);
    painter->setPen( Qt::NoPen );
    int ch=0;
    QColor color;
    color.setAlpha( 128 );
    for ( int i=0; i<numcols; ++i ) {
        if ( i%3 == 0 ) {
            color.setRgb(0xAAAAFF);
            ch = qRound(rect.height()*0.8);
        } else if ( i%2 == 0 ) {
            color.setRgb(0xAAFFAA);
            ch = qRound(rect.height()*0.4);
        } else {
            color.setRgb(0xFFAAAA);
            ch = qRound(rect.height()*0.6);
        }
        painter->setBrush( QBrush(color) );
        painter->drawRect( i*cw, qRound(rect.height())-ch , cw, ch );
    }

    painter->setPen( QPen(Qt::black) );
    painter->setBrush( Qt::NoBrush );

    painter->drawRect( rect );

    painter->setFont( QFont("Arial",8) );
    painter->drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap,
                        QObject::tr("CUSTOM DRAWING (%1)").arg( text() ) );
}

void NCReportGraphItem::setDefaultForEditor()
{
    resize( QSizeF( 30.0, 30.0 ) );
}

void NCReportGraphItem::adjustSize()
{
}

void NCReportGraphItem::updateValue(NCReportOutput *output, NCReportEvaluator *evaluator)
{
    if (isParameterMode()) {
        QHash<QString, QVariant>::iterator i = parametersRef().begin();
        while (i != parametersRef().end()) {

            if (i.value().type() == QVariant::String ) {
                QString stringValue = i.value().toString();
                if (stringValue.contains("$")) {
                    // replace value with evaluated one.
                    evaluator->evaluate(stringValue);
                    i.value().setValue(stringValue);
                }
            }
            ++i;
        }

    } else {
        if (d->sourceType == Static || d->sourceType == File )
            return;
        evaluator->evaluateDynamicGraph( this );
    }

    NCReportItem::updateValue(output, evaluator);
}

bool NCReportGraphItem::isDynamic() const
{
    bool is = NCReportItem::isDynamic() || isParameterMode();
    return is;
}

bool NCReportGraphItem::isParameterMode() const
{
    return !parameters().isEmpty();
}

NCReportGraphRenderer *NCReportGraphItem::renderer()
{
    NCReportGraphRenderer *renderer = 0;
    renderer = d->reportDef->itemRenderers().value( QString::number(typeId()) );    // lookup by type id
    if (!renderer && !classID().isEmpty() )
        renderer = d->reportDef->itemRenderers().value(classID());  // lookup by class id cause of backward compatibility

    qDebug("renderer(): %p",renderer);
    return renderer;
}

void NCReportGraphItem::adjustSize(NCReportOutput* output)
{
    if ( !isAutoHeight() )
        return;

    if ( isAdjusted() )
        return;

    NCReportGraphData* gd = (NCReportGraphData*)d;
    NCReportGraphRenderer *_renderer = renderer();
    if ( !_renderer )
        return;

    d->mRealSize = _renderer->calculateSizeMM(output, d->mSize, gd->graphDefinition);
    if (!d->mRealSize.isValid())
    {
        d->mRealSize = _renderer->calculateSizeMM(output, d->mSize);
        if (!d->mRealSize.isValid())
            d->mRealSize = QSizeF( d->mSize.width(), _renderer->calculateHeightMM(output) );
    }
    if ( output->output() != NCReportOutput::Null )
        setAdjusted(true);
}
