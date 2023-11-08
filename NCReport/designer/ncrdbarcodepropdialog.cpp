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
#include "ncrdbarcodepropdialog.h"
#include "ncrddocument.h"
#include "ncrdformgeometry.h"

#include <QFontDatabase>
#include <QMetaObject>
#include <QMetaEnum>
#include <QFile>
#include <QRadioButton>
#include <QCompleter>

//! 多语言
#include "MDMultiLanguage.h"

NCRDBarcodePropDialog::NCRDBarcodePropDialog(NCReportItem * item, NCRDDocument * parent)
: QDialog( parent ), Ui::NCRDBarcodePropDialogUI(), m_item(item), m_doc(parent), m_optionWidget(0), m_completer(0)
{
    setupUi(this);
    m_barcodeType = -1;
//	connect(chkComposite, SIGNAL(stateChanged( int )), SLOT(compositeEnable()));
//	connect(comboBarcodeType, SIGNAL(currentIndexChanged( int )), SLOT(changeOptions()));
    connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(apply(QAbstractButton*)) );
    connect(comboSourceType, SIGNAL(currentIndexChanged(int)), this, SLOT(sourceChanged(int)) );

    NCReportBarcodeItem *bItem = (NCReportBarcodeItem*)item;

    for ( int i=0; i<= bItem->maxBarcodeID(); ++i ) {
        QString name = bItem->barcodeName(i);
        if ( !name.isEmpty() )
            comboBarcodeType->addItem( name, i );
    }

    comboSourceType->setAvailableItems(item->availableSourceTypes());
    comboSourceType->setValue( bItem->sourceType() );

    comboBarcodeType->setCurrentIndex( comboBarcodeType->findData(bItem->barcodeType()) );
    staticCode->setText( bItem->value().toString() );
    wColumn->setText( bItem->text() );

    //spnScale->setValue( bItem->scale() );
    //chkComposite->setChecked(bItem->isComposite());
    spnWhitespace->setValue(bItem->whiteSpace());
    heightb->setValue(bItem->heightRatio());
    btype->setCurrentIndex( bItem->borderType() );
    bwidth->setValue(bItem->borderWidth());
    printWhenPanel->setOptions(bItem,parent);

    comboForeColor->setStandardColors();
    comboForeColor->setColorDialogEnabled(true);
    comboForeColor->setCurrentColor( bItem->foreColor() );

    comboBackColor->setStandardColors();
    comboBackColor->setColorDialogEnabled(true);
    comboBackColor->setCurrentColor( bItem->backColor() );

    primaryMessage->setText(bItem->primaryText() );
    sbColumns->setValue(bItem->columns());
    sbMode->setValue(bItem->mode());
    sbSecure->setValue(bItem->secure());
    sbVers->setValue(bItem->version());
    cbShowText->setChecked(bItem->isShowCode());
    spRotation->setValue(bItem->rotation());

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    sourceChanged(comboSourceType->currentIndex());

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDBarcodePropDialog::~NCRDBarcodePropDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDBarcodePropDialog::apply( QAbstractButton* btn )
{
    if ( buttonBox->standardButton(btn)==QDialogButtonBox::Apply )
        applyChanges();
}


void NCRDBarcodePropDialog::applyChanges()
{
    NCReportBarcodeItem *bItem = (NCReportBarcodeItem*)m_item;

    bItem->setSourceType( comboSourceType->value() );
    bItem->setBarcodeType( comboBarcodeType->itemData(comboBarcodeType->currentIndex()).toInt() );
    bItem->setText( wColumn->text() );
    bItem->setValue( staticCode->text() );
    //bItem->setScale( spnScale->value() );
    //bItem->setComposite( chkComposite->isChecked() );
    bItem->setWhiteSpace( spnWhitespace->value() );
    bItem->setHeightRatio( heightb->value() );
    bItem->setBorderType( btype->currentIndex() );
    bItem->setBorderWidth( bwidth->value() );
    bItem->setForeColor(comboForeColor->currentColor());
    bItem->setBackColor( comboBackColor->currentColor() );
    //bItem->fontSize = comboFontSize->currentText().toInt();

    printWhenPanel->getOptions(bItem);

    bItem->setPrimaryText( primaryMessage->text() );
    bItem->setColumns(sbColumns->value());
    bItem->setMode(sbMode->value());
    bItem->setSecure(sbSecure->value());
    bItem->setVersion(sbVers->value());
    bItem->setShowCode(cbShowText->isChecked());
    bItem->setRotation(spRotation->value());

    bItem->update_barcode();
    bItem->update();
    m_doc->setDocumentModified();
}

void NCRDBarcodePropDialog::discardChanges()
{
}

void NCRDBarcodePropDialog::sourceChanged(int index)
{
    label_source->setText(comboSourceType->title());
    wColumn->setEnabled(comboSourceType->value() != NCReportItem::Static);

    if (m_completer)
        delete m_completer;

    m_completer = new QCompleter(m_doc->allKeywords((NCReportItem::SourceType)index),this);
    wColumn->setCompleter( m_completer );
}

void NCRDBarcodePropDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDBarcodePropDialog::reject()
{
    discardChanges();
    QDialog::reject();
}
