#include "MDDeviceParameterGetConfig.h"
#include "ui_MDDeviceParameterGetConfig.h"
#include "MDDevice.h"
#include "MDMultiLanguage.h"//多语言
CMDDeviceParameterGetConfig::CMDDeviceParameterGetConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDeviceParameterGetConfig)
{ 
    ui->setupUi(this);
    setWindowTitle(tr("设备配置"));
    m_pDevice = NULL;
    m_nCounter = 0;
    connect(ui->configButton, SIGNAL(clicked()), this, SLOT(configButton_clicked()));
    //****************************多语言*****************************//
    QStringList types;
    types << "QLineEdit" << "QComboBox";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
    //**************************************************************//
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(monitorGetConfigStatus()));
}

CMDDeviceParameterGetConfig::~CMDDeviceParameterGetConfig()
{
    if(m_pTimer!=NULL)
    {
        delete m_pTimer;
        m_pTimer = NULL;
    }
    delete ui;
}
void CMDDeviceParameterGetConfig::setDevicePoint(CMDDevice* pdev)
{
    m_pDevice = pdev;
}
void CMDDeviceParameterGetConfig::configButton_clicked()
{
    if(m_pDevice)
    {
        ui->statusEdit->setText("配置获取中.....................");
        m_pTimer->start(1000);


        QString serverIP = m_pDevice->tcpOrUdpIP_Pro();
        qint16 serverPort = (qint16)m_pDevice->tcpOrUdpRemotePort_Pro().toInt();

        QString serverSlaveIP=m_pDevice->slaveTcpOrUdpIP_Pro();
        qint16 serverSlavePort =(qint16)m_pDevice->slaveTcpOrUdpRemotePort_Pro().toInt();
        /*!
         * \brief:
            * 根据获取到的主站以及从站IP和端口号,
            * 获取配置信息.
         */
        m_pDevice->m_configFlag = false;
        m_pDevice->m_bFromSlave = false;
        m_pDevice->getTabVar(serverIP,serverPort,serverSlaveIP,serverSlavePort);
    }
}
void CMDDeviceParameterGetConfig::monitorGetConfigStatus()
{
    if(m_pDevice)
    {
        if(m_pDevice->m_configFlag == true)
        {
            QString serverIP = m_pDevice->tcpOrUdpIP_Pro();
            qint16 serverPort = (qint16)m_pDevice->tcpOrUdpRemotePort_Pro().toInt();

            QString serverSlaveIP=m_pDevice->slaveTcpOrUdpIP_Pro();
            qint16 serverSlavePort =(qint16)m_pDevice->slaveTcpOrUdpRemotePort_Pro().toInt();
            if(m_pDevice->m_bFromSlave == 0)
            {
                QString temp = "主站配置获取成功--IP:";
                temp = temp + serverIP;
                ui->statusEdit->setText(temp);
                m_nCounter = 0;
            }
            else
            {
                QString temp = "从站配置获取成功--IP:";
                temp = temp + serverSlaveIP;
                ui->statusEdit->setText(temp);
                m_nCounter = 0;
            }
            m_pTimer->stop();
        }
        else
        {
            m_nCounter++;
            if(m_pDevice->m_bFromSlave == 0)
            {
                ui->statusEdit->setText(QString("主站配置获取中.......%1").arg(m_nCounter));
            }
            else
            {
                ui->statusEdit->setText(QString("主站配置获取失败!从站配置获取中.......%1").arg(m_nCounter));
                if(m_nCounter>120)
                {
                    m_nCounter = 0;
                    ui->statusEdit->setText("从站配置获取失败，请确认通信及配置!");
                    m_pTimer->stop();
                }
            }
        }
    }
}
