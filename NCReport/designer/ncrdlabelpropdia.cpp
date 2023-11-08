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
#include "ncrdlabelpropdia.h"
#include "ncrddocument.h"
#include "ncreportlabelitem.h"
#include "ncrddef.h"
#include "ncrdlanguage.h"
#include "ncrdformgeometry.h"
#include "ncrdmainwindow.h"
#include "ncrdtextpropertymanager.h"

NCRDLabelPropDialog::NCRDLabelPropDialog(NCReportItem *item, NCRDDocument* parent)
    : QDialog( parent ), Ui::NCRDLabelPropDialogUI(), m_item(item), m_doc(parent), m_langManager(new NCRDLanguage(this))
{
    setupUi(this);
    //resize( QSize(width(), sizeHint().height() ) );
    tabGeneral->setLabelMode();
    tabSpecial->setLabelMode();

    NCReportLabelItem *label = (NCReportLabelItem*)item;
    m_langManager->setCurrentLanguage( m_doc->reportDef()->option().currentLanguage() );
    m_langManager->setLanguages( m_doc->reportDef()->option().languages() );
    m_langManager->setTabWidget( tabLabel );
    m_langManager->createTabs( parent, item, 0, tr("Label"), false, label->translations(), 0, this );
    m_langManager->assignTranslations( NCRDLanguage::TextsToTabs, label->translations() );

    //objID->setText( label->id );
    tabGeneral->cbWordWrap->setChecked( label->isWordBreak() );

    tabGeneral->cbWordWrap->setChecked( label->isWordBreak() );
    tabGeneral->cbAutoHeight->setChecked( label->isAutoHeight() );

    tabGeneral->comboBackColor->setStandardColors();
    tabGeneral->comboBackColor->setColorDialogEnabled(true);
    tabGeneral->comboBackColor->setCurrentColor( label->backColor() );
    tabGeneral->comboBGMode->setCurrentIndex( (int)label->backgroundMode() );
    tabGeneral->cbBGDesign->setChecked( label->isBackgroundToDesign() );
    tabGeneral->cbPinLeft->setChecked( label->pinToLeft() );
    tabGeneral->cbPinRight->setChecked( label->pinToRight() );

    borderPanel->setOptions(label,parent);
    printWhenPanel->setOptions(label,parent);

    tabSpecial->spRotation->setValue( label->rotation());
    tabSpecial->spSpacing->setValue( label->font().letterSpacing() );
    tabSpecial->comboCapitalization->setCurrentIndex((int)label->font().capitalization());
    tabSpecial->setAlignment(label->alignment());
    btnCondFormat->setConditionalFormatCode(label->conditionalFormatCode());

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //2015年9月8日  mxl   添加    屏蔽暂时不需要的控件
    //tabOpt->setVisible(false);
    //btnCondFormat->setVisible(false);
    //printWhenPanel->setVisible(false);
}

NCRDLabelPropDialog::~NCRDLabelPropDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDLabelPropDialog::applyChanges()
{
    NCReportLabelItem *label = (NCReportLabelItem*)m_item;

    m_langManager->assignTranslations( NCRDLanguage::TabsToTexts, label->translations() );

    borderPanel->getOptions(label);
    printWhenPanel->getOptions(label);

    label->setWordBreak(tabGeneral->cbWordWrap->isChecked());
    label->setAutoHeight( tabGeneral->cbAutoHeight->isChecked() );

    label->setBackColor( tabGeneral->comboBackColor->currentColor() );
    label->setBackgroundMode( (NCReportLabelItem::BGMode)tabGeneral->comboBGMode->currentIndex() );
    label->setBackgroundToDesign( tabGeneral->cbBGDesign->isChecked() );
    label->setRotation(tabSpecial->spRotation->value());
    label->fontRef().setLetterSpacing(QFont::AbsoluteSpacing,tabSpecial->spSpacing->value());
    label->fontRef().setCapitalization((QFont::Capitalization)tabSpecial->comboCapitalization->currentIndex());
    label->setAlignment( tabSpecial->getAlignment(label->alignment()) );
    label->setConditionalFormatCode( btnCondFormat->conditionalFormatCode() );
    label->setPinToLeft( tabGeneral->cbPinLeft->isChecked() );
    label->setPinToRight( tabGeneral->cbPinRight->isChecked() );

    m_item->update();
    if ( !(label->isAutoHeight() || label->isWordBreak()) /*&& !m_language->hasMultiLine()*/ )
        m_item->adjustSize();

    NCRDTextPropertyManager *manager = (NCRDTextPropertyManager*)(m_doc->mainWindow()->propertyManager("pmtxt"));
    manager->setAlignment(label->alignment());

    m_doc->setDocumentModified();
}

void NCRDLabelPropDialog::discardChanges()
{
}

void NCRDLabelPropDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDLabelPropDialog::reject()
{
    discardChanges();
    QDialog::reject();
}


