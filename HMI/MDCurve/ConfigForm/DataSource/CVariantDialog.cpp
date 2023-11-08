#include "CVariantDialog.h"
#include "ui_CVariantDialog.h"
#include "CTranslator.h"
#include <QListWidgetItem>

using namespace Translator;
CVariantDialog::CVariantDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariantDialog),
    m_translatorTool(NULL)
{
    ui->setupUi(this);

    installDialogTranslator(m_translatorTool,this);
}

CVariantDialog::~CVariantDialog()
{
    delete ui;
}

QStringList CVariantDialog::getField(bool *ok, QString &initial, const QStringList &fields, QWidget *parent)
{
    CVariantDialog dialog(parent);
    dialog.setFields(fields);
    dialog.setSelectField(initial);
    dialog.initialize();
    int ret = dialog.exec();

    if (ok)
        *ok = !!ret;

    if (ret) {
        return dialog.selectField();
    }
    return QStringList(initial);
    //    return ret? dialog.selectField() : initial;
}

QStringList CVariantDialog::getField(bool *ok, const QStringList &fields, QWidget *parent)
{

    CVariantDialog dialog(parent);
    dialog.setFields(fields);
    dialog.initialize();
    int ret = dialog.exec();

    if (ok)
        *ok = !!ret;
    return dialog.selectField();
}
QStringList CVariantDialog::fields() const
{
    return m_fields;
}

void CVariantDialog::setFields(const QStringList &fields)
{
    m_fields = fields;
}

void CVariantDialog::initialize()
{
    ui->listWidget->clear();
    addItems(m_fields);
    int row = findItem(*ui->listWidget,m_selectField);
    if(row >= 0){
        ui->listWidget->item(row)->setCheckState(Qt::Checked);
    }
    connect(ui->checkAllButton,SIGNAL(clicked()),this,SLOT(slotCheckAll()));
    connect(ui->uncheckedButton,SIGNAL(clicked()),this,SLOT(slotUnchecked()));
}

void CVariantDialog::addItems(const QStringList &itemNames)
{
    foreach (const QString &name, itemNames)
    {
        ui->listWidget->addItem(createListItem(name));
    }
}

QListWidgetItem *CVariantDialog::createListItem(const QString &name)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setCheckState(Qt::Unchecked);
    item->setText(name);
    return item;
}

void CVariantDialog::slotUnchecked()
{
    for(int i = 0; i < ui->listWidget->count();i++){
        ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
    }
}

void CVariantDialog::slotCheckAll()
{
    for(int i = 0; i < ui->listWidget->count();i++){
        ui->listWidget->item(i)->setCheckState(Qt::Checked);
    }
}
QStringList CVariantDialog::selectField()
{
    QStringList selectFields;
    for(int i = 0; i < ui->listWidget->count();i++){
        QListWidgetItem *item = ui->listWidget->item(i);
        if(item->checkState() == Qt::Checked){
            selectFields.append(item->text());
        }
    }
    return selectFields;
}

void CVariantDialog::setSelectField(const QString &selectField)
{
    m_selectField = selectField;
}

int CVariantDialog::findItem(const QListWidget &widget, const QString &text)
{
    for(int i = 0; i < widget.count();i++) {
        if(text == widget.item(i)->text())
            return i;
    }
    return -1;
}
