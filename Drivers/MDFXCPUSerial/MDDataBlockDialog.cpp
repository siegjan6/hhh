#include "MDDataBlockDialog.h"
#include "ui_MDDataBlockDialog.h"
#include "MDDevice.h"
#include "MDDataBlock.h"
#include <QMessageBox>
#include "MDProtocolCommand.h"

#include"MDMultiLanguage.h"//多语言

CMDDataBlockDialog::CMDDataBlockDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDDataBlockDialog)
{
    ui->setupUi(this);
    setFixedSize(447,350);
    setWindowTitle(tr("数据块参数"));
    m_dataBlock = NULL;
    m_startAddress = 0;
    m_endAddress = 0;
    QRegExp regExp1("^[a-zA-Z]+[a-zA-Z0-9_]*$");
    ui->nameEdit->setValidator(new QRegExpValidator(regExp1,this));
    QRegExp regExp2("^\\d+$");
    ui->endAddressEdit->setValidator(new QRegExpValidator(regExp2,this));
    ui->startAddressEdit->setValidator(new QRegExpValidator(regExp2,this));
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
        ui->startAddressEdit->setText(QString("%1").arg(m_dataBlock->startAddress()));
        m_startAddress = m_dataBlock->startAddress();
        ui->endAddressEdit->setText(QString("%1").arg(m_dataBlock->endAddress()));
        m_endAddress = m_dataBlock->endAddress();
        ui->registerComboBox->setCurrentIndex(m_dataBlock->registerType());
        ui->rateEdit->setText(QString("%1").arg(m_dataBlock->pollRate()));

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
    CMDDevice *device = (CMDDevice*)m_dataBlock->parent(MDTreeItemType::Device);
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

    m_dataBlock->setName(name);
    m_dataBlock->setDescription(ui->descriptionEdit->text());
    m_dataBlock->setEnabled(ui->enableCheckBox->isChecked());
    m_dataBlock->setStartAddress(ui->startAddressEdit->text().toInt());
    m_dataBlock->setEndAddress(ui->endAddressEdit->text().toInt());
    m_dataBlock->setRegisterType(ui->registerComboBox->currentIndex());
    m_dataBlock->setPollRate(ui->rateEdit->text().toInt());

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
    QString str_Start_Address = ui->startAddressEdit->text();
    qint16  int_Start_Address = (qint16)str_Start_Address.toInt();
    if(m_startAddress == int_Start_Address)
    {
        return;
    }
    else
    {
        if(ui->registerComboBox->currentIndex() == ID_MEM_X ||ui->registerComboBox->currentIndex()  == ID_MEM_Y)
        {
            QString str_Start_Address_Temp = QString::number(int_Start_Address,10);
            bool bNEffectiveFlag = 0;
            for(int i = 0;i<str_Start_Address_Temp.size();i++)
            {
                QString strTemp = str_Start_Address_Temp.mid(i,1);
                if(strTemp>"7")
                {
                    bNEffectiveFlag = 1;
                    break;
                }
            }
            if(bNEffectiveFlag)
            {
                bNEffectiveFlag = false;

                promptMessageBox(tr("提示:"),tr("变量地址8进制表示,开始地址中不能含有8或9!"));

                bool bFlag = 0;
                int strLength = str_Start_Address_Temp.size();
                for(int i = 0;i<strLength;i++)
                {
                    QString str_Temp = str_Start_Address_Temp.at(str_Start_Address_Temp.size()-1-i);

                    int nValue = str_Temp.toInt();
                    if(i == str_Start_Address_Temp.size()-1)
                    {
                        if(nValue+bFlag >7)
                        {
                            bFlag = true;
                            str_Start_Address_Temp.replace(str_Start_Address_Temp.size()-1-i,1,"0");
                            str_Start_Address_Temp.prepend("1");
                        }
                        else
                        {
                            int nTempValue = nValue+bFlag;
                            QString strTemp = QString::number(nTempValue,10);
                            str_Start_Address_Temp.replace(str_Start_Address_Temp.size()-1-i,1,strTemp);
                            bFlag = false;
                        }
                    }
                    else
                    {
                        if(nValue+bFlag >7)
                        {
                            str_Start_Address_Temp.replace(str_Start_Address_Temp.size()-1-i,1,"0");
                             bFlag = true;
                        }
                        else
                        {
                            int nTempValue = nValue+bFlag;
                            QString strTemp = QString::number(nTempValue,10);
                            str_Start_Address_Temp.replace(str_Start_Address_Temp.size()-1-i,1,strTemp);
                            bFlag = false;
                        }
                    }
                }
                str_Start_Address = str_Start_Address_Temp;
                int_Start_Address = (qint16)str_Start_Address.toInt();
                ui->startAddressEdit->setText(str_Start_Address);
            }
        }

        QString str_End_Address =ui->endAddressEdit->text();
        qint16   int_End_Address = (qint16)str_End_Address.toInt();
        if(int_Start_Address>int_End_Address)
        {
            ui->endAddressEdit->setText(str_Start_Address);
            m_startAddress = int_Start_Address;
            m_endAddress = int_Start_Address;

        }
        else
        {
            switch (ui->registerComboBox->currentIndex() )
            {
            case ID_MEM_X:
            case ID_MEM_Y:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Length>1000)
                    {
                        promptMessageBox(tr("提示:"),tr("最大长度1000，将自动设置成最大长度！"));

                        int tempEndAddress = int_Start_Address + 999;
                        QString str_End_Address_Temp = QString::number(tempEndAddress,10);
                        bool bFlag = 0;
                        //int strLength = str_End_Address_Temp.size();
                        for(int i = 0;i<str_End_Address_Temp.size();i++)
                        {
                            QString str_Temp = str_End_Address_Temp.at(i);
                            int nValue = str_Temp.toInt();
                            if(bFlag)
                            {
                                str_End_Address_Temp.replace(i,1,"7");
                            }
                            else
                            {
                                if(nValue >7)
                                {
                                    bFlag = true;
                                    str_End_Address_Temp.replace(i,1,"7");
                                }
                            }
                        }
                        bFlag = false;
                        //m_startAddress = int_Start_Address;
                        m_endAddress = (qint16)str_End_Address_Temp.toInt();
                        ui->endAddressEdit->setText(str_End_Address_Temp);
                    }
                }
                break;
            case ID_MEM_M:
            case ID_MEM_S:
            case ID_MEM_CS:
            case ID_MEM_TS:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Length>1000)
                    {
                        promptMessageBox(tr("提示:"),tr("最大长度1000，将自动设置成最大长度！"));

                        int tempEndAddress = int_Start_Address + 999;
                        //m_startAddress = int_Start_Address;
                        m_endAddress = tempEndAddress;
                        QString str_tempEndAddress = QString::number(tempEndAddress,10);
                        ui->endAddressEdit->setText(str_tempEndAddress);
                    }
                }
                break;
            case ID_MEM_C:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Start_Address>= 8000)
                    {
                        if(int_Length >63)
                        {
                            promptMessageBox(tr("提示:"),tr("最大长度63，将自动设置成最大长度！"));
                            int tempEndAddress = int_Start_Address + 62;
                            //m_startAddress = int_Start_Address;
                            m_endAddress = tempEndAddress;
                            QString str_tempEndAddress = QString::number(tempEndAddress,10);
                            ui->endAddressEdit->setText(str_tempEndAddress);
                        }

                    }
                    else
                    {
                        if(int_Length >127)
                        {
                            promptMessageBox(tr("提示:"),tr("最大长度127，将自动设置成最大长度！"));
                            int tempEndAddress = int_Start_Address + 126;
                            //m_startAddress = int_Start_Address;
                            m_endAddress = tempEndAddress;
                            QString str_tempEndAddress = QString::number(tempEndAddress,10);
                            ui->endAddressEdit->setText(str_tempEndAddress);
                        }
                    }
                }
                break;
            case ID_MEM_T:
            case ID_MEM_D:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Length >127)
                    {
                        promptMessageBox(tr("提示:"),tr("最大长度127，将自动设置成最大长度！"));
                        int tempEndAddress = int_Start_Address + 126;

                        m_endAddress = tempEndAddress;
                        QString str_tempEndAddress = QString::number(tempEndAddress,10);
                        ui->endAddressEdit->setText(str_tempEndAddress);
                    }
                }
                break;
            default:
                break;
            }
            m_startAddress = int_Start_Address;
        }
    }
}

