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
#include "ncrdpreferencesdialog.h"
#include "ncrdformgeometry.h"

#include <QFileDialog>

//! 多语言
#include "MDMultiLanguage.h"

NCRDPreferencesDialog::NCRDPreferencesDialog(QWidget* parent)
: QDialog( parent ), Ui::NCRDPreferencesDialogUI()
{
    setupUi(this);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboFontSize->addItem(QString::number(size));

    connect( btnChooseDir, SIGNAL(clicked()), this, SLOT(openDir()) );
    connect( btnChooseOutputDir, SIGNAL(clicked()), this, SLOT(openDir()) );
    connect( btnChooseTemplateDir, SIGNAL(clicked()), this, SLOT(openDir()) );
    connect( btnChooseZintDir, SIGNAL(clicked()), this, SLOT(openDir()) );

    connect( btnGridReset, SIGNAL(clicked()), this, SLOT(resetGrid()) );
    connect( comboFontFamily, SIGNAL(currentFontChanged(const QFont&)), this, SLOT(showFontSample()));
    connect( comboFontSize, SIGNAL(editTextChanged(const QString&)), this, SLOT(showFontSample()));

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDPreferencesDialog::~NCRDPreferencesDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDPreferencesDialog::load(NCRDPreferences & pref )
{
    pathReport->setText( pref.reportDir );
    pathOutput->setText( pref.outputDir );
    pathTemplate->setText( pref.templateDir );
    pathZint->setText( pref.zintDir );

    comboFontFamily->setCurrentFont( QFont(pref.fontFamily) );
    comboFontSize->setCurrentIndex(	comboFontSize->findText(QString::number(pref.fontPointSize)) );
    showFontSample();
    cbGridVisible->setChecked( pref.gridVisible );
    cbGridSnap->setChecked( pref.snapToGrid );
    spGridX->setValue( pref.grid.x() );
    spGridY->setValue( pref.grid.y() );
    cbShowMaximized->setChecked( pref.showMaximized );
    spCounter->setValue( pref.fileCounter );
    comboRes->setCurrentIndex(pref.printerResolution);
    lePrinterName->setText(pref.defaultPrinterName);
    cbNoDialog->setChecked(pref.noPrinterDialog);
    spDPI->setValue(pref.printerDPI);
    cbEnc->setChecked(pref.encodePwd);
    rbOneTableMode->setChecked( pref.htmlOutputStrategy==0 );
    rbMultiTableMode->setChecked( pref.htmlOutputStrategy==1 );
    cssFile->setText( pref.htmlOutputCssFile );
    spFieldWidth->setValue( pref.defaultFieldWidth );
    spFieldHeight->setValue( pref.defaultFieldHeight );
    fieldTxt->setText( pref.defaultFieldText );
}

void NCRDPreferencesDialog::save(NCRDPreferences & pref )
{
    pref.reportDir = pathReport->text();
    pref.outputDir = pathOutput->text();
    pref.templateDir = pathTemplate->text();
    pref.zintDir = pathZint->text();

    pref.fontFamily = comboFontFamily->currentFont().family();
    pref.fontPointSize = comboFontSize->currentText().toInt();
    pref.gridVisible = cbGridVisible->isChecked();
    pref.snapToGrid = cbGridSnap->isChecked();
    pref.grid.setX( spGridX->value() );
    pref.grid.setY( spGridY->value() );
    pref.showMaximized = cbShowMaximized->isChecked();
    pref.fileCounter = spCounter->value();
    pref.printerResolution = comboRes->currentIndex();
    pref.defaultPrinterName = lePrinterName->text();
    pref.noPrinterDialog = cbNoDialog->isChecked();
    pref.printerDPI = spDPI->value();
    pref.encodePwd = cbEnc->isChecked();
    pref.htmlOutputStrategy = (rbOneTableMode->isChecked() ? 0 : 1);
    pref.htmlOutputCssFile = cssFile->text();
    pref.defaultFieldWidth = spFieldWidth->value();
    pref.defaultFieldHeight = spFieldHeight->value();
    pref.defaultFieldText = fieldTxt->text();
}

void NCRDPreferencesDialog::resetGrid()
{
    cbGridVisible->setChecked( false );
    cbGridSnap->setChecked( true );
    spGridX->setValue( 6 );
    spGridY->setValue( 6 );
}

void NCRDPreferencesDialog::openDir()
{
    QToolButton *btn = qobject_cast<QToolButton*>(sender());

    QString path;
    if ( btn == btnChooseDir )
        path = pathReport->text();
    else if ( btn == btnChooseOutputDir )
        path = pathOutput->text();
    else if ( btn == btnChooseTemplateDir )
        path = pathTemplate->text();
    else if ( btn == btnChooseZintDir )
        path = pathZint->text();
    else
        return;

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), path, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if ( !dir.isEmpty() ) {
        if ( btn == btnChooseDir )
            pathReport->setText( dir );
        else if ( btn == btnChooseOutputDir )
            pathOutput->setText( dir );
        else if ( btn == btnChooseTemplateDir )
            pathTemplate->setText( dir );
        else if ( btn == btnChooseZintDir )
            pathZint->setText( dir );
    }
}

void NCRDPreferencesDialog::showFontSample()
{
    QFont f( comboFontFamily->currentFont() );
    f.setPointSize( comboFontSize->currentText().toInt() );

    fontTest->setFont( f );
}


