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
#include "ncrddetaildialog.h"
#include "ncrdgroupdialog.h"
#include "ncrddocument.h"
#include "ncrddef.h"
#include "ncreportdatasource.h"
#include "ncreportsection.h"
#include "ncrdformgeometry.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QHash>
#include <QMessageBox>
#include <QStringListModel>

//! 多语言
#include "MDMultiLanguage.h"

NCRDDetailDialog::NCRDDetailDialog(NCRDDocument *doc, NCReportSection *default_detail, QWidget* parent)
    : QDialog( parent ), Ui::NCRDDetailDialogUI(),
    m_model(0),
//    m_modelDataSource(0),
//    m_modelDataSource2(0),
    m_mapper(0),
    m_document(doc),
    m_reorderRequired(false),
    m_changed(false)
{
    setupUi(this);
    setupModel();
    listView->setModel(m_model);

    //DATASOURCE COMBO
    comboDataSource->addItems( m_document->reportDef()->dataSourceIdList() );
    comboDataSource2->addItems( m_document->reportDef()->dataSourceIdList() );
    comboDataSource2->addItem("");

    //MAPPER
    NCRDDetailItemDelegate *deleg = new NCRDDetailItemDelegate( this );


//    comboDataSource->setModel(m_modelDataSource);
//    comboDataSource2->setModel(m_modelDataSource2);

    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(m_model);
    m_mapper->setItemDelegate( deleg );
    m_mapper->addMapping(leDetailID, Id);
    m_mapper->addMapping(spHeight, Height);
    m_mapper->addMapping(comboDataSource, DataSource );
    m_mapper->addMapping(comboDataSource2, DataSource2 );
    m_mapper->addMapping(pageBreakCond, PageBreakCondition);
    m_mapper->addMapping(cbNewPage, StartOnNewPage);
    m_mapper->addMapping(comboAlternateMode, AlternateRowMode, "currentIndex");
    m_mapper->addMapping(cbAlternateDefaultColor, AlternateRowIsDefaultColor);
    m_mapper->addMapping(alternateColor, AlternateRowColor);
    m_mapper->toFirst();

    connect( listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));

    connect(panelButtons->btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(panelButtons->btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(panelButtons->btnUp, SIGNAL(clicked()), this, SLOT(up()));
    connect(panelButtons->btnDown, SIGNAL(clicked()), this, SLOT(down()));
    connect(btnGrouping, SIGNAL(clicked()), this, SLOT(doGrouping()));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(apply(QAbstractButton*)) );

    if (default_detail)
        setCurrentDetail(default_detail);
    else
        if (m_model->rowCount()>0)
            listView->setCurrentIndex(m_model->index(0,0));
        //mapper->toFirst();

    NCRDFormGeometry *fg = new NCRDFormGeometry(this,splitter);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDDetailDialog::~NCRDDetailDialog()
{
}

void NCRDDetailDialog::doGrouping()
{
    if ( m_model->rowCount() == 0 )
        return;

    int row = m_mapper->currentIndex();
    NCRDStandardPointerItem *item = (NCRDStandardPointerItem*)(m_model->item( row, Pointer ));
    NCReportSection *detail = item->section();

    NCRDGroupDialog dialog( m_document, detail, this);
    switch (dialog.exec()) {
        case QDialog::Accepted:
            dialog.applyChanges();
            break;
        case QDialog::Rejected:
            dialog.discardChanges();
            break;
    }
}

void NCRDDetailDialog::setupModel()
{
//    QStringList dslist = m_document->reportDef()->dataSourceIdList();
//    m_modelDataSource = new QStringListModel(dslist, this);
//    dslist << QString("");
//    m_modelDataSource2 = new QStringListModel(dslist, this);

    int cnt = m_document->reportDef()->details().count();
    m_model = new QStandardItemModel( cnt, ColumnCount, this);

    for ( int i=0; i<cnt; ++i ) {
        NCReportSection *detail = m_document->reportDef()->details().at(i);
        appendModel( detail, i, false );
    }
    connect(m_model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onChanged()));
    m_changed = false;
}

void NCRDDetailDialog::appendModel( NCReportSection *detail, int row, bool inew )
{
    QStandardItem *item =0;
    // ID
    item = new QStandardItem( detail->id() );
    m_model->setItem( row, Id, item);
    // height
    item = new QStandardItem( QString::number(detail->metricHeight()) );
    m_model->setItem( row, Height, item);
    // datasource
    item = new QStandardItem(detail->dataSourceId());
    m_model->setItem( row, DataSource, item);
    // page break expr
    item = new QStandardItem(detail->pageBreakExp());
    m_model->setItem( row, PageBreakCondition, item);

    item = new QStandardItem( detail->startsOnNewPage() ? "true" : "false" );
    m_model->setItem( row, StartOnNewPage, item);

    // detail pointer
    item = new NCRDStandardPointerItem( detail, inew );
    m_model->setItem( row, Pointer, item);

    // datasource2
    item = new QStandardItem(detail->secondaryDatasourceID());
    m_model->setItem( row, DataSource2, item);

    item = new QStandardItem( QString::number(detail->alternateRowBackgroundMode()) );
    m_model->setItem( row, AlternateRowMode, item);

    item = new QStandardItem( detail->useDefaultAlternateRowColor()  ? "true" : "false" );
    m_model->setItem( row, AlternateRowIsDefaultColor, item);

    item = new QStandardItem( detail->alternateRowBackgroundColor().name());
    m_model->setItem( row, AlternateRowColor, item);

    debugModelValues(row, "Create model");
}


