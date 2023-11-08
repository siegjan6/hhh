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
#include "ncrdgraphpropdialog.h"
#include "ncrddocument.h"
#include "ncreportgraphitem.h"
#include "ncrdformgeometry.h"

//! 多语言
#include "MDMultiLanguage.h"

NCRDGraphPropDialog::NCRDGraphPropDialog(NCReportItem *item, NCRDDocument* parent)
: QDialog( parent ), Ui::NCRDGraphPropDialogUI(), m_item(item), m_doc(parent)
{
    setupUi(this);
    //resize( QSize(width(), sizeHint().height() ) );

    NCReportGraphItem *gItem = (NCReportGraphItem*)item;

    QList<int> available;
    available << NCReportItem::Static << NCReportItem::DataSource << NCReportItem::File;
    comboSourceType->setAvailableItems(available);
    comboSourceType->setValue( gItem->sourceType() );
    classID->setText( gItem->classID() );
    text->setText( gItem->text() );
    graphDef->setText( gItem->graphDefinition() );

    printWhenPanel->setOptions(gItem,parent);

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDGraphPropDialog::~NCRDGraphPropDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDGraphPropDialog::applyChanges()
{
    NCReportGraphItem *gItem = (NCReportGraphItem*)m_item;

    gItem->setSourceType( comboSourceType->value() );
    gItem->setClassID( classID->text() );
    gItem->setText( text->text() );
    gItem->setGraphDefinition( graphDef->toPlainText() );

    printWhenPanel->getOptions(gItem);

    m_item->update();
    m_item->adjustSize();
    m_doc->setDocumentModified();

}

void NCRDGraphPropDialog::discardChanges()
{
}


void NCRDGraphPropDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDGraphPropDialog::reject()
{
    discardChanges();
    QDialog::reject();
}
