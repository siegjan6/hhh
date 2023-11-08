#include "VariableSecurityPolicyDialog.h"
#include "ui_VariableSecurityPolicyDialogui.h"
#include "MDMultiLanguage.h"

CVariableSecurityPolicyDialog::CVariableSecurityPolicyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariableSecurityPolicyDialog)
{
    m_isRunning = false;
    ui->setupUi(this);
    ui->securityPolicyTableWidget->setColumnWidth(0, 100);
    ui->securityPolicyTableWidget->setColumnWidth(1, 275);
    connect(ui->securityPolicyTableWidget, SIGNAL(itemDoubleClicked(QTableWidgetItem *)),this,SLOT(onSecurityPolicyTableWidgetItemDoubleClicked(QTableWidgetItem *)));
    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
}

CVariableSecurityPolicyDialog::~CVariableSecurityPolicyDialog()
{
    delete ui;
}

void CVariableSecurityPolicyDialog::setRunModel(bool isRunning)
{
    m_isRunning = isRunning;
}
//!初始对话框
void CVariableSecurityPolicyDialog::initial(CMDPowerGroupConfiger* powerGroupConfiger,IMDRTDBVariable *curRTDBVariable)
{
    if(curRTDBVariable == NULL)
    {
         //默认动作类型：写值
         CMDActionSP*  actionSP = m_actionSPList.append(MDActionType::variable_WriteValue );
         actionSP->actionID().setObjName("");
    }
    else
    {
        m_actionSPList = *curRTDBVariable->actionSPList();
    }
    m_powerGroupConfiger = powerGroupConfiger;
    updateContols();
}
//!双击动作，显示动作对话框
void CVariableSecurityPolicyDialog::onSecurityPolicyTableWidgetItemDoubleClicked(QTableWidgetItem *item)
{
    if(item)
    {
        CMDActionSP* sp = m_actionSPList.at(item->row());
        CMDActionSPUI dlg;
        dlg.init(m_powerGroupConfiger);
        if(dlg.doModalActionSPDialog(sp))
        {
            updateContols();
        }
    }
}
//!刷新动作安全策略列表
void CVariableSecurityPolicyDialog::updateContols()
{
    ui->securityPolicyTableWidget->setRowCount(0);
    ui->securityPolicyTableWidget->clearContents();

    for(int i = 0; i < m_actionSPList.count(); i++)
    {
        ui->securityPolicyTableWidget->insertRow(i);
        CMDActionSP* sp = m_actionSPList.at(i);
        QTableWidgetItem* ti0 = new QTableWidgetItem();
        ti0->setData(Qt::DisplayRole, QVariant(CMDActionID::typeToString(sp->actionID().type())));
        ui->securityPolicyTableWidget->setItem(i, 0, ti0);

        QString spText = CMDMultiLanguage::instance()->value(tr("<未启用>"));
        if(sp->isEnable())
        {
            spText = CMDActionSP::modeToString(sp->protectMode());
            spText += "::";
            spText += CMDActionSP::sigTypeToString(sp->sigType());
        }
        QTableWidgetItem* ti1 = new QTableWidgetItem;
        ti1->setData(Qt::DisplayRole, QVariant(spText));
        ui->securityPolicyTableWidget->setItem(i, 1, ti1);

        QTableWidgetItem* ti2 = new QTableWidgetItem(QTableWidgetItem::ItemType::UserType);
        ti2->setData(Qt::UserRole, QVariant(-1, sp));
        ui->securityPolicyTableWidget->setItem(i, 2, ti2);
    }/*/
    if(m_isRunning)
    {
        ui->securityPolicyTableWidget->setEnabled(false);
    }/*/
}
