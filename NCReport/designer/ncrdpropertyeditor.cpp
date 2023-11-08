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
#include "ncrdpropertyeditor.h"
#include "ncrdmainwindow.h"
#include "ncrddesignsection.h"
#include "ncreportutils.h"

#include <QtCore/QVariant>
#include <QLabel>
#include <QTableView>
#include <QVBoxLayout>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QDoubleSpinBox>
#include <QHeaderView>

#define NCRDPROPERTYEDITOR_PROPCOUNT	7

#define NCRDPROPERTYEDITOR_SECTION_TYPE		0
#define NCRDPROPERTYEDITOR_SECTION_HEIGHT	1
#define NCRDPROPERTYEDITOR_ITEM_NAME		2
#define NCRDPROPERTYEDITOR_ITEM_POSX		3
#define NCRDPROPERTYEDITOR_ITEM_POSY		4
#define NCRDPROPERTYEDITOR_ITEM_WIDTH		5
#define NCRDPROPERTYEDITOR_ITEM_HEIGHT		6
#define NCRDPROPERTYEDITOR_ITEM_ZVALUE		7


NCRDPropertyEditor::NCRDPropertyEditor(NCRDMainWindow * parent, Qt::WindowFlags flags)
 : QDockWidget(parent,flags), m_mainwin( parent )
{
    m_item = 0;
    m_InternalChange = false;

    if (objectName().isEmpty())
        setObjectName(QString::fromUtf8("NCRDPropertyEditor"));
    setWindowTitle( tr("Geometry editor") );
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);


    dockWidgetContents = new QWidget(this);
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    vboxLayout = new QVBoxLayout(dockWidgetContents);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    /*
    label = new QLabel(dockWidgetContents);
    label->setObjectName(QString::fromUtf8("label"));
    vboxLayout->addWidget(label);
    */

    tableView = new QTableView(dockWidgetContents);
    tableView->setObjectName(QString::fromUtf8("tableView"));

    vboxLayout->addWidget(tableView);

    setWidget(dockWidgetContents);

    tableView->setAlternatingRowColors( true );
    tableView->verticalHeader()->setDefaultSectionSize( 21 );
    setupModel();
    tableView->setModel(model);
    tableView->verticalHeader()->hide();
    //tableView->setColumnHidden( 4, true );
    //tableView->setColumnWidth( 1, 200 );

    NCRDPropertyEditorItemDelegate *deleg= new NCRDPropertyEditorItemDelegate( this );
    tableView->setItemDelegate( deleg );

    setSectionPropsEnabled( false );
    setItemPropsEnabled( false );
}


NCRDPropertyEditor::~NCRDPropertyEditor()
{
}

void NCRDPropertyEditor::setDocument(NCRDDocument *doc)
{
    m_doc = doc;
    //connect( doc, SIGNAL(destroyed(QObject*)), this, SLOT(documentClosed(QObject*)) );

}

void NCRDPropertyEditor::setSection(NCRDDesignSection *sect)
{
    if ( m_InternalChange )
        return;

    m_section = sect;
    m_InternalChange = true;
    setSectionPropsEnabled( sect!=0 );
    if ( sect ) {
        setSectionType( sect->sectionScene()->caption() );
        setSectionHeight( sect->sectionScene()->metricHeight() );
    } else {
        setSectionType( "-" );
        setSectionHeight( 0.0 );
    }
    m_InternalChange = false;

/*	if ( m_section->sectionScene()->selectedItems().count() == 0 ) {
        setItemPropsEnabled( false );
    } else {
        QList<QGraphicsItem*> list = sect->sectionScene()->selectedItems();
        NCReportItem *firstItem = qgraphicsitem_cast<NCReportItem*>(m_section->sectionScene()->selectedItems().first() );
        setItem( firstItem );
        setItemPropsEnabled( true );
    }*/
}

void NCRDPropertyEditor::setItem(NCReportItem *item)
{
    if ( m_InternalChange )
        return;

    m_item = item;
    m_InternalChange = true;
    setItemPropsEnabled( item!=0 );
    if ( item ) {
        item->updateForSave();	// update metric info by editor
        setItemName( item->tagname() );
        setItemSize( item->metricSize() );
        setItemPosition( item->metricPos() );
        setItemZValue( item->zValue() );
    } else {
        setItemName( "-" );
        setItemSize( QSizeF(0.0,0.0) );
        setItemPosition( QPointF(0.0,0.0) );
        setItemZValue( 0 );
    }
    m_InternalChange = false;
}

