#include "MDDataBlockDialog.h"
#include "ui_MDDataBlockDialog.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include <QMessageBox>
#include "MDProtocolCommand.h"

#include "MDMultiLanguage.h"//多语言
#include "PropertyBrowser.h"
CMDDataBlockDialog::CMDDataBlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDataBlockDialog)
{
    ui->setupUi(this);
    setFixedSize(447,350);
    setWindowTitle(tr("数据块参数"));
    m_dataBlock = NULL;
    m_startAddress = "0";
    m_endAddress = "0";
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
    //**************************************************************//
    connect(ui->startAddressEdit,SIGNAL(editingFinished()),this,SLOT(startAddressEdit_editingFinished()));
    connect(ui->endAddressEdit,SIGNAL(editingFinished()),this,SLOT(endAddressEdit_editingFinished()));
    connect(ui->okButton,SIGNAL(clicked()),this,SLOT(okButton_clicked()));
    connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelButton_clicked()));
    connect(ui->registerComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(registerComBox_currentIndexChanged(int )));
    connect(this,SIGNAL(refreshStartLableStatus(int)),this,SLOT(registerComBox_currentIndexChanged(int )));
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
        ui->startAddressEdit->setText(m_dataBlock->strStartAddress());
        m_startAddress = m_dataBlock->strStartAddress();
        ui->endAddressEdit->setText(m_dataBlock->strEndAddress());
        m_endAddress = m_dataBlock->strEndAddress();
        ui->registerComboBox->setCurrentIndex(m_dataBlock->registerType());
        ui->rateEdit->setText(QString("%1").arg(m_dataBlock->pollRate()));
        if(m_dataBlock->verifyFlag() == FLAG_REGISTER_ERROR)
        {
            ui->registerComboBox->setCurrentIndex(-1);
            ui->startAddressEdit->setEnabled(false);
            ui->endAddressEdit->setEnabled(false);
        }
        else
        {
            ui->registerComboBox->setCurrentIndex(m_dataBlock->registerType());
            emit refreshStartLableStatus(m_dataBlock->registerType());
        }

    }
    CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device)
    {
         if(device->isRuntimeMode()||device->isRunning())
        {
            ui->okButton->setEnabled(false);
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
        //****************************PKX20160215***************************//

        promptMessageBox(tr("错误:"),tr("数据块名称不能为空！"));
       //******************************************************************//

       return;
    }

    IDataBlock *dataBlockTemp = device->getDataBlock(name);
    if( dataBlockTemp!=NULL && m_dataBlock!=dataBlockTemp)
    {
        //****************************PKX20160215***************************//

        promptMessageBox(tr("错误:"),tr("数据块名称重复！"));
       //******************************************************************//

       return;
    }
    qint16 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = m_dataBlock->verifyRegisterFormat(QString::number(ui->registerComboBox->currentIndex(),10),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
    {
        QMessageBox box;
        box.setText(tr("寄存器不能为空!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
       return;
    }
    qint16 nTempStartAddress = 0;
    int tempStartAddressFlag = m_dataBlock->verifyAddressFormat(ui->startAddressEdit->text(),nTempStartAddress,nTempAddressFormat);
    if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
    {
        QMessageBox box;
        box.setText(tr("开始地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        return;
    }

    qint16 nTempEndAddress = 0;
    int tempEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->endAddressEdit->text(),nTempEndAddress,nTempAddressFormat);
    if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
    {
        QMessageBox box;
        box.setText(tr("结束地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        return;
    }
    int tempLengthFlag = m_dataBlock->verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
    if(tempLengthFlag == LENGTH_NEGATIVENUMBER_ERROR||tempLengthFlag == LENGTH_OVERRUN_ERROR)
    {
        QMessageBox box;
        box.setText(tr("长度错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        return;
    }
    m_dataBlock->setVerifyFlag(FLAG_RIGHT);
    m_dataBlock->setStrStartAddress(ui->startAddressEdit->text());
    m_dataBlock->setStrEndAddress(ui->endAddressEdit->text());
    m_dataBlock->setStrRegisterType(QString::number(ui->registerComboBox->currentIndex(),10));
    if(nTempRegister == ID_MEM_X||
            nTempRegister == ID_MEM_Y)
    {
        m_dataBlock->setStartAddress(QString::number(nTempStartAddress,8).toInt());
        m_dataBlock->setEndAddress(QString::number(nTempEndAddress,8).toInt());
    }
    else
    {
        m_dataBlock->setStartAddress(nTempStartAddress);
        m_dataBlock->setEndAddress(nTempEndAddress);
    }
    m_dataBlock->setRegisterType(nTempRegister);

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
    this->reject ();
}

void CMDDataBlockDialog::startAddressEdit_editingFinished()
{
    CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device)
    {
         if(device->isRuntimeMode()||device->isRunning())
        {
            return;
        }
    }
    if(ui->startAddressEdit->text() == m_startAddress)
    {
        return;
    }

    qint16 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = m_dataBlock->verifyRegisterFormat(QString::number(ui->registerComboBox->currentIndex(),10),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
    {
        QMessageBox box;
        box.setText(tr("寄存器不能为空!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        ui->endAddressEdit->setText(m_endAddress);
        ui->startAddressEdit->setText(m_startAddress);
       return;
    }

    qint16 nTempStartAddress = 0;
    int tempStartAddressFlag = m_dataBlock->verifyAddressFormat(ui->startAddressEdit->text(),nTempStartAddress,nTempAddressFormat);


    qint16 nTempEndAddress = 0;
    int tempEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->endAddressEdit->text(),nTempEndAddress,nTempAddressFormat);
    if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR&&tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
    {
        ui->startAddressEdit->setText(m_startAddress);
        ui->endAddressEdit->setText(m_endAddress);
        QMessageBox box;
        box.setText(tr("开始结束地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        emit refreshStartLableStatus(nTempRegister);
        return;
    }
    else if(tempEndAddressFlag == ADDRESS_FORMAT_RIGHT&&tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
    {
        ui->startAddressEdit->setText(m_startAddress);
        QMessageBox box;
        box.setText(tr("开始地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        emit refreshStartLableStatus(nTempRegister);
        return;
    }
    else if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR&&tempStartAddressFlag == ADDRESS_FORMAT_RIGHT)
    {
        m_startAddress = ui->startAddressEdit->text();
        ui->endAddressEdit->setText(m_endAddress);
        QMessageBox box;
        box.setText(tr("结束地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        emit refreshStartLableStatus(nTempRegister);
        return;
    }

    m_startAddress = ui->startAddressEdit->text();

    int tempLengthFlag = m_dataBlock->verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
    if(tempLengthFlag == LENGTH_NEGATIVENUMBER_ERROR)
    {
        ui->endAddressEdit->setText(ui->startAddressEdit->text());
        m_endAddress = ui->startAddressEdit->text();
    }
    else if(tempLengthFlag == LENGTH_OVERRUN_ERROR)
    {

        QMessageBox box;
        QString strMaxLength = CMDMultiLanguage::instance()->value(tr("最大长度%1，将自动设置成最大长度！")).arg(nTempLength);
        box.setText(strMaxLength);
        box.setWindowTitle(tr("提示："));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        qint16 tempEndAddress = nTempStartAddress + nTempLength-1;
        QString str_tempEndAddress = QString::number(tempEndAddress,nTempAddressFormat);
        ui->endAddressEdit->setText(str_tempEndAddress);
        m_endAddress = str_tempEndAddress;
    }
    emit refreshStartLableStatus(nTempRegister);
}

void CMDDataBlockDialog::endAddressEdit_editingFinished()
{
    CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device)
    {
         if(device->isRuntimeMode()||device->isRunning())
        {
            return;
        }
    }
    if(ui->endAddressEdit->text() == m_endAddress)
    {
        return;
    }

    qint16 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = m_dataBlock->verifyRegisterFormat(QString::number(ui->registerComboBox->currentIndex(),10),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
    {
        QMessageBox box;
        box.setText(tr("寄存器不能为空!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        ui->endAddressEdit->setText(m_endAddress);
        ui->startAddressEdit->setText(m_startAddress);
       return;
    }

    qint16 nTempStartAddress = 0;
    int tempStartAddressFlag = m_dataBlock->verifyAddressFormat(ui->startAddressEdit->text(),nTempStartAddress,nTempAddressFormat);


    qint16 nTempEndAddress = 0;
    int tempEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->endAddressEdit->text(),nTempEndAddress,nTempAddressFormat);
    if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR&&tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
    {
        ui->startAddressEdit->setText(m_startAddress);
        ui->endAddressEdit->setText(m_endAddress);
        QMessageBox box;
        box.setText(tr("开始结束地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        emit refreshStartLableStatus(nTempRegister);
        return;
    }
    else if(tempEndAddressFlag == ADDRESS_FORMAT_RIGHT&&tempStartAddressFlag == ADDRESS_FORMAT_ERROR)
    {
        ui->startAddressEdit->setText(m_startAddress);
        m_endAddress = ui->endAddressEdit->text();
        QMessageBox box;
        box.setText(tr("开始地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        emit refreshStartLableStatus(nTempRegister);
        return;
    }
    else if(tempEndAddressFlag == ADDRESS_FORMAT_ERROR&&tempStartAddressFlag == ADDRESS_FORMAT_RIGHT)
    {
        ui->endAddressEdit->setText(m_endAddress);
        QMessageBox box;
        box.setText(tr("结束地址格式错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        emit refreshStartLableStatus(nTempRegister);
        return;
    }
    m_endAddress = ui->endAddressEdit->text();
    int tempLengthFlag = m_dataBlock->verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
    if(tempLengthFlag == LENGTH_NEGATIVENUMBER_ERROR)
    {
        ui->endAddressEdit->setText(ui->startAddressEdit->text());
        m_endAddress = ui->startAddressEdit->text();
    }
    else if(tempLengthFlag == LENGTH_OVERRUN_ERROR)
    {

        QMessageBox box;
        QString strMaxLength = CMDMultiLanguage::instance()->value(tr("最大长度%1，将自动设置成最大长度！")).arg(nTempLength);
        box.setText(strMaxLength);
        box.setWindowTitle(tr("提示："));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
        qint16 tempEndAddress = nTempStartAddress + nTempLength-1;
        QString str_tempEndAddress = QString::number(tempEndAddress,nTempAddressFormat);
        ui->endAddressEdit->setText(str_tempEndAddress);
        m_endAddress = str_tempEndAddress;
    }
    emit refreshStartLableStatus(nTempRegister);
}
void CMDDataBlockDialog::promptMessageBox(const QString &title,const QString &details)
{
    QMessageBox box;
    box.setText(details);
    box.setWindowTitle(title);
    box.setStandardButtons(QMessageBox::Ok);

    CMDMultiLanguage::instance()->translateWidget(&box);

    box.exec();
}
void CMDDataBlockDialog::registerComBox_currentIndexChanged(int registerIndex)
{
    CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device)
    {
         if(device->isRuntimeMode()||device->isRunning())
        {
            return;
        }
    }
    if(registerIndex != -1)
    {
        if(!ui->startAddressEdit->isEnabled()&&!ui->endAddressEdit->isEnabled())
        {
            ui->startAddressEdit->setEnabled(true);
            ui->endAddressEdit->setEnabled(true);
        }
    }


    QString strPrompt = "";
    qint16 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = m_dataBlock->verifyRegisterFormat(QString::number(registerIndex,10),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_RIGHT)
    {
        qint16 nTempStartAddress = 0;
        int tempStartAddressFlag = m_dataBlock->verifyAddressFormat(ui->startAddressEdit->text(),nTempStartAddress,nTempAddressFormat);


        qint16 nTempEndAddress = 0;
        int tempEndAddressFlag = m_dataBlock->verifyAddressFormat(ui->endAddressEdit->text(),nTempEndAddress,nTempAddressFormat);
        if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR&&tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
        {
           strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>开始地址(%1进制:地址范围最大长度%2):<span style='color:#ff0000;'>地址错误!</span></p></body></html>")).arg(nTempAddressFormat).arg(nTempLength);
           ui->label_6->setText(strPrompt);
           strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>结束地址(%1进制:地址范围最大长度%2):<span style='color:#ff0000;'>地址错误!</span></p></body></html>")).arg(nTempAddressFormat).arg(nTempLength);
           ui->label_7->setText(strPrompt);
            return;

        }
        else if(tempStartAddressFlag == ADDRESS_FORMAT_ERROR&& tempEndAddressFlag == ADDRESS_FORMAT_RIGHT)
        {
            strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>开始地址(%1进制:地址范围最大长度%2):<span style='color:#ff0000;'>地址错误!</span></p></body></html>")).arg(nTempAddressFormat).arg(nTempLength);
            ui->label_6->setText(strPrompt);
            strPrompt  = CMDMultiLanguage::instance()->value(tr("结束地址(%1进制:地址范围最大长度%2)")).arg(nTempAddressFormat).arg(nTempLength);
            ui->label_7->setText(strPrompt);
            return;
        }
        else if(tempStartAddressFlag == ADDRESS_FORMAT_RIGHT && tempEndAddressFlag == ADDRESS_FORMAT_ERROR)
        {
            strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>结束地址(%1进制:地址范围最大长度%2):<span style='color:#ff0000;'>地址错误!</span></p></body></html>")).arg(nTempAddressFormat).arg(nTempLength);
            ui->label_7->setText(strPrompt);
            strPrompt  = CMDMultiLanguage::instance()->value(tr("开始地址(%1进制:地址范围最大长度%2)")).arg(nTempAddressFormat).arg(nTempLength);
            ui->label_6->setText(strPrompt);

            return;
        }

        int tempLengthFlag = m_dataBlock->verifyLength(nTempStartAddress,nTempEndAddress,nTempLength);
        if(tempLengthFlag == LENGTH_NEGATIVENUMBER_ERROR || tempLengthFlag == LENGTH_OVERRUN_ERROR)
        {
            strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>开始地址(%1进制:地址范围最大长度%2):<span style='color:#ff0000;'>长度错误!</span></p></body></html>")).arg(nTempAddressFormat).arg(nTempLength);
            ui->label_6->setText(strPrompt);
            strPrompt  = CMDMultiLanguage::instance()->value(tr("<html><head/><body><p>结束地址(%1进制:地址范围最大长度%2):<span style='color:#ff0000;'>长度错误!</span></p></body></html>")).arg(nTempAddressFormat).arg(nTempLength);
            ui->label_7->setText(strPrompt);
            return;
        }
        strPrompt  = CMDMultiLanguage::instance()->value(tr("开始地址(%1进制:地址范围最大长度%2)")).arg(nTempAddressFormat).arg(nTempLength);
        ui->label_6->setText(strPrompt);
        strPrompt  = CMDMultiLanguage::instance()->value(tr("结束地址(%1进制:地址范围最大长度%2)")).arg(nTempAddressFormat).arg(nTempLength);
        ui->label_7->setText(strPrompt);
    }
    else
    {
        ui->label_6->setText(tr("开始地址"));
        ui->label_7->setText(tr("结束地址"));
    }
}
