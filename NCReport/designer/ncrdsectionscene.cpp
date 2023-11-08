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
#include "ncrdsectionscene.h"
#include "ncrdmainwindow.h"
#include "ncrddocument.h"
#include "ncrdcommands.h"
#include "ncrdtextpropertymanager.h"
#include "ncrdshapepropertymanager.h"
#include "ncrdpropertyeditor.h"
#include "ncrddatasourcetree.h"

#include "ncreportitem.h"
#include "ncreportlabelitem.h"
#include "ncreportfielditem.h"
#include "ncreportlineitem.h"
#include "ncreportrectitem.h"
#include "ncreportimageitem.h"
#include "ncreportbarcodeitem.h"
#include "ncreportgraphitem.h"
#include "ncreportdatasource.h"

#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QAction>
#include <QMenu>
#include <QCursor>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QTransform>

//! 多语言
#include "MDMultiLanguage.h"

//#define NCRD_DRAGDROP_LABEL_AS_FIELD

NCRDSectionScene::NCRDSectionScene( NCRDMainWindow *mw, NCRDDocument * doc, QObject * parent)
    : NCReportSection( parent )
{
    m_mainwin = mw;
    m_document = doc;
    m_lastInsertedItem =0;
    m_firstSelectedItem =0;
    insertingLineItem = 0;
    insertingRectItem =0;

    setSceneRect(0, 0, 1600, 500);
    //setSceneRect( QRectF() );
    connect( this, SIGNAL(selectionChanged()), this, SLOT(slotSelectionChanged()) );
    connect( this, SIGNAL(firstItemSelected(NCReportItem*)), doc, SLOT(slotSectionFirstItemSelected(NCReportItem*)) );
    //connect( this, SIGNAL(changed(const QList<QRectF>&)), doc, SLOT(sceneContentModified(const QList<QRectF>&)) );
}

NCRDSectionScene::~NCRDSectionScene()
{
}

void NCRDSectionScene::setEditorMode()
{
    //item->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );
}

void NCRDSectionScene::contextMenuEvent(QGraphicsSceneContextMenuEvent * e)
{
#if QT_VERSION>=0x050000
    QGraphicsItem* item = itemAt(e->scenePos(), QTransform());
#else
    QGraphicsItem* item = itemAt(e->scenePos());
#endif

    executeContextMenu(item, snapToPoint(e->scenePos()));
}

void NCRDSectionScene::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
    if ( m_mainwin->currentToolState() == NCRDMainWindow::Tool ) {

        //clearSelection();
        QPointF pos = snapToPoint(e->scenePos());

        switch( m_mainwin->currentTool() ) {
            case NCReportItem::Line:
            {
                insertingLineItem = new QGraphicsLineItem( QLineF(pos, pos) );
                //insertingLineItem->setPen(QPen(Qt::black,1));
                NCRDShapePropertyManager *pmShape = (NCRDShapePropertyManager*)(m_mainwin->propertyManager("pmshape"));
                insertingLineItem->setPen(pmShape->pen());
                addItem( insertingLineItem );
                break;
            }
            case NCReportItem::Rectangle:
            case NCReportItem::Image:
            case NCReportItem::Graph:
            {
                insertingRectItem = new QGraphicsRectItem( QRectF(pos, pos) );
                insertingRectItem->setPen(QPen(Qt::black,1,Qt::DashLine));
                addItem( insertingRectItem );
                break;
            }
            case NCReportItem::Ellipse:
            {
                insertingEllipseItem = new QGraphicsEllipseItem( QRectF(pos, pos) );
                insertingEllipseItem->setPen(QPen(Qt::black,1,Qt::DashLine));
                addItem( insertingEllipseItem );
                break;
            }

            default:
                break;
        }

        return;
    }

    QGraphicsScene::mousePressEvent(e);
}