void NCRDDetailDialog::add()
{
    NCReportSection *detail = m_document->reportDef()->addDetail();
    detail->setMetricHeight(10.0);
    //document->updateSectionsByReportDef();
    qDebug("Add detail %p",detail);
    int nextID = m_model->rowCount();
    appendModel( detail, nextID, true );

    listView->setCurrentIndex( m_model->index(nextID,Id) );
    m_changed = true;
}

void NCRDDetailDialog::remove()
{
    if ( m_model->rowCount() == 1 )	// must have at least 1 detail
        return;

    int row = m_mapper->currentIndex();
    NCRDStandardPointerItem *item = (NCRDStandardPointerItem*)(m_model->item( row, Pointer ));
    NCReportSection *detail = item->section();

    if ( item->insertedAsNew() ) {
        // nothing else to do
    } else {
        switch ( QMessageBox::warning( this, tr("Remove warning"),
                                       tr("Are you sure to remove an already existed detail section:\n%1?").arg( detail->caption() ),
            QMessageBox::Yes | QMessageBox::No ) ) {

            case QMessageBox::Yes:
                m_removedDetails.append( detail );
                break;
            default:
                return;
        }
    }
    m_model->removeRow( row );
    m_changed = true;
}

void NCRDDetailDialog::up()
{
    if ( m_model->rowCount() == 1 )	// must have at least 1 detail
        return;

    int row = m_mapper->currentIndex();

    QList<QStandardItem *> items = m_model->takeRow(row);
    m_model->insertRow(row - 1, items );
    listView->setCurrentIndex( m_model->index(row-1,Id) );
    m_reorderRequired = true;
    m_changed = true;
}

void NCRDDetailDialog::down()
{
    int row = m_mapper->currentIndex();
    if (row == -1 || row == m_model->rowCount() - 1)
        return; // nothing to do

    QList<QStandardItem *> items = m_model->takeRow(row);
    m_model->insertRow(row + 1, items );
    listView->setCurrentIndex( m_model->index(row+1,Id) );
    m_reorderRequired = true;
    m_changed = true;
}

void NCRDDetailDialog::slotItemChanged(const QModelIndex & current, const QModelIndex &)
{
    m_mapper->setCurrentModelIndex( current );
}

void NCRDDetailDialog::onChanged()
{
    m_changed = true;
    debugModelValues(m_mapper->currentIndex(), "Model changed.");
}

bool NCRDDetailDialog::check()
{
    if (!m_changed)
        return true;

    for (int row = 0; row <  m_model->rowCount(); ++row) {

        if (m_model->item(row,DataSource)->text() == m_model->item(row,DataSource2)->text()) {
            QMessageBox::warning( this, tr("Invalid data"), tr("Primary and secondary data source must be different.") );
            return false;
        }
    }

    return true;
}

void NCRDDetailDialog::debugModelValues( int row, const QString &label)
{
#ifdef QT_DEBUG
    qDebug("------------------------");
    for (int col=0; col<m_model->columnCount(); col++)
        qDebug("%s Row:%i Col:%i = %s", qPrintable(label), row, col, qPrintable(m_model->item(row,col)->text()));
#else
    Q_UNUSED(label)
#endif
    /*
    int row = m_mapper->currentIndex();
    qDebug("DataSource = %s", qPrintable(m_model->item(row,DataSource)->text()));
    qDebug("setUseDefaultAlternateRowColor = %s", qPrintable(m_model->item(row,AlternateRowIsDefaultColor)->text()));
    qDebug("AlternateRowMode = %s", qPrintable(m_model->item(row,AlternateRowMode)->text()));
    qDebug("AlternateRowColor = %s", qPrintable(m_model->item(row,AlternateRowColor)->text()));
    */
}

