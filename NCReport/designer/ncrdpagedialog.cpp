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
#include "ncrdpagedialog.h"
#include "ncrddef.h"
#include "ncrddocument.h"
#include "ncreportpageoption.h"
#include "ncrdlanguage.h"
#include "ncrdformgeometry.h"

#include <QHash>
#include <QMessageBox>
#include <QFontDatabase>

#include "MDMultiLanguage.h"

static const char * const _localenames[] = {
    "Latin1",
    "Big5",
    "Big5-HKSCS",
    "eucJP",
    "eucKR",
    "GB2312",
    "GBK",
    "GB18030",
    "JIS7",
    "Shift-JIS",
    "TSCII",
    "utf8",
    "UTF-8",
    "KOI8-R",
    "KOI8-U",
    "ISO8859-1",
    "ISO8859-2",
    "ISO8859-3",
    "ISO8859-4",
    "ISO8859-5",
    "ISO8859-6",
    "ISO8859-7",
    "ISO8859-8",
    "ISO8859-9",
    "ISO8859-10",
    "ISO8859-13",
    "ISO8859-14",
    "ISO8859-15",
    "IBM 850",
    "IBM 866",
    "CP874",
    "CP1250",
    "CP1251",
    "CP1252",
    "CP1253",
    "CP1254",
    "CP1255",
    "CP1256",
    "CP1257",
    "CP1258",
    "Apple Roman",
    "TIS-620",
0 };

static const char * const _pagesizes[] = {
    "A4", "B5", "Letter", "Legal", "Executive", "A0","A1","A2","A3","A5","A5M",
    "A6","A7","A8","A9","B0","B1","B10","B2","B3","B4","B6","B7",
    "B8","B9","C5E","COMM10E","DLE","FOLIO","LEDGER","TABLOID","CUSTOM",
0 };


NCRDPageDialog::NCRDPageDialog(NCRDDocument *doc, QWidget* parent)
: QDialog( parent ), Ui::NCRDPageDialogUI(), document(doc)
{
    setupUi(this);
    //resize( QSize(width(), sizeHint().height() ) );

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboFontSize->addItem(QString::number(size));

    int i=0;
    while ( _localenames[i] )
        comboEncoding->addItem( _localenames[i++] );

    //i=0;
    //while ( _pagesizes[i] )
    //	comboPageSize->addItem( _pagesizes[i++] );

    QVector<NCReportPageOption::PageData> v;
    NCReportPageOption::loadPageDataVector( v );
    for (int i = 0; i < v.size(); ++i) {
        NCReportPageOption::PageData pd = v.at(i);
        comboPageSize->addItem( pd.name );
    }
    connect( comboPageSize, SIGNAL(activated(QString)), this, SLOT(pageSizeChanged(QString)) );
    setupPage();

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();

    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDPageDialog::~NCRDPageDialog()
{
}

/*$SPECIALIZATION$*/
void NCRDPageDialog::setupPage()
{
    NCReportPageOption opt = document->reportDef()->option();

    reportName->setText( opt.reportName() );
    comboReportType->setCurrentIndex( (int)opt.reportType() );

    comboPageSize->setCurrentIndex( comboPageSize->findText( NCReportPageOption::paperSizeToName(opt.paperSize()) ) );
    spWidth->setValue( opt.pageWidthMM() );
    spHeight->setValue( opt.pageHeightMM() );
    pageSizeChanged( comboPageSize->currentText() );

    comboEncoding->setCurrentIndex(comboEncoding->findText(opt.encoding()));
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(opt.defaultFont()).family()));
    comboFontSize->setCurrentIndex(	comboFontSize->findText(QString::number(opt.defaultFont().pointSize())) );

    cbBold->setChecked(opt.defaultFont().bold());
    cbItalic->setChecked(opt.defaultFont().italic());
    cbUnderline->setChecked(opt.defaultFont().underline());
    cbStrikeout->setChecked(opt.defaultFont().strikeOut());

    comboColor->setColor( opt.backColor() );
    //comboQuality->setCurrentIndex( opt.printQuality() == QPrinter::HighResolution ? 0 : 1 ) ;
    //spDPI->setValue( opt.printResolution() );

    if ( opt.pageOrientation() == QPrinter::Portrait )
        radioP->setChecked(true);
    else
        radioL->setChecked(true);

    if ( document->reportDef()->pageHeader() ) {
        cbPageHeader->setChecked(true);
        sbPH->setValue( document->reportDef()->pageHeader()->metricHeight() );
    }
    if ( document->reportDef()->pageFooter() ) {
        cbPageFooter->setChecked(true);
        sbPF->setValue( document->reportDef()->pageFooter()->metricHeight() );
    }
    if ( document->reportDef()->reportHeader() ) {
        cbReportHeader->setChecked(true);
        sbRH->setValue( document->reportDef()->reportHeader()->metricHeight() );
    }
    if ( document->reportDef()->reportFooter() ) {
        cbReportFooter->setChecked(true);
        sbRF->setValue( document->reportDef()->reportFooter()->metricHeight() );
    }

    spLeft->setValue( opt.leftMarginMM() );
    spRight->setValue( opt.rightMarginMM() );
    spTop->setValue( opt.topMarginMM() );
    spBottom->setValue( opt.bottomMarginMM() );
    sbZoneSpacing->setValue( opt.zoneSpacing() );
    cbDoublePass->setChecked( opt.isDoublePassMode() );
    cbDataTrim->setChecked(opt.isDataTrimming());
    cbZoneBreak->setChecked( opt.zonesAreBreakable() );
    cbSubReportNewPage->setChecked( opt.subReportOnNewPage());
    wHtmlBodyOptions->setPlainText(opt.htmlBodyStyle());
    wHtmlHeadOptions->setPlainText(opt.htmlHead());
    wCustomOptions->setPlainText(opt.customData());
    cbUpdateCurrentVars->setChecked(opt.updateOnlyCurrentDataSourceVariables());

    //---- LANG
    //NCRDLanguage::fillComboBox( comboDefLang );
    NCRDLanguage lang;
    lang.fillComboBox( comboLang );
    comboLang->setCurrentIndex( comboLang->findData("en") );

    rbUseDefault->setChecked( opt.languageRole() == NCReportPageOption::UseDefaultLanguage );
    rbLeaveBlank->setChecked( opt.languageRole() == NCReportPageOption::LeaveEmpty );

    addLanguages(opt.languages());
}