void NCRDSectionScene::mouseMoveEvent( QGraphicsSceneMouseEvent * e )
{
    //qDebug("NCRDSectionScene::mouseMoveEvent: Item newPos: %f,%f", e->scenePos().x(),e->scenePos().y());

    if ( m_mainwin->currentToolState() == NCRDMainWindow::Tool && e->buttons() == Qt::LeftButton ) {

        switch( m_mainwin->currentTool() ) {
            case NCReportItem::Line:
                if ( insertingLineItem ) {
                    QLineF line( insertingLineItem->line().p1(), snapToPoint(e->scenePos()) );
                    if ( e->modifiers() == Qt::ShiftModifier || e->modifiers() == Qt::ControlModifier ) {
                        line = adjustLine( line );
                    }

                    insertingLineItem->setLine(line);
                }
                break;
            case NCReportItem::Rectangle:
            case NCReportItem::Image:
            case NCReportItem::Graph:
                if ( insertingRectItem ) {
                    insertingRectItem->setRect( QRectF( insertingRectItem->rect().topLeft(), snapToPoint(e->scenePos()) ).normalized() );
                }
                break;
            case NCReportItem::Ellipse:
                if ( insertingEllipseItem ) {
                    insertingEllipseItem->setRect( QRectF( insertingEllipseItem->rect().topLeft(), snapToPoint(e->scenePos()) ));
                }
                break;
            default:
                break;
        }
        QGraphicsScene::mouseMoveEvent(e);
    } else {
        QGraphicsScene::mouseMoveEvent(e);
    }

    //if ( m_mainwin->currentState() == NCRDMainWindow::Arrow )
}

void NCRDSectionScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event )
{
    //qDebug("(Scene) NCRDSectionScene::mouseReleaseEvent: Item newPos: %f,%f", e->scenePos().x(),e->scenePos().y());
    insertItem( event );
    QGraphicsScene::mouseReleaseEvent(event);
}

//画出网格点
void NCRDSectionScene::paintGrid( QPainter* p )
{
    //if ( !m_mainwin || !m_mainwin->isShowGrid() )
    if ( !gridVisible() )
        return;

    p->setPen( QPen(Qt::gray) );
    for ( int y = 0; y < height(); y += deltaY() ) {
        for ( int x = 0; x < width(); x += deltaX() )
            p->drawPoint( x, y );
    }
}

void NCRDSectionScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    QTreeWidget *source = qobject_cast<QTreeWidget *>(event->source());
    if(source) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void NCRDSectionScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dragMoveEvent(event);
    QTreeWidget *source = qobject_cast<QTreeWidget *>(event->source());
    if(source) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
}

void NCRDSectionScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    QGraphicsScene::dropEvent(event);
    NCRDDataSourceTree *source = qobject_cast<NCRDDataSourceTree *>(event->source());
    if(source) {
//        const QMimeData *mimeData = event->mimeData();
//        if (mimeData->hasImage()) {
//             text = tr("IMAGE");
//         } else if (mimeData->hasHtml()) {
//             text = mimeData->html();
//             //setTextFormat(Qt::RichText);
//         } else if (mimeData->hasText()) {
//             text = mimeData->text();
//             //setTextFormat(Qt::PlainText);
//         } else {
//             text = mimeData->formats().join(":");
//         }

        Qt::Orientation orient;
        //bool insertLabel=(event->modifiers()==Qt::ShiftModifier || event->modifiers()==(Qt::ShiftModifier|Qt::AltModifier) );

        if (event->modifiers()==Qt::AltModifier) {
            orient = Qt::Horizontal;
        } else {
            orient = Qt::Vertical;
        }
        clearSelection();

        const qreal margin = 6;
        QPointF pos = event->scenePos();
        QGraphicsItem* lastItem =0;
        QList<QTreeWidgetItem *> selectList = source->selectedItems();

        for (int i=0;i<selectList.count();i++) {

            NCRDDataSourceTree::ItemDescriptor d = source->parseItem(selectList.at(i), this );
            if (d.itemSourceType == 0)
                continue;

            NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, (NCReportItem::ObjectType)d.objectType, d.objectType, snapToPoint(pos) );
            m_document->undoStack()->push( command );
            NCReportLabelItem *item = (NCReportLabelItem*)(command->item());
            item->setSourceType( (NCReportItem::SourceType)d.itemSourceType);
            item->setText( d.id );
            item->setDescription(d.title);

            if (lastItem) {
                if (orient == Qt::Vertical)
                    pos.ry()+= lastItem->boundingRect().height()-margin;
                else
                    pos.rx()+= lastItem->boundingRect().width()-margin;
            }
            item->setPos( pos );
            lastItem = item;
        }

        //createWidget(static_cast<Widget::WidgetType>(event->mimeData()->text().toInt()), event->scenePos());
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }

}

