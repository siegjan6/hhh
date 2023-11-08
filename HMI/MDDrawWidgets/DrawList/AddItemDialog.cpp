#include "AddItemDialog.h"
#include "ui_AddItemDialog.h"
#include "MDMultiLanguage.h"
CAddItemDialog::CAddItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddItemDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("List子项编辑框"));
    CMDMultiLanguage::instance()->translateWidget(this);
}

CAddItemDialog::~CAddItemDialog()
{
    delete ui;
}
/*!
\section  编辑子项
*/
QString CAddItemDialog::items() const
{
    return ui->textEdit->toPlainText();
}

void CAddItemDialog::setItems(const QStringList &str)
{
    QString showString;
    for(int i = 0;i <str.count();i++)
    {
        showString.append(str.at(i));
        showString.append(breakString());
    }
    ui->textEdit->setPlainText(showString);
}

void CAddItemDialog::on_okButton_clicked()
{
    accept();
}

void CAddItemDialog::on_cancelButton_clicked()
{
    reject();
}