void NCRDPropertyEditor::setupModel()
{
    model = new QStandardItemModel( NCRDPROPERTYEDITOR_PROPCOUNT, 2, this);
    connect( model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(itemChanged(QStandardItem*)) );

    QStringList l;
    l << tr("Property") << tr("Value");
    model->setHorizontalHeaderLabels ( l );

    QStandardItem *item =0;
    //------------------- Object type -----------------------
    item = new QStandardItem( tr("Section type") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_SECTION_TYPE, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_SECTION_TYPE, 1, item);
    //------------------- Section height -----------------------
    item = new QStandardItem( tr("Section height") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_SECTION_HEIGHT, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(true);
    item->setData( 0.0, Qt::EditRole );
    item->setTextAlignment(Qt::AlignRight);
    model->setItem( NCRDPROPERTYEDITOR_SECTION_HEIGHT, 1, item);
    //------------------- Object name -----------------------
    item = new QStandardItem( tr("Object name") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_NAME, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_NAME, 1, item);
    //------------------- X -----------------------
    item = new QStandardItem( tr("X position") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_POSX, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(true);
    item->setData( 0.0, Qt::EditRole );
    item->setTextAlignment(Qt::AlignRight);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_POSX, 1, item);
    //------------------- Y -----------------------
    item = new QStandardItem( tr("Y position") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_POSY, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(true);
    item->setData( 0.0, Qt::EditRole );
    item->setTextAlignment(Qt::AlignRight);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_POSY, 1, item);
    //------------------- WIDTH -----------------------
    item = new QStandardItem( tr("Width") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_WIDTH, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(true);
    item->setData( 0.0, Qt::EditRole );
    item->setTextAlignment(Qt::AlignRight);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_WIDTH, 1, item);
    //------------------- HEIGHT -----------------------
    item = new QStandardItem( tr("Height") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_HEIGHT, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(true);
    item->setData( 0.0, Qt::EditRole );
    item->setTextAlignment(Qt::AlignRight);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_HEIGHT, 1, item);
    //------------------- ZVALUE -----------------------
    item = new QStandardItem( tr("Z-value") );
    item->setEditable(false);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_ZVALUE, 0, item);
    item = new QStandardItem( QString::null );
    item->setEditable(true);
    item->setData( 0, Qt::EditRole );
    item->setTextAlignment(Qt::AlignRight);
    model->setItem( NCRDPROPERTYEDITOR_ITEM_ZVALUE, 1, item);
}

void NCRDPropertyEditor::itemChanged(QStandardItem * item)
{
    if ( m_InternalChange || !m_section || !m_section->sectionScene() )
        return;

    m_InternalChange = true;
    switch ( item->index().row() ) {
        case NCRDPROPERTYEDITOR_SECTION_HEIGHT:
            if ( m_section ) {
                m_section->setHeightMM( item->data( Qt::EditRole ).toDouble() );
                if ( m_doc )
                    m_doc->adjustContent();
            }
            break;
        case NCRDPROPERTYEDITOR_ITEM_POSX:
            if ( m_item ) {
                double newPosX = item->data( Qt::EditRole ).toDouble();
                m_item->setMetricX( newPosX );
                m_item->updatePosition();
                m_section->sectionScene()->moveItemCommand( m_item );
            }
            break;
        case NCRDPROPERTYEDITOR_ITEM_POSY:
            if ( m_item ) {
                m_item->setMetricY( item->data( Qt::EditRole ).toDouble() );
                m_item->updatePosition();
                m_section->sectionScene()->moveItemCommand( m_item );
            }
            break;
        case NCRDPROPERTYEDITOR_ITEM_WIDTH:
            if ( m_item ) {
                m_item->setMetricWidth( item->data( Qt::EditRole ).toDouble() );
                m_item->updateSize();
                m_section->sectionScene()->resizeItemCommand( m_item );
            }
            break;
        case NCRDPROPERTYEDITOR_ITEM_HEIGHT:
            if ( m_item ) {
                m_item->setMetricHeight( item->data( Qt::EditRole ).toDouble() );
                m_item->updateSize();
                m_section->sectionScene()->resizeItemCommand( m_item );
            }
            break;
        case NCRDPROPERTYEDITOR_ITEM_ZVALUE:
            if ( m_item ) {
                m_item->setZValue( item->data( Qt::EditRole ).toInt() );
            }
            break;
    }
    m_InternalChange = false;
}

void NCRDPropertyEditor::setSectionType(const QString & type )
{
    model->item( NCRDPROPERTYEDITOR_SECTION_TYPE, 1 )->setText( type );
}

void NCRDPropertyEditor::setItemName(const QString & name )
{
    model->item( NCRDPROPERTYEDITOR_ITEM_NAME, 1 )->setText( name );
}

void NCRDPropertyEditor::setItemSize(const QSizeF & size )
{
    model->item( NCRDPROPERTYEDITOR_ITEM_WIDTH, 1 )->setData( displayNumber(size.width()), Qt::EditRole );
    model->item( NCRDPROPERTYEDITOR_ITEM_HEIGHT, 1 )->setData( displayNumber(size.height()), Qt::EditRole );
}

