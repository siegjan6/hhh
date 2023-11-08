#include "MDBatchChangeVariableDialog.h"
#include "ui_BatchChangeVariableDialog.h"

CMDBatchChangeVariableDialog::CMDBatchChangeVariableDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDBatchChangeVariableDialog)
{
    ui->setupUi(this);


    //!当前变量名+地址
    m_curVarName = "";
    m_curVarAddr = "";

    //!批量添加变量个数
    m_batchVarCnt = 0;

    //!批量变量名
    m_batchVarName = "";
    m_batchVarNameStart = "";
    m_varNameFmt10 = true;
    ui->nameFmt10Check->setChecked(true);
    ui->nameFmt16Check->setChecked(false);

    //!批量变量地址
    m_batchVarAddr = "";
    m_batchVarAddrStart = "";
    m_varAddrFmt10 = true;
    ui->addrFmt10Check->setChecked(true);
    ui->addrFmt16Check->setChecked(false);

    //!批量变量属性
    m_originalDataType = MDDataType::Bool;
    m_projectDataType = MDDataType::Bool;
    m_curDevName = "";
    m_curGroupName = "";

    m_driverManager = NULL;
    m_groupList.clear();

    connect(ui->nameFmt10Check,SIGNAL(clicked()),this,SLOT(onbatchVarNameFmt10clicked()));
    connect(ui->nameFmt16Check,SIGNAL(clicked()),this,SLOT(onbatchVarNameFmt16clicked()));
    connect(ui->addrFmt10Check,SIGNAL(clicked()),this,SLOT(onbatchVarAddrFmt10clicked()));
    connect(ui->addrFmt16Check,SIGNAL(clicked()),this,SLOT(onbatchVarAddrFmt16clicked()));
}

CMDBatchChangeVariableDialog::~CMDBatchChangeVariableDialog()
{
    delete ui;
}

void CMDBatchChangeVariableDialog::setGroupList(QStringList& groupList)
{
    m_groupList.clear();
    m_groupList.append(groupList);
}

void CMDBatchChangeVariableDialog::setDriverManager(IMDDriverManager  *driverManager)
{
    m_driverManager = driverManager;
}

void CMDBatchChangeVariableDialog::setBatchOption(batchOption operation)
{
    m_batchOption = operation;
}

void CMDBatchChangeVariableDialog::initial(IMDRTDBVariable* curVar)
{
    //!添加变量组到下拉列表batchVarNameFmt16
    ui->batchGroupListCombox->addItems(m_groupList);

    //!添加设备到下拉列表
    if(m_driverManager != NULL)
    {
        int deviceCnt = m_driverManager->getDeviceCount();
        for(int i=0;i<deviceCnt;i++)
        {
              IDevice* device = m_driverManager->getDeviceAt(i);
              ui->batchDeviceListCombox->addItem(device->name());
        }
    }
    if(curVar != NULL)
    {
        m_curVarName = curVar->localName();
        ui->curVarNameEdit->setPlainText(m_curVarName);
        m_curVarAddr = curVar->address();
        ui->curVarAddrEdit->setPlainText(m_curVarAddr);

        //!批量变量属性
        m_curDevName = curVar->deviceName();
        int nindex = curVar->name().lastIndexOf('.');
        if(nindex != -1)
            m_curGroupName = curVar->name().left(nindex);

        m_originalDataType = curVar->originalDataType();
        ui->batchOriginalDataTypeBox->setCurrentIndex((int)m_originalDataType);

        m_projectDataType = curVar->dataType();
        ui->batchProjectDataTypeBox->setCurrentIndex((int)m_projectDataType);
    }
    //!根据选项设定控件的使能属性
    //!批量增加变量
    if(m_batchOption == batchOption::AddVar)
    {
        ui->curVarNameEdit->setEnabled(true);
        ui->curVarAddrEdit->setEnabled(true);
        ui->batchVarAddrEdit->setEnabled(true);
        ui->nameFmt10Check->setEnabled(true);
        ui->nameFmt16Check->setEnabled(true);
        ui->batchVarNameEdit->setEnabled((true));
        ui->addrFmt10Check->setEnabled(true);
        ui->addrFmt16Check->setEnabled(true);
        ui->batchVarCntEdit->setEnabled(true);
    }
    else
    {
        ui->curVarNameEdit->setEnabled(false);
        ui->curVarAddrEdit->setEnabled(false);
        ui->batchVarAddrEdit->setEnabled(false);
        ui->nameFmt10Check->setEnabled(false);
        ui->nameFmt16Check->setEnabled(false);
        ui->batchVarNameEdit->setEnabled(false);
        ui->addrFmt10Check->setEnabled(false);
        ui->addrFmt16Check->setEnabled(false);
        ui->batchVarCntEdit->setEnabled(false);
    }

    //!设置当前group
    if(!m_curGroupName.isEmpty())
        ui->batchGroupListCombox->setCurrentText(m_curGroupName);

    //!设置当前Device
    if(!m_curDevName.isEmpty())
        ui->batchDeviceListCombox->setCurrentText(curVar->deviceName());
}
int CMDBatchChangeVariableDialog::getBatchVarCnt()
{
    return m_batchVarCnt;
}

