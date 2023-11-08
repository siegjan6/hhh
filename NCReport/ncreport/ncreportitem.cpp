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
#include "ncreportitem.h"
#include "ncreportxmlreader.h"
#include "ncreportxmlwriter.h"
#include "ncreportscale.h"
#include "ncreportdata.h"
#include "ncreportsection.h"
#include "ncreportdef.h"
#include "ncreportoutput.h"
#include "ncreportuserfunction.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#ifndef NCREPORT_NO_CURSOR
#include <QCursor>
#endif

#define SELECTION_BOX_COLOR				Qt::green
#define SELECTION_BOX_LOCKEDCOLOR		Qt::red

NCReportItem::NCReportItem( QGraphicsItem * parent) : QGraphicsItem( parent )
{
    //qDebug("Create item."),
    d = 0;
#if (QT_VERSION >= 0x050000)
    setAcceptHoverEvents(true);
#else
    setAcceptsHoverEvents(true);
#endif

    m_selectionHoverState = None;
    //d->currentSize = QSizeF(0,0);
    //m_isMoving = false;
    setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable );
#if QT_VERSION >= 0x040600
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
#endif
}

NCReportItem::~ NCReportItem()
{
    if (d) {
#ifdef NCREPORT_DEBUG_OUTPUT
        qDebug("Delete item data: %s", qPrintable(d->tagname) );
#endif
        delete d;
        d = 0;
    }
}

bool NCReportItem::read(NCReportXMLReader *r)
{
    Q_ASSERT(r->isStartElement() && r->name() == d->tagname );

    if ( r->isAttrib("id") )
        setId( r->attribToString("id") );
    if ( r->isAttrib("typeId") )
        setTypeId( r->attribToInt("typeId") );
    if ( r->isAttrib("zValue") )
        setZValue( r->attribToInt("zValue") );
    if ( r->isAttrib("printWhen") )
        setPrintWhen(r->attribToString("printWhen"));
    if ( r->isAttrib("htmlOptions"))
        setHtmlOptions( r->attribToString("htmlOptions"));

    if ( r->isAttrib("resource") ) {
        QStringRef s = r->attrib("resource");
        if ( s=="static" )
            d->sourceType = Static;
        else if ( s=="datasource" )
            d->sourceType = DataSource;
        else if ( s=="file" )
            d->sourceType = File;
        else if ( s=="filefromds" )
            d->sourceType = FileNameFromDataSource;
        else if ( s=="param" )
            d->sourceType = Parameter;
        else if ( s=="url" )
            d->sourceType = Url;
        else if ( s=="urlfromds" )
            d->sourceType = UrlFromDataSource;
        else if ( s=="custom" || s=="internal")
            d->sourceType = InternalDocument;
        else if ( s=="var" )
            d->sourceType = Variable;
        else if ( s=="sysvar" )
            d->sourceType = SystemVariable;
        else if ( s=="exp" )
            d->sourceType = ScriptExpression;
        else if ( s=="tpl" ) {
            d->sourceType = Template;
            d->templateMode = true;
        }
        else if ( s=="tdatasource" ) {
            // backward compatibility
            d->sourceType = DataSource;
            d->templateMode = true;
        }
        else if ( s=="tfile" ) {
            // backward compatibility
            d->sourceType = File;
            d->templateMode = true;
        }
        else if ( s=="tfilefromds" ) {
            // backward compatibility
            d->sourceType = FileNameFromDataSource;
            d->templateMode = true;
        }
    }

    if ( r->isAttrib("description") )
        d->description = r->attribToString("description");
    if ( r->isAttrib("template") )
        d->templateMode = r->attribToBool("template");
    if ( r->isAttrib("autoHeight") )
        d->autoHeight = r->attribToBool("autoHeight");
    if ( r->isAttrib("zoneID") )
        d->zoneID = r->attribToInt("zoneID");
    if ( r->isAttrib("canBreak") )
        d->breakToPage = r->attribToBool("canBreak");
    if ( r->isAttrib("pinToLeft") )
        d->pinToLeft = r->attribToBool("pinToLeft");
    if ( r->isAttrib("pinToRight") )
        d->pinToRight = r->attribToBool("pinToRight");
    if ( r->isAttrib("funcID") )
        d->functionId = r->attribToString("funcID");


    if (hasSizeAndPosition()) {
        setMetricPos(r->readPos());
        setMetricSize(r->readSize());
    }
    return true;
}

