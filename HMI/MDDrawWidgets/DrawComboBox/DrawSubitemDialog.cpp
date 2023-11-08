#include "DrawSubitemDialog.h"
#include "ui_DrawSubitemDialog.h"
#include "MDMultiLanguage.h"
CDrawSubitemDialog::CDrawSubitemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DrawSubitemDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("ComboBox子项编辑框"));
//    CMDMultiLanguage::instance()->translateWidget(this);
}

CDrawSubitemDialog::~CDrawSubitemDialog()
{
    delete ui;
}
/*!
\section  编辑子项
*/
void CDrawSubitemDialog::setItems(const QStringList &str)
{
    QString showString;
    for(int i=0;i<str.count();i++)
    {
        showString.append(str.at(i));
        showString.append(breakString());
    }
    ui->textEdit->setPlainText(showString);
}

QString CDrawSubitemDialog::items()
{
    return ui->textEdit->toPlainText();
}

void CDrawSubitemDialog::on_okButton_clicked()
{
    accept();
}

void CDrawSubitemDialog::on_cancelButton_clicked()
{
    reject();
}