void NCRDSectionScene::executeContextMenu(QGraphicsItem *item, const QPointF& insertPos)
{
    NCReportItem *reportitem = 0;

    QAction *actItemProp=0;
    QAction *actFront=0;
    QAction *actBack=0;
    QAction *actDel=0;
    QAction *actSectProp=0;
    QAction *actDetailSection=0;
    QAction *actLabel=0;
    QAction *actText=0;
    QAction *actField=0;
    QAction *actBarcode=0;
    QAction *actCopy=0;
    QAction *actCut=0;
    QAction *actPaste=0;
    QAction *actLock=0;
    QAction *actGroup=0;
    QAction *result=0;

    NCReportItem::ObjectType objectType = NCReportItem::NoObject;
    QMenu menu;

    if( item ) {
        // CONTEXT MENU OF AN EXISTING ITEM
        reportitem = qgraphicsitem_cast<NCReportItem*>(item);

        actItemProp = menu.addAction( tr("Item properties...") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actItemProp);
        actCopy = menu.addAction( QIcon(":/designer/images/editcopy.png"), tr("Copy") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actCopy);
        actCut = menu.addAction( QIcon(":/designer/images/editcut.png"), tr("Cut") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actCut);
        actDel = menu.addAction( tr("Delete item") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actDel);

        if ( reportitem->isLocked() )
        {
            actLock = menu.addAction( QIcon(":/designer/images/lock_open.png"), tr("Unlock item") );
            // 多语言
            CMDMultiLanguage::instance()->translateWidget(actLock);
        }

        else
        {
            actLock = menu.addAction( QIcon(":/designer/images/lock.png"), tr("Lock item") );
            // 多语言
            CMDMultiLanguage::instance()->translateWidget(actLock);
        }
        menu.addSeparator();
        actFront = menu.addAction(QIcon(":/designer/images/shape_move_front.png"), tr("Bring to front") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actFront);
        actBack = menu.addAction(QIcon(":/designer/images/shape_move_back.png"), tr("Move to back") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actBack);
        menu.addSeparator();
    }

    actSectProp = menu.addAction( tr("Section properties...") );
    // 多语言
    CMDMultiLanguage::instance()->translateWidget(actSectProp);
    if ( type() == Detail )
    {
        actDetailSection = menu.addAction(QIcon(":/designer/images/details.png"), tr("Detail and groups...") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actDetailSection);
    }
    else if ( type() == GroupHeader || type() == GroupFooter )
    {
        actGroup = menu.addAction( tr("Group settings...") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actGroup);
    }

    menu.addSeparator();
    if ( !item ) {
        menu.addSeparator();
        actPaste = menu.addAction(QIcon(":/designer/images/editpaste.png"), tr("Paste") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actPaste);
        menu.addSeparator();
        actField = menu.addAction(QIcon(":/designer/images/database_table.png"), tr("Insert Field") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actField);
        actLabel = menu.addAction(QIcon(":/designer/images/label.png"), tr("Insert Label") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actLabel);
        actText = menu.addAction(QIcon(":/designer/images/html.png"), tr("Insert Text") );
        // 多语言
        CMDMultiLanguage::instance()->translateWidget(actText);
        actBarcode = menu.addAction(QIcon(":/designer/images/barcode.png"), tr("Insert Barcode") );
		//多语言
		CMDMultiLanguage::instance()->translateWidget(actBarcode);
    }

    result = menu.exec( QCursor::pos() );

    if ( !result )
        return;

    if (result == actDetailSection) {
        m_mainwin->detailsDialog(m_document, this);
        return;
    }
    if ( result == actSectProp ) {
        m_document->sectionPropDialog( this );
        return;
    }
    if ( result == actGroup ) {
        m_mainwin->groupSettings( m_document, this );
        return;
    }

    if ( result == actDel )
        removeItemCommand(reportitem, true );
    else if ( result == actItemProp )
        m_document->itemEdit( reportitem );
    else if ( result == actFront )
    {
        item->setSelected(true);
        m_document->bringToFront();
    }
    else if ( result == actBack )
    {
        item->setSelected(true);
        m_document->moveToBack();
    }
    else if ( result == actCopy )
        m_mainwin->copy();
    else if ( result == actCut )
        m_mainwin->cut();
    else if ( result == actLock )
    {
        reportitem->setLocked( !reportitem->isLocked() );
        //reportitem->setFlag( QGraphicsItem::ItemIsMovable, !reportitem->isLocked() );
        reportitem->update();
    }
    else if ( result == actPaste )
        m_mainwin->paste();
    else if ( result == actLabel )
        objectType = NCReportItem::Label;
    else if ( result == actField )
        objectType = NCReportItem::Field;
    else if ( result == actText )
        objectType = NCReportItem::Text;
    else if ( result == actBarcode )
        objectType = NCReportItem::Barcode;

    if ( objectType != NCReportItem::NoObject ) {
        NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, objectType, 0, insertPos );
        m_document->undoStack()->push( command );
        m_document->itemEdit( command->item() );
    }

}

