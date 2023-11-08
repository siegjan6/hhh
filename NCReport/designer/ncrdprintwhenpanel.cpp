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
#include "ncrdprintwhenpanel.h"
#include "ncrdcodeeditdialog.h"
#include "ncreportitem.h"
#include "ncrdexpressionbuilderdialog.h"

NCRDPrintWhenPanel::NCRDPrintWhenPanel(QWidget* parent)
: QWidget( parent ), Ui::NCRDPrintWhenPanelUI(), m_item(0), m_document(0)
{
    setupUi(this);

    connect(btnEditPrintWhen,SIGNAL(clicked()),this,SLOT(editPrintWhen()));
    connect(btnEditHtmlOpt,SIGNAL(clicked()),this,SLOT(editHtmlOpt()));
}

NCRDPrintWhenPanel::~NCRDPrintWhenPanel()
{
}

void NCRDPrintWhenPanel::setOptions(NCReportItem *item, NCRDDocument *document)
{
    m_item = item;
    m_document = document;
    if (item) {
        printWhen->setText( item->printWhen() );
        zoneID->setValue( item->zoneID() );
        itemID->setText( item->id() );
        htmlOptions->setText( item->htmlOptions() );
        functionID->setText( item->functionId() );
    }
}

void NCRDPrintWhenPanel::getOptions(NCReportItem *item)
{
    item->setPrintWhen( printWhen->text() );
    item->setZoneID(zoneID->value());
    item->setId( itemID->text() );
    item->setHtmlOptions( htmlOptions->text() );
    item->setFunctionId( functionID->text() );
}

void NCRDPrintWhenPanel::disableItemOptions()
{
    itemID->setVisible(false);
    zoneID->setVisible(false);
    functionID->setVisible(false);
    labelFunctionId->setVisible(false);
    labelItemId->setVisible(false);
    labelZoneId->setVisible(false);
}

void NCRDPrintWhenPanel::editPrintWhen()
{
    //openEditDialog(printWhen);
    QList<int> types;
//    if (m_item)
//        types = m_item->availableSourceTypes();
//    else
    types << NCReportItem::ScriptExpression;

    NCRDExpressionBuilderDialog dialog(m_document, types, NCReportItem::ScriptExpression, parentWidget());
    dialog.setExpression(printWhen->text());

    if (dialog.exec()==QDialog::Accepted)
        printWhen->setText( dialog.expression() );
}

void NCRDPrintWhenPanel::editHtmlOpt()
{
    //openEditDialog(htmlOptions);
    NCRDCodeEditDialog dialog(parentWidget());
    dialog.setCodeText(htmlOptions->text());
    if (dialog.exec()==QDialog::Accepted)
        htmlOptions->setText(dialog.codeText());
}

void NCRDPrintWhenPanel::openEditDialog(QLineEdit *editor)
{
    NCRDCodeEditDialog dialog(parentWidget());
    dialog.setCodeText(editor->text());
    if (dialog.exec()==QDialog::Accepted)
        editor->setText(dialog.codeText());

}


