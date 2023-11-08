#include "MDDataBlockDialog.h"
#include "ui_MDDataBlockDialog.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include <QMessageBox>

#include "MDMultiLanguage.h"//多语言
#include "PropertyBrowser.h"
CMDDataBlockDialog::CMDDataBlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDataBlockDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("数据块参数"));
    m_dataBlock = NULL;
    QRegExp regExp1("^[A-Za-z][A-Za-z0-9]+$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp1,this));
    QRegExp regExp2("^\\d+$");
    //ui->tabNumberEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->dataTabNumberEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->rateEdit->setValidator(new QRegExpValidator(regExp2,this));

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
    //ui->tableWidget->setItem(0,1,new QTableWidgetItem("s"));
    //ui->tableWidget->clearContents();
    //connect(this,SIGNAL(refresh()),this,SLOT(refreshDataBlock()));
    pTimer = new QTimer(this);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(refreshDataBlock()));
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

CMDDataBlockDialog::~CMDDataBlockDialog()
{
    if(NULL!=pTimer)
    {
        pTimer->deleteLater();
        pTimer = NULL;
    }
    delete ui;
}
void CMDDataBlockDialog::setDataBlock(CMDDataBlock *dataBlock)
{
    m_dataBlock = dataBlock;
    if(m_dataBlock!=NULL)
    {
        ui->nameEdit->setText(m_dataBlock->name());
        ui->descriptionEdit->setText(m_dataBlock->description());
        ui->enableCheckBox->setChecked(m_dataBlock->isEnabled());
        //ui->tabNumberEdit->setText(QString("%1").arg(m_dataBlock->tabNumber()));
        ui->dataTabNumberEdit->setText(QString("%1").arg(m_dataBlock->dataTabNumber()));

        ui->rateEdit->setText(QString("%1").arg(m_dataBlock->pollRate()));
        CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
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

void CMDDataBlockDialog::okButton_clicked()
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
    QString name= ui->nameEdit->text();
    name.trimmed();
    if(name.isEmpty())
    {

       //*****************************PKX20160216****************************//
      //QMessageBox::warning(NULL, "Error", "数据块名称不能为空！", QMessageBox::Ok);
       QMessageBox box;
       box.setText(tr("数据块名称不能为空！"));
       box.setWindowTitle(tr("Error"));
       box.setStandardButtons(QMessageBox::Ok);

       CMDMultiLanguage::instance()->translateWidget(&box);

       box.exec();
       //********************************************************************//
       return;
    }

    IDataBlock *dataBlockTemp = device->getDataBlock(name);
    if( dataBlockTemp!=NULL && m_dataBlock!=dataBlockTemp)
    {

       //*****************************PKX20160216****************************//
      //QMessageBox::warning(NULL, "Error", "数据块名称重复！", QMessageBox::Ok);
       QMessageBox box;
       box.setText(tr("数据块名称重复！"));
       box.setWindowTitle(tr("Error"));
       box.setStandardButtons(QMessageBox::Ok);

       CMDMultiLanguage::instance()->translateWidget(&box);

       box.exec();
       //********************************************************************//
       return;
    }

    m_dataBlock->setName(name);
    m_dataBlock->setDescription(ui->descriptionEdit->text());
    m_dataBlock->setEnabled(ui->enableCheckBox->isChecked());
    m_dataBlock->setDataTabNumber(ui->dataTabNumberEdit->text().toInt());
    m_dataBlock->setPollRate(ui->rateEdit->text().toInt());

    m_dataBlock->device()->getPropertyBrowse()->updateProperty();

    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDDataBlockDialog::cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject ();
}

void CMDDataBlockDialog::refreshDataBlock()
{
    CMDDevice *device = (CMDDevice*)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device->m_configFlag == true)
    {
        qint16 currentDataTabNO= (qint16)ui->dataTabNumberEdit->text().toInt();
        refreshVarTab(currentDataTabNO);
        pTimer->stop();
    }

}

void CMDDataBlockDialog::dataTabNumberEdit_editingFinished()
{
    qint16 currentDataTabNO= (qint16)ui->dataTabNumberEdit->text().toInt();
    refreshVarTab(currentDataTabNO);
}
void CMDDataBlockDialog::refreshVarTab(qint16 currentDataTableNO)
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

void CMDDataBlockDialog::refreshVarTabButton_clicked()
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
            pTimer->start(1000);
            device->getTabVar(serverIP,serverPort,serverSlaveIP,serverSlavePort);

        }
    }
}
QString CMDDataBlockDialog::varTypeReflect(qint16 vartype)
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

