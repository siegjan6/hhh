#include "VariablesBatchChangeDialog.h"
#include "ui_VariablesBatchChangeDialog.h"

#include "IMDRTDBManager.h"
#include "IMDVariableGroup.h"
#include "PropertyBrowser.h"
#include "MDRTDBBatchChangeVariable.h"
#include "MDRTDBVariablesWidget.h"
CMDVariablesBatchChangeDialog::CMDVariablesBatchChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDVariablesBatchChangeDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("批量修改变量属性"));

    //创建PropertyBrowse控件
    m_propertyBrowser = new CPropertyBrowser(this);
    m_propertyBrowser->setGeometry(QRect(0, 0, 380, 510));
    ui->cancelButton->setText(tr("取消"));
    ui->okButton->setText(tr("确定"));
}

CMDVariablesBatchChangeDialog::~CMDVariablesBatchChangeDialog()
{
    delete ui;
    delete m_propertyBrowser;
}

CPropertyBrowser* CMDVariablesBatchChangeDialog::getPropertyBrowse()
{
    return m_propertyBrowser;
}

//void CMDVariablesBatchChangeDialog::initial(IMDRTDBVariable* curVar)
//{
    /*
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
        ui->batchDeviceListCombox->setCurrentText(curVar->deviceName());*/
//}
void CMDVariablesBatchChangeDialog::on_okButton_clicked()
{
    QDialog::accept();
}

void CMDVariablesBatchChangeDialog::on_cancelButton_clicked()
{
    QDialog::reject();
}
