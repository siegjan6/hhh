#include "MDDataBlockDialog.h"
#include "ui_MDDataBlockDialog.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include "MDProtocolCommand.h"//PKX
#include <QMessageBox>

#include "MDMultiLanguage.h"//多语言
#include "PropertyBrowser.h"
CMDDataBlockDialog::CMDDataBlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDataBlockDialog)
{
    ui->setupUi(this);
    setFixedSize(467,388);
    setWindowTitle(tr("数据块参数"));

    m_dataBlock = NULL;
    m_startAddress = "0";
    m_endAddress = "0";
    m_dbNumber = "1";
    //**************************PKX**************************//
//    ui->DBNumberEdit->setEnabled(false);
//    ui->endAddressEdit->setEnabled(false);
//    ui->startAddressEdit->setEnabled(false);

    QRegExp Reg_DataBlock_Name("^[a-zA-Z][a-zA-Z0-9]+$");
    QValidator *Validator_DataBlock_Name = new QRegExpValidator(Reg_DataBlock_Name,this);
    ui->nameEdit->setValidator(Validator_DataBlock_Name);

    QRegExp Reg_DataBlock_Rate("^\\d+$");
    QValidator *Validator_DataBlock_Rate = new QRegExpValidator(Reg_DataBlock_Rate,this);
    ui->rateEdit->setValidator(Validator_DataBlock_Rate);

//    QRegExp Reg_DataBlock_startAddress("^\\d+$");
//    QValidator *Validator_DataBlock_startAddress = new QRegExpValidator(Reg_DataBlock_startAddress,this);
//    ui->startAddressEdit->setValidator(Validator_DataBlock_startAddress);

//    QRegExp Reg_DataBlock_endAddress("^\\d+$");
//    QValidator *Validator_DataBlock_endAddress = new QRegExpValidator(Reg_DataBlock_endAddress,this);
//    ui->endAddressEdit->setValidator(Validator_DataBlock_endAddress);
    //*******************************************************//
    //****************************多语言*****************************//
        QStringList types;
        types << "QLineEdit" << "QComboBox";
        CMDMultiLanguage::instance()->translateWidget(this, &types);
    //**************************************************************//
     connect(ui->registerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(registerComboBox_currentIndexChanged(int)));
     connect(this,SIGNAL(refreshStartLableStatus(int)),this,SLOT(registerComboBox_currentIndexChanged(int )));
     connect(ui->startAddressEdit,SIGNAL(editingFinished()),this,SLOT(startAddressEdit_editingFinished()));
     connect(ui->endAddressEdit,SIGNAL(editingFinished()),this,SLOT(endAddressEdit_editingFinished()));
     connect(ui->cancelButton,SIGNAL(clicked()),this,SLOT(cancelButton_clicked()));
     connect(ui->okButton,SIGNAL(clicked()),this,SLOT(okButton_clicked()));
     connect(ui->DBNumberEdit,SIGNAL(editingFinished()),this,SLOT(dbNumberEdit_editingFinished()));
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
        ui->registerComboBox->setCurrentIndex(m_dataBlock->getRegisterType());
        ui->rateEdit->setText(QString("%1").arg(m_dataBlock->pollRate()));
        ui->DBNumberEdit->setText(m_dataBlock->strDBNumber());
        m_dbNumber = m_dataBlock->strDBNumber();
        CBaseDevice *device= (CBaseDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
        if(m_dataBlock->verifyFlag() == FLAG_REGISTER_ERROR)
        {
            bool okRegister = false;
            qint16 tempRegister = m_dataBlock->strRegisterType().toInt(&okRegister,10);
            if((okRegister!=false)&&(tempRegister>=0&&tempRegister<=5))
            {
                ui->registerComboBox->setCurrentIndex(tempRegister);
            }
            else
            {
                ui->registerComboBox->setCurrentIndex(-1);
                tempRegister = -1;
            }
            ui->DBNumberEdit->setEnabled(false);
            ui->startAddressEdit->setEnabled(false);
            ui->endAddressEdit->setEnabled(false);
            emit refreshStartLableStatus(tempRegister);
        }
        else if(m_dataBlock->verifyFlag() == FLAG_DB_NUMBER_ERROR)
        {
            ui->startAddressEdit->setEnabled(false);
            ui->endAddressEdit->setEnabled(false);
            emit refreshStartLableStatus(m_dataBlock->getRegisterType());
        }
        else
        {
            if(m_dataBlock->getRegisterType() == ID_MEM_DB)
            {
                ui->DBNumberEdit->setEnabled(true);
            }
            else
            {
                ui->DBNumberEdit->setEnabled(false);
            }
            ui->registerComboBox->setCurrentIndex(m_dataBlock->getRegisterType());
            emit refreshStartLableStatus(m_dataBlock->getRegisterType());
        }

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

       //*****************************PKX20160215****************************//
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

       //*****************************PKX20160215****************************//
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
    qint16 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = m_dataBlock->verifyRegisterFormat(QString::number(ui->registerComboBox->currentIndex(),10),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_ERROR)
    {
        QMessageBox box;
        box.setText(tr("寄存器错误!"));
        box.setWindowTitle(tr("提示:"));
        box.setStandardButtons(QMessageBox::Ok);
        CMDMultiLanguage::instance()->translateWidget(&box);
        box.exec();
       return;
    }
    qint16 nTempDBNumber = 0;
    if(nTempRegister == ID_MEM_DB)
    {
        int tempDBnumberFlag = m_dataBlock->verifyDBNumber(ui->DBNumberEdit->text(),nTempDBNumber);
        if(tempDBnumberFlag == DB_NUMBER_ERROR)
        {
            QMessageBox box;
            box.setText(tr("DB块号错误!"));
            box.setWindowTitle(tr("提示:"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
           return;
        }
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
    if(nTempRegister == ID_MEM_DB)
    {
        m_dataBlock->setstrDBNumber(ui->DBNumberEdit->text());
        m_dataBlock->setDBRegisterNum(nTempDBNumber);
    }
    m_dataBlock->setStrStartAddress(ui->startAddressEdit->text());
    m_dataBlock->setStrEndAddress(ui->endAddressEdit->text());
    m_dataBlock->setStrRegisterType(QString::number(ui->registerComboBox->currentIndex(),10));
    m_dataBlock->setStartAddress(nTempStartAddress);
    m_dataBlock->setEndAddress(nTempEndAddress);
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
//*********************************PKX**************************//
void CMDDataBlockDialog::registerComboBox_currentIndexChanged(int index)
{
    CMDDevice *device= (CMDDevice *)m_dataBlock->parentItem(MDTreeItemType::Device);
    if(device)
    {
        if(device->isRuntimeMode()||device->isRunning())
        {
            return;
        }
        if(index!=-1)
        {
            if(index == ID_MEM_DB)
            {
                if(!ui->DBNumberEdit->isEnabled())
                {
                    ui->DBNumberEdit->setEnabled(true);
                }
                qint16 nDBNumber = 0;
                int tempDBNumberFlag = m_dataBlock->verifyDBNumber(ui->DBNumberEdit->text(),nDBNumber);
                if(tempDBNumberFlag == DB_NUMBER_ERROR)
                {
                    if(ui->startAddressEdit->isEnabled())
                    {
                        ui->startAddressEdit->setEnabled(false);
                    }
                    if(ui->endAddressEdit->isEnabled())
                    {
                        ui->endAddressEdit->setEnabled(false);
                    }
                }
                else
                {
                    if(!ui->startAddressEdit->isEnabled())
                    {
                        ui->startAddressEdit->setEnabled(true);
                    }
                    if(!ui->endAddressEdit->isEnabled())
                    {
                        ui->endAddressEdit->setEnabled(true);
                    }
                }
            }
            else
            {
                if(ui->DBNumberEdit->isEnabled())
                {
                    ui->DBNumberEdit->setEnabled(false);
                }
                if(!ui->startAddressEdit->isEnabled())
                {
                    ui->startAddressEdit->setEnabled(true);
                }
                if(!ui->endAddressEdit->isEnabled())
                {
                    ui->endAddressEdit->setEnabled(true);
                }
            }
        }
        else
        {
            if(ui->DBNumberEdit->isEnabled())
            {
                ui->DBNumberEdit->setEnabled(false);
            }
            if(ui->startAddressEdit->isEnabled())
            {
                ui->startAddressEdit->setEnabled(false);
            }
            if(ui->endAddressEdit->isEnabled())
            {
                ui->endAddressEdit->setEnabled(false);
            }
        }
    }

    QString strPrompt = "";
    qint16 nTempRegister = 0;
    qint16 nTempLength = 0;
    int nTempAddressFormat = 0;
    int tempRegisterFlag = m_dataBlock->verifyRegisterFormat(QString::number(index,10),nTempRegister,nTempAddressFormat,nTempLength);
    if(tempRegisterFlag == REGISTER_FORMAT_RIGHT)
    {
        ui->label_8->setText("寄存器类型");

        if(nTempRegister == ID_MEM_DB)
        {
            qint16 nDBNumber = 0;
            int tempDBNumberFlag = m_dataBlock->verifyDBNumber(ui->DBNumberEdit->text(),nDBNumber);
            if(tempDBNumberFlag == DB_NUMBER_ERROR)
            {
                ui->label->setText(tr("<html><head/><body><p>DB寄存器编号(10进制数):<span style='color:#ff0000;'>:编号错误!</span></p></body></html>"));
                ui->label_6->setText(tr("开始地址"));
                ui->label_7->setText(tr("结束地址"));
                return;
            }
            else
            {
                ui->label->setText(tr("DB寄存器编号"));
            }
        }
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
        ui->label->setText(tr("DB寄存器编号"));
        ui->label_6->setText(tr("开始地址"));
        ui->label_7->setText(tr("结束地址"));
        //ui->label_8->setText(tr("<html><head/><body><p>寄存器类型:<span style='color:#ff0000;'>该寄存器不属于当前设备</span></p></body></html>"));
    }
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
        emit refreshStartLableStatus(-1);
       return;
    }
    if(nTempRegister == ID_MEM_DB)
    {
        qint16 nTempDBNumber = 0;
        int tempDBNumberFlag = m_dataBlock->verifyDBNumber(ui->DBNumberEdit->text(),nTempDBNumber);
        if(tempDBNumberFlag == DB_NUMBER_ERROR)
        {
            QMessageBox box;
            box.setText(tr("DB寄存器编号错误!"));
            box.setWindowTitle(tr("提示:"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            ui->endAddressEdit->setText(m_endAddress);
            ui->startAddressEdit->setText(m_startAddress);
            emit refreshStartLableStatus(nTempRegister);
           return;
        }
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
        emit refreshStartLableStatus(nTempRegister);
       return;
    }
    if(nTempRegister == ID_MEM_DB)
    {
        qint16 nTempDBNumber = 0;
        int tempDBNumberFlag = m_dataBlock->verifyDBNumber(ui->DBNumberEdit->text(),nTempDBNumber);
        if(tempDBNumberFlag == DB_NUMBER_ERROR)
        {
            QMessageBox box;
            box.setText(tr("DB寄存器编号错误!"));
            box.setWindowTitle(tr("提示:"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            ui->endAddressEdit->setText(m_endAddress);
            ui->startAddressEdit->setText(m_startAddress);
            emit refreshStartLableStatus(nTempRegister);
           return;
        }
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
//***************************************************************//
void CMDDataBlockDialog::dbNumberEdit_editingFinished()
{
    if(ui->registerComboBox->currentIndex()!=ID_MEM_DB)
    {
        return;
    }
    else
    {
        if(m_dbNumber==ui->DBNumberEdit->text())
        {
            return;
        }
        qint16 nTempDBNumber = 0;
        int tempDBNumberFlag = m_dataBlock->verifyDBNumber(ui->DBNumberEdit->text(),nTempDBNumber);
        if(tempDBNumberFlag == DB_NUMBER_ERROR)
        {
            QMessageBox box;
            box.setText(tr("DB寄存器编号错误!"));
            box.setWindowTitle(tr("提示:"));
            box.setStandardButtons(QMessageBox::Ok);
            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            ui->DBNumberEdit->setText(m_dbNumber);
            emit refreshStartLableStatus(ID_MEM_DB);
           return;
        }
        m_dbNumber = ui->DBNumberEdit->text();
        emit refreshStartLableStatus(ID_MEM_DB);
    }
}
