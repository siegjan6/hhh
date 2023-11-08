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
#include "ncrdcommands.h"
#include "ncrdsectionscene.h"
#include "ncrdmainwindow.h"
#include "ncrdpropertymanager.h"
#include "ncrddef.h"

#include "ncreportlabelitem.h"
#include "ncreportrectitem.h"
#include "ncreportellipseitem.h"
#include "ncreportfielditem.h"
#include "ncreportlineitem.h"
#include "ncreportimageitem.h"
#include "ncreportgraphitem.h"
#include "ncreportbarcodeitem.h"
#include "ncreporttextitem.h"
#include "ncreportcrosstabitem.h"
#include "ncreportchartitem.h"
#include "ncreporttableitem.h"

#include <QGraphicsItem>

static const int moveCommandID = 1;

NCRDCommand::NCRDCommand( NCReportItem* item, NCRDSectionScene * scene, QUndoCommand * parent )
    : QUndoCommand( parent )
{
    m_scene = scene;
    m_item = item;
}

NCRDSectionScene * NCRDCommand::scene()
{
    return m_scene;
}

NCReportItem * NCRDCommand::item()
{
    return m_item;
}


//-------------------------------------------------------------------------------------------------------

NCRDInsertItemCommand::NCRDInsertItemCommand(NCReportItem* item, NCRDSectionScene * scene,
    const NCReportItem::ObjectType objectType, int typeID, const QPointF & pos, QUndoCommand * parent )
    : NCRDCommand( item, scene, parent ), m_objectType(objectType), m_typeId(typeID), m_pos(pos)
{
}

void NCRDInsertItemCommand::undo()
{
    m_scene->removeItem(m_item);
    //delete m_item;
    //m_item = 0;
}

void NCRDInsertItemCommand::redo()
{
    if ( m_item )
        m_scene->addItemZ( m_item );
    else
        insertItem();
}

void NCRDInsertItemCommand::insertItem()
{
    NCRDPropertyManager *pmTxt = m_scene->mainWindow()->propertyManager("pmtxt");
    NCRDPropertyManager *pmShape = m_scene->mainWindow()->propertyManager("pmshape");

    m_item=0;
    switch ( m_objectType ) {
        case NCReportItem::Label:
            m_item = new NCReportLabelItem( NCReportItem::Label, m_scene->document()->reportDef() );
            break;
        case NCReportItem::Field:
            m_item = new NCReportFieldItem( m_scene->document()->reportDef() );
            break;
        case NCReportItem::Text:
            m_item = new NCReportTextItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Line:
            m_item = new NCReportLineItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Rectangle:
            m_item = new NCReportRectItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Ellipse:
            m_item = new NCReportEllipseItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Image:
            m_item = new NCReportImageItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Barcode:
            m_item = new NCReportBarcodeItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Graph:
            m_item = new NCReportGraphItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::CrossTab:
            m_item = new NCReportCrossTabItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Table:
            m_item = new NCReportTableItem(m_scene->document()->reportDef());
            break;
        case NCReportItem::Chart:
#ifdef HIGHCHARTS_INTEGRATION
            m_item = new NCReportChartItem(m_scene->document()->reportDef());
#endif
            break;
        default:
            return;
    }

    m_item->setTypeId(m_typeId);    // type ID for custom graph

    switch ( m_objectType ) {
        case NCReportItem::Field:
        {
            int w = 20*SELECTION_BOXSIZE;
            int h = 3 *SELECTION_BOXSIZE;
            QString txt = QObject::tr("Field");

            if (m_scene->mainWindow()->preferences().defaultFieldWidth > 0)
                w = m_scene->mainWindow()->preferences().defaultFieldWidth;
            if (m_scene->mainWindow()->preferences().defaultFieldHeight > 0)
                h = m_scene->mainWindow()->preferences().defaultFieldHeight;
            if (!m_scene->mainWindow()->preferences().defaultFieldText.isEmpty())
                txt = m_scene->mainWindow()->preferences().defaultFieldText;

            m_item->resize( QSizeF(w,h) );
            m_item->setText(txt);
            pmTxt->applyChangesToItem( m_item, NCRDPropertyManager::All );
            m_item->setPos( m_pos );
            break;
        }
        case NCReportItem::Label:
        case NCReportItem::Text:
        {
            m_item->setDefaultForEditor();
            pmTxt->applyChangesToItem( m_item, NCRDPropertyManager::All );
            m_item->setPos( m_pos );
            break;
        }
        case NCReportItem::Line:
        {
            ((NCReportLineItem*)m_item)->setLine( m_line );
            pmShape->applyChangesToItem( m_item, NCRDPropertyManager::All );
            pmShape->setEnabled(true);
            break;
        }
        case NCReportItem::Rectangle:
        {
            m_item->setDefaultForEditor();
            QRectF r = m_rect.normalized();
            m_item->resize( r.size() );
            m_item->setPos( r.topLeft() );
            pmShape->applyChangesToItem( m_item, NCRDPropertyManager::All );
            pmShape->setEnabled(true);
            break;
        }
        case NCReportItem::Ellipse: {
            m_item->setDefaultForEditor();
            QRectF r = m_rect.normalized();
            m_item->resize( r.size() );
            m_item->setPos( r.topLeft() );
            pmShape->applyChangesToItem( m_item, NCRDPropertyManager::All );
            pmShape->setEnabled(true);
            break;
        }
        case NCReportItem::Image: {
            m_item->setDefaultForEditor();
            QRectF r = m_rect.normalized();
            m_item->resize( r.size() );
            m_item->setPos( r.topLeft() );
            break;
        }
        case NCReportItem::Barcode: {
            m_item->setDefaultForEditor();
            m_item->setPos( m_pos );
            break;
        }
        case NCReportItem::Graph: {
            m_item->setDefaultForEditor();
            QRectF r = m_rect.normalized();
            m_item->resize( r.size() );
            m_item->setPos( r.topLeft() );
            break;
        }
        case NCReportItem::CrossTab: {
            m_item->setDefaultForEditor();
            m_item->setPos( m_pos );

            break;
        }
        case NCReportItem::Table: {
            m_item->setDefaultForEditor();
            m_item->setPos( m_pos );

            break;
        }
        case NCReportItem::Chart: {
#ifdef HIGHCHARTS_INTEGRATION
            m_item->setDefaultForEditor();
            m_item->setPos( m_pos );
#endif
            break;
        }

        default:
            return;
    }

    m_item->setSelected(true);
    m_item->saveSelectState();
    m_scene->addItemZ( m_item );

}