void NCRDPageDialog::applyChanges()
{
    const NCReport::ReportType rtype[] = { NCReport::Report, NCReport::TextDocumentPrint };

    document->reportDef()->option().setReportName(reportName->text());
    document->reportDef()->option().setReportType(rtype[comboReportType->currentIndex()]);
    document->reportDef()->option().setOrientation(radioP->isChecked() ? QPrinter::Portrait : QPrinter::Landscape );
    qreal w= spWidth->value();
    qreal h= spHeight->value();
    document->reportDef()->option().setPaperSize(comboPageSize->currentText(), QSizeF(w,h) );
    /*
    document->reportDef()->option().pageSizeByName( comboPageSize->currentText(),
        document->reportDef()->option().pageOrientation(),
        document->reportDef()->option().pageSize(), w, h );

    document->reportDef()->option().setPageWidth(w);
    document->reportDef()->option().setPageHeight(h);
    */
    document->reportDef()->option().setEncoding(comboEncoding->currentText());
    QFont f( comboFont->currentText(), comboFontSize->currentText().toInt() );
    f.setBold(cbBold->isChecked());
    f.setItalic(cbItalic->isChecked());
    f.setUnderline(cbUnderline->isChecked());
    f.setStrikeOut(cbStrikeout->isChecked());
    document->reportDef()->option().setDefaultFont(f);
    document->reportDef()->option().setBackColor(comboColor->color());

    //QPrinter::PrinterMode quality[] = {QPrinter::HighResolution, QPrinter::ScreenResolution };
    //document->reportDef()->option().setPrintQuality( quality[comboQuality->currentIndex()] );
    //document->reportDef()->option().setPrintResolution( spDPI->value() );

    if ( cbPageHeader->isChecked() ) {
        if ( !document->reportDef()->pageHeader() )
            document->reportDef()->addPageHeader();
        document->reportDef()->pageHeader()->setMetricHeight( sbPH->value() );
    } else {
        document->removeSection( document->reportDef()->pageHeader() );
        document->reportDef()->removePageHeader();
    }
    if ( cbPageFooter->isChecked() ) {
        if ( !document->reportDef()->pageFooter() )
            document->reportDef()->addPageFooter();
        document->reportDef()->pageFooter()->setMetricHeight( sbPF->value() );
    } else {
        document->removeSection( document->reportDef()->pageFooter() );
        document->reportDef()->removePageFooter();
    }
    if ( cbReportHeader->isChecked() ) {
        if ( !document->reportDef()->reportHeader() )
            document->reportDef()->addReportHeader();
        document->reportDef()->reportHeader()->setMetricHeight( sbRH->value() );
    } else {
        document->removeSection( document->reportDef()->reportHeader() );
        document->reportDef()->removeReportHeader();
    }
    if ( cbReportFooter->isChecked() ) {
        if ( !document->reportDef()->reportFooter() )
            document->reportDef()->addReportFooter();
        document->reportDef()->reportFooter()->setMetricHeight( sbRF->value() );
    } else {
        document->removeSection( document->reportDef()->reportFooter() );
        document->reportDef()->removeReportFooter();
    }

    document->reportDef()->option().setLeftMarginMM(spLeft->value());
    document->reportDef()->option().setRightMarginMM(spRight->value());
    document->reportDef()->option().setTopMarginMM(spTop->value());
    document->reportDef()->option().setBottomMarginMM(spBottom->value());
    document->reportDef()->option().setZoneSpacing(sbZoneSpacing->value());
    document->reportDef()->option().setDoublePassMode(cbDoublePass->isChecked());
    document->reportDef()->option().setDataTrimming(cbDataTrim->isChecked());
    document->reportDef()->option().setLanguages( languages() );
    document->reportDef()->option().setZonesAreBreakable( cbZoneBreak->isChecked() );
    document->reportDef()->option().setSubReportOnNewPage( cbSubReportNewPage->isChecked() );
    document->reportDef()->option().setLanguageRole( rbUseDefault->isChecked() ? NCReportPageOption::UseDefaultLanguage : NCReportPageOption::LeaveEmpty );
    document->reportDef()->option().setHtmlBodyStyle(wHtmlBodyOptions->toPlainText());
    document->reportDef()->option().setHtmlHead(wHtmlHeadOptions->toPlainText());
    document->reportDef()->option().setCustomData(wCustomOptions->toPlainText());
    document->reportDef()->option().setupdateOnlyCurrentDataSourceVariables(cbUpdateCurrentVars->isChecked());

    document->updateSectionsByReportDef();
    document->updatePageSettings();
    document->setDocumentModified();
}

