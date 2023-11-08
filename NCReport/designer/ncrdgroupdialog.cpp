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
#include "ncrdgroupdialog.h"
#include "ncrddetaildialog.h"
#include "ncrddef.h"
#include "ncrddocument.h"
#include "ncreportsection.h"
#include "ncreportgroup.h"
#include "ncreportvariable.h"
#include "ncrdformgeometry.h"
#include "ncrdexpressionbuilderdialog.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QHash>
#include <QMessageBox>
#include <QCompleter>

//! 多语言
#include "MDMultiLanguage.h"

#define	COL_ID					0
#define	COL_EXP					1
#define	COL_HEADER				2
#define	COL_FOOTER				3
#define	COL_REPRINT				4
#define	COL_HEADERHEIGHT		5
#define	COL_FOOTERHEIGHT		6
#define	COL_RESETVARS			7
#define	COL_POINTER				8
#define	COL_STARTSONNEWPAGE		9
#define	COL_BREAKCOND           10

#define	COL_COUNT				11


NCRDGroupDialog::NCRDGroupDialog(NCRDDocument *doc, NCReportSection *detail, QWidget* parent)
: QDialog( parent ), Ui::NCRDGroupDialogUI(), m_document(doc), m_detail( detail )
{
    setupUi(this);
    setWindowTitle( tr("Data grouping [%1]").arg( detail->id() ) );
    listVars->setSortingEnabled(true);

    setupModel();
    setupListVars();
    m_internalChange = false;
    m_sortGroups = false;

    listGroups->setModel(m_model);
    //btnUp->setEnabled(false);
    //btnDown->setEnabled(false);
    //MAPPER
    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(m_model);
    m_mapper->addMapping(groupID, COL_ID);
    m_mapper->addMapping(wGroupExp, COL_EXP);
    m_mapper->addMapping(cbHeader, COL_HEADER);
    m_mapper->addMapping(cbFooter, COL_FOOTER);
    m_mapper->addMapping(cbReprint, COL_REPRINT);
    m_mapper->addMapping(spHeader, COL_HEADERHEIGHT);
    m_mapper->addMapping(spFooter, COL_FOOTERHEIGHT);
    m_mapper->addMapping(cbPrintOnNewPage, COL_STARTSONNEWPAGE);
    m_mapper->addMapping(pageBreakCond, COL_BREAKCOND);

    //COMPLETER
    QStringList columns;
    columns << "%CONST";
    m_document->loadDataSourceColumnNames( columns );
    QCompleter *completer = new QCompleter( columns, this);
    completer->setCaseSensitivity ( Qt::CaseInsensitive );
    wGroupExp->setCompleter(completer);
    //comboGroupExp->addItems(columns);

    connect( listGroups->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));
    connect( listVars, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(slotVarListChanged(QListWidgetItem*)));

    connect(panelButtons->btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(panelButtons->btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(panelButtons->btnUp, SIGNAL(clicked()), this, SLOT(up()));
    connect(panelButtons->btnDown, SIGNAL(clicked()), this, SLOT(down()));
    connect(btnExpBuild,SIGNAL(clicked()),this,SLOT(openExpressionBuilder()));

    m_mapper->toFirst();

    updateEnable();
    NCRDFormGeometry *fg = new NCRDFormGeometry(this,splitter);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDGroupDialog::~NCRDGroupDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDGroupDialog::add()
{
    NCReportGroup *group = m_document->reportDef()->addGroup( m_detail );
    qDebug("Add group %p",group);
    int nextID = m_model->rowCount();

    group->setId( tr("Group%1").arg( m_model->rowCount() ) );

    appendModel( group, nextID, true );
    //DEFAULT VALUES
    m_model->item(nextID,COL_HEADER)->setText("true");
    m_model->item(nextID,COL_FOOTER)->setText("true");
    m_model->item(nextID,COL_HEADERHEIGHT)->setText("4.0");
    m_model->item(nextID,COL_FOOTERHEIGHT)->setText("4.0");

    updateEnable();
    listGroups->setCurrentIndex( m_model->index(nextID,COL_ID) );
    groupID->setFocus();
    groupID->selectAll();
}

void NCRDGroupDialog::remove()
{
    if ( m_model->rowCount() == 0 )
        return;

    int row = m_mapper->currentIndex();
    NCRDStandardPointerItem *pItem = (NCRDStandardPointerItem*)(m_model->item( row, COL_POINTER ));
    NCReportGroup *group = pItem->group();

    if ( pItem->insertedAsNew() ) {
        qDebug("Remove group %p",group);
        m_document->reportDef()->removeGroup( m_detail, group );
    } else {
        m_removedGroups.append( group );
    }
    m_model->removeRow( row );
    updateEnable();
}

void NCRDGroupDialog::up()
{
    int row = m_mapper->currentIndex();
    if (row <= 0)
        return; // nothing to do

    //mapper->submit();
    QList<QStandardItem *> items = m_model->takeRow(row);
    m_model->insertRow(row - 1, items );
    //mapper->revert();
    //mapper->setCurrentIndex( row-1 );
    listGroups->setCurrentIndex( m_model->index(row-1,COL_ID) );
    m_sortGroups=true;
}

void NCRDGroupDialog::down()
{
    int row = m_mapper->currentIndex();
    if (row == -1 || row == m_model->rowCount() - 1)
        return; // nothing to do

    QList<QStandardItem *> items = m_model->takeRow(row);
    m_model->insertRow(row + 1, items );
    listGroups->setCurrentIndex( m_model->index(row+1,COL_ID) );
    m_sortGroups=true;
}

void NCRDGroupDialog::slotItemChanged(const QModelIndex & current, const QModelIndex & )
{
    m_mapper->setCurrentModelIndex( current );
    updateVarListFromModel();
    qDebug( "slotItemChanged");
/*	for (int row = 0; row < model->rowCount(); ++row) {
        qDebug( "Item(row,4).text() = %s", qPrintable(model->item( row, 4 )->text()) );
    }*/
}


void NCRDGroupDialog::setupModel()
{
    int cnt = m_detail->groups().count();
    m_model = new QStandardItemModel( cnt, COL_COUNT, this);

    for ( int i=0; i<cnt; ++i ) {
        NCReportGroup *group = m_detail->groups().at(i);
        appendModel( group, i, false );
    }
}

void NCRDGroupDialog::setupListVars()
{

    QHash<QString, NCReportVariable*>::const_iterator i = m_document->reportDef()->variables().constBegin();
    while (i != m_document->reportDef()->variables().constEnd()) {
        NCReportVariable* var = i.value();
        if ( var->scope() == NCReportVariable::Group ) {
            QListWidgetItem *item = new QListWidgetItem( var->id() );
            item->setCheckState( Qt::Unchecked );
            listVars->addItem( item );
        }
        ++i;
    }

}

void NCRDGroupDialog::appendModel( NCReportGroup *group, int row, bool flagNew )
{
    QStandardItem *item =0;
    // ID
    item = new QStandardItem( group->groupId() );
    m_model->setItem( row, COL_ID, item);
    // expression
    item = new QStandardItem( group->expression() );
    m_model->setItem( row, COL_EXP, item);
    // isHeader
    item = new QStandardItem( group->header()!=NULL ? "true" : "false" );
    m_model->setItem( row, COL_HEADER, item);
    // isFooter
    item = new QStandardItem( group->footer()!=NULL ? "true" : "false" );
    m_model->setItem( row, COL_FOOTER, item);
    // reprintHdr
    item = new QStandardItem( group->repeatHeaderOnNextPage() ? "true" : "false" );
    m_model->setItem( row, COL_REPRINT, item);
    // spin Hdr height
    item = new QStandardItem();
    if ( group->header() )
        item->setText( QString::number(group->header()->metricHeight()) );
    m_model->setItem( row, COL_HEADERHEIGHT, item);
    // spin Ftr height
    item = new QStandardItem();
    if ( group->footer() )
        item->setText( QString::number(group->footer()->metricHeight()) );
    m_model->setItem( row, COL_FOOTERHEIGHT, item);
    // Resetvars
    item = new QStandardItem( group->resetVarList() );
    m_model->setItem( row, COL_RESETVARS, item);

    // starts on new page
    item = new QStandardItem( group->startsOnNewPage() ? "true" : "false" );
    m_model->setItem( row, COL_STARTSONNEWPAGE, item);

    // bage break exp
    item = new QStandardItem( group->pageBreakExp() );
    m_model->setItem( row, COL_BREAKCOND, item);

    // group pointer
    NCRDStandardPointerItem *pItem = new NCRDStandardPointerItem( m_detail, flagNew );
    pItem->setGroup( group );
    m_model->setItem( row, COL_POINTER, pItem);
}


void NCRDGroupDialog::applyChanges()
{
    for (int row = 0; row < m_model->rowCount(); ++row) {
        NCRDStandardPointerItem *sItem = (NCRDStandardPointerItem*)(m_model->item( row, COL_POINTER ));
        NCReportGroup *group = sItem->group();

        //0 ID
        //1 expression
        //2 isHeader
        //3 isFooter
        //4 reprintHdr
        //5 spin Hdr height
        //6 spin Ftr height
        //7 Resetvars
        //8 group pointer

        group->setId( m_model->item(row,COL_ID)->text() );
        group->setNestedLevel(row);
        group->setGroupExp( m_model->item(row,COL_EXP)->text() );

        bool isheader = (m_model->item(row,COL_HEADER)->text() == "true");
        bool isfooter = (m_model->item(row,COL_FOOTER)->text() == "true");

        if ( isheader ) {
            if (!group->header())
                m_document->reportDef()->addGroupHeader( group );	// add if don't exist
            group->header()->setMetricHeight( m_model->item(row,COL_HEADERHEIGHT)->text().toDouble() );
        } else {
            if (group->header()) { // remove if exists
                m_document->removeSection( group->header() );
                m_document->reportDef()->removeGroupHeader( group );
            }
        }

        if ( isfooter ) {
            if (!group->footer())
                m_document->reportDef()->addGroupFooter( group );
            group->footer()->setMetricHeight( m_model->item(row,COL_FOOTERHEIGHT)->text().toDouble() );
        } else {
            if (group->footer()) {  // remove if exists
                m_document->removeSection( group->footer() );
                m_document->reportDef()->removeGroupFooter( group );
            }
        }
        group->setReprintHeaderOnNextPage( m_model->item(row,COL_REPRINT)->text() == "true" );
        group->setStartsOnNewPage( m_model->item(row,COL_STARTSONNEWPAGE)->text() == "true" );
        group->setResetVarList( m_model->item(row,COL_RESETVARS)->text() );
        group->setPageBreakExp( m_model->item(row,COL_BREAKCOND)->text() );
    }

    if (m_removedGroups.count()>0) {
        QString groupnames;
        for (int i=0; i<m_removedGroups.count(); ++i)
            groupnames += "\n" +m_removedGroups.at(i)->groupId();

        switch ( QMessageBox::warning( this, tr("Remove warning"), tr("Are you sure to remove group(s):%1?").arg( groupnames ),
            QMessageBox::Yes | QMessageBox::No ) ) {

            case QMessageBox::Yes: {

                for (int i=0; i<m_removedGroups.count(); ++i) {
                    NCReportGroup *group = m_removedGroups.at(i);
                    qDebug("Remove group %p",group);

                    m_document->removeSection( group->header() );
                    m_document->removeSection( group->footer() );
                    m_document->reportDef()->removeGroup( m_detail, group );
                }
                break;
            }
            case QMessageBox::No:
            default:
                break;
        }

    }
    //document->updateGroupSections( m_detail );
    //document->updateGrid();
    if (m_sortGroups)
        m_document->reportDef()->sortGroups(m_detail);
    m_document->updateSectionsByReportDef();
    m_document->setDocumentModified();
}

void NCRDGroupDialog::discardChanges()
{
    for (int row = 0; row <  m_model->rowCount(); ++row) {
        NCRDStandardPointerItem *pItem = (NCRDStandardPointerItem*)(m_model->item( row, COL_POINTER ));
        NCReportGroup *group = pItem->group();
        if ( pItem->insertedAsNew() ) {
            qDebug("(discard) Remove detail %p",group);
            m_document->reportDef()->removeGroup( m_detail, group );
        }
    }
}

void NCRDGroupDialog::updateEnable()
{
    bool enable = (m_model->rowCount() > 0);
    gbGroup->setEnabled( enable );
    gbHF->setEnabled( enable );
    listVars->setEnabled( enable );
}

void NCRDGroupDialog::updateVarListFromModel()
{
    m_internalChange = true;
    int row = m_mapper->currentIndex();
    QString varlist =  m_model->item(row,COL_RESETVARS)->text();

    for (int i=0; i<listVars->count(); ++i)
        listVars->item(i)->setCheckState( varlist.contains( listVars->item(i)->text() )
                ? Qt::Checked : Qt::Unchecked );
    m_internalChange = false;

}

void NCRDGroupDialog::updateModelFromVarList()
{
    int row = m_mapper->currentIndex();
    QString varlist;

    for (int i=0; i<listVars->count(); ++i) {
        if ( listVars->item(i)->checkState() == Qt::Checked )
            varlist += listVars->item(i)->text()+",";
    }
    varlist.truncate( varlist.length()-1 );
    m_model->item(row,COL_RESETVARS)->setText( varlist );
}

void NCRDGroupDialog::slotVarListChanged(QListWidgetItem *)
{
    if ( !m_internalChange )
        updateModelFromVarList();
}

void NCRDGroupDialog::setCurrentGroup( NCReportGroup* group )
{
    // locate the group pointer
    for (int row = 0; row < m_model->rowCount(); ++row) {
        NCRDStandardPointerItem *sItem = (NCRDStandardPointerItem*)(m_model->item( row, COL_POINTER ));
        if ( sItem->group() == group ) {
            listGroups->setCurrentIndex( m_model->index(row,0) );
        }
    }
}

void NCRDGroupDialog::openExpressionBuilder()
{
    QList<int> types;
    types << NCReportItem::DataSource;

    NCRDExpressionBuilderDialog dialog(m_document, types, NCReportItem::DataSource, parentWidget());
    dialog.setExpression(wGroupExp->text());

    if (dialog.exec()==QDialog::Accepted)
        wGroupExp->setText( dialog.expression() );

}
