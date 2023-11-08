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
#include "ncrdrunnerdialog.h"
#include "ncrddocument.h"
#include "ncrdmainwindow.h"
#include "ncrdparametereditor.h"
#include "ncreportutils.h"

#include <QTableWidgetItem>
#include <QSettings>
#include <QHeaderView>

//! 多语言
#include "MDMultiLanguage.h"

NCRDRunnerDialog::NCRDRunnerDialog(NCRDMainWindow *mw, NCRDDocument *doc, QWidget* parent)
: QDialog( parent ), Ui::NCRDRunnerDialogUI(), mainwindow(mw), defaultDocument(doc), pEditor(0), btnParam(0)
{
    setupUi(this);

    pEditor = new NCRDParameterEditor( mainwindow->dummyReportDef(), mainwindow->removedReportParameters(), tabWidget );
    tabParams->layout()->addWidget(pEditor);

    if (false) {
        btnParam = new QPushButton(tr("Parameters..."));
        btnParam->setIcon( QIcon(":/designer/images/tag.png"));
        btnParam->setCheckable(true);
        buttonBox->addButton(btnParam, QDialogButtonBox::ActionRole);

        connect(btnParam, SIGNAL(toggled(bool)), this, SLOT(extend(bool)));
    }
    buttonBox->button( QDialogButtonBox::Ok )->setText( tr("Run") );
    rbPreview->setChecked( true );

    documents = mw->openedDocuments();
    for (int i=0; i<documents.count(); i++)
        batchListWidget->addItem(documents.at(i)->fileName());

    QList<QListWidgetItem *> foundItems = batchListWidget->findItems(doc->fileName(), Qt::MatchExactly);
    if (foundItems.count()>0)
        batchListWidget->setCurrentItem( foundItems.at(0) );

    if (batchListWidget->count()==1)
        cbBatchMode->setEnabled(false);


    pEditor->load();
    extend(false);

    connect(cbBatchMode,SIGNAL(clicked(bool)),this,SLOT(enableBatchMode(bool)));

    //! 多语言
    CMDMultiLanguage::instance()->translateWidget(this);

//    QSettings settings;
//    if (batchListWidget->count()>1) {
//        bool c = settings.value("rundia_batch", false ).toBool();
//        cbBatchMode->setChecked(c);
//        enableBatchMode(c);
//    }
//    wPageRange->setText(settings.value("rundia_pagerange", QString()).toString());
}

NCRDRunnerDialog::~NCRDRunnerDialog()
{
    pEditor->save();

//    QSettings settings;
//    settings.setValue("rundia_batch", cbBatchMode->isChecked() );
//    settings.setValue("rundia_pagerange", wPageRange->text() );
}

void NCRDRunnerDialog::runReport()
{
    NCReportOutput::OutputType ot = NCReportOutput::Preview;

    if ( rbPreview->isChecked() )
        ot = NCReportOutput::Preview;
    else if ( rbQtPreview->isChecked()  )
        ot = NCReportOutput::QtPreview;
    else if ( rbPrinter->isChecked()  )
        ot = NCReportOutput::Printer;
    else if ( rbPdf->isChecked()  )
        ot = NCReportOutput::Pdf;
    else if ( rbSvg->isChecked()  )
        ot = NCReportOutput::Svg;
    else if ( rbHtml->isChecked()  )
        ot = NCReportOutput::Html;
    else if ( rbImage->isChecked()  )
        ot = NCReportOutput::Image;
    else if ( rbText->isChecked()  )
        ot = NCReportOutput::Text;
    else if ( rbExcel->isChecked()  )
        ot = NCReportOutput::ExcelHtml;

    QStringList reports;

    if (cbBatchMode->isChecked()) {
        for (int i=0; i<batchListWidget->count(); i++) {
            reports << batchListWidget->item(i)->text();    // add report file names to batch
        }
    }

    mainwindow->runReport( ot, defaultDocument, reports, wPageRange->text() );
}

void NCRDRunnerDialog::extend(bool visible)
{
    Q_UNUSED(visible)
//    tabWidget->setVisible(visible);
//    if (!visible)
    //        setMinimumWidth( 250 );
}

void NCRDRunnerDialog::enableBatchMode(bool enable)
{
    batchListWidget->setEnabled(enable);
}

