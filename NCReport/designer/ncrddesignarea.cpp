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
#include "ncrddesignarea.h"
#include "ncrdmainwindow.h"
#include "ncrddocument.h"
#include "ncrdcommands.h"
#include "ncrdsectionscene.h"
#include "ncrddesignsection.h"
#include "ncreportitem.h"

#include <QCursor>
#include <QMatrix>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>


NCRDDesignArea::NCRDDesignArea( NCRDMainWindow * mwin, NCRDDocument *doc, NCRDDesignSection* parent) : QGraphicsView(parent)
        ,m_scene(0), m_zoomLevel(0), m_zoomValue(1.0f)
{

    m_designArea = this;
    m_designSection = parent;
    m_doc = doc;
    m_mainwin = mwin;

    //setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);
    //setDragMode( QGraphicsView::RubberBandDrag );
    setAlignment( Qt::AlignLeft | Qt::AlignTop );
    //setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Preferred );
    setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    setMinimumHeight( 1 );
    setFocusPolicy( Qt::StrongFocus );
    setFrameStyle( QFrame::NoFrame );

    connect( this, SIGNAL(sectionClickFocus()), doc, SLOT(slotSectionClickFocus()) );

}

NCRDDesignArea::~NCRDDesignArea()
{
}

void NCRDDesignArea::setReportSection( NCRDSectionScene* ca)
{
    if(m_scene == ca)
        return;

    m_scene = ca;
    QGraphicsView::setScene(ca);
}

NCRDDocument* NCRDDesignArea::reportDocument() const
{
    return m_doc;
}

NCRDMainWindow * NCRDDesignArea::mainWindow() const
{
    return m_mainwin;
}

NCRDSectionScene* NCRDDesignArea::sectionScene() const
{
    return m_scene;
}

int NCRDDesignArea::zoomLevel() const
{
    return m_zoomLevel;
}

qreal NCRDDesignArea::zoomScale() const
{
    return m_zoomValue;
}

void NCRDDesignArea::setZoomLevel(int zoomLevel)
{
    if(m_zoomLevel == zoomLevel)
        return;

    // Clip zoom level between -10 and +10.
    if(zoomLevel < -10)
        zoomLevel = -10;
    else if(zoomLevel > 10)
        zoomLevel = 10;

    if(m_zoomLevel == zoomLevel)
        return;

    m_zoomLevel = zoomLevel;
    m_zoomValue = calculateScaleFor(zoomLevel);

    // Set the zoom to the graphics view.
    QApplication::setOverrideCursor(Qt::BusyCursor);
    QMatrix matrix;
    matrix.scale(m_zoomValue, m_zoomValue);
    setMatrix(matrix);
    QApplication::restoreOverrideCursor();

    emit zoomLevelChanged(m_zoomLevel);
    emit zoomScaleChanged(m_zoomValue);
}

void NCRDDesignArea::zoomIn()
{
    setZoomLevel(m_zoomLevel+1);
}

void NCRDDesignArea::zoomOut()
{
    if ( m_zoomLevel > 0 )
        setZoomLevel(m_zoomLevel-1);
}

void NCRDDesignArea::zoomFull()
{
    setZoomLevel(10);
}

void NCRDDesignArea::zoomOne()
{
    setZoomLevel(0);
}

void NCRDDesignArea::zoomFit()
{
    QRectF iRect = m_scene->itemsBoundingRect();
    fitInView(iRect, Qt::KeepAspectRatio);

    qreal scale = matrix().m11();
    m_zoomLevel = calculateZoomLevelFor(scale);

    emit zoomLevelChanged(m_zoomLevel);
    emit zoomScaleChanged(scale);
}

