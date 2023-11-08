#include "VariableAlarmDialog.h"
#include "ui_VariableAlarmDialog.h"
#include "MDMultiLanguage.h"
#include "SelectVariableDialog.h"

 CVariableAlarmDialog::CVariableAlarmDialog(QWidget *parent) :
     QDialog(parent),
     ui(new Ui::CVariableAlarmDialog)
 {
     m_isRunning = false;
     m_variableAlarmParameter.m_enableAlarm = true;
     m_variableAlarmParameter.m_HHLValue = "";
     m_variableAlarmParameter.m_HLValue = "";
     m_variableAlarmParameter.m_LLLValue = "";
     m_variableAlarmParameter.m_LLValue = "";
     ui->setupUi(this);
     connect(ui->HHLEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onHHLEditTextChanged(const QString &)));
     connect(ui->HLEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onHLEditTextChanged(const QString &)));
     connect(ui->LLLEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onLLLEditTextChanged(const QString &)));
     connect(ui->LLEdit,SIGNAL(textChanged(const QString &)),this,SLOT(onLLEditTextChanged(const QString &)));
     connect(ui->enableVarLimitCheckBox,SIGNAL(clicked()),this,SLOT(onEnableAlarmCheckBoxClicked()));
     //多语言
     CMDMultiLanguage::instance()->translateWidget(this);
 }

 CVariableAlarmDialog::~CVariableAlarmDialog()
 {
     delete ui;
 }
 void CVariableAlarmDialog::setRunModel(bool isRunning)
 {
     m_isRunning = isRunning;
 }
 void CVariableAlarmDialog::onHHLEditTextChanged(const QString & arg1)
 {
     m_variableAlarmParameter.m_HHLValue = ui->HHLEdit->text();
 }
 void CVariableAlarmDialog::onHLEditTextChanged(const QString & arg1)
 {
     m_variableAlarmParameter.m_HLValue  = ui->HLEdit->text();
 }
 void CVariableAlarmDialog::onLLLEditTextChanged(const QString & arg1)
 {
     m_variableAlarmParameter.m_LLLValue = ui->LLLEdit->text();
 }
 void CVariableAlarmDialog::onLLEditTextChanged(const QString & arg1)
 {
     m_variableAlarmParameter.m_LLValue  = ui->LLEdit->text();
 }
 void CVariableAlarmDialog::onEnableAlarmCheckBoxClicked()
 {
     if(ui->enableVarLimitCheckBox->checkState() == Qt::CheckState::Checked)
     {
         m_variableAlarmParameter.m_enableAlarm = true;
     }
     else
     {
         m_variableAlarmParameter.m_enableAlarm = false;
     }
     updateContols();
 }

 void CVariableAlarmDialog::initial(IMDRTDBVariable *curRTDBVariable)
 {
     if(curRTDBVariable!= NULL)
     {
         m_variableAlarmParameter = curRTDBVariable->variableAlarmParameter();
     }
     updateContols();
 }
 void CVariableAlarmDialog::updateContols()
 {
     if(m_variableAlarmParameter.m_enableAlarm)
     {
         ui->enableVarLimitCheckBox->setChecked(true);
         ui->HHLEdit->setEnabled(true);
         ui->HLEdit->setEnabled(true);
         ui->LLLEdit->setEnabled(true);
         ui->LLEdit->setEnabled(true);
         ui->HHLExpressButton->setEnabled(true);
         ui->HLExpressButton->setEnabled(true);
         ui->LLLExpressButton->setEnabled(true);
         ui->LLExpressButton->setEnabled(true);
     }
     else
     {
         ui->enableVarLimitCheckBox->setChecked(false);
         ui->HHLEdit->setEnabled(false);
         ui->HLEdit->setEnabled(false);
         ui->LLLEdit->setEnabled(false);
         ui->LLEdit->setEnabled(false);
         ui->HHLExpressButton->setEnabled(false);
         ui->HLExpressButton->setEnabled(false);
         ui->LLLExpressButton->setEnabled(false);
         ui->LLExpressButton->setEnabled(false);
     }

     ui->HHLEdit->setText(m_variableAlarmParameter.m_HHLValue);
     ui->HLEdit->setText(m_variableAlarmParameter.m_HLValue);
     ui->LLLEdit->setText(m_variableAlarmParameter.m_LLLValue);
     ui->LLEdit->setText(m_variableAlarmParameter.m_LLValue);

    /*/
     if(m_isRunning)
     {
         ui->HHLEdit->setEnabled(false);
         ui->HLEdit->setEnabled(false);
         ui->LLLEdit->setEnabled(false);
         ui->LLEdit->setEnabled(false);
         ui->HHLExpressButton->setEnabled(false);
         ui->HLExpressButton->setEnabled(false);
         ui->LLLExpressButton->setEnabled(false);
         ui->LLExpressButton->setEnabled(false);
     }/*/

}

 void CVariableAlarmDialog::setRTDBManager(IMDRTDBManager  *rtdbManager)
 {
     m_rtdbManager = rtdbManager;
 }
 //!获取选中变量名相加后的字符串赋值给le
 bool CVariableAlarmDialog::selectVariables(QLineEdit* le)
 {
     QStringList varList;
     if(selectVariables(varList))
     {
         for(int i=0;i<varList.count();i++)
         {
             QString variable = varList[i];
             if(variable==NULL)
              {
                 continue;
              }
              //拼接表达式
              QString express = le->text();
              express.trimmed();
              if(!express.isEmpty())
              {
                  //默认+法
                  express=express+"+"+variable;
              }
              else
              {
                  express = variable;
              }
              le->setText(express);
         }
         return true;
     }
     return false;
 }
 //!获取选中变量名
 bool CVariableAlarmDialog::selectVariables(QStringList &varNameList)
 {
     bool ret = false;
     QList<IMDVariable*>varList;
     if(selectVariables(varList))
     {
         for(int i=0;i<varList.count();i++)
         {
             IMDVariable *variable = (IMDVariable *)varList[i];
             varNameList.append(variable->name());
         }
         ret = true;
     }
     return ret;
 }
 //!获取选中变量
 bool CVariableAlarmDialog::selectVariables(QList<IMDVariable*>&varList)
 {
     bool ret = false;
     CSelectVariableDialog dlg;
     dlg.setRTDBManager(m_rtdbManager);
     dlg.exec();
     if(QDialog::Accepted == dlg.result())
     {
      dlg.getSelectedVariables(varList);
      ret = true;
     }
     return ret;
 }

 void CVariableAlarmDialog::on_HHLExpressButton_clicked()
 {
     selectVariables(ui->HHLEdit);
 }

void CVariableAlarmDialog::on_HLExpressButton_clicked()
{
    selectVariables(ui->HLEdit);
}

void CVariableAlarmDialog::on_LLExpressButton_clicked()
{
    selectVariables(ui->LLEdit);
}

void CVariableAlarmDialog::on_LLLExpressButton_clicked()
{
    selectVariables(ui->LLLEdit);
}