bool NCReportItem::write(NCReportXMLWriter *w)
{
    if (id()=="0" || id().isEmpty())
        setId("%");

    w->writeAttribute("id", id() );
    if (typeId()>0)
        w->writeAttribute("typeId", QString::number(typeId()) );
    w->writeAttribute("zValue", QString::number(zValue()) );

    if (hasSizeAndPosition()) {
        w->writePos( metricPos() );
        w->writeSize( metricSize() );
    }
    if ( !d->description.isEmpty() )
        w->writeAttribute("description", d->description );
    if ( !d->printWhen.isEmpty() )
        w->writeAttribute("printWhen", d->printWhen );
    if ( d->zoneID != 0 )
        w->writeAttribute("zoneID", QString::number(d->zoneID) );
    if ( !htmlOptions().isEmpty() )
        w->writeAttribute("htmlOptions", htmlOptions() );

    QString sist("static");
    switch (d->sourceType) {
    case Static: sist="static"; break;
    case DataSource: sist="datasource"; break;
    case File: sist="file"; break;
    case FileNameFromDataSource: sist="filefromds"; break;
    case Parameter: sist="param"; break;
    case Url: sist="url"; break;
    case UrlFromDataSource: sist="urlfromds"; break;
    case InternalDocument: sist="internal"; break;
    case Variable: sist="var"; break;
    case SystemVariable: sist="sysvar"; break;
    case ScriptExpression: sist="exp"; break;
    case Template: sist="tpl"; break;
    }
    if (d->sourceType!=Static)
        w->writeAttribute( "resource", sist );

    if ( d->templateMode )
        w->writeAttribute("template", "true" );
    if ( d->autoHeight )
        w->writeAttribute("autoHeight", "true" );
    if ( d->breakToPage )
        w->writeAttribute("canBreak","true");
    if ( d->pinToLeft )
        w->writeAttribute("pinToLeft","true");
    if ( d->pinToRight )
        w->writeAttribute("pinToRight","true");
    if (!d->functionId.isEmpty())
        w->writeAttribute("funcID",d->functionId);

    return true;
}

QSizeF NCReportItem::currentSize() const
{
    return d->currentSize;
}

//const QSizeF NCReportItem::lastSize() const
//{
//	return QSizeF();
//}

void NCReportItem::setSnapPos(const QPointF & point )
{
    QPointF newPos = sectionScene()->snapToPoint( point ); 	// snap to grid. value is the new position.
    setPos( newPos );
}

void NCReportItem::resize(const QSizeF & newsize )
{
    if ( newsize != d->currentSize ) {
        prepareGeometryChange();
        d->currentSize = newsize;

        if ( d->currentSize.width() < sizeHint().width() )
            d->currentSize.setWidth( sizeHint().width() );
        if ( d->currentSize.height() < sizeHint().height() )
            d->currentSize.setHeight( sizeHint().height() );

        NCReportScale sc;
        d->mSize = QSizeF( sc.pixelToMM( newsize.width() ), sc.pixelToMM( newsize.height() ) );
        update();
    }
}

void NCReportItem::resizeInMM(const QSizeF & newsize )
{
    if (!d)
        return;
    //if ( newsize == d->mSize )
    //	return;

    NCReportScale sc;
    prepareGeometryChange();
    //d-> newsize;
    d->currentSize = sc.mmToSize( newsize );
    update();
}

