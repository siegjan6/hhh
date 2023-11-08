/****************************************************************************
*
*  Copyright (C) 2002-2011 Helta Kft. / NociSoft Software Solutions
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

#include "ncrdcrosstabdialog.h"
#include "ui_ncrdcrosstabdialog.h"
#include "ncrddocument.h"
#include "ncrdformgeometry.h"
#include "ncreportcrosstabitem.h"
#include "ncrdcrosstabcellmodel.h"
#include "ncreportdatasource.h"
#include "ncreportdef.h"

#include <QStandardItemModel>
#include <QDataWidgetMapper>

//! 多语言
#include "MDMultiLanguage.h"

//#define CELL_COL_					0

//#define CELL_COLS					32

NCRDCrossTabDialog::NCRDCrossTabDialog(NCReportItem *item, NCRDDocument* parent) : QDialog(parent),
    ui(new Ui::NCRDCrossTabDialog), m_item((NCReportCrossTabItem*)item)
{
    ui->setupUi(this);
    ui->comboDS->addItems(m_item->reportDef()->dataSourceIdList());
    ui->printWhenPanel->setOptions(item,parent);

    m_cellModel = new NCRDCrossTabCellModel(this);
    //NCRDCrossTabCellModelDelegate *delegate = new NCRDCrossTabCellModelDelegate(this);

    ui->listViewCells->setModel(m_cellModel);
    ui->listViewCells->setModelColumn(0);
    ui->listViewCells->setCurrentIndex( m_cellModel->index(0,0, QModelIndex()) );
    //ui->listViewCells->setSelectionMode(QAbstractItemView::MultiSelection);

    setupMapper();
    setupDialog();

    connect( ui->listViewCells->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)),
             this, SLOT(slotItemChanged(const QModelIndex&,const QModelIndex&)));

    m_mapper->toFirst();

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDCrossTabDialog::~NCRDCrossTabDialog()
{
    delete ui;
}

void NCRDCrossTabDialog::setupMapper()
{
    m_mapper = new QDataWidgetMapper(this);
    m_mapper->setModel(m_cellModel);
    m_mapper->setItemDelegate( new QItemDelegate(this) );

    m_mapper->addMapping(ui->comboAlignH, CELL_DATA_ALIGNH, "currentIndex");
    m_mapper->addMapping(ui->comboAlignV, CELL_DATA_ALIGNV, "currentIndex");
    m_mapper->addMapping(ui->lineEditFont, CELL_DATA_FONTFAMILY );
    m_mapper->addMapping(ui->spFontSize, CELL_DATA_FONTSIZE );
    m_mapper->addMapping(ui->cbBold, CELL_DATA_BOLD);
    m_mapper->addMapping(ui->cbItalic, CELL_DATA_ITALIC);
    m_mapper->addMapping(ui->cbUnderline, CELL_DATA_UNDERLINE);
    m_mapper->addMapping(ui->textColor->colorEdit(), CELL_DATA_FORECOLOR );
    m_mapper->addMapping(ui->backColor->colorEdit(), CELL_DATA_BACKCOLOR );
    m_mapper->addMapping(ui->comboFillPattern, CELL_DATA_FILLSTYLE, "currentIndex");
    m_mapper->addMapping(ui->comboBgMode, CELL_DATA_BGMODE, "currentIndex");
    m_mapper->addMapping(ui->lineEditArgString, CELL_DATA_FORMATSTRING);
    m_mapper->addMapping(ui->spFieldWidth, CELL_DATA_FIELDWIDTH);
    m_mapper->addMapping(ui->lineEditFormatCode, CELL_DATA_FORMATCODE);
    m_mapper->addMapping(ui->spPrec, CELL_DATA_PRECISION);
    m_mapper->addMapping(ui->cbBlankIfZero, CELL_DATA_BLANKIFZERO);
    m_mapper->addMapping(ui->cbLocalized, CELL_DATA_LOCALIZED);
    m_mapper->addMapping(ui->cbNumIsFormatted, CELL_DATA_FORMATTED);
    m_mapper->addMapping(ui->lineEditFillChar, CELL_DATA_FILLCHAR);
    m_mapper->addMapping(ui->comboDT, CELL_DATA_DATATYPE, "currentIndex");
    m_mapper->addMapping(ui->lineEditCellText, CELL_DATA_TEXT);
    m_mapper->addMapping(ui->spRotation, CELL_DATA_ROTATION);

    m_mapper->addMapping(ui->lseTop->enabledCheckBox(), CELL_BORDER_TOP_ENABLED );
    m_mapper->addMapping(ui->lseTop->lineWidthSpinBox(), CELL_BORDER_TOP_LINEWIDTH );
    m_mapper->addMapping(ui->lseTop->lineStyleCombo(), CELL_BORDER_TOP_LINESTYLE, "currentIndex");
    m_mapper->addMapping(ui->lseTop->colorPicker()->colorEdit(), CELL_BORDER_TOP_LINECOLOR );

    m_mapper->addMapping(ui->lseRight->enabledCheckBox(), CELL_BORDER_RIGHT_ENABLED );
    m_mapper->addMapping(ui->lseRight->lineWidthSpinBox(), CELL_BORDER_RIGHT_LINEWIDTH );
    m_mapper->addMapping(ui->lseRight->lineStyleCombo(), CELL_BORDER_RIGHT_LINESTYLE, "currentIndex");
    m_mapper->addMapping(ui->lseRight->colorPicker()->colorEdit(), CELL_BORDER_RIGHT_LINECOLOR );

    m_mapper->addMapping(ui->lseBottom->enabledCheckBox(), CELL_BORDER_BOTTOM_ENABLED );
    m_mapper->addMapping(ui->lseBottom->lineWidthSpinBox(), CELL_BORDER_BOTTOM_LINEWIDTH );
    m_mapper->addMapping(ui->lseBottom->lineStyleCombo(), CELL_BORDER_BOTTOM_LINESTYLE, "currentIndex");
    m_mapper->addMapping(ui->lseBottom->colorPicker()->colorEdit(), CELL_BORDER_BOTTOM_LINECOLOR );

    m_mapper->addMapping(ui->lseLeft->enabledCheckBox(), CELL_BORDER_LEFT_ENABLED );
    m_mapper->addMapping(ui->lseLeft->lineWidthSpinBox(), CELL_BORDER_LEFT_LINEWIDTH );
    m_mapper->addMapping(ui->lseLeft->lineStyleCombo(), CELL_BORDER_LEFT_LINESTYLE, "currentIndex");
    m_mapper->addMapping(ui->lseLeft->colorPicker()->colorEdit(), CELL_BORDER_LEFT_LINECOLOR );
}


void NCRDCrossTabDialog::setupDialog()
{
    if (m_item->dataSource())
        ui->comboDS->setCurrentIndex( ui->comboDS->findText(m_item->dataSource()->id()) );

    ui->lineEditHiddenCols->setText( m_item->hiddenDataSourceColumns() );
    ui->lineEditColTitleSource->setText( m_item->columnTitleSource() );
    ui->spColWidths->setValue( m_item->columnWidthMM() );
    ui->spRowHeights->setValue(m_item->rowHeightMM());
    ui->spCellPadding->setValue(m_item->cellPaddingMM());
    ui->spCellSpacing->setValue(m_item->cellSpacingMM());
    ui->spTableSpacing->setValue(m_item->tableSpacingMM());
    ui->spHeadColWidth->setValue(m_item->regionSize(NCReportCrossTabItem::HeaderColumn));
    ui->spDataColWidth->setValue(m_item->regionSize(NCReportCrossTabItem::DataColumn));
    ui->spTotalColWidth->setValue(m_item->regionSize(NCReportCrossTabItem::TotalColumn));
    ui->spHeadRowHeight->setValue(m_item->regionSize(NCReportCrossTabItem::HeaderRow));
    ui->spDataRowHeight->setValue(m_item->regionSize(NCReportCrossTabItem::DataRow));
    ui->spTotalRowHeight->setValue(m_item->regionSize(NCReportCrossTabItem::TotalRow));

    ui->cbShowColHeader->setChecked(m_item->showColumnHeader());
    ui->cbShowRowHeader->setChecked(m_item->showRowHeader());
    ui->cbShowBottomSummary->setChecked(m_item->showBottomSummary());
    ui->cbShowSideSummary->setChecked(m_item->showSideSummary());
    ui->cbBreakTable->setChecked(m_item->breakTable());

    ui->cbPivotTable->setChecked(m_item->isPivotTable());
    ui->cbPivotSort->setChecked(m_item->isPivotAutoSort());
    ui->comboAggregate->setCurrentIndex(m_item->pivotTableAggregation());
    ui->lineEditPTRow->setText(m_item->pivotTableRowSource());
    ui->lineEditPTColumn->setText(m_item->pivotTableColumnSource());
    ui->lineEditPTValue->setText(m_item->pivotTableValueSource());

    for (int i=0; i<NCREPORTCROSSTABITEMCELL_COUNT; ++i)
        m_cellModel->setCell( m_item->cell(i), i );

}

void NCRDCrossTabDialog::applyChanges()
{
    m_item->setDataSourceByID(ui->comboDS->currentText());
    m_item->setHiddenDataSourceColumns(ui->lineEditHiddenCols->text());

    m_item->setColumnTitleSource(ui->lineEditColTitleSource->text());
    m_item->setColumnWidthMM(ui->spColWidths->value());
    m_item->setRowHeightMM(ui->spRowHeights->value());
    m_item->setCellPaddingMM(ui->spCellPadding->value());
    m_item->setCellSpacingMM(ui->spCellSpacing->value());
    m_item->setTableSpacingMM(ui->spTableSpacing->value());
    m_item->setRegionSize(ui->spHeadColWidth->value(),NCReportCrossTabItem::HeaderColumn);
    m_item->setRegionSize(ui->spDataColWidth->value(),NCReportCrossTabItem::DataColumn);
    m_item->setRegionSize(ui->spTotalColWidth->value(),NCReportCrossTabItem::TotalColumn);
    m_item->setRegionSize(ui->spHeadRowHeight->value(),NCReportCrossTabItem::HeaderRow);
    m_item->setRegionSize(ui->spDataRowHeight->value(),NCReportCrossTabItem::DataRow);
    m_item->setRegionSize(ui->spTotalRowHeight->value(),NCReportCrossTabItem::TotalRow);

    m_item->setShowColumnHeader(ui->cbShowColHeader->isChecked());
    m_item->setShowRowHeader(ui->cbShowRowHeader->isChecked());
    m_item->setShowBottomSummary(ui->cbShowBottomSummary->isChecked());
    m_item->setShowSideSummary(ui->cbShowSideSummary->isChecked());
    m_item->setBreakTable(ui->cbBreakTable->isChecked());

    m_item->setPivotTable(ui->cbPivotTable->isChecked());
    m_item->setPivotAutoSort(ui->cbPivotSort->isChecked());
    m_item->setPivotTableAggregation(ui->comboAggregate->currentIndex());
    m_item->setPivotTableRowSource(ui->lineEditPTRow->text());
    m_item->setPivotTableColumnSource(ui->lineEditPTColumn->text());
    m_item->setPivotTableValueSource(ui->lineEditPTValue->text());

    for (int i=0; i<NCREPORTCROSSTABITEMCELL_COUNT; ++i)
        m_item->setCell(m_cellModel->cell(i),i);

    ui->printWhenPanel->getOptions(m_item);

    m_item->update();
    ((NCRDDocument*)parent())->setDocumentModified();
}

void NCRDCrossTabDialog::discardChanges()
{
}

void NCRDCrossTabDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDCrossTabDialog::reject()
{
    discardChanges();
    QDialog::accept();
}

void NCRDCrossTabDialog::slotItemChanged(const QModelIndex &current, const QModelIndex &)
{
    m_mapper->setCurrentModelIndex( current );
}

