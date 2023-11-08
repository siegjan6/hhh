#include "AddDeviceDialog.h"
#include "ui_AddDeviceDialog.h"
#include <QMessageBox>
#include "MDMultiLanguage.h"
CAddDeviceDialog::CAddDeviceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddDeviceDialog)
{
    ui->setupUi(this);
    m_driverManager = NULL;
    setWindowTitle(tr("添加设备"));
    CMDMultiLanguage::instance()->translateWidget(this);
    QRegExp regExp("^[a-zA-Z]+[a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp,this));
}

CAddDeviceDialog::~CAddDeviceDialog()
{
    delete ui;
}
//!设置驱动管理器，同时添加驱动信息到驱动列表Combox
void CAddDeviceDialog::setDriverManager(IMDDriverManager *driverManager)
{
    m_driverManager = driverManager;
    if(m_driverManager!= NULL)
    {
        for(int i=0;i<m_driverManager->getDriverInformationCount();i++)
        {
            IMDDriverInformation *driverInformation = m_driverManager->getDriverInformationAt(i);
            if(m_driverManager!= NULL)
            {
                ui->driverBox->addItem(driverInformation->name(),QVariant((qulonglong)driverInformation));
            }
        }
    }

}
//!确认添加设备
void CAddDeviceDialog::on_okButton_clicked()
{
    QString name= ui->nameEdit->text();
    QString driverName= ui->driverBox->currentText();
    QVariant driver= ui->driverBox->currentData();
    QString description= ui->descriptionEdit->text();
    name = name.trimmed();
    if(name.isEmpty())
    {
       //****************************** Y2hw *****************************
       /*
       QMessageBox::warning(NULL, "Error", "设备名称不能为空！", QMessageBox::Ok);
       */
       QMessageBox box;
       box.setText(tr("设备名称不能为空！"));
       box.setWindowTitle(tr("Error"));
       box.setStandardButtons(QMessageBox::Ok);

       CMDMultiLanguage::instance()->translateWidget(&box);
       box.exec();
       //----------------------------------------------------------------
       return;
    }

    if(driverName.isNull()||driverName.isEmpty())
    {
       //****************************** Y2hw *****************************
       /*
       QMessageBox::warning(NULL, "Error", "请选择驱动！", QMessageBox::Ok);
       */
       QMessageBox box;
       box.setText(tr("请选择驱动！"));
       box.setWindowTitle(tr("Error"));
       box.setStandardButtons(QMessageBox::Ok);

       CMDMultiLanguage::instance()->translateWidget(&box);
       box.exec();
       //----------------------------------------------------------------
       return;
    }

    if(m_driverManager!= NULL)
    {
        QString error;
        IMDDriverInformation *driverInformation = (IMDDriverInformation *)driver.toULongLong();

        if(m_driverManager->addDevice(name,description,driverInformation,error))
        {
            this->setResult(QDialog::Accepted);
            this->accept();
        }
        else
        {
            QMessageBox::warning(NULL, "Error", error, QMessageBox::Ok);
        }
    }
}
//!取消添加设备
void CAddDeviceDialog::on_cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}
//!刷新驱动描述
void CAddDeviceDialog::on_driverBox_currentIndexChanged(int index)
{
    IMDDriverInformation * driverInformation = (IMDDriverInformation *)ui->driverBox->itemData(index).toLongLong();

    if(driverInformation!= NULL)
    {
        ui->driverNoteTextBrowser->setText(driverInformation->description());
    }
}
QString CAddDeviceDialog::getDeviceName()
{
    return ui->nameEdit->text();
}