QRectF NCReportItem::boundingRect() const
{
    return shape().boundingRect();
    //QRectF r( 0, 0, d->currentSize.width(), d->currentSize.height() );
    //QRectF r( -SELECTION_BOXSIZE/2, -SELECTION_BOXSIZE/2, d->currentSize.width()+SELECTION_BOXSIZE, d->currentSize.height()+SELECTION_BOXSIZE );
    //return QRectF( 0, 0, d->currentSize.width()+SELECTION_BOXSIZE, d->currentSize.height()+SELECTION_BOXSIZE );
}

QRectF NCReportItem::itemRect(NCReportOutput *output, const QPointF &painterPosMM)
{
    QRectF rect = QRectF( output->scale().mmToPoint( painterPosMM + metricPos() ), output->scale().mmToSize( realMetricSize() ) );
    if (d->reportDef) {
        if (d->pinToLeft)
            rect.setLeft( output->scale().mmToRealPixel( painterPosMM.x() ) );
        if (d->pinToRight)
            rect.setRight( output->scale().mmToRealPixel(d->reportDef->option().pageWidthMM() - d->reportDef->option().rightMarginMM()) );
    }
    return rect;
}

QVariant NCReportItem::itemChange(GraphicsItemChange change, const QVariant & value)
{
    if (change == ItemPositionChange && sectionScene() ) {

        if ( isLocked() )
            return QVariant( pos() );

        QPointF p = value.toPointF();
        QPointF newPos = sectionScene()->snapToPoint( p ); 	// snap to grid. value is the new position.

        //QRectF rect = scene()->sceneRect();
        //if (!rect.contains(newPos)) {
        //	// Keep the item inside the scene rect.
        //	newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
        //	newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
        //}

        itemPositionChange( newPos );
        sectionScene()->emitContentChanged();
        sectionScene()->emitItemChanged( this );
        return newPos;
    } else if ( change == ItemSelectedChange && sectionScene() ) {
        // save current (click) position
        m_selectPos = scenePos();

    }
    //QVariant dv = QGraphicsItem::itemChange(change, value);
    //if ( sectionScene() )
    //	sectionScene()->emitItemChanged( this );

    return QGraphicsItem::itemChange(change, value);
}

void NCReportItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if ( !isSelected() || m_selectionHoverState == None || isLocked() )  {
        QGraphicsItem::mouseMoveEvent(event);
        return;
    }

    // RESIZE ITEM
    NCReportSection *sect = sectionScene();
    qreal event_x = event->pos().x();
    qreal event_y = event->pos().y();
    qreal width = currentSize().width();
    qreal height = currentSize().height();

    switch ( m_selectionHoverState ) {
        case TopLeft: {
            if ( event_x > width ) event_x = width;
            if ( event_y > height ) event_y = height;
            qreal newWidth = sect->snapToX( width - event_x );
            qreal newHeight = sect->snapToY( height - event_y );
            resize( QSizeF(newWidth, newHeight) );
            moveBy( width-newWidth , height-newHeight );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case Top: {
            if ( event_y > height ) event_y = height;
            qreal newHeight = sect->snapToY( height - event_y );
            resize( QSizeF(width,newHeight) );
            moveBy( 0, height-newHeight );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case TopRight: {
            if ( event_x<0.0 ) event_x = 0.0;
            if ( event_y > height ) event_y = height;
            qreal newWidth = sect->snapToX( event_x );
            qreal newHeight = sect->snapToY( height - event_y );
            resize( QSizeF(newWidth,newHeight) );
            moveBy( 0, height-newHeight );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case Right: {

            if ( event_x<0.0 ) event_x = 0.0;
            event_x = sect->snapToX( event_x );
            resize( QSizeF(event_x, height) );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case BottomRight: {
            if ( event_x<0.0 ) event_x = 0.0;
            if ( event_y<0.0 ) event_y = 0.0;
            event_x = sect->snapToX( event_x );
            event_y = sect->snapToY( event_y );
            resize( QSizeF(event_x, event_y) );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case Bottom: {

            if ( event_y<0.0 ) event_y = 0.0;
            event_y = sect->snapToY( event_y );
            resize( QSizeF( width, event_y) );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case BottomLeft: {
            if ( event_x > width ) event_x = width;
            if ( event_y<0.0 ) event_y = 0.0;
            qreal newWidth = sect->snapToX( width - event_x );
            qreal newHeight = sect->snapToY( event_y );
            resize( QSizeF(newWidth, newHeight) );
            moveBy( width-newWidth, 0 );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case Left: {
            if ( event_x > width ) event_x = width;
            qreal newWidth = sect->snapToX( width - event_x );
            resize( QSizeF(newWidth, height) );
            moveBy( width-newWidth , 0 );
            sect->emitContentChanged();
            sect->emitItemChanged( this );
            break;
        }
        case LinePoint1:
        case LinePoint2:
            break;
        case None:
            break;
    }

}

void NCReportItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void NCReportItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    QGraphicsItem::mouseReleaseEvent(event);
}

void NCReportItem::paintSelection(QPainter * painter)
{
    if ( !isSelected() )
        return;

    QRectF r( 0, 0, d->currentSize.width(), d->currentSize.height() );
    //painter->setPen( Qt::NoPen );
    preparePaintSelection(painter);
    SelectionDirection directions[] = { TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
    const int numDirections = 8;

    bool shortWidth = ( d->currentSize.width() < SELECTION_BOXSIZE*3 );
    bool shortHeight = ( d->currentSize.height() < SELECTION_BOXSIZE*3 );

    for ( int i = 0; i < numDirections; ++i ) {
        //painter->fillRect( selectionRect(directions[i],size), Qt::green );
        if ( shortHeight && ( directions[i] == Left || directions[i] == Right ) )
            continue;
        if ( shortWidth && ( directions[i] == Top || directions[i] == Bottom ) )
            continue;

        painter->drawRect( selectionRect(directions[i],d->currentSize) );
    }
}

void NCReportItem::preparePaintSelection(QPainter * painter)
{
    painter->setPen( QPen( Qt::black ) );
    painter->setBrush( QBrush( isLocked() ? SELECTION_BOX_LOCKEDCOLOR : SELECTION_BOX_COLOR ) );
    painter->setOpacity( 0.5 );
}

QRectF NCReportItem::selectionRect(const SelectionDirection direction, const QSizeF& itemsize ) const
{
    QRectF r;
    switch ( direction ) {
        case TopLeft: r = QRectF( -SELECTION_BOXSIZE/2, -SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case Top: r = QRectF( itemsize.width()/2-SELECTION_BOXSIZE/2, -SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case TopRight: r = QRectF( itemsize.width()-SELECTION_BOXSIZE/2, -SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case Right: r = QRectF( itemsize.width()-SELECTION_BOXSIZE/2, itemsize.height()/2-SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case BottomRight: r = QRectF( itemsize.width()-SELECTION_BOXSIZE/2, itemsize.height()-SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case Bottom: r = QRectF( itemsize.width()/2-SELECTION_BOXSIZE/2, itemsize.height()-SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case BottomLeft: r = QRectF( -SELECTION_BOXSIZE/2, itemsize.height()-SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case Left: r = QRectF( -SELECTION_BOXSIZE/2, itemsize.height()/2-SELECTION_BOXSIZE/2, SELECTION_BOXSIZE, SELECTION_BOXSIZE ); break;
        case LinePoint1:
        case LinePoint2:
        case None:
            break;
    }

    return r;
}

void NCReportItem::updateCursor(const SelectionDirection dir)
{
#ifndef NCREPORT_NO_CURSOR
    if ( !isSelected() ) {
        setCursor( QCursor(Qt::ArrowCursor) );
        return;
    }

    switch ( dir ) {
        case TopLeft:
            setCursor( QCursor(Qt::SizeFDiagCursor) );
            break;
        case Top:
            setCursor( QCursor(Qt::SizeVerCursor) );
            break;
        case TopRight:
            setCursor( QCursor(Qt::SizeBDiagCursor) );
            break;
        case Right:
            setCursor( QCursor(Qt::SizeHorCursor) );
            break;
        case BottomRight:
            setCursor( QCursor(Qt::SizeFDiagCursor) );
            break;
        case Bottom:
            setCursor( QCursor(Qt::SizeVerCursor) );
            break;
        case BottomLeft:
            setCursor( QCursor(Qt::SizeBDiagCursor) );
            break;
        case Left:
            setCursor( QCursor(Qt::SizeHorCursor) );
            break;
        case LinePoint1:
        case LinePoint2:
            setCursor( QCursor(Qt::CrossCursor) );
        case None:
            setCursor( QCursor(Qt::ArrowCursor) );
            break;
    }
#endif
}

void NCReportItem::hoverMoveEvent(QGraphicsSceneHoverEvent * event)
{
/*	if ( cursor().shape() != Qt::ArrowCursor )
        return;*/

    if ( !isSelected() ) {
        m_selectionHoverState = None;
#ifndef NCREPORT_NO_CURSOR
        setCursor( QCursor(Qt::ArrowCursor) );
#endif
        return;
    }
    m_selectionHoverState = None;
    SelectionDirection directions[] = { TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left, LinePoint1, LinePoint2 };
    const int numDirections = 10;
    for ( int i = 0; i < numDirections; ++i ) {
        if ( selectionRect(directions[i],d->currentSize).contains(event->pos()) ) {
            m_selectionHoverState = directions[i];
            updateCursor(directions[i]);
        }
    }

#ifndef NCREPORT_NO_CURSOR
    if ( m_selectionHoverState == None )
        setCursor( QCursor(Qt::ArrowCursor) );
#endif
}

NCReportItem::SelectionDirection NCReportItem::selectionHoverState() const
{
    return m_selectionHoverState;
}

QSizeF NCReportItem::sizeHint() const
{
    return QSizeF(12.0,12.0);
}

void NCReportItem::updateForSave()
{
    NCReportScale s;
    d->mSize = s.sizeToMM( d->currentSize );
    d->mPos = s.pointToMM( scenePos() );
}

void NCReportItem::updateForEditor()
{
    updateSize();
    updatePosition();
}

void NCReportItem::updateSize()
{
    resizeInMM( d->mSize );
}

void NCReportItem::updatePosition()
{
    NCReportScale s;
    setPos( s.mmToPoint( d->mPos ) ); // designer position in scene
}
/*!
  UPDATES THE VALUE OF ITEMS SUCH AS FIELD/TEXT/IMAGE ON RUNTIME
  */
void NCReportItem::updateValue(NCReportOutput *output, NCReportEvaluator* evaluator)
{
    Q_UNUSED(output)

    if (d->reportDef && !d->functionId.isEmpty() ) {
        NCReportUserFunction *func = d->reportDef->userFunction(d->functionId);
        if (func) {
            func->updateValue(this, output, evaluator);
        }
    }
}

void NCReportItem::updateContent()
{
    // virtual
}

void NCReportItem::itemPositionChange(const QPointF &)
{
    // virtual method for handling change position event if needed
}

void NCReportItem::adjustSize()
{
}

void NCReportItem::adjustSize( NCReportOutput* )
{
}

NCReportSection * NCReportItem::sectionScene() const
{
    if ( !scene() )
        return 0;
    return (NCReportSection*)scene();
}

void NCReportItem::offsetPastePos()
{
    setPos( QPointF( scenePos().x() + sectionScene()->deltaX(),
            scenePos().y() + sectionScene()->deltaY() ) );
}

void NCReportItem::saveSelectState()
{
    m_selectPos = scenePos();
    m_selectSize = currentSize();
}

QPointF NCReportItem::selectPos() const
{
    return m_selectPos;
}

QSizeF NCReportItem::selectSize() const
{
    return m_selectSize;
}


bool NCReportItem::isDynamic() const
{
    bool m_dynamic = false;
    switch( d->sourceType ) {
        case Static:
        case File:
        case InternalDocument:
            m_dynamic = false;
            break;
        case Url:
        case DataSource:
        case Parameter:
        case UrlFromDataSource:
        case FileNameFromDataSource:
            m_dynamic = true;
            break;
    }
    if ( d->templateMode )
        m_dynamic = true;

    return m_dynamic;
}

QSizeF NCReportItem::realMetricSize() const
{
    if ( d->autoHeight || d->pinToLeft || d->pinToRight )
        return d->mRealSize;
    else
        return d->mSize;

    return QSizeF();
}

bool NCReportItem::isEmpty() const
{
    return false; //virtual
}

void NCReportItem::enableOffsetPosition()
{
    d->mPos -= metricOffset();  //QPointF(0,zoneY);
}

void NCReportItem::disableOffsetPosition()
{
    d->mPos += metricOffset();
}

QPainterPath NCReportItem::shape() const
{
    QPainterPath path;
    QRectF rect( 0, 0, d->currentSize.width(), d->currentSize.height() );
    path.addRect(rect);
    SelectionDirection directions[] = { TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left };
    const int numDirections = 8;
    for ( int i = 0; i < numDirections; ++i )
        path.addRect( selectionRect(directions[i],d->currentSize ) );
    return path;
}

/*! Returns true if item can break to an available space */
bool NCReportItem::isAbleToBreak() const
{
    return d->breakToPage;
}

/*!
  Switches on / off the item capability to be able to break or not.
*/
void NCReportItem::setAbleToBreak(bool set)
{
    d->breakToPage = set;
}

/*! Shrink or slice item to a metric height. Returns the real calculated height in millimeters if item can be sliced to the specified height.
  Returns -1 if the heightMM is not enough the item to slice to.
*/
qreal NCReportItem::breakTo( qreal, NCReportOutput* )
{
    return false;
}

/*!
Returns true if item is devided to parts and prints only the specified part.
*/
bool NCReportItem::isBroken() const
{
    return false;
}

/*!
Item is a normal object that has topleft position and size.
This is not valid for lines for example.
  */
bool NCReportItem::hasSizeAndPosition() const
{
    return true;
}

bool NCReportItem::pinToRight() const
{
    return d->pinToRight;
}

void NCReportItem::setPinToRight(bool set)
{
    d->pinToRight = set;
}

bool NCReportItem::pinToLeft() const
{
    return d->pinToLeft;
}

void NCReportItem::setPinToLeft(bool set)
{
    d->pinToLeft = set;
}

void NCReportItem::setPaintRect(const QRectF &rect)
{
    d->paintRect = rect;
}

QRectF NCReportItem::paintRect() const
{
    return d->paintRect;
}

QList<int> NCReportItem::availableSourceTypes() const
{
    QList<int> types;
    types << Static;
    return types;
}

/*!
 * \brief NCReportItem::printWhenTrueAction
 * \param evaluator
 * The method is executed after the item's print when expression evaluated and the result was true.
 * It can be used for item specific actions when the item will surely be printed.
 */
void NCReportItem::printWhenTrueAction(NCReportEvaluator *evaluator)
{
    Q_UNUSED(evaluator);
}

const QHash<QString, QVariant> &NCReportItem::parameters() const
{
    return d->parameters;
}

QHash<QString, QVariant> &NCReportItem::parametersRef()
{
    return d->parameters;
}

QVariant NCReportItem::parameter(const QString &id) const
{
    return d->parameters.value(id);
}

bool NCReportItem::hasParameter() const
{
    return !d->parameters.isEmpty();
}

void NCReportItem::dummyPaint(NCReportOutput *, const QPointF &)
{
    //nothing to do by default
}


