#include "FormTypeDialog.h"
#include "ui_FormTypeDialog.h"
#include "MDMultiLanguage.h"

CFormTypeDialog::CFormTypeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFormTypeDialog)
{
    ui->setupUi(this);

    CMDMultiLanguage::instance()->translateWidget(this);
}
CFormTypeDialog::~CFormTypeDialog()
{
    delete ui;
}
//////////////////////////////////////////////////////////////////////////
void CFormTypeDialog::on_comboBoxType_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0://normal
        ui->checkBoxFrame->setEnabled(true);
        ui->checkBoxFullScreen->setEnabled(true);
        break;
    case 1://modal
        ui->checkBoxFrame->setEnabled(true);
        ui->checkBoxFullScreen->setEnabled(false);
        break;
    case 2://topmost
        ui->checkBoxFrame->setEnabled(true);
        ui->checkBoxFullScreen->setEnabled(false);
        break;
    default:
        break;
    }
}
void CFormTypeDialog::on_pushButtonOk_clicked()
{
    accept();
}
void CFormTypeDialog::on_pushButtonCancel_clicked()
{
    reject();
}
//////////////////////////////////////////////////////////////////////////
void CFormTypeDialog::init(FormType type, bool hasFrame, bool fullScreen)
{
    ui->comboBoxType->setCurrentIndex((int)type);
    ui->checkBoxFrame->setChecked(hasFrame);
    ui->checkBoxFullScreen->setChecked(fullScreen);

    on_comboBoxType_currentIndexChanged(int(type));
}
FormType CFormTypeDialog::type()
{
    return (FormType)ui->comboBoxType->currentIndex();
}
bool CFormTypeDialog::hasFrame()
{
    return (ui->checkBoxFrame->checkState() == Qt::Checked);
}
bool CFormTypeDialog::fullScreen()
{
    return (ui->checkBoxFullScreen->checkState() == Qt::Checked);
}
//////////////////////////////////////////////////////////////////////////