void CMDDataBlockDialog::endAddressEdit_editingFinished()
{
    QString str_End_Address = ui->endAddressEdit->text();
    qint16 int_End_Address =(qint16)str_End_Address.toInt();
    if(int_End_Address == m_endAddress)
    {
        return;
    }
    else
    {
        if(ui->registerComboBox->currentIndex() == ID_MEM_X ||ui->registerComboBox->currentIndex()  == ID_MEM_Y)
        {
            QString str_End_Address_Temp = QString::number(int_End_Address,10);
            bool bNEffectiveFlag = 0;
            for(int i = 0;i<str_End_Address_Temp.size();i++)
            {
                QString strTemp = str_End_Address_Temp.mid(i,1);
                if(strTemp>"7")
                {
                    bNEffectiveFlag = 1;
                    break;
                }
            }
            if(bNEffectiveFlag)
            {
                promptMessageBox(tr("提示:"),tr("变量地址8进制表示,开始地址中不能含有8或9!"));
                bool bFlag = 0;
                QString str_Start_Address_Temp = ui->startAddressEdit->text();
                qint16 int_Start_Address_Temp = (qint16)str_Start_Address_Temp.toInt();
                qint16 nLength = int_End_Address - int_Start_Address_Temp +1;
                if(nLength>1000)
                {
                    int_End_Address = int_Start_Address_Temp + 999;
                    str_End_Address_Temp = QString::number(int_End_Address,10);
                }
                for(int i = 0;i<str_End_Address_Temp.size();i++)
                {
                    QString str_Temp = str_End_Address_Temp.at(i);
                    int nValue = str_Temp.toInt();
                    if(bFlag)
                    {
                        str_End_Address_Temp.replace(i,1,"7");
                    }
                    else
                    {
                        if(nValue >7)
                        {
                            bFlag = true;
                            str_End_Address_Temp.replace(i,1,"7");
                        }
                    }
                }
                bFlag = 0;
                str_End_Address = str_End_Address_Temp;
                int_End_Address = (qint16)str_End_Address.toInt();
                m_endAddress = int_End_Address;
                ui->endAddressEdit->setText(str_End_Address);
            }

        }
        QString str_Start_Address = ui->startAddressEdit->text();
        qint16 int_Start_Address = (qint16)str_Start_Address.toInt();
        if(int_Start_Address>int_End_Address)
        {
            ui->endAddressEdit->setText(str_Start_Address);
            m_endAddress = int_Start_Address;
        }
        else
        {
            m_endAddress = int_End_Address;
            switch (ui->registerComboBox->currentIndex() )
            {
            case ID_MEM_X:
            case ID_MEM_Y:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Length>1000)
                    {
                        promptMessageBox(tr("提示:"),tr("最大长度1000，将自动设置成最大长度！"));
                        qint16 tempEndAddress = int_Start_Address + 999;
                        QString str_End_Address_Temp = QString::number(tempEndAddress,10);
                        bool bFlag = 0;
                        for(int i = 0;i<str_End_Address_Temp.size();i++)
                        {
                            QString str_Temp = str_End_Address_Temp.at(i);
                            int nValue = str_Temp.toInt();
                            if(bFlag)
                            {
                                str_End_Address_Temp.replace(i,1,"7");
                            }
                            else
                            {
                                if(nValue >7)
                                {
                                    bFlag = true;
                                    str_End_Address_Temp.replace(i,1,"7");
                                }
                            }
                        }
                        bFlag = 0;
                        m_endAddress = (qint16)str_End_Address_Temp.toInt();
                        ui->endAddressEdit->setText(str_End_Address_Temp);
                    }
                }
                break;
            case ID_MEM_M:
            case ID_MEM_S:
            case ID_MEM_CS:
            case ID_MEM_TS:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Length>1000)
                    {
                         promptMessageBox(tr("提示:"),tr("最大长度1000，将自动设置成最大长度！"));
                        qint16 tempEndAddress = int_Start_Address + 999;
                        m_endAddress = tempEndAddress;
                        QString str_tempEndAddress = QString::number(tempEndAddress,10);
                        ui->endAddressEdit->setText(str_tempEndAddress);
                    }
                }
                break;
            case ID_MEM_C:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Start_Address>= 8000)
                    {
                        if(int_Length >63)
                        {
                            promptMessageBox(tr("提示:"),tr("最大长度63，将自动设置成最大长度！"));

                            qint16 tempEndAddress = int_Start_Address + 62;
                            m_endAddress = tempEndAddress;
                            QString str_tempEndAddress = QString::number(tempEndAddress,10);
                            ui->endAddressEdit->setText(str_tempEndAddress);
                        }

                    }
                    else
                    {
                        if(int_Length >127)
                        {
                            promptMessageBox(tr("提示:"),tr("最大长度127，将自动设置成最大长度！"));

                            qint16 tempEndAddress = int_Start_Address + 126;
                            m_endAddress = tempEndAddress;
                            QString str_tempEndAddress = QString::number(tempEndAddress,10);
                            ui->endAddressEdit->setText(str_tempEndAddress);
                        }
                    }

                }
                break;
            case ID_MEM_T:
            case ID_MEM_D:
                {
                    int int_Length = int_End_Address - int_Start_Address+1;
                    if(int_Length >127)
                    {
                        promptMessageBox(tr("提示:"),tr("最大长度127，将自动设置成最大长度！"));

                        qint16 tempEndAddress = int_Start_Address + 126;
                        m_endAddress = tempEndAddress;
                        QString str_tempEndAddress = QString::number(tempEndAddress,10);
                        ui->endAddressEdit->setText(str_tempEndAddress);
                    }
                }
                break;
            default:
                break;
            }
        }
    }
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