void NCRDPageDialog::discardChanges()
{
}

QTabWidget *NCRDPageDialog::mainTabWidget()
{
    return tabWidget;
}

void NCRDPageDialog::pageSizeChanged( const QString & text)
{
    bool custom=false;
    if ( text == "CUSTOM" )
        custom = true;
    else {
        QPrinter::PaperSize papersize = QPrinter::A4;
        QSizeF size;
        NCReportPageOption::setPaperSizeByName( text, radioP->isChecked() ? QPrinter::Portrait : QPrinter::Landscape, papersize, size );
        spWidth->setValue( size.width() );
        spHeight->setValue( size.height() );
    }
    spWidth->setEnabled(custom);
    spHeight->setEnabled(custom);
}

void NCRDPageDialog::addLang()
{
    /*
    QListWidgetItem *item = new QListWidgetItem;
    item->setText( comboTrLang->currentText() );
    item->setData( Qt::EditRole, comboTrLang->itemData( comboTrLang->currentIndex() ) );
    listLang->addItem(item);
    */
    addLanguages( comboLang->itemData( comboLang->currentIndex() ).toString() );
}

void NCRDPageDialog::removeLang()
{
    listLang->takeItem( listLang->currentRow() );
}

void NCRDPageDialog::addLanguages( const QString& languageCodes )
{
    QStringList llist = languageCodes.split( ',', QString::SkipEmptyParts );
    for ( int i=0; i<llist.count(); ++i ) {
        QListWidgetItem *item = new QListWidgetItem;
        QString name = comboLang->itemText(comboLang->findData(llist.at(i)));
        if (listLang->findItems( name, Qt::MatchExactly ).count() == 0) {
            item->setData( Qt::UserRole, llist.at(i) );
            item->setData( Qt::DisplayRole, name );
            item->setData( Qt::DecorationRole, QImage(":/designer/images/world.png") );
            listLang->addItem(item);
        }
    }
}

QString NCRDPageDialog::languages() const
{
    QString codes;
    for ( int i=0; i<listLang->count(); ++i ) {
        QListWidgetItem *item = listLang->item(i);
        QVariant data = item->data(Qt::UserRole);
        QVariant data2 = item->data(Qt::DisplayRole);
        codes += data.toString() +",";
    }
    codes.truncate( codes.length()-1 );
    return codes;
}

void NCRDPageDialog::langUp()
{
    moveLangItem(-1);
}

void NCRDPageDialog::langDown()
{
    moveLangItem(1);
}


void NCRDPageDialog::moveLangItem( int moveBy )
{
    int row = listLang->currentRow();
    if (row < 0)
        return;

    QListWidgetItem *item = listLang->takeItem(row);
    listLang->insertItem(row + moveBy, item );
    listLang->setCurrentItem(item);
}