void NCRDInsertItemCommand::setLine(const QLineF & l)
{
    m_line = l;
}

void NCRDInsertItemCommand::setRect(const QRectF & r)
{
    m_rect = r;
}

//-------------------------------------------------------------------------------------------------------
NCRDMoveItemCommand::NCRDMoveItemCommand( const QList<QGraphicsItem*>& items, NCRDSectionScene *scene, QUndoCommand * parent )
    : NCRDCommand( 0, scene, parent )
{
    m_items = items;
    initial = true; // initially avoid redo()

    if ( items.count()>0 ) {
        NCReportItem *item = qgraphicsitem_cast<NCReportItem*>(m_items.at(0));
        m_offset = item->scenePos() - item->selectPos();
        qDebug("NCRDMoveItemCommand: offset=%f,%f",m_offset.x(),m_offset.y());

    }
}

void NCRDMoveItemCommand::undo()
{
    moveBy( -m_offset );
    m_scene->slotSelectionChanged(); 	// UPDATE PROPEDIT
}

void NCRDMoveItemCommand::redo()
{
    moveBy( m_offset );
}

void NCRDMoveItemCommand::moveBy(const QPointF & by )
{
    for (int i = 0; i < m_items.size(); ++i) {
        NCReportItem *item = qgraphicsitem_cast<NCReportItem*>(m_items.at(i));
        if (item) {
            if ( !initial )
                item->setPos( item->scenePos() + by );
            //item->saveSelectPos();	// save the new position
            item->saveSelectState();
        }
    }
    initial = false;
}


bool NCRDMoveItemCommand::isValid()
{
    return !m_offset.isNull();
}


//-------------------------------------------------------------------------------------------------------

NCRDRemoveItemCommand::NCRDRemoveItemCommand( const QList<QGraphicsItem*>& items, NCRDSectionScene *scene, QUndoCommand * parent )
    : NCRDCommand(  0, scene, parent )
{
    m_items = items;
}

void NCRDRemoveItemCommand::undo()
{
    for (int i = 0; i < m_items.size(); ++i) {
        NCReportItem *item = qgraphicsitem_cast<NCReportItem*>(m_items.at(i));
        if (!item->isLocked())
            m_scene->addItem( item );
    }
    m_scene->emitContentChanged();
}