void NCRDSectionScene::insertItem(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = snapToPoint(event->scenePos());

    if ( m_mainwin->currentToolState() == NCRDMainWindow::Tool ) {
        // insert finished
        clearSelection();

        switch( m_mainwin->currentTool() ) {
            case NCReportItem::Label:
            case NCReportItem::Field:
            case NCReportItem::Text:
            case NCReportItem::Barcode:
            case NCReportItem::CrossTab:
            case NCReportItem::Table:
            case NCReportItem::Chart:
            {
                NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, m_mainwin->currentTool(), m_mainwin->currentItemTypeID(), pos );
                m_document->undoStack()->push( command );
                m_document->itemEdit( command->item() );
                break;
            }
            case NCReportItem::Line:
                if ( insertingLineItem ) {
                    NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, m_mainwin->currentTool(), m_mainwin->currentItemTypeID(), pos );
                    QLineF fline(insertingLineItem->line());
                    if ( fline.p1() != fline.p2() ) {
                        command->setLine( fline );
                        m_document->undoStack()->push( command );
                    }
                    removeItem( insertingLineItem );
                    delete insertingLineItem;
                    insertingLineItem =0;
                }
                break;
            case NCReportItem::Rectangle:
                if ( insertingRectItem ) {
                    NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, m_mainwin->currentTool(), m_mainwin->currentItemTypeID(), pos );
                    command->setRect( insertingRectItem->rect() );
                    m_document->undoStack()->push( command );

                    removeItem( insertingRectItem );
                    delete insertingRectItem;
                    insertingRectItem =0;
                }
                break;
            case NCReportItem::Ellipse:
                if ( insertingEllipseItem ) {
                    NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, m_mainwin->currentTool(), m_mainwin->currentItemTypeID(), pos );
                    command->setRect( insertingEllipseItem->rect() );
                    m_document->undoStack()->push( command );

                    removeItem( insertingEllipseItem );
                    delete insertingEllipseItem;
                    insertingEllipseItem =0;
                }
                break;
            case NCReportItem::Image:
                if ( insertingRectItem ) {
                    NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, m_mainwin->currentTool(), m_mainwin->currentItemTypeID(), pos );
                    command->setRect( insertingRectItem->rect() );
                    m_document->undoStack()->push( command );

                    removeItem( insertingRectItem );
                    delete insertingRectItem;
                    insertingRectItem =0;

                    m_document->itemEdit( command->item() );
                }
                break;
            case NCReportItem::Graph:
                if ( insertingRectItem ) {
                    // insert with item type id
                    NCRDInsertItemCommand *command = new NCRDInsertItemCommand( 0, this, m_mainwin->currentTool(), m_mainwin->currentItemTypeID(), pos );
                    command->setRect( insertingRectItem->rect() );
                    m_document->undoStack()->push( command );

                    removeItem( insertingRectItem );
                    delete insertingRectItem;
                    insertingRectItem =0;

                    m_document->itemEdit( command->item() );
                }
                break;
            default:
                break; //insertItemCommand( e->scenePos() );
        }
        m_document->setDocumentModified();
        m_mainwin->activate_Arrow();
        return;

    } else if (  m_mainwin->currentToolState() == NCRDMainWindow::Arrow ) {
        if ( event->button() == Qt::LeftButton ) {
            qDebug("NCRDSectionScene::mouseReleaseEvent -> Arrow mode and LeftButton" );
            // object move recording
#if QT_VERSION>=0x050000
            QGraphicsItem* item = itemAt(pos, QTransform());
#else
            QGraphicsItem* item = itemAt(pos);
#endif
            if (!item) {
                if ( selectedItems().count() > 0 )
                    item = selectedItems().first();
            }
            if (item) {
                /****************************************
                  ITEM MOVE/RESIZE COMMAND
                *****************************************/
                NCReportItem *i = qgraphicsitem_cast<NCReportItem*>(item);
                switch(i->selectionHoverState()) {
                case NCReportItem::None:
                    moveItemCommand( i, true );		// applies for all selected items
                    break;
                case NCReportItem::LinePoint1:
                case NCReportItem::LinePoint2:
                    if ( event->modifiers() & Qt::ControlModifier )
                        moveItemCommand( i );
                    else
                        lineItemChangeCommand( i );
                    break;
                default:
                    resizeItemCommand( i );
                    break;
                }
            }
        }
    }

}

void NCRDSectionScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    QGraphicsScene::drawBackground( painter, rect );
    paintGrid( painter );
}


//void NCRDSectionScene::insertItemCommand( const QPointF& pos )
//{
//    m_document->undoStack()->push( new NCRDInsertItemCommand( 0, this, m_mainwin->currentTool(), pos ) );
//}

///////////////////////////////////////////////////////////////////////////////////////////
//    2015年8月29日 mxl  修改
//报表源码中自身存在的执行剪切两次操作,设计器一定崩溃的bug,定位到是removeSelected该变量在执行第二次剪切操作是记录的仍然是之前操作对象的bool值一直为true,
//导致执行removeItemCommand函数中的else模块,找不到之前被剪切的项目指针,程序崩溃,现在暂时先将else部分代码屏蔽
///////////////////////////////////////////////////////////////////////////////////////////
void NCRDSectionScene::removeItemCommand( NCReportItem *item, bool removeSelected )
{
    Q_UNUSED(item);
    if ( removeSelected && selectedItems().count()>0 ) {
        m_document->undoStack()->push( new NCRDRemoveItemCommand( selectedItems(), this ) );
    } /*else {
        QList<QGraphicsItem*> list;
        list << item;
        m_document->undoStack()->push( new NCRDRemoveItemCommand( list, this ) );
    }*/
}

void NCRDSectionScene::moveItemCommand( NCReportItem *item, bool moveSelected )
{
    //qDebug("moveItemCommand");
    NCRDMoveItemCommand *cmd =0;
    QList<QGraphicsItem*> list;
    if ( moveSelected )
        cmd = new NCRDMoveItemCommand( selectedItems(), this );
    else
    {
        if ( item ) {
            list << item;
            cmd = new NCRDMoveItemCommand( list, this );
        }
    }

    if ( cmd->isValid() )
        m_document->undoStack()->push( cmd );
    else
        delete cmd;

}

void NCRDSectionScene::resizeItemCommand(NCReportItem *item )
{
    qDebug("resizeItemCommand" );
    m_document->undoStack()->push( new NCRDResizeItemCommand( item, this ));
}

void NCRDSectionScene::lineItemChangeCommand(NCReportItem *item )
{
    qDebug("lineItemChangeCommand" );
    m_document->undoStack()->push( new NCRDLineItemChangeCommand( item, this ));
}