void NCRDDetailDialog::applyChanges()
{
    if (!m_changed)
        return;
    m_document->reportDef()->clearDetails(); // remove details because of re-order

    for (int row = 0; row <  m_model->rowCount(); ++row) {

        debugModelValues(row, "Apply changes.");

        NCRDStandardPointerItem *sItem = (NCRDStandardPointerItem*)(m_model->item( row, Pointer ));
        //NCRDSectionScene *detail = (NCRDSectionScene *)sItem->section();
        NCReportSection *detail = sItem->section();

        detail->setID( m_model->item(row,Id)->text() );
        detail->setMetricHeight( m_model->item(row,Height)->text().toDouble() );

        detail->setDataSourceId( m_model->item(row,DataSource)->text() );
        detail->setSecondaryDatasourceID( m_model->item(row,DataSource2)->text() );
        detail->setPageBreakExp( m_model->item(row,PageBreakCondition)->text() );
        detail->setStartsOnNewPage( (m_model->item(row,StartOnNewPage)->text() == "true") );

        detail->setUseDefaultAlternateRowColor( m_model->item(row,AlternateRowIsDefaultColor)->text() == "true");
        detail->setAlternateRowBackgroundMode( (NCReportSection::AlternateRowMode)m_model->item(row,AlternateRowMode)->text().toInt());
        detail->setAlternateRowBackgroundColor(m_model->item(row,AlternateRowColor)->text());

        m_document->reportDef()->appendDetails(detail);	// put back details in right order.
        sItem->setInsertedAsNew(false);	 //set as applied
    }

    for (int i=0; i<m_removedDetails.count(); ++i)
        removeDetail(m_removedDetails.at(i));

    if (m_reorderRequired) {
        m_document->reportDef()->reindexDetails();
    }
    m_document->updateSectionsByReportDef();
    m_document->setDocumentModified();
    m_changed = false;
}

void NCRDDetailDialog::discardChanges()
{
    return;

    if (!m_changed)
        return;

    switch ( QMessageBox::warning( this, tr("Undo warning"), tr("Do you want to undo last changes?"),
        QMessageBox::Yes | QMessageBox::No ) )
    {
    case QMessageBox::Yes:
        undoChanges();
        break;
    case QMessageBox::No:
    default:
        break;
    }
}

void NCRDDetailDialog::removeDetail(NCReportSection *detail)
{
    qDebug("Remove detail %p",detail);
    m_document->removeGroupSections( detail );
    m_document->removeSection( detail );
    m_document->reportDef()->removeDetail( detail );
}

void NCRDDetailDialog::undoChanges()
{
    bool doUpdate=false;
    for (int row = 0; row <  m_model->rowCount(); ++row) {
        NCRDStandardPointerItem *sItem = (NCRDStandardPointerItem*)(m_model->item( row, Pointer ));
        if ( sItem->insertedAsNew() ) {
            removeDetail(sItem->section());
            doUpdate = true;
        }
    }
    if (doUpdate)
        m_document->updateSectionsByReportDef();
}

QComboBox * NCRDDetailDialog::comboDS()
{
    return comboDataSource;
}

QComboBox *NCRDDetailDialog::comboDS2()
{
    return comboDataSource2;
}

QComboBox *NCRDDetailDialog::comboAltMode()
{
    return comboAlternateMode;
}

void NCRDDetailDialog::apply(QAbstractButton* btn)
{
    if ( buttonBox->standardButton(btn)==QDialogButtonBox::Apply ) {
        if (check())
            applyChanges();
    }
}

void NCRDDetailDialog::setCurrentDetail( NCReportSection* detail )
{
    // locate the group pointer
    for (int row = 0; row < m_model->rowCount(); ++row) {
        NCRDStandardPointerItem *pItem = (NCRDStandardPointerItem*)(m_model->item( row, Pointer ));
        if ( pItem->section() == detail ) {
            listView->setCurrentIndex( m_model->index(row,0) );
        }
    }
}

void NCRDDetailDialog::setChanged(bool set)
{
    m_changed = set;
}

void NCRDDetailDialog::accept()
{
    if (check())
        QDialog::accept();
}


//------------------------------------------------------------------------------
// NCRDDataSourceItemDelegate
//------------------------------------------------------------------------------
NCRDDetailItemDelegate::NCRDDetailItemDelegate( NCRDDetailDialog *parent )
    : QItemDelegate( parent ), dialog(parent)
{
}

QWidget * NCRDDetailItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    return QItemDelegate::createEditor( parent, option, index);

//    switch (index.column()) {
//    case NCRDDetailDialog::DataSource: return dialog->comboDS();
//    case NCRDDetailDialog::DataSource2:return dialog->comboDS2();
//    case NCRDDetailDialog::AlternateRowMode: return dialog->comboAltMode();
//    default: return QItemDelegate::createEditor( parent, option, index);
//    }
}

void NCRDDetailItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    switch (index.column()) {
    case NCRDDetailDialog::DataSource:
    case NCRDDetailDialog::DataSource2:
    {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            combo->setCurrentIndex(combo->findText(index.model()->data(index, Qt::EditRole).toString()));
            //combo->setCurrentIndex( index.data(Qt::EditRole).toInt() );
        }
        break;
    }
    default:
        QItemDelegate::setEditorData( editor, index);
        break;
    }
}

void NCRDDetailItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    switch (index.column()) {
    case NCRDDetailDialog::DataSource:
    case NCRDDetailDialog::DataSource2:
    {
        QComboBox *combo = qobject_cast<QComboBox*>(editor);
        if ( combo ) {
            model->setData( index, combo->currentText() );
            //model->setData( index, combo->currentIndex() );
        }
        break;
    }
    default:
        QItemDelegate::setModelData( editor, model, index);
        break;
    }
}
