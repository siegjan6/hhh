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
#include "ncrdvariabledialog.h"
#include "ncrddocument.h"
#include "ncrddef.h"
#include "ncreportvariable.h"
#include "ncrdformgeometry.h"
#include "ncrdmainwindow.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QHash>
#include <QHeaderView>
//#include <QMessageBox>

//! 多语言
#include "MDMultiLanguage.h"

#define NCRDVARIABLEDIALOG_POINTER_COLUMN	5

NCRDVariableDialog::NCRDVariableDialog(NCRDDocument *doc, QWidget* parent)
    : QDialog( parent ), Ui::NCRDVariableDialogUI(), document( doc )
{
    setupUi(this);
    tableView->setAlternatingRowColors( true );
    tableView->verticalHeader()->setDefaultSectionSize( 20 );
    setupModel();
    tableView->setModel(m_model);
    tableView->setColumnHidden( 4, true );
    tableView->setColumnHidden( 5, true );
    tableView->setColumnWidth( 1, 200 );


    //ITEm DELEGATE
    NCRDVariableItemDelegate *delegTable = new NCRDVariableItemDelegate( this, true );
    tableView->setItemDelegate( delegTable );


    NCRDVariableItemDelegate *deleg = new NCRDVariableItemDelegate( this, false );
    deleg->setComboBoxes( comboFunc, comboScope );
    //MAPPER
    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(m_model);
    m_mapper->setItemDelegate( deleg );
    m_mapper->addMapping(variableID, 0);
    m_mapper->addMapping(expression, 1);
    m_mapper->addMapping(comboFunc, 2);
    m_mapper->addMapping(comboScope, 3);
    m_mapper->addMapping(initValue, 4);

    connect( tableView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));

    connect(btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(btnRemove, SIGNAL(clicked()), this, SLOT(remove()));

    m_mapper->toFirst();
    updateEnable();
    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDVariableDialog::~NCRDVariableDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDVariableDialog::setupModel()
{
    int cnt = document->reportDef()->variables().count();
    m_model = new QStandardItemModel( cnt, 6, this);

    QStringList l;
    l << tr("ID") << tr("Expression") << tr("Function") << tr("Scope") << tr("Init.val.") ;
    m_model->setHorizontalHeaderLabels ( l );

    int row=0;
    QHash<QString, NCReportVariable*>::const_iterator i = document->reportDef()->variables().constBegin();
    while (i != document->reportDef()->variables().constEnd()) {
        NCReportVariable* var = i.value();
        //tableView->setRowHeight( row, 19 );
        //tableView->resizeRowToContents( row );
        appendModel( var, row++, false );
        ++i;
    }

    m_model->sort(0);
}

void NCRDVariableDialog::appendModel(NCReportVariable *var, int row, bool flagNew)
{
    QStandardItem *item =0;
    // ID
    item = new QStandardItem( var->id() );
    m_model->setItem( row, 0, item);
    // Expression
    item = new QStandardItem( var->expression() );
    m_model->setItem( row, 1, item);
    // function
    item = new QStandardItem( var->functionToName() );
    m_model->setItem( row, 2, item);
    // scope
    item = new QStandardItem( var->scopeToName() );
    m_model->setItem( row, 3, item);
    // initvalue
    item = new QStandardItem( QString::number(var->initValue()) );
    m_model->setItem( row, 4, item);

    item = new NCRDVariableItem( var, flagNew );
    m_model->setItem( row, NCRDVARIABLEDIALOG_POINTER_COLUMN, item);
}

void NCRDVariableDialog::add()
{
    NCReportVariable *var = new NCReportVariable();
    int nextID = m_model->rowCount();
    var->setID( tr("var%1").arg(nextID) );

    appendModel( var, nextID, true );

    updateEnable();
    tableView->setCurrentIndex( m_model->index(nextID,0) );
    variableID->setFocus();
    variableID->selectAll();

}

void NCRDVariableDialog::remove()
{
    if ( m_model->rowCount() == 0 )
        return;

    int row = m_mapper->currentIndex();
    NCRDVariableItem *vItem = (NCRDVariableItem*)(m_model->item( row, NCRDVARIABLEDIALOG_POINTER_COLUMN ));
    NCReportVariable *var = vItem->variable();

    if ( vItem->flagNew() ) {
        qDebug("Remove variable %p",var);
        delete var;
    } else {
        removeVars.append( var );
    }
    m_model->removeRow( row );
    updateEnable();

}

void NCRDVariableDialog::slotItemChanged(const QModelIndex & current, const QModelIndex &)
{
    m_mapper->setCurrentModelIndex( current );
}

void NCRDVariableDialog::applyChanges()
{
    for (int row = 0; row <  m_model->rowCount(); ++row) {
        NCRDVariableItem *vItem = (NCRDVariableItem*)(m_model->item( row, NCRDVARIABLEDIALOG_POINTER_COLUMN ));
        NCReportVariable *var = vItem->variable();

        var->setID( m_model->item(row,0)->text() );
        var->setExpression( m_model->item(row,1)->text() );
        var->setFunction( m_model->item(row,2)->text() );
        var->setScope( m_model->item(row,3)->text() );
        var->setInitValue( m_model->item(row,4)->text().toDouble() );

        if ( vItem->flagNew() )
            document->reportDef()->addVariable( var );
    }
    for (int i=0; i<removeVars.count(); ++i) {
        NCReportVariable *var = removeVars.at(i);
        qDebug("Remove variable %p",var);
        document->reportDef()->removeVariable( var );
    }
    document->setDocumentModified();
    document->updateDataSourceTree(document->mainWindow()->dataSourceTree(), false,true,false,true);
}

void NCRDVariableDialog::discardChanges()
{
    for (int row = 0; row <  m_model->rowCount(); ++row) {
        NCRDVariableItem *vItem = (NCRDVariableItem*)(m_model->item( row, NCRDVARIABLEDIALOG_POINTER_COLUMN ));
        NCReportVariable *var = vItem->variable();
        if ( vItem->flagNew() ) {
            qDebug("(discard) Remove variable %p",var);
            delete var;
        }
    }
}

void NCRDVariableDialog::setCurrentVariable(const QString &varID)
{
    if (varID.isEmpty())
        return;
    QModelIndexList indexList = m_model->match(m_model->index(0,0), Qt::EditRole, varID, 1, Qt::MatchExactly);
    if (indexList.count()>=1) {
        tableView->setCurrentIndex(indexList.at(0));
    }
}

void NCRDVariableDialog::updateEnable()
{
    bool enable = (m_model->rowCount() > 0);
    gbProp->setEnabled( enable );
}



//------------------------------------------------------------------------------
// NCRDVariableItemDelegate
//------------------------------------------------------------------------------
NCRDVariableItemDelegate::NCRDVariableItemDelegate( NCRDVariableDialog *parent, bool intable )
    : QItemDelegate( parent ), dialog(parent), useInTableView( intable )
{
    comboboxCol2 =0;
    comboboxCol3 =0;
}

QWidget * NCRDVariableItemDelegate::createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    if ( index.column() == 2 ) {
        if ( useInTableView ) {
            ((NCRDVariableItemDelegate*)this)->comboboxCol2 = new QComboBox( parent );
            comboboxCol2->insertItems(0, QStringList() << "COUNT" << "SUM" << "STD" << "AVERAGE" << "MIN" << "MAX" );
        }
        return comboboxCol2;
    } else if ( index.column() == 3 ) {
        if ( useInTableView ) {
            ((NCRDVariableItemDelegate*)this)->comboboxCol3 = new QComboBox( parent );
            comboboxCol3->insertItems(0, QStringList() << "Report" << "Page" << "Group"  );
        }
        return comboboxCol3;
    } else {
        return QItemDelegate::createEditor( parent, option, index);
    }
}

void NCRDVariableItemDelegate::setEditorData(QWidget * editor, const QModelIndex & index) const
{
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    if ( combo ) {
        //combo->setCurrentIndex( index.model()->data(index, Qt::EditRole).toInt());
        combo->setCurrentIndex(combo->findText(index.model()->data(index, Qt::EditRole).toString()));
    } else {
        QItemDelegate::setEditorData( editor, index);
    }
}

void NCRDVariableItemDelegate::setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const
{
    QComboBox *combo = qobject_cast<QComboBox*>(editor);
    if ( combo ) {
        //model->setData( index, combo->currentIndex() );
        model->setData( index, combo->currentText() );
    } else {
        QItemDelegate::setModelData( editor, model, index);
    }
}

void NCRDVariableItemDelegate::updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QItemDelegate::updateEditorGeometry( editor, option, index );
}

void NCRDVariableItemDelegate::setComboBoxes(QComboBox * combo1, QComboBox * combo2)
{
    comboboxCol2 = combo1;
    comboboxCol3 = combo2;
}

// void NCRDVariableItemDelegate::drawDisplay(QPainter * painter, const QStyleOptionViewItem & option, const QRect & rect, const QString & text) const
// {
// }



