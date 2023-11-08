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
#include "ncrdfieldpropdialog.h"
#include "ncrddocument.h"
#include "ncrdmainwindow.h"
#include "ncreportfielditem.h"
#include "ncrddef.h"
#include "ncrdformgeometry.h"
#include "ncreportdatasource.h"
#include "ncreportevaluator.h"
#include "ncrdlanguage.h"
#include "ncrditemwidgetfactory.h"
#include "ncrdfieldexpwidget.h"

#include <QCompleter>
#include <QMessageBox>

//! 多语言
#include "MDMultiLanguage.h"

NCRDFieldPropDialog::NCRDFieldPropDialog(NCReportItem *item, NCRDDocument *parent)
: QDialog( parent ), Ui::NCRDFieldPropDialogUI(),
    m_item(item),
    m_doc(parent),
    m_completer(0),
    m_langManager(new NCRDLanguage(this)),
    m_customizedMainWidget(false),
    m_addCustomTab(false)
{
    setupUi(this);
    setDefaultValues();
    //pageCustom->setDialog(this);

    NCReportFieldItem *field = (NCReportFieldItem*)item;

    QWidget *customWidget = 0;
    if (m_doc->mainWindow()->fieldItemDialogWidgetFactory()) {

        m_doc->mainWindow()->fieldItemDialogWidgetFactory()->reset();   // resets the widget registry
        //----------------------
        // CUSTOM FIELD WIDGET
        //----------------------
        // if we have a custom widget factory for customized field settings
        customWidget = m_doc->mainWindow()->fieldItemDialogWidgetFactory()->createWidget(field, NCRDItemWidgetFactory::Main, this);
        if (customWidget)
            formLayout->insertWidget(1, customWidget, 5);
        //----------------------
        // CUSTOM TAB
        //----------------------
        // if we have a custom widget factory for existing field tab widget settings
        QWidget *customTab1 = m_doc->mainWindow()->fieldItemDialogWidgetFactory()->createWidget(field, NCRDItemWidgetFactory::Tab1, tabWidgetOptions);
        if (customTab1)
            tabWidgetOptions->addTab(customTab1, customTab1->windowTitle());

        QWidget *customTab2 = m_doc->mainWindow()->fieldItemDialogWidgetFactory()->createWidget(field, NCRDItemWidgetFactory::Tab2, tabWidgetOptions);
        if (customTab2)
            tabWidgetOptions->addTab(customTab2, customTab2->windowTitle());

        m_doc->mainWindow()->fieldItemDialogWidgetFactory()->updateWidget(item); // update all widgets
    }
    //-------------------
    // DEFAULT CASE
    //-------------------
    if (!customWidget) {
        QTabWidget *tabWidget = new QTabWidget(this);
        formLayout->insertWidget(1, tabWidget, 5);
        tabWidget->addTab( new NCRDFieldExpWidget(tabWidget), tr("Field") );

        m_langManager->setCurrentLanguage( m_doc->reportDef()->option().currentLanguage() );
        m_langManager->setLanguages( m_doc->reportDef()->option().languages() );
        m_langManager->setTabWidget( tabWidget );
        m_langManager->createTabs( parent, item, comboSourceType, tr("Field"), true, field->translations(), 0, this );
        m_langManager->assignTranslations( NCRDLanguage::TextsToTabs, field->translations() );
        connect(m_langManager,SIGNAL(itemSourceTypeChanged(int)),comboSourceType,SLOT(setCurrentIndex(int)));
    }
    comboSourceType->setAvailableItems(item->availableSourceTypes());
    comboSourceType->setValue( field->sourceType() );
    comboDataType->setCurrentIndex( field->dataType() );

    tabGeneral->cbWordWrap->setChecked( field->isWordBreak() );
    tabGeneral->cbAutoHeight->setChecked( field->isAutoHeight() );
    tabGeneral->argText->setText( field->arg() );

    cbFormating->setChecked( field->isNumFormat() );
    cbLocalize->setChecked( field->isLocalized() );
    cbBlankIfZero->setChecked( field->isNumBlankIfZero() );
    spinDecimals->setValue( field->precision() );
    spinFWidth->setValue( field->fieldWidth() );
    formatChar->setText( QString(field->format()) );
    fillChar->setText( field->fillChar() );

    dateFormat->setText( field->dateFormat() );

    borderPanel->setOptions(field,parent);
    printWhenPanel->setOptions(field,parent);

    tabGeneral->leDescription->setText( field->description() );

    tabGeneral->comboBackColor->setStandardColors();
    tabGeneral->comboBackColor->setColorDialogEnabled(true);
    tabGeneral->comboBackColor->setCurrentColor( field->backColor() );
    tabGeneral->comboBGMode->setCurrentIndex( field->backgroundMode() );
    tabGeneral->cbHideRepeated->setChecked( field->hideIfRepeated() );
    tabGeneral->cbUseLastNoneNull->setChecked( field->useLastNoneNull() );
    tabGeneral->cbBGDesign->setChecked( field->isBackgroundToDesign() );

    tabSpecial->spRotation->setValue(field->rotation());
    tabSpecial->spSpacing->setValue( field->font().letterSpacing() );
    tabSpecial->comboCapitalization->setCurrentIndex((int)field->font().capitalization());
    tabSpecial->setAlignment(field->alignment());
    tabSpecial->spCharLength->setValue( field->numCharacters() );
    btnCondFormat->setConditionalFormatCode(field->conditionalFormatCode());

    tabGeneral->cbPinLeft->setChecked( field->pinToLeft() );
    tabGeneral->cbPinRight->setChecked( field->pinToRight() );

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    connect(comboSourceType,SIGNAL(activated(int)),this,SLOT(sourceTypeChanged(int)));
    fg->restore();

    sourceTypeChanged(field->sourceType());
    setWindowTitle(tr("Field settings. ID:%1").arg(field->id()));

    parseColumnName();

    //2015年9月8日  mxl   添加    屏蔽暂时不需要的控件
    //btnCondFormat->setVisible(false);
    //printWhenPanel->setVisible(false);

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDFieldPropDialog::~NCRDFieldPropDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDFieldPropDialog::applyChanges()
{
    NCReportFieldItem *field = (NCReportFieldItem*)m_item;

    m_langManager->assignTranslations( NCRDLanguage::TabsToTexts, field->translations() );
    //field->hasTranslation = (field->translations.count() > 0);
    field->setSourceType( comboSourceType->value() );
    field->setDataType( (NCReportData::DataType)comboDataType->currentIndex() );

    field->setWordBreak( tabGeneral->cbWordWrap->isChecked() );
    field->setArg( tabGeneral->argText->text() );

    field->setNumFormat( cbFormating->isChecked() );
    field->setLocalized( cbLocalize->isChecked() );
    field->setNumBlankIfZero( cbBlankIfZero->isChecked() );
    field->setPrecision( spinDecimals->value() );
    field->setFieldWidth( spinFWidth->value() );
    field->setFormat( formatChar->text()[0].toLatin1() );
    field->setFillChar( fillChar->text()[0].toLatin1() );
    field->setDateFormat( dateFormat->text() );

    borderPanel->getOptions(field);
    printWhenPanel->getOptions(field);

    field->setAutoHeight( tabGeneral->cbAutoHeight->isChecked() );
    field->setDescription( tabGeneral->leDescription->text() );
    field->setNumberOfCharacters( tabSpecial->spCharLength->value() );
    field->setBackColor( tabGeneral->comboBackColor->currentColor() );
    field->setBackgroundMode( (NCReportLabelItem::BGMode)tabGeneral->comboBGMode->currentIndex() );
    field->setHideIfRepeated( tabGeneral->cbHideRepeated->isChecked() );
    field->setUseLastNoneNull( tabGeneral->cbUseLastNoneNull->isChecked() );
    field->setBackgroundToDesign( tabGeneral->cbBGDesign->isChecked() );
    //field->setHasTranslation( cbTr->isChecked() );
    field->setRotation(tabSpecial->spRotation->value());
    field->fontRef().setLetterSpacing(QFont::AbsoluteSpacing,tabSpecial->spSpacing->value());
    field->fontRef().setCapitalization((QFont::Capitalization)tabSpecial->comboCapitalization->currentIndex());
    field->setAlignment( tabSpecial->getAlignment(field->alignment()) );
    field->setConditionalFormatCode( btnCondFormat->conditionalFormatCode() );
    field->setPinToLeft( tabGeneral->cbPinLeft->isChecked() );
    field->setPinToRight( tabGeneral->cbPinRight->isChecked() );

    m_item->update();
    m_item->adjustSize();
    m_doc->setDocumentModified();
    m_doc->updateManagers();
}

void NCRDFieldPropDialog::discardChanges()
{
}

void NCRDFieldPropDialog::multiLangClicked(bool )
{
    /*
    if (!on && m_languageManager->isMultiEdited()) {
        int ret = QMessageBox::warning(this, tr("Warning"),tr("Translations will be deleted!"),
                                        QMessageBox::Yes, QMessageBox::No | QMessageBox::Default );
        if ( ret == QMessageBox::No )
            cbTr->setChecked(true);
    }
    */

}
void NCRDFieldPropDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDFieldPropDialog::reject()
{
    discardChanges();
    QDialog::reject();
}

void NCRDFieldPropDialog::setDefaultValues()
{
}

void NCRDFieldPropDialog::sourceTypeChanged(int type)
{
    if (m_completer)
        delete m_completer;

    m_completer = new QCompleter(m_doc->allKeywords((NCReportItem::SourceType)type),this);
    m_langManager->setCompleter( m_completer );
}

void NCRDFieldPropDialog::parseColumnName()
{
}

void NCRDFieldPropDialog::columnComboChanged(const QString &)
{
}

void NCRDFieldPropDialog::generateColumnName()
{
}

void NCRDFieldPropDialog::dataSourceComboChanged(const QString &)
{
}


