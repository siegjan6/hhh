/****************************************************************************
*
*  Copyright (C) 2002-2015 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 2015. 03. 02. (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrdscriptdialog.h"
#include "ui_ncrdscriptdialog.h"
#include "scripthighlighter.h"
#include "ncrddocument.h"
#include "ncrdformgeometry.h"
#include "ncrddef.h"
#include "ncrdsqltextedit.h"
#include "ncrdexpressionbuilderdialog.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

//! 多语言
#include "MDMultiLanguage.h"

NCRDScriptDialog::NCRDScriptDialog(NCRDDocument* doc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NCRDScriptDialog),
    m_document(doc),
    m_model( 0 ),
    m_mapper( new QDataWidgetMapper(this) ),
    m_changed(false)
{
    ui->setupUi(this);

    //-------------------
    // MODEL
    //-------------------
    setupModel();
    m_mapper->addMapping(ui->wScripId, Id);
    m_mapper->addMapping(ui->wScriptEditor, ScriptCode);
    m_mapper->toFirst();

    connect(ui->btnAdd, SIGNAL(clicked()), this, SLOT(add()));
    connect(ui->btnRemove, SIGNAL(clicked()), this, SLOT(remove()));
    connect(ui->btnExpBuilder, SIGNAL(clicked()), this, SLOT(openExpressionBuilder()));
    connect( ui->listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));

    //-------------------
    // EDITOR
    //-------------------
    ui->wScriptEditor->setFont(QFont("Courier",10));
    new ScriptHighlighter(ui->wScriptEditor->document());
    ui->wScriptEditor->installEventFilter(new NCRDTextEditKeyPressEater(ui->wScriptEditor, "    "));

// SHOW TAB SPACES
//    QTextOption option = ui->wScriptEditor->document()->defaultTextOption();
//    if (true)
//        option.setFlags(option.flags() | QTextOption::ShowTabsAndSpaces);
//    else
//        option.setFlags(option.flags() & ~QTextOption::ShowTabsAndSpaces);

//    option.setFlags(option.flags() | QTextOption::AddSpaceForLineAndParagraphSeparators);
//    ui->wScriptEditor->document()->setDefaultTextOption(option);

    //-------------------
    // STATE SAVER
    //-------------------
    NCRDFormGeometry *fg = new NCRDFormGeometry(this,ui->splitter);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    updateEnabled();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDScriptDialog::~NCRDScriptDialog()
{
    delete ui;
}

void NCRDScriptDialog::setupModel()
{
    m_model = new QStandardItemModel( m_document->reportDef()->scripts().count(), ColumnCount, this);
    QHash<QString,QString>::const_iterator i = m_document->reportDef()->scripts().constBegin();
    int row = 0;
    while ( i != m_document->reportDef()->scripts().constEnd()) {
        appendModel( row, i.key(), i.value() );
        ++i;
        ++row;
    }
    m_model->sort(Id);
    connect(m_model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(onChanged()));
    ui->listView->setModel(m_model);
    m_mapper->setModel(m_model);
    m_changed = false;
}


void NCRDScriptDialog::applyChanges()
{
    if (!m_changed)
        return;

    //QTextDocument doc;
    for (int row = 0; row<m_model->rowCount(); ++row) {
        //doc.setHtml(m_model->item(row,ScriptCode)->text());
        //m_document->reportDef()->addScript( m_model->item(row,Id)->text(), doc.toPlainText() );
        m_document->reportDef()->addScript( m_model->item(row,Id)->text(), m_model->item(row,ScriptCode)->text() );
    }

    for (int i=0; i<m_removedScripts.count(); i++)
        m_document->reportDef()->removeScript(m_removedScripts.value(i));

    m_document->setDocumentModified();
    m_changed = false;
}

void NCRDScriptDialog::discardChanges()
{
}

void NCRDScriptDialog::add()
{
    int nextID = m_model->rowCount();
    appendModel( nextID, QString("func%1").arg(nextID), QString() );
    updateEnabled();
    ui->listView->setCurrentIndex( m_model->index(nextID,Id) );
    ui->wScripId->setFocus();
    ui->wScripId->selectAll();
    m_changed = true;
}

void NCRDScriptDialog::remove()
{
    int row = m_mapper->currentIndex();

    if (!m_model->item(row,ScriptCode)->text().isEmpty()) {
        switch ( QMessageBox::warning( this, tr("Remove warning"),
                                       tr("Are you sure to remove script %1?").arg( m_model->item(row,Id)->text() ),
            QMessageBox::Yes | QMessageBox::No ) ) {

            case QMessageBox::Yes:
                break;
            default:
                return;
        }
    }
    m_removedScripts.append(m_model->item(row,Id)->text());
    m_model->removeRow( row );
    m_changed = true;
    updateEnabled();
}

void NCRDScriptDialog::onChanged()
{
    m_changed = true;
}

void NCRDScriptDialog::slotItemChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous)
    m_mapper->setCurrentModelIndex( current );
}

void NCRDScriptDialog::openExpressionBuilder()
{
    QList<int> types;
    types << NCReportItem::ScriptExpression;

    NCRDExpressionBuilderDialog dialog(m_document, types, NCReportItem::ScriptExpression, this);
    dialog.setExpression(ui->wScriptEditor->toPlainText());

    if (dialog.exec()==QDialog::Accepted)
        ui->wScriptEditor->setPlainText( dialog.expression() );
}

void NCRDScriptDialog::appendModel(int row, const QString &id, const QString &script)
{
    QStandardItem *item =0;

    item = new QStandardItem( id );    //id
    m_model->setItem( row, Id, item);

    item = new QStandardItem( script );  // script
    m_model->setItem( row, ScriptCode, item);
}

void NCRDScriptDialog::updateEnabled()
{
    bool enable = (m_model->rowCount() > 0);
    ui->wScripId->setEnabled( enable );
    ui->wScriptEditor->setEnabled( enable );
    ui->btnExpBuilder->setEnabled( enable );
    ui->btnRemove->setEnabled( enable );

    if (!enable) {
        ui->wScripId->clear();
        ui->wScriptEditor->clear();
    }
}
