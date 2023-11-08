#include "CDrawTextEditTextDialog.h"
#include "ui_CDrawTextEditTextDialog.h"
#include "MDMultiLanguage.h"

CDrawTextEditTextDialog::CDrawTextEditTextDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CDrawTextEditTextDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("编辑框输入框"));
}

CDrawTextEditTextDialog::~CDrawTextEditTextDialog()
{
    delete ui;
}

/*!
\section  编辑子项
*/
void CDrawTextEditTextDialog::setItems(const QString &str)
{
    ui->textEdit->setPlainText(str);
}

QString CDrawTextEditTextDialog::items()
{
    return ui->textEdit->toPlainText();
}

void CDrawTextEditTextDialog::on_okButton_clicked()
{
    this->accept();
}

void CDrawTextEditTextDialog::on_cancelButton_clicked()
{
    this->reject();
}
