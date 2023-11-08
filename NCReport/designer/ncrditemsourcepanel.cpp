/***************************************************************************
 *   Copyright (C) 2010 by NociSoft Software Solutions
 *   support@nocisoft.com
 ***************************************************************************/
#include "ncrditemsourcepanel.h"
#include "ui_ncrditemsourcepanel.h"
#include "ncrdexpressionbuilderdialog.h"

#include <QFileInfo>
#include <QStandardItemModel>

NCRDItemSourcePanel::NCRDItemSourcePanel(QWidget *parent) : QGroupBox(parent),
    ui(new Ui::NCRDItemSourcePanel), m_item(0), m_document(0)
{
    ui->setupUi(this);
    //connect( ui->resource, SIGNAL(editingFinished()), this, SLOT(fileChanged()) );

    connect( ui->comboSourceType, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
    connect(ui->btnExpBuild,SIGNAL(clicked()),this,SLOT(openExpressionBuilder()));
    typeChanged(0);
}

NCRDItemSourcePanel::~NCRDItemSourcePanel()
{
    delete ui;
}

void NCRDItemSourcePanel::changeEvent(QEvent *e)
{
    QGroupBox::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void NCRDItemSourcePanel::typeChanged(int type)
{
    ui->labelResource->setText( ui->comboSourceType->title()+":" );
    ui->btnSelectFile->setEnabled(type == NCReportItem::Static || type == NCReportItem::File);
    ui->btnExpBuild->setEnabled(type == NCReportItem::DataSource || type == NCReportItem::Parameter ||
                                type == NCReportItem::FileNameFromDataSource || type == NCReportItem::UrlFromDataSource);
    ui->btnPreview->setEnabled(type == NCReportItem::Static || type == NCReportItem::File);
    emit sourceTypeChanged(type);
}

QAbstractButton* NCRDItemSourcePanel::buttonSelect()
{
    return ui->btnSelectFile;
}

QAbstractButton* NCRDItemSourcePanel::buttonPreview()
{
    return ui->btnPreview;
}

QAbstractButton *NCRDItemSourcePanel::buttonExpBuild()
{
    return ui->btnExpBuild;
}

QWidget* NCRDItemSourcePanel::sourceEdit()
{
    return ui->resource;
}

void NCRDItemSourcePanel::setSourceType(NCReportItem::SourceType type)
{
    ui->comboSourceType->setCurrentIndex( (int)type );
}

NCReportItem::SourceType NCRDItemSourcePanel::sourceType() const
{
    return (NCReportItem::SourceType)ui->comboSourceType->currentIndex();
}

void NCRDItemSourcePanel::setSourceText(const QString & text)
{
    ui->resource->setText(text);
}

QString NCRDItemSourcePanel::sourceText() const
{
    return ui->resource->text();
}

void NCRDItemSourcePanel::setSourceTypeEnabled( NCReportItem::SourceType type, bool enable )
{
    QStandardItemModel* model = qobject_cast<QStandardItemModel*>(ui->comboSourceType->model());
    if (model) {
        QModelIndex index = model->index((int)type, ui->comboSourceType->modelColumn(), ui->comboSourceType->rootModelIndex());
        QStandardItem* item = model->itemFromIndex(index);
        if (item) {
            item->setEnabled(enable);
        }
    }
}

void NCRDItemSourcePanel::openExpressionBuilder()
{
    if (!m_item)
        return;

    NCRDExpressionBuilderDialog dialog(m_document, m_item->availableSourceTypes(), ui->comboSourceType->currentIndex(), parentWidget());
    dialog.setExpression(ui->resource->text());

    if (dialog.exec()==QDialog::Accepted) {
        ui->resource->setText( dialog.expression() );
        ui->comboSourceType->setCurrentIndex(dialog.sourceType());
    }
}

void NCRDItemSourcePanel::setItem(NCReportItem *item)
{
    m_item = item;
    ui->comboSourceType->setAvailableItems(item->availableSourceTypes());
}

void NCRDItemSourcePanel::setDocument(NCRDDocument *document)
{
    m_document = document;
}