void NCRDSectionScene::keyPressEvent(QKeyEvent * keyEvent)
{
    if ( insertingLineItem ) {
        if ( keyEvent->key() == Qt::Key_Shift ) {
            QLineF line( insertingLineItem->line() );
            line = adjustLine( line );
            insertingLineItem->setLine( line );
        }
    } else if ( (keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) && selectedItems().count()==1 ) {
        m_document->itemEdit( qgraphicsitem_cast<NCReportItem*>(selectedItems().at(0)) );
    } else
        m_document->keyboardAction( keyEvent );
}

void NCRDSectionScene::keyboardAction(QKeyEvent *e )
{
    if ( selectedItems().count() == 0 )
        return;

    QPointF move;
    int mFactor = 1;

    if (e->modifiers() == Qt::ControlModifier)
        mFactor = 5;

    switch ( e->key() ) {
        case Qt::Key_Up: move.setY( snapY() ? -deltaY()*mFactor : -1.0*mFactor ); break;
        case Qt::Key_Down: move.setY( snapY() ? deltaY()*mFactor : 1.0*mFactor ); break;
        case Qt::Key_Left: move.setX( snapX() ? -deltaX()*mFactor : -1.0*mFactor ); break;
        case Qt::Key_Right: move.setX( snapX() ? deltaX()*mFactor : 1.0*mFactor ); break;
        case Qt::Key_Delete:
        {
            removeItemCommand(0,true);
            m_document->setDocumentModified();
            return;
        }
        default:
            return;
    }

    QList<QGraphicsItem*> list = selectedItems();
    for (int i = 0; i < list.size(); ++i) {
        NCReportItem *item = (NCReportItem*)list.at(i);
        item->setPos( item->pos() + move );
    }
    moveItemCommand( 0, true );
}


void NCRDSectionScene::keyReleaseEvent(QKeyEvent * keyEvent)
{
    QGraphicsScene::keyReleaseEvent(keyEvent);
}

QLineF NCRDSectionScene::adjustLine(const QLineF & line ) const
{
    QPointF p1 = line.p1();
    QPointF p2 = line.p2();

    qreal dx = line.dx();
    qreal dy = line.dy();

    QLineF lineX( line.p1(), QPointF( dx, p1.y() ) );
    //QLineF lineY( line.p1(), QPointF( p1.x(), dy ) );
    //qDebug( "dx: %f dy: %f Angle %f", dx,dy,line.angle( lineX ) );

    const qreal ang = line.angle( lineX );

    if ( ang >= 0.0 && ang<=45.0 )
        p2 = QPointF( p1.x() + dx, p1.y() );
    else if ( ang >= 45.0 && ang<=90.0 )
        p2 = QPointF( p1.x(), p1.y() + dy );
    else if ( ang >= 90.0 && ang<=135.0 )
        p2 = QPointF( p1.x(), p1.y() + dy );
    else if ( ang >= 135.0 && ang<=180.0 )
        p2 = QPointF( p1.x() + dx, p1.y() );

    return QLineF( p1, snapToPoint(p2) );
}