void NCRDPropertyEditor::setItemPosition(const QPointF & pos )
{
    model->item( NCRDPROPERTYEDITOR_ITEM_POSX, 1 )->setData( displayNumber(pos.x()), Qt::EditRole );
    model->item( NCRDPROPERTYEDITOR_ITEM_POSY, 1 )->setData( displayNumber(pos.y()), Qt::EditRole );
}

void NCRDPropertyEditor::setItemZValue(int value)
{
    model->item( NCRDPROPERTYEDITOR_ITEM_ZVALUE, 1 )->setData( value, Qt::EditRole );
}

QString NCRDPropertyEditor::displayNumber(qreal value) const
{
    return QString("%1").arg( NCReportUtils::fround(value,4), 0, 'f', 4 );
}


void NCRDPropertyEditor::setSectionHeight( qreal h )
{
    model->item( NCRDPROPERTYEDITOR_SECTION_HEIGHT, 1 )->setData( NCReportUtils::fround(h,4), Qt::EditRole );
}

void NCRDPropertyEditor::setItemPropsEnabled(bool set)
{
    model->item( NCRDPROPERTYEDITOR_ITEM_NAME, 0 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_POSX, 0 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_POSY, 0 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_WIDTH, 0 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_HEIGHT, 0 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_ZVALUE, 0 )->setEnabled( set );

    model->item( NCRDPROPERTYEDITOR_ITEM_NAME, 1 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_POSX, 1 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_POSY, 1 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_WIDTH, 1 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_HEIGHT, 1 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_ITEM_ZVALUE, 1 )->setEnabled( set );
}

void NCRDPropertyEditor::setSectionPropsEnabled(bool set)
{
    model->item( NCRDPROPERTYEDITOR_SECTION_TYPE, 0 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_SECTION_HEIGHT, 0 )->setEnabled( set );

    model->item( NCRDPROPERTYEDITOR_SECTION_TYPE, 1 )->setEnabled( set );
    model->item( NCRDPROPERTYEDITOR_SECTION_HEIGHT, 1 )->setEnabled( set );
}

bool NCRDPropertyEditor::isNumericRow( int row ) const
{
    bool is = ( row == NCRDPROPERTYEDITOR_SECTION_HEIGHT ||
            row == NCRDPROPERTYEDITOR_ITEM_POSY ||
            row == NCRDPROPERTYEDITOR_ITEM_POSY ||
            row == NCRDPROPERTYEDITOR_ITEM_WIDTH ||
            row == NCRDPROPERTYEDITOR_ITEM_HEIGHT );
    return is;
}

NCRDDocument * NCRDPropertyEditor::document()
{
    return m_doc;
}





//------------------------------------------------------------------------------
// NCRDPropertyEditorItemDelegate
//------------------------------------------------------------------------------
NCRDPropertyEditorItemDelegate::NCRDPropertyEditorItemDelegate(NCRDPropertyEditor * parent)
    : QItemDelegate( parent ), pEditor( parent ), m_row(0), m_col(0)
{
}

QWidget * NCRDPropertyEditorItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if ( index.column() == 1 && isMetricProperty(index.row()) ) {
        QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
        editor->setMinimum(0.0);
        editor->setMaximum(5000.0);
        editor->setSingleStep(0.1);
        editor->setDecimals(4);
        //editor->setSuffix(" mm");
        return editor;
    }

    return QItemDelegate::createEditor( parent, option, index);
}

void NCRDPropertyEditorItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QDoubleSpinBox *sp = qobject_cast<QDoubleSpinBox*>(editor);
    if ( sp )
        sp->setValue( index.model()->data(index, Qt::EditRole).toDouble() );
    else
        QItemDelegate::setEditorData( editor, index);
}

void NCRDPropertyEditorItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    QDoubleSpinBox *sp = qobject_cast<QDoubleSpinBox*>(editor);
    if ( sp ) {
        sp->interpretText();
        model->setData(index, sp->value(), Qt::EditRole);
    } else
        QItemDelegate::setModelData( editor, model, index );

    if (pEditor->document())
        pEditor->document()->setModified(true);
}

void NCRDPropertyEditorItemDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QItemDelegate::updateEditorGeometry( editor, option, index );
}

void NCRDPropertyEditorItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QItemDelegate::paint( painter, option, index );
}

void NCRDPropertyEditorItemDelegate::drawDisplay(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, const QString & text) const
{
    bool ok=true;
    double val = text.toDouble( &ok );
    if ( ok )
        QItemDelegate::drawDisplay( painter, option, rect, QString("%L1").arg( val, 0, 'f', 4 ) );
    else
        QItemDelegate::drawDisplay( painter, option, rect, text );
}

bool NCRDPropertyEditorItemDelegate::isMetricProperty(int row) const
{
    return (row!= NCRDPROPERTYEDITOR_ITEM_ZVALUE);
}

