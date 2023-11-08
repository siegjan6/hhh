#include "MDDeviceConfigDialog.h"
#include "ui_MDDeviceConfigDialog.h"
#include <QMessageBox>

#include "MDMultiLanguage.h"//多语言
#include "PropertyBrowser.h"
CMDDeviceConfigDialog::CMDDeviceConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDeviceConfigDialog)
{
    ui->setupUi(this);
    setFixedSize(466,476);
    setWindowTitle(tr("设备参数"));
    QRegExp regExp1("^[a-zA-Z]+[a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp1,this));
    QRegExp regExp2("\\d+");
    ui->serverPortEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->retriesEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->replyTimeoutEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->stationEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->localPortEdit->setValidator(new QRegExpValidator(regExp2,this));
    QRegExp Reg_Device_serverIP("^([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])\\.([1-9]?\\d|1\\d{2}|2[0-4]\\d|25[0-5])$");
    QValidator *Validator_Device_serverIP = new QRegExpValidator(Reg_Device_serverIP,this);
    ui->serverIPEdit->setValidator(Validator_Device_serverIP);
    QRegExp regExp3("^[0-9a-fA-F]$");
    ui->overTimerEdit->setValidator(new QRegExpValidator(regExp3,this));



    //******************************PKX20151109******************************//
#ifdef Q_OS_LINUX
   ui->portComboBox->addItem( "/dev/ttyUSB0");
   ui->portComboBox->addItem( "/dev/ttyUSB1");
   ui->portComboBox->addItem( "/dev/ttyUSB2");
   ui->portComboBox->addItem( "/dev/ttyUSB3");
   ui->portComboBox->addItem( "/dev/ttyUSB4");
   ui->portComboBox->addItem( "/dev/ttyUSB5");
   ui->portComboBox->addItem( "/dev/ttyS0");
   ui->portComboBox->addItem( "/dev/ttyS1");
   ui->portComboBox->addItem( "/dev/ttyS2");
   ui->portComboBox->addItem( "/dev/ttyS3");
   ui->portComboBox->addItem( "/dev/ttyS4");
   ui->portComboBox->addItem( "/dev/ttyS5");
#elif defined (Q_OS_WIN)
   ui->portComboBox->addItem("COM1");
   ui->portComboBox->addItem("COM2");
   ui->portComboBox->addItem("COM3");
   ui->portComboBox->addItem("COM4");
   ui->portComboBox->addItem("COM5");
   ui->portComboBox->addItem("COM6");
   ui->portComboBox->addItem("COM7");
   ui->portComboBox->addItem("COM8");
   ui->portComboBox->addItem("COM9");
   ui->portComboBox->addItem("COM10");

#endif
   //****************************************************************************//
   //****************************多语言*****************************//
   QStringList types;
   types << "QLineEdit" << "QComboBox";
   CMDMultiLanguage::instance()->translateWidget(this, &types);
   //**************************************************************//
   connect(ui->communicationTypeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(communicationTypeBox_currentIndexChanged(int)));
   connect(ui->okButton,SIGNAL(clicked()),this,SLOT(okButton_clicked()));
   connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelButton_clicked()));
}