void NCRDDesignArea::mouseMoveEvent(QMouseEvent* e)
{
    emit mouseMoved( e->pos() );
    if ( m_mainwin->currentToolState() == NCRDMainWindow::Tool ) {
        setDragMode( QGraphicsView::NoDrag );
        if ( viewport()->cursor().shape() != Qt::CrossCursor ) {
            viewport()->setCursor( QCursor(Qt::CrossCursor) );
        }
        if ( e->buttons() == Qt::NoButton )
            return;
    } else {
        //qDebug("e->button()=%i, shape=%i", e->button(), viewport()->cursor().shape() );
        if ( e->buttons() == Qt::LeftButton &&
                viewport()->cursor().shape() != Qt::ArrowCursor &&
                viewport()->cursor().shape() != Qt::CrossCursor) {

            // resize item is pending
        } else {
            setDragMode( QGraphicsView::RubberBandDrag );
            if ( viewport()->cursor().shape() != Qt::ArrowCursor ) {
                viewport()->setCursor( QCursor(Qt::ArrowCursor) );
            }
        }
    }
    QGraphicsView::mouseMoveEvent(e);
}

void NCRDDesignArea::mouseReleaseEvent(QMouseEvent* e)
{
    QGraphicsView::mouseReleaseEvent(e);
}

void NCRDDesignArea::contextMenuEvent(QContextMenuEvent * e)
{
    QGraphicsView::contextMenuEvent(e);
}


void NCRDDesignArea::resizeEvent(QResizeEvent* re)
{
    //if ( scene() )
    //	scene()->setSceneRect( QRectF(0.0,0.0,re->size().width(),re->size().height() ) );
    QGraphicsView::resizeEvent(re);
}

void NCRDDesignArea::wheelEvent(QWheelEvent *)
{
    //QGraphicsView::wheelEvent(e);
}

void NCRDDesignArea::drawBackground(QPainter* paint, const QRectF & rect)
{
    QGraphicsView::drawBackground( paint, rect);
}

void NCRDDesignArea::focusInEvent(QFocusEvent *e)
{
    //qDebug("NCRDDesignArea focused %p", this );
    QGraphicsView::focusInEvent(e);
    m_doc->setCurrentDesignSection( m_designSection );
    m_doc->setCurrentScene( m_scene );

#ifdef NCREPORT_DEBUG_OUTPUT
    qDebug("focusInEvent %i", e->reason() );
#endif
    if ( e->reason() == Qt::MouseFocusReason )
        emit sectionClickFocus();

}

qreal NCRDDesignArea::calculateScaleFor(int zoomLevel) const
{
    if(!m_scene)
        return 1.0f;

    qreal canvasWidth = m_scene->width();
    qreal canvasHeight = m_scene->height();
    qreal canvasSize = canvasWidth > canvasHeight ? canvasWidth : canvasHeight;
    qreal viewSize = canvasWidth > canvasHeight ?
                        qreal(m_designArea->viewport()->width()) :
                        qreal(m_designArea->viewport()->height());

    // At zoom level -10 canvasSize must become viewSize;
    qreal zoomOutFactor = (viewSize*0.95f / canvasSize);
    // At zoom level +10 canvasSize must be 3 times its size.
    qreal zoomInFactor = 3.0f / 10.0f;
    // Calculate the zoom
    qreal zoom = 1.0f;
    if(zoomLevel < 0)
        zoom = 1.0f + qreal(zoomLevel) * (1.0f - zoomOutFactor) / 10.0f;
    else
        zoom = 1.0f + zoomInFactor * qreal(zoomLevel);

    return zoom;
}

int NCRDDesignArea::calculateZoomLevelFor(qreal scale) const
{
    if(!m_scene)
        return 0;

    qreal canvasWidth = m_scene->width();
    qreal canvasHeight = m_scene->height();
    qreal canvasSize = canvasWidth > canvasHeight ? canvasWidth : canvasHeight;
    qreal viewSize = canvasWidth > canvasHeight ?
                        qreal(m_designArea->viewport()->width()) :
                        qreal(m_designArea->viewport()->height());

    // At zoom level -10 canvasSize must become viewSize;
    qreal zoomOutFactor = (viewSize*0.95f / canvasSize);
    // At zoom level +10 canvasSize must be 3 times its size.
    qreal zoomInFactor = 3.0f / 10.0f;
    qreal zoomLevel = 1.0f;
    if(scale < 1)
        zoomLevel = (scale - 1.0f) * 10.0f / (1.0f - zoomOutFactor);
    else
        zoomLevel = (scale - 1.0f) / zoomInFactor;

    return int(zoomLevel);
}
