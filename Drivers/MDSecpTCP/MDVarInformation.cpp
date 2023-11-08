#include "MDVarInformation.h"
#include "ui_MDVarInformation.h"
#include "MDVarInformation.h"
#include "MDDataBlock.h"
#include "MDDevice.h"
#include "MDMultiLanguage.h"//多语言

CMDVarInformation::CMDVarInformation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDVarInformation)
{
    ui->setupUi(this);
    setWindowTitle(tr("数据表变量"));
    m_dataBlock = NULL;
    QRegExp regExp2("^\\d+$");
    ui->dataTabNumberEdit->setValidator(new QRegExpValidator(regExp2,this));

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->verticalHeader()->setHidden(true);
    for(int i = 0;i<10;i++)
    {
         ui->tableWidget->setRowHeight(i,20);
    }
    ui->tableWidget->setColumnWidth(0,51);
    ui->tableWidget->setColumnWidth(1,200);
    ui->tableWidget->setColumnWidth(2,101);
    ui->tableWidget->setColumnWidth(3,101);

    QStringList tempIDTable;
    tempIDTable<<"变量ID"<<"变量名字"<<"变量类型"<<"变量长度";
    ui->tableWidget->setHorizontalHeaderLabels(tempIDTable);

    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(refreshDataBlock()));
    //****************************多语言*****************************//
    QStringList types;
    types << "QLineEdit" << "QComboBox";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
    //**************************************************************//
    connect(ui->dataTabNumberEdit, SIGNAL(editingFinished()), this, SLOT(dataTabNumberEdit_editingFinished()));
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButton_clicked()));
    connect(ui->refreshVarTabButton, SIGNAL(clicked()), this, SLOT(refreshVarTabButton_clicked()));
}

CMDVarInformation::~CMDVarInformation()
{
    if(NULL!=m_pTimer)
    {
        m_pTimer->deleteLater();
        m_pTimer = NULL;
    }
    delete ui;
}
void CMDVarInformation::setDataBlock(CMDDataBlock *dataBlock)
{
    m_dataBlock = dataBlock;
    if(m_dataBlock!=NULL)
    {
        ui->dataTabNumberEdit->setText(QString("%1").arg(m_dataBlock->dataTabNumber()));

        CMDDevice *device= (CMDDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
        qint16 currentDataTabNO = m_dataBlock->dataTabNumber();
        refreshVarTab(currentDataTabNO);

        if(device)
        {
             if(device->isRuntimeMode()||device->isRunning())
            {
                ui->okButton->setEnabled(false);
                ui->refreshVarTabButton->setEnabled(false);
            }
        }
    }
}

void CMDVarInformation::okButton_clicked()
{
    if(m_dataBlock== NULL)
    {
        this->setResult(QDialog::Rejected);
        this->reject ();
        return;
    }
    CMDDevice *device = (CMDDevice*)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device== NULL)
    {
        this->setResult(QDialog::Rejected);
        this->reject ();
        return;
    }
    m_dataBlock->setDataTabNumber(ui->dataTabNumberEdit->text().toInt());

    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDVarInformation::cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}

void CMDVarInformation::refreshDataBlock()
{
    CMDDevice *device = (CMDDevice*)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device->m_configFlag == true)
    {
        qint16 currentDataTabNO= (qint16)ui->dataTabNumberEdit->text().toInt();
        refreshVarTab(currentDataTabNO);
        m_pTimer->stop();
    }

}

void CMDVarInformation::dataTabNumberEdit_editingFinished()
{
    qint16 currentDataTabNO= (qint16)ui->dataTabNumberEdit->text().toInt();
    refreshVarTab(currentDataTabNO);
}
void CMDVarInformation::refreshVarTab(qint16 currentDataTableNO)
{
    CMDDevice *pDevice =  (CMDDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(pDevice)
    {
       if(pDevice->m_configFlag)
       {
            if(pDevice->dataTabNOAndPrope.contains(currentDataTableNO))
            {
                DataTabNoAndPrope::iterator currentDataTabPrope =pDevice->dataTabNOAndPrope.find(currentDataTableNO);
                IDAndPro currentIDAndPrope = currentDataTabPrope.value();
                ui->tableWidget->clearContents();
                int  tabSize = currentIDAndPrope.size();
                ui->tableWidget->setRowCount(tabSize);
                for(int i = 0;i<tabSize;i++)
                {
                     ui->tableWidget->setRowHeight(i,20);

                }
                int tempCounter = 0;
                for(IDAndPro::iterator refreshCounte = currentIDAndPrope.begin();refreshCounte!=currentIDAndPrope.end();++refreshCounte)
                {
                    MDVarPrope *pEveryIDPrope = refreshCounte.value();
                    ui->tableWidget->setItem(tempCounter,0,new QTableWidgetItem(QString("%1").arg(pEveryIDPrope->m_tagID)));
                    ui->tableWidget->setItem(tempCounter,1,new QTableWidgetItem(pEveryIDPrope->m_tagName));
                    QString strTempType = varTypeReflect(pEveryIDPrope->m_tagTyp);
                    ui->tableWidget->setItem(tempCounter,2,new QTableWidgetItem(strTempType));
                    ui->tableWidget->setItem(tempCounter,3,new QTableWidgetItem(QString("%1").arg(pEveryIDPrope->m_tagLength)));
                    tempCounter++;
                }
            }
            else
            {
                ui->tableWidget->clearContents();
                ui->tableWidget->setRowCount(10);
                for(int i = 0;i<10;i++)
                {
                     ui->tableWidget->setRowHeight(i,20);
                }
            }
       }
    }
}

void CMDVarInformation::refreshVarTabButton_clicked()
{
    if( NULL!= m_dataBlock)
    {
        CMDDevice *device = (CMDDevice*)m_dataBlock->parentItem(MDTreeItemType::Device);
        if(NULL!= device)
        {
            //device->m_configFlag = false;
            QStringList  paramList = device->commParameter().split(';');

            QString server = paramList.at(0);
            QString serverSlave = paramList.at(1);
            QStringList serverList = server.split(",");
            QStringList serverSlaveList = serverSlave.split(",");

            QString serverIP=serverList[1];
            qint16 serverPort =(qint16) serverList[2].toInt();

            QString serverSlaveIP=serverSlaveList[1];
            qint16 serverSlavePort =(qint16) serverSlaveList[2].toInt();
            device->m_configFlag = false;
            device->m_bFromSlave = false;
            m_pTimer->start(1000);
            device->getTabVar(serverIP,serverPort,serverSlaveIP,serverSlavePort);

        }
    }
}
QString CMDVarInformation::varTypeReflect(qint16 vartype)
{
    QString strTempType = "???";
    switch(vartype)
    {
    case 0:
        strTempType= "Undef";
        break;
    case 1:
        strTempType= "Bool";
        break;
    case 2:
        strTempType= "Byte";
        break;
    case 3:
        strTempType= "Word";
        break;
    case 4:
        strTempType= "Dword";
        break;
    case 5:
        strTempType= "Usint";
        break;
    case 6:
        strTempType= "Uint";
        break;
    case 7:
        strTempType= "Udint";
        break;
    case 8:
        strTempType= "Sint";
        break;
    case 9:
        strTempType= "Int";
        break;
    case 10:
        strTempType= "Dint";
        break;
    case 11:
        strTempType= "Real";
        break;
    case 20:
        strTempType= "String";
        break;
    case 24:
        strTempType= "Bit";
        break;
    default:
        break;
    }
    return strTempType;
}

