#include "MDActionSPDialog.h"
#include "ui_MDActionSPDialog.h"
#include "MDActionSPUIImp.h"
#include "MDActionID.h"
#include "MDStringResource.h"

CMDActionSPDialog::CMDActionSPDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDActionSPDialog)
{
    ui->setupUi(this);

    m_pgConfiger = NULL;
    m_sourceProp = NULL;
    m_twTool = CMDMultiLanguage::instance()->createWidgetTool();
}

CMDActionSPDialog::~CMDActionSPDialog()
{
    delete ui;
    if(m_twTool)
        delete m_twTool;
}

void CMDActionSPDialog::initial(CMDPowerGroupConfiger* configer, CMDActionSP *sourceProperty)
{
    setWindowTitle(STRANS(CMDActionID::typeToString(sourceProperty->actionID().type())) + " - " + sourceProperty->actionID().objName());

    m_pgConfiger = configer;
    m_sourceProp = sourceProperty;

    // 保护模式
    QStringList modeList;
    CMDActionSP::enumProtectModes(modeList);
    foreach (const QString& modeID, modeList)
    {
        MDActionProtectMode pMode = CMDActionSP::stringToMode(modeID);
        ui->protectModeComboBox->addItem(modeID, QVariant((int)pMode));
    }
    ui->protectModeComboBox->setCurrentText(CMDActionSP::modeToString(sourceProperty->protectMode()));

    // 签名类型
    QStringList typeList;
    CMDActionSP::enumSigTypes(typeList);
    foreach (const QString& typeID, typeList)
    {
        MDSigType sType = CMDActionSP::stringToSigType(typeID);
        ui->sigTypeComboBox->addItem(typeID, QVariant((int)sType));
    }
    ui->sigTypeComboBox->setCurrentText(CMDActionSP::sigTypeToString(sourceProperty->sigType()));

    ui->recordSigResultCheckBox->setCheckState(sourceProperty->isEnableLog() ? Qt::Checked : Qt::Unchecked);
    QStringList list1 = sourceProperty->operatorGroupList();
    ui->operatorGroupListWidget->addItems(list1);
    ui->verifierGroupListWidget->addItems(sourceProperty->verifierGroupList());

    enableUI(sourceProperty->protectMode(), sourceProperty->sigType());

    if(m_twTool)
    {
        m_twTool->init(this);
        m_twTool->changeLanguage();
    }
}

bool CMDActionSPDialog::UI2Property()
{
    MDActionProtectMode mode = (MDActionProtectMode)ui->protectModeComboBox->currentData().toInt();
    m_sourceProp->setProtectMode(mode);
    MDSigType type = (MDSigType)ui->sigTypeComboBox->currentData().toInt();
    m_sourceProp->setSigType(type);

    QStringList operatorGroupList;
    int ocount = ui->operatorGroupListWidget->count();
    for(int i = 0; i < ocount; i++)
    {
        QListWidgetItem* item = ui->operatorGroupListWidget->item(i);
        operatorGroupList.append(item->text());
    }
    m_sourceProp->setOperatorGroupList(operatorGroupList);

    QStringList verifierGroupList;
    int vcount = ui->verifierGroupListWidget->count();
    for(int i = 0; i < vcount; i++)
    {
        QListWidgetItem* item = ui->verifierGroupListWidget->item(i);
        verifierGroupList.append(item->text());
    }
    m_sourceProp->setVerifierGroupList(verifierGroupList);

    m_sourceProp->setEnableLog(Qt::Checked == ui->recordSigResultCheckBox->checkState());

    return true;
}

