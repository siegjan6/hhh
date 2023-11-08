#include "AddVarGroupDialog.h"
#include "ui_AddVarGroupDialog.h"
#include <QMessageBox>
#include "MDMultiLanguage.h"
#include <QRegExpValidator>

CAddVarGroupDialog::CAddVarGroupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddVarGroupDialog)
{
    ui->setupUi(this);
    m_name = "";
    m_description = "";
    m_isRunning = false;
    m_configModel= false;
    QRegExp regExp("^[a-zA-Z]+[a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp,this));
}

CAddVarGroupDialog::~CAddVarGroupDialog()
{
    delete ui;
}
void CAddVarGroupDialog::setRTDBManager(IMDRTDBManager *nsRTDBManager)
{
    m_mdRTDBManager = nsRTDBManager;
}
//!设置句柄
void CAddVarGroupDialog::setCurrentVariableGroup(IMDVariableGroup *varGroup)
{
    m_curVariableGroup = varGroup;
}
void CAddVarGroupDialog::setConfigModel(bool isConfigModel)
{
    m_configModel = isConfigModel;
}
void CAddVarGroupDialog::setRunModel(bool isRunning)
{
    m_isRunning = isRunning;
}
//!设置组名
void CAddVarGroupDialog::on_nameEdit_textChanged(const QString &arg1)
{
    m_name = arg1;
}
//!设置组描述
void CAddVarGroupDialog::on_descriptionEdit_textChanged()
{
    m_description = ui->descriptionEdit->toPlainText();
}
 void  CAddVarGroupDialog::initial()
 {
     if(NULL!=m_curVariableGroup && m_configModel)
     {
         setWindowTitle(tr("变量组属性"));
         CMDMultiLanguage::instance()->translateWidget(this);
         ui->nameEdit->setText( m_curVariableGroup->localName());
         ui->descriptionEdit->setPlainText(m_curVariableGroup->description());
         if(m_isRunning)
         {
             ui->nameEdit->setEnabled(false);
             ui->descriptionEdit->setEnabled(false);
         }
     }
 }
//!确认添加变量组
void CAddVarGroupDialog::on_okButton_clicked()
{
    if(m_isRunning)
    {
        this->setResult(QDialog::Accepted);
        this->accept();
        return;
    }

    m_name.trimmed();
    if(m_name=="")
    {
       //****************************** Y2hw *****************************
       /*
       QMessageBox::warning(NULL, "Error", "变量组名称不能为空！", QMessageBox::Ok);
       */
       QMessageBox box;
       box.setText(tr("变量组名称不能为空！"));
       box.setWindowTitle(tr("Error"));
       box.setStandardButtons(QMessageBox::Ok);

       CMDMultiLanguage::instance()->translateWidget(&box);
       box.exec();
       //----------------------------------------------------------------
       return;
    }
    if(NULL!=m_curVariableGroup)
    {
        if(false == m_configModel)
        {
            QString error;
            if(m_curVariableGroup->addChildVariableGroup(m_name,m_description,error))
            {
                this->setResult(QDialog::Accepted);
                this->accept();
            }
            else
            {
                QMessageBox::warning(NULL, "Error", error, QMessageBox::Ok);
            }
        }
        else
        {
            IMDVariableGroup *parentVariableGroup = (IMDVariableGroup *)m_curVariableGroup->parentItem(MDTreeItemType::VariableGroup);
            if(NULL!=parentVariableGroup)
            {
                if(0!=QString::compare(m_curVariableGroup->localName(),m_name, Qt::CaseInsensitive))
                {
                    if(NULL!=parentVariableGroup->getChildVariableGroup(m_name))
                    {
                        //****************************** Y2hw *****************************
                        /*
                        QMessageBox::warning(NULL, "Error", "变量组已经存在！", QMessageBox::Ok);
                        */
                        QMessageBox box;
                        box.setText(tr("变量组已经存在！"));
                        box.setWindowTitle(tr("Error"));
                        box.setStandardButtons(QMessageBox::Ok);

                        CMDMultiLanguage::instance()->translateWidget(&box);
                        box.exec();
                        //----------------------------------------------------------------
                        return;
                    }
                }
                m_curVariableGroup->setLocalName(m_name);
                m_curVariableGroup->setDescription(m_description);
                this->setResult(QDialog::Accepted);
                this->accept();
            }
        }
    }
}
//!取消添加变量组
void CAddVarGroupDialog::on_cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}

QString CAddVarGroupDialog::getGroupName()
{
    return m_name;
}
