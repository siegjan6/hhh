/****************************************************************************
*
*  Copyright (C) 2002-2013 Helta Kft. - NociSoft Software Solutions
*  All rights reserved.
*  Author: Norbert Szabo
*  E-mail: norbert@nocisoft.com, office@nocisoft.com
*  Web: www.nocisoft.com
*
*  Created: 7 Mar 2013 (nszabo)
*  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
*  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*
****************************************************************************/

#include "ncrdtableitemdialog.h"
#include "ui_ncrdtableitemdialog.h"
#include "ncrddocument.h"

#include "ncreporttableitem.h"

//! 多语言
#include "MDMultiLanguage.h"

NCRDTableItemDialog::NCRDTableItemDialog(NCReportItem *item, NCRDDocument *parent) :
    QDialog(parent),
    ui(new Ui::NCRDTableItemDialog), m_item(item), m_doc(parent)
{
    ui->setupUi(this);

    NCReportTableItem *tableItem = (NCReportTableItem*)item;
    ui->wModelID->setText(tableItem->modelID());
    ui->wTableViewID->setText(tableItem->tableViewID());

    ui->wCellSpacing->setValue(tableItem->cellSpacing());
    ui->wElidedMode->setChecked(tableItem->isElidedMode());
    ui->wShowCellBorders->setChecked(tableItem->showBorderAndBG());
    ui->wShowHHeader->setChecked(tableItem->isHorizontalHeaderVisible());
    ui->wShowVHeader->setChecked(tableItem->isVerticalHeaderVisible());
    ui->wPinLeft->setChecked(tableItem->pinToLeft());
    ui->wPinRight->setChecked(tableItem->pinToRight());
    ui->wHiddenRows->setText(tableItem->hiddenRowsList());
    ui->wHiddenColumns->setText(tableItem->hiddenColumnsList());

    ui->printWhenPanel->setOptions(tableItem,parent);

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDTableItemDialog::~NCRDTableItemDialog()
{
    delete ui;
}

void NCRDTableItemDialog::applyChanges()
{
    NCReportTableItem *tableItem = (NCReportTableItem*)m_item;

    tableItem->setModelID(ui->wModelID->text());
    tableItem->setTableViewID(ui->wTableViewID->text());

    tableItem->setCellSpacing(ui->wCellSpacing->value());
    tableItem->setElidedMode(ui->wElidedMode->isChecked());
    tableItem->setshowBorderAndBG(ui->wShowCellBorders->isChecked());
    tableItem->setHorizontalHeaderVisible(ui->wShowHHeader->isChecked());
    tableItem->setVerticalHeaderVisible(ui->wShowVHeader->isChecked());
    tableItem->setPinToLeft(ui->wPinLeft->isChecked());
    tableItem->setPinToRight(ui->wPinRight->isChecked());
    tableItem->setHiddenRows(ui->wHiddenRows->text());
    tableItem->setHiddenColumns(ui->wHiddenColumns->text());

    ui->printWhenPanel->getOptions(tableItem);

    m_item->update();
    m_doc->setDocumentModified();
}

void NCRDTableItemDialog::discardChanges()
{
}

void NCRDTableItemDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDTableItemDialog::reject()
{
    discardChanges();
    QDialog::reject();
}
