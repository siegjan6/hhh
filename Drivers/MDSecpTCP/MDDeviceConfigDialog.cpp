#include "MDDeviceConfigDialog.h"
#include "ui_MDDeviceConfigDialog.h"
#include <QMessageBox>

#include"MDMultiLanguage.h"//多语言
#include "PropertyBrowser.h"
CMDDeviceConfigDialog::CMDDeviceConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDeviceConfigDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("设备参数"));

    m_nCounter = 0;
    m_uiStatuFlag = 0;

    QRegExp regExp1("^[A-Za-z][A-Za-z0-9]+$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp1,this));
    QRegExp regExp2("\\d+");
    ui->serverPortEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->retriesEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->replyTimeoutEdit->setValidator(new QRegExpValidator(regExp2,this));

    QRegExp regExp3("^([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    ui->serverIPEdit->setValidator(new QRegExpValidator(regExp3,this));

    connect(ui->communicationTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCommunicationTypeBoxCurrentIndexChanged(int)));

    QStringList items;
    items.append("");
    for(int i=1;i<256;i++)
    {
        items.append(QString("COM%1").arg(i));
    }
    ui->portComboBox->addItems(items);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(monitorGetConfigStatus()));
    //****************************多语言*****************************//
        QStringList types;
        types << "QLineEdit" << "QComboBox";
        CMDMultiLanguage::instance()->translateWidget(this, &types);
    //**************************************************************//
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));
    connect(ui->configButton, SIGNAL(clicked()), this, SLOT(configButton_clicked()));

}

CMDDeviceConfigDialog::~CMDDeviceConfigDialog()
{
    if(m_pTimer!=NULL)
    {
        delete m_pTimer;
        m_pTimer = NULL;
    }
    delete ui;
}
void CMDDeviceConfigDialog::setDevice(CMDDevice *device)
{
    m_device= device;
    if(m_device!=NULL)
    {
        if(m_device->commType()== CommunicationType::Serial)
        {
            QStringList  paramList = m_device->commParameter().split(',');
            ui->communicationTypeBox->setCurrentIndex(0);
            ui->stackedWidget->setCurrentIndex(0);
            if(paramList.size()>4)
            {
                 ui->portComboBox->setCurrentText(paramList.at(0).toUpper());
                 ui->baudRateComboBox->setCurrentText(paramList.at(1));
                 ui->dataBitsComboBox->setCurrentText(paramList.at(2));
                 ui->stopBitsComboBox->setCurrentIndex(paramList.at(3).toInt());
                 ui->parityComboBox->setCurrentIndex(paramList.at(4).toUInt());
            }
        }
        else if(m_device->commType()== CommunicationType::TCP)
        {
            QStringList  paramList = m_device->commParameter().split(';');

            ui->communicationTypeBox->setCurrentIndex(1);
            ui->stackedWidget->setCurrentIndex(1);
            if(paramList.count()>=2)
            {
                QString server = paramList.at(0);
                QString serverSlave = paramList.at(1);
                QStringList serverList = server.split(",");
                QStringList serverSlaveList = serverSlave.split(",");
                if(serverList.count()>=2&&serverSlaveList.count()>=2)
                {
                    ui->serverIPEdit->setText(serverList.at(1));
                    ui->serverPortEdit->setText(serverList.at(2));
                    ui->serverSlaveIPEdit->setText(serverSlaveList.at(1));
                    ui->serverSlavePortEdit->setText(serverSlaveList.at(2));
                }
            }
        }
        else if(m_device->commType()== CommunicationType::UDP)
        {
            QStringList  paramList = m_device->commParameter().split(';');

            ui->communicationTypeBox->setCurrentIndex(2);
            ui->stackedWidget->setCurrentIndex(1);
            if(paramList.count()>=2)
            {
                QString server = paramList.at(0);
                QString serverSlave = paramList.at(1);
                QStringList serverList = server.split(",");
                QStringList serverSlaveList = serverSlave.split(",");
                if(serverList.count()>=2&&serverSlaveList.count()>=2)
                {
                    ui->serverIPEdit->setText(serverList.at(1));
                    ui->serverPortEdit->setText(serverList.at(2));
                    ui->serverSlaveIPEdit->setText(serverSlaveList.at(1));
                    ui->serverSlavePortEdit->setText(serverSlaveList.at(2));
                }
            }
        }
        else if(m_device->commType()== CommunicationType::USB)
        {
            ui->communicationTypeBox->setCurrentIndex(3);
            ui->stackedWidget->setCurrentIndex(2);
            ui->usbCommParameterEdit->setText(m_device->commParameter());
        }
        else if(m_device->commType()== CommunicationType::Board)
        {
            ui->communicationTypeBox->setCurrentIndex(4);
            ui->stackedWidget->setCurrentIndex(3);
            ui->boardCommParameterEdit->setText(m_device->commParameter());
        }
        else
        {
            ui->communicationTypeBox->setCurrentIndex(5);
            ui->stackedWidget->setCurrentIndex(4);
            ui->otherCommParameterEdit->setText(m_device->commParameter());
        }

        ui->nameEdit->setText( m_device->name());
        ui->enableCheckBox->setChecked(m_device->isEnabled());
        ui->descriptionEdit->setText( m_device->description());

        ui->replyTimeoutEdit->setText(QString("%1").arg(m_device->replyTimeout()));
        ui->retriesEdit->setText(QString("%1").arg(m_device->retries()));

        if(m_device->isRuntimeMode()||m_device->isRunning())
        {
            ui->okButton->setEnabled(false);
            ui->configButton->setEnabled(false);
        }
    }
}
void CMDDeviceConfigDialog::onCommunicationTypeBoxCurrentIndexChanged(int index)
{
    if(index == 1||index == 2)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else
    {
        if(index>=3)
        {
            ui->stackedWidget->setCurrentIndex(index-1);
        }
        else
        {
            ui->stackedWidget->setCurrentIndex(index);
        }
    }
}