void NCRDRemoveItemCommand::redo()
{
    for (int i = 0; i < m_items.size(); ++i) {
        NCReportItem *item = qgraphicsitem_cast<NCReportItem*>(m_items.at(i));
        if (!item->isLocked())
            m_scene->removeItem(item);
    }
    m_scene->emitContentChanged();
}

//-------------------------------------------------------------------------------------------------------

NCRDResizeItemCommand::NCRDResizeItemCommand(NCReportItem * item, NCRDSectionScene * scene, QUndoCommand * parent)
    : NCRDCommand( item, scene, parent )
{
    initial = true;
    m_sizeChange = item->currentSize() - item->selectSize();
    m_offset = item->scenePos() - item->selectPos();

    qDebug("NCRDResizeItemCommand: NewSize: %f,%f OldSize: %f,%f Change: %f,%f",
           item->currentSize().width(),
            item->currentSize().height(),
            item->selectSize().width(),
            item->selectSize().height(),
            m_sizeChange.width(),
            m_sizeChange.height()
          );
}

void NCRDResizeItemCommand::undo()
{
    resizeBy( -1*m_sizeChange, -1*m_offset );
    m_scene->slotSelectionChanged(); 	// UPDATE PROPEDIT
}

void NCRDResizeItemCommand::redo()
{
    resizeBy( m_sizeChange, m_offset );
}

void NCRDResizeItemCommand::resizeBy(const QSizeF & bySize, const QPointF & byPos)
{
    if ( !initial ) {
        m_item->setPos( m_item->scenePos() + byPos );
        m_item->resize( m_item->currentSize() + bySize );
    }
    // save the new position and size
    m_item->saveSelectState();

    initial = false;
}


//-------------------------------------------------------------------------------------------------------

NCRDAlignItemCommand::NCRDAlignItemCommand(NCRDSectionScene * scene, QUndoCommand * parent)
    : NCRDCommand( 0, scene, parent )
{
}

void NCRDAlignItemCommand::addItem(NCReportItem *item, const QPointF& currentPos, const QPointF& newPos)
{
    itemPosHash.insert( item, AlignPos(currentPos, newPos) );
}

void NCRDAlignItemCommand::addItem(NCReportItem *item, const QSizeF & currentSize, const QSizeF & newSize)
{
    itemSizeHash.insert( item, AlignSize(currentSize, newSize) );
}

void NCRDAlignItemCommand::undo()
{
    align( OldPos );
    align( OldSize );
}

void NCRDAlignItemCommand::redo()
{
    align( NewPos );
    align( NewSize );
}

void NCRDAlignItemCommand::align( const AlignMode m )
{
    if ( m == NewPos || m == OldPos ) {
        QHash<NCReportItem*,AlignPos>::const_iterator i = itemPosHash.constBegin();
        while (i != itemPosHash.constEnd()) {
            NCReportItem *item = i.key();
            item->setPos( m == NewPos ? i.value().newPos : i.value().oldPos );
            ++i;
        }
    }
    if ( m == NewSize || m == OldSize ) {
        QHash<NCReportItem*,AlignSize>::const_iterator i = itemSizeHash.constBegin();
        while (i != itemSizeHash.constEnd()) {
            //qDebug("Align size...");
            NCReportItem *item = i.key();
            item->resize( m == NewSize ? i.value().newSize : i.value().oldSize );
            ++i;
        }
    }
}


//----------------------------------------------------------------------------------------------------------
NCRDLineItemChangeCommand::NCRDLineItemChangeCommand( NCReportItem* item, NCRDSectionScene *scene, QUndoCommand *parent )
    : NCRDCommand( item, scene, parent )
{
    initial = true;
    if ( item->objectType() != NCReportItem::Line )
        return;

    NCReportLineItem *lineItem = (NCReportLineItem*)item;
    m_oldLine = lineItem->selectLine();
    m_newLine = lineItem->line();
}

void NCRDLineItemChangeCommand::undo()
{
    setLine( m_oldLine );
    m_scene->slotSelectionChanged(); 	// UPDATE PROPEDIT
}

void NCRDLineItemChangeCommand::redo()
{
    setLine(m_newLine);
}

void NCRDLineItemChangeCommand::setLine(const QLineF & line)
{
    NCReportLineItem *lineItem = (NCReportLineItem*)m_item;
    if ( !initial )
        lineItem->setLine( line );

    // save the new position and size
    m_item->saveSelectState();
    initial = false;
}