CMDDeviceConfigDialog::~CMDDeviceConfigDialog()
{
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
                 ui->portComboBox->setCurrentText(paramList.at(0));//pkx2016119paramList.at(0).toUpper()
                 ui->baudRateComboBox->setCurrentText(paramList.at(1));
                 ui->dataBitsComboBox->setCurrentText(paramList.at(2));
                 ui->stopBitsComboBox->setCurrentIndex(paramList.at(3).toInt());
                 ui->parityComboBox->setCurrentIndex(paramList.at(4).toUInt());
            }
        }
        else if(m_device->commType()== CommunicationType::TCP)
        {
            QStringList  paramList = m_device->commParameter().split(',');
            ui->communicationTypeBox->setCurrentIndex(1);
            ui->stackedWidget->setCurrentIndex(1);
            /*if(m_device->commType()== CommunicationType::TCP)
            {
                ui->tcpRadioButton->setChecked(true);
                ui->udpRadioButton->setChecked(false);
            }
            else
            {
                ui->tcpRadioButton->setChecked(false);
                ui->udpRadioButton->setChecked(true);
            }
            if(paramList.size()>4)
            {
                if(paramList.at(0).toUpper() == "SERVER")
                {
                    ui->servertRadioButton->setChecked(true);
                    ui->clientRadioButton->setChecked(false);
                }
                else
                {
                    ui->servertRadioButton->setChecked(false);
                    ui->clientRadioButton->setChecked(true);
                }*/
                 ui->serverIPEdit->setText(paramList.at(1));
                 ui->serverPortEdit->setText(paramList.at(2));
                 ui->localPortEdit->setText(paramList.at(4));
                 if(paramList.at(0).toUpper() == "SERVER")
                 {
                     ui->serverRadioButton->setChecked(true);
                     ui->clientRadioButton->setChecked(false);
                 }
                 else
                 {
                     ui->serverRadioButton->setChecked(false);
                     ui->clientRadioButton->setChecked(true);
                 }
        }
        else if(m_device->commType()== CommunicationType::UDP)
        {
            QStringList  paramList = m_device->commParameter().split(',');
            ui->communicationTypeBox->setCurrentIndex(2);
            ui->stackedWidget->setCurrentIndex(1);
            ui->serverIPEdit->setText(paramList.at(1));
            ui->serverPortEdit->setText(paramList.at(2));
            ui->localPortEdit->setText(paramList.at(4));
            if(paramList.at(0).toUpper() == "SERVER")
            {
                ui->serverRadioButton->setChecked(true);
                ui->clientRadioButton->setChecked(false);
            }
            else
            {
                ui->serverRadioButton->setChecked(false);
                ui->clientRadioButton->setChecked(true);
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
        ui->stationEdit->setText(QString("%1").arg(m_device->station()));
        ui->replyTimeoutEdit->setText(QString("%1").arg(m_device->replyTimeout()));
        ui->retriesEdit->setText(QString("%1").arg(m_device->retries()));
        ui->sumCheckBox->setChecked(m_device->checkSum());
        QString strComOverTimer = QString::number(m_device->comOverTimer(),16).toUpper();
        ui->overTimerEdit->setText(strComOverTimer);
        ui->protocalTypeComboBox->setCurrentIndex(m_device->protocalFormat());
        if(m_device->isRuntimeMode()||m_device->isRunning())
        {
            ui->okButton->setEnabled(false);
        }

    }
}
void CMDDeviceConfigDialog::communicationTypeBox_currentIndexChanged(int index)
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
            //****************************PKX20160215***************************//
           //QMessageBox::warning(NULL, "Error", "设备名不能为空！", QMessageBox::Ok);
            QMessageBox box;
            box.setText(tr("设备名不能为空！"));
            box.setWindowTitle(tr("Error"));
            box.setStandardButtons(QMessageBox::Ok);

            CMDMultiLanguage::instance()->translateWidget(&box);

            box.exec();
           //******************************************************************//

           return;
        }

        IDevice *deviceTemp = driverManager->getDevice(name);
        if( deviceTemp!=NULL && m_device!=deviceTemp)
        {
            //****************************PKX20160215***************************//
           //QMessageBox::warning(NULL, "Error", "设备名重复！", QMessageBox::Ok);
            QMessageBox box;
            box.setText(tr("设备名重复！"));
            box.setWindowTitle(tr("Error"));
            box.setStandardButtons(QMessageBox::Ok);

            CMDMultiLanguage::instance()->translateWidget(&box);

            box.exec();
           //******************************************************************//

           return;
        }

        if(ui->communicationTypeBox->currentIndex()==0)
        {
            if(m_device->commType()!=(CommunicationType)0)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)0);
            }
            m_device->setCommType(CommunicationType::Serial);
            QString  param = QString("%1,%2,%3,%4,%5").arg(ui->portComboBox->currentText())
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
            QString commModel = "";
            if(ui->clientRadioButton->isChecked())
            {
                commModel = "Client";
            }
            else
            {
                commModel = "Server";
            }
            QString  param = QString("%1,%2,%3,,%4").arg(commModel)
                                                        .arg(ui->serverIPEdit->text())
                                                        .arg(ui->serverPortEdit->text())
                                                        .arg(ui->localPortEdit->text());
            m_device->setCommParameter(param);
        }
        else if(ui->communicationTypeBox->currentIndex()== 2)
        {
            if(m_device->commType()!=(CommunicationType)2)
            {
                m_device->hideAllAndShowOne_Pro((CommunicationType)2);
            }
            m_device->setCommType(CommunicationType::UDP);
            QString commModel = "";
            if(ui->clientRadioButton->isChecked())
            {
                commModel = "Client";
            }
            else
            {
                commModel = "Server";
            }
            QString  param = QString("%1,%2,%3,,%4").arg(commModel)
                                                        .arg(ui->serverIPEdit->text())
                                                        .arg(ui->serverPortEdit->text())
                                                        .arg(ui->localPortEdit->text());
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
        m_device->setStation(ui->stationEdit->text().toInt());
        m_device->setReplyTimeout_Pro(ui->replyTimeoutEdit->text());
        m_device->setRetries_Pro(ui->retriesEdit->text());
        m_device->setCheckSum(ui->sumCheckBox->isChecked());
        bool ok = 0;
        m_device->setComOverTimer(ui->overTimerEdit->text().toUpper().toInt(&ok,16));
        m_device->setprotocalFormat(ui->protocalTypeComboBox->currentIndex());

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