void CMDDeviceConfigDialog::okButton_clicked()
{
    if(m_device!=NULL)
    {

        IMDDriverManager *driverManager = (IMDDriverManager*)m_device->parentItem(MDTreeItemType::DriverManager);
        if(driverManager== NULL)
        {
            this->setResult(QDialog::Rejected);
            this->reject ();
            return;
        }
        QString name= ui->nameEdit->text();
        name.trimmed();
        if(name.isEmpty())
        {

           //*****************************PKX20160216****************************//
          //QMessageBox::warning(NULL, "Error", "设备名不能为空！", QMessageBox::Ok);
           QMessageBox box;
           box.setText(tr("设备名不能为空！"));
           box.setWindowTitle(tr("Error"));
           box.setStandardButtons(QMessageBox::Ok);

           CMDMultiLanguage::instance()->translateWidget(&box);

           box.exec();
           //********************************************************************//
           return;
        }

        IDevice *deviceTemp = driverManager->getDevice(name);
        if( deviceTemp!=NULL && m_device!=deviceTemp)
        {

           //*****************************PKX20160216****************************//
          //QMessageBox::warning(NULL, "Error", "设备名重复！", QMessageBox::Ok);
           QMessageBox box;
           box.setText(tr("设备名重复！"));
           box.setWindowTitle(tr("Error"));
           box.setStandardButtons(QMessageBox::Ok);

           CMDMultiLanguage::instance()->translateWidget(&box);

           box.exec();
           //********************************************************************//
           return;
        }

        if(ui->communicationTypeBox->currentIndex()==0)
        {
            if(m_device->commType()!=(CommunicationType)0)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)0);
            }
            m_device->setCommType(CommunicationType::Serial);
            QString  param = QString("%1,%2,%3,%4,%5)").arg(ui->portComboBox->currentText())
                                                        .arg(ui->baudRateComboBox->currentText())
                                                        .arg(ui->dataBitsComboBox->currentText())
                                                        .arg(ui->stopBitsComboBox->currentIndex())
                                                        .arg(ui->parityComboBox->currentIndex());
            m_device->setCommParameter(param);

        }
        else if(ui->communicationTypeBox->currentIndex()== 1)
        {
            if(m_device->commType()!=(CommunicationType)1)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)1);
            }
            m_device->setCommType(CommunicationType::TCP);
            QString commModel = "Client";
            QString  param = QString("%1,%2,%3,,;%1,%4,%5,,").arg(commModel)
                                                        .arg(ui->serverIPEdit->text())
                                                        .arg(ui->serverPortEdit->text())
                                                        .arg(ui->serverSlaveIPEdit->text())
                                                        .arg(ui->serverSlavePortEdit->text());
            m_device->setCommParameter(param);
        }
        else if(ui->communicationTypeBox->currentIndex()== 2)
        {
            if(m_device->commType()!=(CommunicationType)2)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)2);
            }
            m_device->setCommType(CommunicationType::UDP);
            QString commModel = "Client";
            QString  param = QString("%1,%2,%3,,;%1,%4,%5,,").arg(commModel)
                                                        .arg(ui->serverIPEdit->text())
                                                        .arg(ui->serverPortEdit->text())
                                                        .arg(ui->serverSlaveIPEdit->text())
                                                        .arg(ui->serverSlavePortEdit->text());
            m_device->setCommParameter(param);
        }
        else if(ui->communicationTypeBox->currentIndex()== 3)
        {
            if(m_device->commType()!=(CommunicationType)3)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)3);
            }
            m_device->setCommType(CommunicationType::USB);
            m_device->setCommParameter(ui->usbCommParameterEdit->text());
        }
        else if(ui->communicationTypeBox->currentIndex()== 4)
        {
            if(m_device->commType()!=(CommunicationType)4)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)4);
            }
            m_device->setCommType(CommunicationType::Board);
            m_device->setCommParameter(ui->boardCommParameterEdit->text());
        }
        else
        {
            if(m_device->commType()!=(CommunicationType)5)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)5);
            }
            m_device->setCommType(CommunicationType::Other);
            m_device->setCommParameter(ui->otherCommParameterEdit->text());
        }

        m_device->setName(ui->nameEdit->text());
        m_device->setDescription(ui->descriptionEdit->text());
        m_device->setEnabled(ui->enableCheckBox->isChecked());

        m_device->setReplyTimeout_Pro(ui->replyTimeoutEdit->text());
        m_device->setRetries_Pro(ui->retriesEdit->text());
        //!更新属性框

        m_device->getPropertyBrowse()->updateProperty();
    }

    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDDeviceConfigDialog::cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}