MDDataType CMDBatchChangeVariableDialog::getOriginalDataType()
{
    return m_originalDataType;
}

MDDataType CMDBatchChangeVariableDialog::getProjectDataType()
{
    return m_projectDataType;
}

QString CMDBatchChangeVariableDialog::getDevName()
{
    return m_curDevName;
}
QString CMDBatchChangeVariableDialog::getGroupName()
{
    return m_curGroupName;
}

QString CMDBatchChangeVariableDialog::getbatchVarName()
{
    return m_batchVarName;
}

QString CMDBatchChangeVariableDialog::getbatchVarNameStart()
{
    return m_batchVarNameStart;
}

bool CMDBatchChangeVariableDialog::getvarNameFmt10()
{
    return m_varNameFmt10;
}

QString CMDBatchChangeVariableDialog::getbatchVarAddr()
{
    return m_batchVarAddr;
}

QString CMDBatchChangeVariableDialog::getbatchVarAddrStart()
{
    return m_batchVarAddrStart;
}

bool CMDBatchChangeVariableDialog::getvarAddrFmt10()
{
    return m_varAddrFmt10;
}

void CMDBatchChangeVariableDialog::on_okButton_clicked()
{
    m_batchVarCnt = QString(ui->batchVarCntEdit->toPlainText()).toInt();
    m_batchVarNameStart = ui->batchVarNameStartEdit->toPlainText();
    m_batchVarName = ui->batchVarNameEdit->toPlainText();
    m_batchVarAddrStart = ui->batchVarAddrStartEdit->toPlainText();
    m_batchVarAddr = ui->batchVarAddrEdit->toPlainText();

    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDBatchChangeVariableDialog::on_cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject();
}

void CMDBatchChangeVariableDialog::on_batchGroupListCombox_currentTextChanged(const QString &arg1)
{
    m_curGroupName = arg1;
}

void CMDBatchChangeVariableDialog::on_batchDeviceListCombox_currentTextChanged(const QString &arg1)
{    
    m_curDevName = arg1;
}

void CMDBatchChangeVariableDialog::on_originalDataTypeBox_currentIndexChanged(int index)
{
    m_originalDataType = (MDDataType)index;
}

void CMDBatchChangeVariableDialog::on_projectDataTypeBox_currentIndexChanged(int index)
{
    m_projectDataType = (MDDataType)index;
}


void CMDBatchChangeVariableDialog::onbatchVarNameFmt10clicked()
{
    m_varNameFmt10 = true;
    ui->nameFmt16Check->setChecked(false);
}

void CMDBatchChangeVariableDialog::onbatchVarNameFmt16clicked()
{
    m_varNameFmt10 = false;
    ui->nameFmt10Check->setChecked(false);
}

void CMDBatchChangeVariableDialog::onbatchVarAddrFmt10clicked()
{
    m_varAddrFmt10 = true;
    ui->addrFmt16Check->setChecked(false);
}

void CMDBatchChangeVariableDialog::onbatchVarAddrFmt16clicked()
{
    m_varAddrFmt10 = false;
    ui->addrFmt10Check->setChecked(false);
}