void CMDActionSPDialog::enableUI(MDActionProtectMode mode, MDSigType sType)
{
    switch (mode)
    {
    case MDActionProtectMode::null_mode:
    {
        setUIVisible(false);
    }
        break;
    case MDActionProtectMode::MindSCADA_Sig:
    {
        setUIVisible(true);

        ui->sigTypeComboBox->setCurrentText(STRANS(CMDActionSP::sigTypeToString(MDSigType::operator_only)));
        ui->sigTypeComboBox->setEnabled(false);

        ui->powerGroupTabWidget->setCurrentWidget(ui->operatorGroupTab);
        ui->powerGroupTabWidget->removeTab(1);

        ui->recordSigResultCheckBox->setEnabled(true);
        ui->recordSigResultCheckBox->setCheckState(m_sourceProp->isEnableLog() ? Qt::Checked : Qt::Unchecked);
    }
        break;
    case MDActionProtectMode::Electronic_Sig:
    {
        setUIVisible(true);

        ui->sigTypeComboBox->setEnabled(true);

        // 关于 tab 控件的页面数：
        // MDActionSPDialog.ui 文件设计了 2 个页面，即操作组和校验组；
        // 操作组页面一定会被保留，校验组页面则可能被 removeTab(1);
        // 因此页面数至少为 1。
        if(MDSigType::operator_only == sType)
        {
            ui->powerGroupTabWidget->removeTab(1);
        }
        else if(MDSigType::operator_then_verifier == sType)
        {
            if(1 == ui->powerGroupTabWidget->count())
                ui->powerGroupTabWidget->addTab(ui->verifierGroupTab, STRANS(STRING_VERIFIER_GROUP));
        }

        ui->recordSigResultCheckBox->setEnabled(false);
        ui->recordSigResultCheckBox->setCheckState(Qt::Checked);
    }
        break;
    default:
        break;
    }
}

void CMDActionSPDialog::setUIVisible(bool visible)
{
    ui->sigTypeLabel->setVisible(visible);
    ui->sigTypeComboBox->setVisible(visible);
    ui->powerGroupTabWidget->setVisible(visible);
    ui->recordSigResultCheckBox->setVisible(visible);
}

void CMDActionSPDialog::on_OKButton_clicked()
{
    UI2Property();

    setResult(QDialog::Accepted);
    accept();
}

void CMDActionSPDialog::on_cancelButton_clicked()
{
    setResult(QDialog::Rejected);
    reject();
}

void CMDActionSPDialog::on_removeOperatorGroupButton_clicked()
{
    ui->operatorGroupListWidget->takeItem(ui->operatorGroupListWidget->currentRow());
}

void CMDActionSPDialog::on_modifyOperatorGroupButton_clicked()
{
    if(!m_pgConfiger)
        return;

    QStringList sourceList;
    int count = ui->operatorGroupListWidget->count();
    for(int i = 0; i < count; i++)
    {
        QListWidgetItem* item = ui->operatorGroupListWidget->item(i);
        sourceList.append(item->text());
    }
    QStringList destList;
    if(CMDActionSPUIImp::selectPowerGroups(m_pgConfiger, sourceList, destList))
    {
        ui->operatorGroupListWidget->clear();
        ui->operatorGroupListWidget->addItems(destList);
    }
}

void CMDActionSPDialog::on_removeVerifierGroupButton_clicked()
{
    ui->verifierGroupListWidget->takeItem(ui->verifierGroupListWidget->currentRow());
}

void CMDActionSPDialog::on_modifyVerifierGroupButton_clicked()
{
    if(!m_pgConfiger)
        return;

    QStringList sourceList;
    int count = ui->verifierGroupListWidget->count();
    for(int i = 0; i < count; i++)
    {
        QListWidgetItem* item = ui->verifierGroupListWidget->item(i);
        sourceList.append(item->text());
    }
    QStringList destList;
    if(CMDActionSPUIImp::selectPowerGroups(m_pgConfiger, sourceList, destList))
    {
        ui->verifierGroupListWidget->clear();
        ui->verifierGroupListWidget->addItems(destList);
    }
}

void CMDActionSPDialog::on_protectModeComboBox_currentIndexChanged(const QString &arg1)
{
    MDActionProtectMode mode = (MDActionProtectMode)ui->protectModeComboBox->currentData().toInt();
    MDSigType type = (MDSigType)ui->sigTypeComboBox->currentData().toInt();
    enableUI(mode, type);
}

void CMDActionSPDialog::on_sigTypeComboBox_currentIndexChanged(int index)
{
    MDActionProtectMode mode = (MDActionProtectMode)ui->protectModeComboBox->currentData().toInt();
    MDSigType type = (MDSigType)ui->sigTypeComboBox->currentData().toInt();
    enableUI(mode, type);
}