void NCRDSectionScene::slotSelectionChanged()
{
    qDebug("NCRDSectionScene::slotSelectionChanged(%p)",this);
    NCRDPropertyManager *pmTxt = m_mainwin->propertyManager("pmtxt");
    NCRDPropertyManager *pmShape = m_mainwin->propertyManager("pmshape");

    pmTxt->setScene( this );
    pmTxt->setDocument( m_document );
    pmShape->setScene( this );
    pmShape->setDocument( m_document );
    int scount = selectedItems().count();
    switch ( scount ) {
        case 0:
        {
            if ( m_mainwin->propertyEditor() )
                m_mainwin->propertyEditor()->setItem( 0 );

            m_firstSelectedItem = 0;
            emit firstItemSelected( 0 );
            return;
        }
        case 1:
        {
            m_firstSelectedItem = qgraphicsitem_cast<NCReportItem*>(selectedItems().first());
            emit firstItemSelected( m_firstSelectedItem );
        }
        default:
        {
            //more than one
        }
    }
    NCReportItem *item = qgraphicsitem_cast<NCReportItem*>(selectedItems().first());

    if ( m_mainwin->propertyEditor() )
        m_mainwin->propertyEditor()->setItem( item );


//	switch (item->objectType()) { //Label, Field, Text, Line, Rectangle, Ellipse, Image, Barcode, Graph, Custom
//		case NCReportItem::Label:
//		case NCReportItem::Field:
//		case NCReportItem::Text:
//		case NCReportItem::Barcode:
//			// **** this item becomes the topmost (2010.05.25) ! Reason: Easier to resize and move ****
//			item->setZValue( nextZValue() );
//			// **** Z value is able to set via property editor 2010.12.28
//			break;
//		default:
//			break;
//	}

    switch (item->objectType()) { //Label, Field, Text, Line, Rectangle, Ellipse, Image, Barcode, Graph, Custom
        case NCReportItem::Label:
        case NCReportItem::Field:
        case NCReportItem::Text:
        {
            if (scount==1)
                pmTxt->applyChangesToManager( item );
            break;
        }
        case NCReportItem::Line:
        case NCReportItem::Rectangle:
        case NCReportItem::Ellipse:
        {
            if (scount==1)
                pmShape->applyChangesToManager( item );
            break;

        }
        default:
            break;
    }
}

void NCRDSectionScene::actionManagerChanged( int property )
{
    if ( selectedItems().count() == 0 ) {
        return;
    }

    NCRDPropertyManager *pmTxt = m_mainwin->propertyManager("pmtxt");
    NCRDPropertyManager *pmShape = m_mainwin->propertyManager("pmshape");

    QList<QGraphicsItem*> list = selectedItems();
    for (int i = 0; i < list.size(); ++i) {
        NCReportItem *item = (NCReportItem*)list.at(i);
        switch (item->objectType()) { //Label, Field, Text, Line, Rectangle, Ellipse, Image, Barcode, Graph, Custom
            case NCReportItem::Label:
            case NCReportItem::Field:
            case NCReportItem::Text:
            {
                pmTxt->applyChangesToItem( item, property );
                //item->update();
                break;
            }
            case NCReportItem::Line:
            case NCReportItem::Rectangle:
            case NCReportItem::Ellipse:
            {
                pmShape->applyChangesToItem( item, property );
                //item->update();
                break;

            }
            default:
                break;
        }
    }
}

void NCRDSectionScene::emitContentChanged()
{
    m_document->setDocumentModified();
}

void NCRDSectionScene::emitItemChanged(NCReportItem *item)
{
    if ( m_mainwin->propertyEditor() )
        m_mainwin->propertyEditor()->setItem( item );
}

void NCRDSectionScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    event->ignore();
}

void NCRDSectionScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
#if QT_VERSION>=0x050000
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());
#else
    QGraphicsItem* item = itemAt(event->scenePos());
#endif

    if(item)
        m_document->itemEdit( qgraphicsitem_cast<NCReportItem*>(item) );
    else
        m_document->sectionPropDialog( this );
}

NCReportItem * NCRDSectionScene::firstSelectedItem()
{
    return m_firstSelectedItem;
}

NCRDMainWindow * NCRDSectionScene::mainWindow()
{
    return m_mainwin;
}

// void NCRDSectionScene::focusInEvent(QFocusEvent * focusEvent)
// {
// 	NCReportSection::focusInEvent(focusEvent);
// 	qDebug("focusInEvent %i", focusEvent->reason() );
// 	emit sectionGotFocus();
// }

void NCRDSectionScene::addItemZ(NCReportItem *item)
{
    int nz = nextZValue();
    item->setZValue( nz );
    //item->setID( QString::number(nz) );
    if (item->id().isEmpty() || item->id()=="0")
        item->setId("%");

    addItem( item );
}

void NCRDSectionScene::addItemForEngine( NCReportItem* )
{
    // nothing to do
}

NCRDDocument * NCRDSectionScene::document()
{
    return m_document;
}

