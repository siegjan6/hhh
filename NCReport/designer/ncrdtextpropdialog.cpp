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
#include "ncrdtextpropdialog.h"
#include "ncrddocument.h"
#include "ncreporttextitem.h"
#include "ncrdformgeometry.h"
#include "ncreporthttpclient.h"
#include "ncrdhtmlhighlighter.h"

#include <QColorDialog>
#include <QFileInfo>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPlainTextEdit>

//! 多语言
#include "MDMultiLanguage.h"

NCRDTextPropDialog::NCRDTextPropDialog(NCReportItem *item, NCRDDocument* parent)
    : QDialog( parent ), Ui::NCRDTextPropDialogUI(), m_item(item), m_doc(parent)
{
    setupUi(this);

    new NCRDHtmlHighlighter(htmlEdit->document());

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(isPanel->buttonSelect(), SIGNAL(clicked()), this, SLOT(selectFile()));
    connect(isPanel->buttonPreview(), SIGNAL(clicked()), this, SLOT(loadContent()) );
    connect(isPanel, SIGNAL(sourceTypeChanged(int)), this, SLOT(sourceTypeChanged(int)));
    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    connect(textEdit, SIGNAL(currentCharFormatChanged(const QTextCharFormat &)), this, SLOT(currentCharFormatChanged(const QTextCharFormat &)));
    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));

    connect(tbBold, SIGNAL(clicked()), this, SLOT(textBold()));
    connect(tbUnderline, SIGNAL(clicked()), this, SLOT(textUnderline()));
    connect(tbItalic, SIGNAL(clicked()), this, SLOT(textItalic()));

    connect(tbLeft, SIGNAL(clicked()), this, SLOT(textAlign()));
    connect(tbCenter, SIGNAL(clicked()), this, SLOT(textAlign()));
    connect(tbRight, SIGNAL(clicked()), this, SLOT(textAlign()));
    connect(tbJusti, SIGNAL(clicked()), this, SLOT(textAlign()));

    connect(tbColor, SIGNAL(clicked()), this, SLOT(textColor()));

    connect(comboFont, SIGNAL(activated(const QString &)), this, SLOT(textFamily(const QString &)));
    connect(comboSize, SIGNAL(activated(const QString &)), this, SLOT(textSize(const QString &)));

    connect(cbAutoHeight, SIGNAL(stateChanged(int)), this, SLOT(cbAutoHeightChanged(int)) );
    connect(cbPlain, SIGNAL(stateChanged(int)), this, SLOT(cbPlainTextChanged(int)) );
    connect(cbMarkdown, SIGNAL(stateChanged(int)), this, SLOT(cbMarkdownChanged(int)) );

#ifdef MARKDOWN_INTEGRATION
    cbMarkdown->setVisible(true);
    connect(cbMarkdown, SIGNAL(stateChanged(int)), this, SLOT(cbMarkdownChanged(int)) );
#else
    cbMarkdown->setVisible(false);
#endif

    NCReportTextItem *tItem = (NCReportTextItem*)item;
    // SET DATA
    isPanel->setSourceType(tItem->sourceType());
    isPanel->setSourceText( tItem->text() );
    isPanel->setItem(item);
    isPanel->setDocument(parent);

    //fileName->setText( tItem->fileName() );

    printWhenPanel->setOptions(tItem,parent);

    //cbExpand->setChecked( tItem->expandDetail );
    cbAutoHeight->setChecked( tItem->isAutoHeight() );
    cbFix->setChecked( tItem->isSizeFix() );
    cbDefaultFont->setChecked( tItem->isUsedDefaultFont() );
    cbTemplate->setChecked( tItem->isTemplateMode() );
    cbDefault->setChecked( tItem->isDefaultPosAndSize() );
    cbBreakToPage->setChecked( tItem->isAbleToBreak() );
    cbPlain->setChecked( tItem->isPlainText() );
    cbPinToRight->setChecked( tItem->pinToRight() );
    cbMarkdown->setChecked( tItem->isMarkdown() );

    //textEdit->setDocument( tItem->document );
    if ( tItem->sourceType() == NCReportItem::Static ) {

        if (tItem->documentTemplate().isEmpty()) {
            textEdit->setHtml( tItem->document()->toHtml() );
            htmlEdit->setPlainText( tItem->document()->toHtml() );
        } else {
            textEdit->setHtml( tItem->documentTemplate() );
            htmlEdit->setPlainText( tItem->documentTemplate() );
        }

        textEdit->setFocus();
    }
    //else if ( tItem->document->toPlainText() != NCRD_DEFAULT_TEXT )
    //textEdit->selectAll();

    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());

    cbAutoHeightChanged(-1);

    m_htmlChanged = false;
    m_textChanged = false;
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(textEditChanged()));
    connect(htmlEdit, SIGNAL(textChanged()), this, SLOT(htmlEditChanged()));

    http = new NCReportHttpClient(this);
    connect(http, SIGNAL(finished()), this, SLOT(httpFinished()));

    NCRDFormGeometry *fg = new NCRDFormGeometry(this);
    connect(this,SIGNAL(finished(int)),fg,SLOT(save()));
    fg->restore();
    loadContent();

    //2015年9月8日  mxl   添加    屏蔽暂时不需要的控件
    //printWhenPanel->setVisible(false);

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

