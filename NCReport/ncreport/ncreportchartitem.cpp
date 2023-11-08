/****************************************************************************
*
*  Copyright (C) 2002-2012 Helta Kft. / NociSoft Software Solutions
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
#ifdef HIGHCHARTS_INTEGRATION

#include "ncreportchartitem.h"
#include "ncreportoutput.h"
#include "ncreportdatasource.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportdef.h"
#include "ncreportdirector.h"
#include "ncreporthighchartsmanager.h"

#include <QPainter>
#include <QScriptEngine>
#include <QSvgRenderer>
#include <QEventLoop>
#include <QTimer>

#define CHART_RUN_TIMEOUT	15000

NCReportChartItem::NCReportChartItem( NCReportDef* rdef, QGraphicsItem * parent ) : NCReportItem(parent)
{
    d = new NCReportChartItemData;
    d->objectType = Chart;
    d->tagname = "chart";
    d->reportDef = rdef;
    manager = new NCReportHighChartsManager;
    manager->setReportDef(rdef);
    //eventLoop = new QEventLoop;
}

NCReportChartItem::~NCReportChartItem()
{
    delete manager;
}

bool NCReportChartItem::read(NCReportXMLReader *r )
{
    NCReportItem::read( r );
    if ( r->isAttrib("template") )
        setTemplateFile( r->attribToString("template") );
    //if ( r->isAttrib("result") )
    //	setResultFile( r->attribToString("result") );
    setText(r->readElementText());
    return true;
}

bool NCReportChartItem::write(NCReportXMLWriter *w)
{
    w->writeStartElement( d->tagname );
    NCReportItem::write(w);
    w->writeAttribute("template", templateFile() );
    //w->writeAttribute("result", resultFile() );

    w->writeCharacters( text() );
    w->writeEndElement();
    return true;
}

void NCReportChartItem::setDefaultForEditor()
{
    resize( QSizeF(400,300) );
}

void NCReportChartItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    //QRectF rect(boundingRect());
    QRectF rect( QPointF(0, 0), currentSize() );
    QPen pen(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter->setPen( pen );
    bool hasSvg = manager->chartSvg().startsWith("<svg");

    if (!manager->error() && hasSvg ) {
        render( painter, rect );	// render as svg
        painter->drawRect( rect );
    } else {
        QString txt = QObject::tr("HIGHCHARTS OBJECT")+"\n"+manager->errorMsg()+ ( hasSvg ? QString() : manager->chartSvg().prepend("\n"));
        painter->setBrush(QBrush(Qt::lightGray));
        painter->drawRect(rect);
        painter->drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextWordWrap, txt );
    }

    paintSelection( painter );
}

void NCReportChartItem::paint(NCReportOutput *o, const QPointF & mPos )
{
    //NCReportBarcodeData* bd = (NCReportBarcodeData*)d;
    QRect rect( o->scale().mmToPixel( mPos.x() + metricPos().x() ),
             o->scale().mmToPixel( mPos.y() + metricPos().y() ),
             o->scale().mmToPixel( metricSize().width() ),
             o->scale().mmToPixel( metricSize().height() ) );

    switch ( o->output() ) {
        case NCReportOutput::Printer:
        case NCReportOutput::Pdf:
            render( o->painter(), rect, false);	//image mode
            break;
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



void NCReportChartItem::dummyPaint(NCReportOutput *output, const QPointF &point)
{
    NCReportItem::dummyPaint(output,point);
}

bool NCReportChartItem::isEmpty() const
{
    return manager->chartSvg().isEmpty();
}

void NCReportChartItem::updateValue(NCReportOutput*, NCReportEvaluator *)
{
    if (!updateChart(text(),manager)) {
        return;
    }

    // Execute the event loop here, now we will wait here until chartIsReady() signal is emitted
    // which in turn will trigger event loop quit.
    QEventLoop eventLoop;
    QTimer timer;
    QObject::connect(manager, SIGNAL(chartIsReady()), &eventLoop, SLOT(quit()));
    QObject::connect(&timer, SIGNAL(timeout()), &eventLoop, SLOT(quit()));
    QObject::connect(&timer, SIGNAL(timeout()), manager, SLOT(timeoutError()));
    timer.start(CHART_RUN_TIMEOUT);
    eventLoop.exec();
}

void NCReportChartItem::updateForEditor()
{
    NCReportItem::updateForEditor();
    updateValue(0,0);
}

QString NCReportChartItem::templateFile() const
{
    return manager->templateFile();
}

void NCReportChartItem::setTemplateFile(const QString &fileName)
{
    manager->setTemplateFile(fileName);
}

QString NCReportChartItem::resultFile() const
{
    return manager->resultFile();
}

void NCReportChartItem::setResultFile(const QString &fileName)
{
    manager->setResultFile(fileName);
}

void NCReportChartItem::adjustSize(NCReportOutput*)
{
}

void NCReportChartItem::adjustHeight(NCReportOutput*)
{
}


QRectF NCReportChartItem::outputRect(NCReportOutput*) const
{
    return QRectF();
}

QRectF NCReportChartItem::boundingRect() const
{
    return NCReportItem::boundingRect();
}

NCReportHighChartsManager *NCReportChartItem::highChartsManager()
{
    return manager;
}

QPixmap NCReportChartItem::pixmap() const
{
    NCReportScale s;    // screen scale
    //QRect rect( 0, 0, s.mmToPixel( metricSize().width() ), s.mmToPixel( metricSize().height() ) );
    //QRect rect( 0, 0, s.mmToPixel( metricSize().width() ), s.mmToPixel( metricSize().height() ) );

    QSvgRenderer svgrenderer;
    svgrenderer.load( manager->chartSvg() );

    QSizeF size(svgrenderer.defaultSize());
    size.scale( paintRect().size(), Qt::KeepAspectRatio );
    QSize pixmapSize = size.toSize() + QSize(1,1);

    QPixmap pm(pixmapSize);
    pm.fill( Qt::white );
    QRectF source( QPointF(0,0), size );
    QPainter p(&pm);
    svgrenderer.render( &p, source );

    return pm;
}

bool NCReportChartItem::updateChart( const QString& chartScript, NCReportHighChartsManager *hcm )
{
    hcm->setError("");
    QScriptEngine engine;
    QScriptValue global;

    if (!hcm->parseReportData(engine, global, chartScript))
        return false;

    QString dedicatedDataSource = global.property("reportData").property("resultSet").property("datasource").toString();
    QScriptValue seriesList = global.property("reportData").property("resultSet").property("seriesList");

    if (dedicatedDataSource.isEmpty()) {
        // No dedicated data source is defined.
        if (!hcm->evaluateDataSource( seriesList ))
            return false;
    } else {
        // We build own seriesList
        QScriptValue newSeriesList = engine.newArray();

        if (!hcm->evaluateDedicatedDataSource(dedicatedDataSource, engine, seriesList, newSeriesList ))
            return false;
        global.property("reportData").property("resultSet").setProperty("seriesList",newSeriesList);
    }

    QString updatedReportData = hcm->objectToScript( global.property("reportData"), 0, false, "reportData" );

    hcm->loadTemplate(NCReportHighChartsManager::WriteMode, updatedReportData);
    hcm->startToLoadChart();

    return true;
}

void NCReportChartItem::render( QPainter* painter, const QRectF& rect, bool directMode )
{
    if (manager->error())
        return;

    QSvgRenderer svgrenderer;
    svgrenderer.load( manager->chartSvg() );
    bool hasSvg = manager->chartSvg().startsWith("<svg");
    qDebug("Has svg: %i",hasSvg);

    if (!hasSvg)
        return;

    QRectF r(rect);
    QSizeF size = svgrenderer.defaultSize();
    size.scale( r.size(), Qt::KeepAspectRatio );
    r.setSize(size);

    if (directMode) {
        // RENDER AS DIRECT SVG
        svgrenderer.render( painter, r );
    } else {
        // RENDER AS INDIRECT SVG
        QSize pixmapSize = size.toSize()/4;
        pixmapSize += QSize(1,1);
        QPixmap pm(pixmapSize);
        pm.fill( Qt::white );
        QRectF source( QPointF(0,0), size/4 );
        QRectF target( rect.topLeft(), size );

        QPainter p(&pm);
        svgrenderer.render( &p, source );

        painter->drawPixmap( target, pm, source );

        // RENDER AS IMAGE
        // IT NEEDS BECAUSE OF HIGH RESOLUTION SVG RENDERING BUG
//		QPixmap pm;
//		pm.loadFromData( manager->chartSvg() );

//		QSize imageSize(pm.size());
//		QSizeF imageTargetSize( imageSize );
//		imageTargetSize.scale( rect.size(), Qt::KeepAspectRatio );
//		QRectF source( QPointF(0,0), imageSize );
//		QRectF target( rect.topLeft(), imageTargetSize );

//		painter->drawPixmap( target, pm, source );
    }
}

#endif // HIGHCHARTS_INTEGRATION
