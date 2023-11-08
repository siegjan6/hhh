#include "MDDataBlockDialog.h"
#include "ui_MDDataBlockDialog.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include <QMessageBox>

#include"MDMultiLanguage.h"//多语言
#include "PropertyBrowser.h"

#ifdef Q_OS_WIN
    //#pragma execution_character_set("utf-8")   //解决 VS编译器下中文乱码
#endif

CMDDataBlockDialog::CMDDataBlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDataBlockDialog)
{
    ui->setupUi(this);
    setFixedSize(487,379);
    setWindowTitle(tr("数据块参数"));
    m_dataBlock = NULL;
    QRegExp regExp1("^[a-zA-Z]+[a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp1,this));
    QRegExp regExp2("^\\d+$");
    //ui->endAddressEdit->setValidator(new QRegExpValidator(regExp2,this));
    //ui->startAddressEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->rateEdit->setValidator(new QRegExpValidator(regExp2,this));
//****************************多语言*****************************//
    QStringList types;
    types << "QLineEdit" << "QComboBox";
    CMDMultiLanguage::instance()->translateWidget(this, &types);
    //types << "QLineEdit" << "QComboBox" << "QLabel";
    //QStringList names;
    //names << "l1" << "l3";
    //CMDMultiLanguage::instance()->translateWidget(this, &types, &names);
    //label1.setText(CMDMultiLanguage::instance()->value(m_text)) ;
//**************************************************************//
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(okButton_clicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelButton_clicked()));
    connect(this,SIGNAL(refreshStartLableStatus(int)),this,SLOT(registerComBox_currentIndexChanged(int)));
}

CMDDataBlockDialog::~CMDDataBlockDialog()
{
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
        ui->rateEdit->setText(QString("%1").arg(m_dataBlock->pollRate()));

        CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
        if(device)
        {
             if(device->isRuntimeMode()||device->isRunning())
            {
                ui->okButton->setEnabled(false);
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
       //QMessageBox::warning(NULL, "Error", "数据块名称不能为空！", QMessageBox::Ok);
        QMessageBox box;
        box.setText(tr("数据块名称不能为空！"));
        box.setWindowTitle(tr("Error"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();

       return;
    }

    IDataBlock *dataBlockTemp = device->getDataBlock(name);
    if( dataBlockTemp!=NULL && m_dataBlock!=dataBlockTemp)
    {
       //QMessageBox::warning(NULL, "Error", "数据块名称重复！", QMessageBox::Ok);
        QMessageBox box;
        box.setText(tr("数据块名称重复！"));
        box.setWindowTitle(tr("Error"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
       return;
    }

    m_dataBlock->setVerifyFlag(FLAG_RIGHT);
    //m_dataBlock->setStrStartAddress(ui->startAddressEdit->text());
    //m_dataBlock->setStrEndAddress(ui->endAddressEdit->text());
    //m_dataBlock->setStrRegisterType(QString::number(ui->registerComboBox->currentIndex(),10));

    m_dataBlock->calculateDataBufferSize();

    m_dataBlock->setName(name);
    m_dataBlock->setDescription(ui->descriptionEdit->text());
    m_dataBlock->setEnabled(ui->enableCheckBox->isChecked());
    m_dataBlock->setPollRate(ui->rateEdit->text().toInt());

    m_dataBlock->device()->getPropertyBrowse()->updateProperty();

    this->setResult(QDialog::Accepted);
    this->accept();
}

void CMDDataBlockDialog::cancelButton_clicked()
{
    this->setResult(QDialog::Rejected);
    this->reject();
}