NCRDTextPropDialog::~NCRDTextPropDialog()
{
}

/*$SPECIALIZATION$*/

void NCRDTextPropDialog::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(tbBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void NCRDTextPropDialog::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(tbUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void NCRDTextPropDialog::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(tbItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void NCRDTextPropDialog::textFamily(const QString & f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void NCRDTextPropDialog::textSize(const QString & p)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(p.toFloat());
    mergeFormatOnWordOrSelection(fmt);
}

void NCRDTextPropDialog::textStyle(int)
{
}

void NCRDTextPropDialog::textColor()
{
    QColor col = QColorDialog::getColor(textEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void NCRDTextPropDialog::textAlign()
{
    QToolButton *tb = qobject_cast<QToolButton*>(sender());
    bool left = false;
    bool right = false;
    bool center = false;
    bool justi = false;

    if ( tb == tbLeft ) {
        textEdit->setAlignment(Qt::AlignLeft);
        left = true;
    } else if ( tb == tbCenter) {
        textEdit->setAlignment(Qt::AlignHCenter);
        center = true;
    } else if ( tb == tbRight) {
        textEdit->setAlignment(Qt::AlignRight);
        right = true;
    } else if ( tb == tbJusti) {
        textEdit->setAlignment(Qt::AlignJustify);
        justi = true;
    }

    tbLeft->setChecked( left );
    tbRight->setChecked( right );
    tbCenter->setChecked( center );
    tbJusti->setChecked( justi );
}

void NCRDTextPropDialog::currentCharFormatChanged(const QTextCharFormat & format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void NCRDTextPropDialog::cursorPositionChanged()
{
}

void NCRDTextPropDialog::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}

 void NCRDTextPropDialog::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    tbBold->setChecked(f.bold());
    tbItalic->setChecked(f.italic());
    tbUnderline->setChecked(f.underline());
}

 void NCRDTextPropDialog::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    tbColor->setIcon(pix);
}

 void NCRDTextPropDialog::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft) {
        tbLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter) {
        tbCenter->setChecked(true);
    } else if (a & Qt::AlignRight) {
        tbRight->setChecked(true);
    } else if (a & Qt::AlignJustify) {
        tbJusti->setChecked(true);
    }
}

void NCRDTextPropDialog::applyChanges()
{
    if ( m_textChanged )
        htmlEdit->setPlainText( textEdit->toHtml() );

    NCReportTextItem *tItem = (NCReportTextItem*)m_item;

    tItem->setSourceType( isPanel->sourceType() );
    tItem->setText( isPanel->sourceText() );
    //tItem->setFileName( fileName->text() );

    tItem->setHtml( htmlEdit->toPlainText() );
    //tItem->expandDetail = cbExpand->isChecked();
    //tItem->multiPage = cbMulti->isChecked();
    tItem->setAutoHeight( cbAutoHeight->isChecked() );
    tItem->setSizeFix( cbFix->isChecked() );
    tItem->setUseDefaultFont( cbDefaultFont->isChecked() );
    tItem->setTemplateMode( cbTemplate->isChecked() );
    tItem->setDefaultPosAndSize( cbDefault->isChecked() );

    printWhenPanel->getOptions(tItem);

    tItem->setAbleToBreak( cbBreakToPage->isChecked() );
    tItem->setPlainText( cbPlain->isChecked() );
    tItem->setPinToRight( cbPinToRight->isChecked() );
    tItem->setMarkdown( cbMarkdown->isChecked() );

    switch (tItem->sourceType()) {
    case NCReportItem::Static:
    case NCReportItem::File:
    case NCReportItem::Url:
        tItem->setDocumentTemplate( htmlEdit->toPlainText() );
        break;
    default: break;
    }

    m_item->update();
    if ( cbAdjust->isChecked() )
        m_item->adjustSize();

    m_doc->setDocumentModified();
}

void NCRDTextPropDialog::discardChanges()
{
}


void NCRDTextPropDialog::selectFile()
{
    QFileInfo fi( isPanel->sourceText() );
    QString s = QFileDialog::getOpenFileName( this, tr("Select html file"), fi.absoluteFilePath(), "Text files (*.html *.txt *.xml *.csv)" );
    if ( s.isEmpty() )
        return;

    isPanel->setSourceText( s );
    loadContent();
}

void NCRDTextPropDialog::setDefaultFont(const QFont & f)
{
    if ( !textEdit->document()->isEmpty() )
        return;

    fontChanged(f);
    QTextCharFormat fmt;

    fmt.setFont(f);
    mergeFormatOnWordOrSelection(fmt);
}

void NCRDTextPropDialog::cbAutoHeightChanged(int state)
{
    if ( state == Qt::Checked )
        cbFix->setChecked( false );
    cbFix->setEnabled( state != Qt::Checked );
}

void NCRDTextPropDialog::cbPlainTextChanged(int state)
{
    if (state == Qt::Checked) {
        tab->setCurrentIndex( tab->indexOf(tabHtml) );
        tab->setTabEnabled(tab->indexOf(tabDocument),false);
    } else {
        tab->setTabEnabled(tab->indexOf(tabDocument),true);
    }
}

void NCRDTextPropDialog::cbMarkdownChanged(int state)
{
    if (state==Qt::Checked)
        cbPlain->setChecked(true);
}

void NCRDTextPropDialog::htmlEditChanged()
{
    m_htmlChanged = true;
}

void NCRDTextPropDialog::textEditChanged()
{
    m_textChanged = true;
}

void NCRDTextPropDialog::tabChanged(int tabIdx )
{
    if ( tabIdx == 0 && m_htmlChanged ) {
        textEdit->setHtml( htmlEdit->toPlainText() );

    } else if ( tabIdx == 1 && m_textChanged ) {
        htmlEdit->setPlainText( textEdit->toHtml() );

    }
    m_htmlChanged = false;
    m_textChanged = false;
}

void NCRDTextPropDialog::sourceTypeChanged(int i)
{
    NCReportItem::SourceType ist = (NCReportItem::SourceType)i;
    tab->setEnabled( ist == NCReportItem::Static || ist == NCReportItem::Url || ist == NCReportItem::File );
}

void NCRDTextPropDialog::loadContent()
{
    switch (isPanel->sourceType() ) {
    case NCReportItem::Url:
        http->downloadContent(isPanel->sourceText());	//start download
        break;
    case NCReportItem::Static:
    case NCReportItem::Template:
    case NCReportItem::File:
        if ( /*tabDocument->isEnabled()*/ true ) {
            QFile file(isPanel->sourceText());
            if ( file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream ts(&file);
                textEdit->setHtml( ts.readAll() );
            }
        }
        break;
    case NCReportItem::DataSource:
    case NCReportItem::FileNameFromDataSource:
    case NCReportItem::UrlFromDataSource:
    case NCReportItem::Parameter:
    case NCReportItem::InternalDocument:
    default:
        break;
    }
}

void NCRDTextPropDialog::httpFinished()
{
    textEdit->setHtml( http->content() );
    htmlEdit->setPlainText( http->content() );
}

void NCRDTextPropDialog::accept()
{
    applyChanges();
    QDialog::accept();
}

void NCRDTextPropDialog::reject()
{
    discardChanges();
    QDialog::reject();
}