void CMDDeviceConfigDialog::monitorGetConfigStatus()
{
    if(m_device->m_configFlag == true)
    {
        QString serverIP = ui->serverIPEdit->text();
        qint16 serverPort = (qint16)ui->serverPortEdit->text().toInt();

        QString serverSlaveIP=ui->serverSlaveIPEdit->text();
        qint16 serverSlavePort =(qint16) ui->serverSlavePortEdit->text().toInt();
        if(m_device->m_bFromSlave == 0)
        {
            QString temp = "主站配置获取成功--IP:";
            temp = temp + serverIP;
            ui->statuEdit->setText(temp);
            m_nCounter = 0;
        }
        else
        {
            QString temp = "从站配置获取成功--IP:";
            temp = temp + serverSlaveIP;
            ui->statuEdit->setText(temp);
            m_nCounter = 0;
        }
        m_pTimer->stop();
    }
    else
    {
        m_nCounter++;
        if(m_device->m_bFromSlave == 0)
        {
            ui->statuEdit->setText(QString("主站配置获取中.......%1").arg(m_nCounter));
        }
        else
        {
            ui->statuEdit->setText(QString("主站配置获取失败!从站配置获取中.......%1").arg(m_nCounter));
            if(m_nCounter>120)
            {
                m_nCounter = 0;
                ui->statuEdit->setText("从站配置获取失败，请确认通信及配置!");
                m_pTimer->stop();
            }
        }

    }
}

void CMDDeviceConfigDialog::configButton_clicked()
{

    ui->statuEdit->setText("配置获取中.....................");
    m_pTimer->start(1000);


    QString serverIP = ui->serverIPEdit->text();
    qint16 serverPort = (qint16)ui->serverPortEdit->text().toInt();

    QString serverSlaveIP=ui->serverSlaveIPEdit->text();
    qint16 serverSlavePort =(qint16) ui->serverSlavePortEdit->text().toInt();
    /*!
     * \brief:
        * 根据获取到的主站以及从站IP和端口号,
        * 获取配置信息.
     */
    m_device->m_configFlag = false;
    m_device->m_bFromSlave = false;
    m_device->getTabVar(serverIP,serverPort,serverSlaveIP,serverSlavePort);


}
