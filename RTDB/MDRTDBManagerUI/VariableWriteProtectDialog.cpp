#include "VariableWriteProtectDialog.h"
#include "ui_VariableWriteProtectDialog.h"
#include "MDMultiLanguage.h"

 CVariableWriteProtectDialog::CVariableWriteProtectDialog(QWidget *parent) :
     QDialog(parent),
     ui(new Ui::CVariableWriteProtectDialog)
 {
     m_isRunning = false;
     m_writeProtectParameter.m_enableWrite = true;
     m_writeProtectParameter.m_enableWriteProtect = true;
     m_writeProtectParameter.m_maxValue = "";
     m_writeProtectParameter.m_minValue = "";
     m_writeProtectParameter.m_writeProtectType = 1;
     ui->setupUi(this);
     connect(ui->maxValueEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onMaxValueEditTextChanged(const QString &)));
     connect(ui->minValueEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onMinValueEditTextChanged(const QString &)));
     connect(ui->proectTypeRadioButton0,SIGNAL(clicked()),this,SLOT(onProectTypeRadioButtonClicked()));
     connect(ui->proectTypeRadioButton1,SIGNAL(clicked()),this,SLOT(onProectTypeRadioButtonClicked()));
     connect(ui->enableWriteCheckBox,SIGNAL(clicked()),this,SLOT(onEnableWriteCheckBoxClicked()));
     connect(ui->enableWriteProtectCheckBox,SIGNAL(clicked()),this,SLOT(onEnableWriteProtectCheckBoxClicked()));
     //多语言
     CMDMultiLanguage::instance()->translateWidget(this);
 }

 CVariableWriteProtectDialog::~CVariableWriteProtectDialog()
 {
     delete ui;
 }
 void CVariableWriteProtectDialog::setRunModel(bool isRunning)
 {
     m_isRunning = isRunning;
 }
 void CVariableWriteProtectDialog::onMaxValueEditTextChanged(const QString & arg1)
 {
     m_writeProtectParameter.m_maxValue  = ui->maxValueEdit->text();
 }
 void CVariableWriteProtectDialog::onMinValueEditTextChanged(const QString & arg1)
 {
     m_writeProtectParameter.m_minValue  = ui->minValueEdit->text();
 }
 void CVariableWriteProtectDialog::onEnableWriteCheckBoxClicked()
 {
     if(ui->enableWriteCheckBox->checkState() == Qt::CheckState::Checked)
     {
         m_writeProtectParameter.m_enableWrite = true;
     }
     else
     {
         m_writeProtectParameter.m_enableWrite = false;
     }
     updateContols();
 }
 void CVariableWriteProtectDialog::onEnableWriteProtectCheckBoxClicked()
 {
     if(ui->enableWriteProtectCheckBox->checkState() == Qt::CheckState::Checked)
     {
         m_writeProtectParameter.m_enableWriteProtect = true;
     }
     else
     {
         m_writeProtectParameter.m_enableWriteProtect = false;
     }
     updateContols();
 }
 void CVariableWriteProtectDialog::onProectTypeRadioButtonClicked()
 {
     m_writeProtectParameter.m_writeProtectType = 0;
     if(ui->proectTypeRadioButton1->isChecked())
     {
          m_writeProtectParameter.m_writeProtectType = 1;
     }
     updateContols();
 }

 void CVariableWriteProtectDialog::initial(IMDRTDBVariable *curRTDBVariable)
 {
     if(curRTDBVariable!= NULL)
     {
         m_writeProtectParameter = curRTDBVariable->writeProtectParameter();
     }
     updateContols();
 }
 void CVariableWriteProtectDialog::updateContols()
 {

     if(m_writeProtectParameter.m_enableWriteProtect)
     {
         ui->enableWriteProtectCheckBox->setChecked(true);
         ui->maxValueEdit->setEnabled(true);
         ui->minValueEdit->setEnabled(true);
         ui->proectTypeRadioButton0->setEnabled(true);
         ui->proectTypeRadioButton1->setEnabled(true);
     }
     else
     {
         ui->enableWriteProtectCheckBox->setChecked(false);
         ui->maxValueEdit->setEnabled(false);
         ui->minValueEdit->setEnabled(false);
         ui->proectTypeRadioButton0->setEnabled(false);
         ui->proectTypeRadioButton1->setEnabled(false);
     }

     if(m_writeProtectParameter.m_enableWrite)
     {
         ui->enableWriteCheckBox->setChecked(true);
         ui->enableWriteProtectCheckBox->setEnabled(true);
     }
     else
     {
         ui->enableWriteCheckBox->setChecked(false);
         ui->enableWriteProtectCheckBox->setEnabled(false);
         ui->maxValueEdit->setEnabled(false);
         ui->minValueEdit->setEnabled(false);
         ui->proectTypeRadioButton0->setEnabled(false);
         ui->proectTypeRadioButton1->setEnabled(false);
     }

     if(m_writeProtectParameter.m_writeProtectType ==0)
     {
         ui->proectTypeRadioButton0->setChecked(true);
         ui->proectTypeRadioButton1->setChecked(false);
     }
     else
     {
         ui->proectTypeRadioButton0->setChecked(false);
         ui->proectTypeRadioButton1->setChecked(true);
     }
     ui->maxValueEdit->setText(m_writeProtectParameter.m_maxValue);
     ui->minValueEdit->setText(m_writeProtectParameter.m_minValue);

     ui->expressButton1->setVisible(false);
     ui->expressButton2->setVisible(false);

    /*/
     if(m_isRunning)
     {
         ui->maxValueEdit->setEnabled(false);
         ui->minValueEdit->setEnabled(false);
         ui->proectTypeRadioButton0->setEnabled(false);
         ui->proectTypeRadioButton1->setEnabled(false);
         ui->enableWriteCheckBox->setEnabled(false);
         ui->enableWriteProtectCheckBox->setEnabled(false);
         ui->expressButton1->setEnabled(false);
         ui->expressButton2->setEnabled(false);
     }/*/

 }
