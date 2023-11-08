#include "VariableWriteValueDialog.h"
#include "ui_VariableWriteValueDialog.h"
#include <QMessageBox>
#include "MDMultiLanguage.h"
#include <QValidator>
#include <QClipboard>
CVariableWriteValueDialog::CVariableWriteValueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CVariableWriteValueDialog)
{
    m_variable = NULL;
    ui->setupUi(this);
    connect(ui->writeVarButton, SIGNAL(clicked()), this, SLOT(onWriteVarButtonClicked()));
    connect(ui->closeButton, SIGNAL(clicked()), this, SLOT(onCloseButtonClicked()));
    m_timer.setInterval(50);
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    ui->writeValueEdit->setFocus();
    this->setFixedSize(389,267);
    m_timer.start();
    //多语言
    CMDMultiLanguage::instance()->translateWidget(this);
    //写按钮是否使能
    QString strValue = ui->writeValueEdit->text();

    if(strValue.isNull()||strValue.isEmpty())
        ui->writeVarButton->setEnabled(false);
    else
        ui->writeVarButton->setEnabled(true);
}

CVariableWriteValueDialog::~CVariableWriteValueDialog()
{
    delete ui;
}
void CVariableWriteValueDialog::setVariable(IMDRTDBVariable *variable)
{
    m_objectMutex.lock();
    m_variable = variable;
    //如果不可写，待写值为灰色

    if(m_variable != NULL)
    {
        if(m_variable->writeProtectParameter().m_enableWrite)
            ui->writeValueEdit->setEnabled(true);
        else
            ui->writeValueEdit->setEnabled(false);

        //!限制变量数据类型为非字符时，智能填写数字
        switch(variable->dataType())
        {
        case MDDataType::Double:
        case MDDataType::Float:
            {
                QDoubleValidator* validator = new QDoubleValidator(-1.79E+308,+1.79E+308,variable->precision());
                ui->writeValueEdit->setValidator(validator);
            }
            break;
        case MDDataType::Bool:
            {
                QIntValidator* validator = new QIntValidator(0,1);
                ui->writeValueEdit->setValidator(validator);
            }
            break;
        case MDDataType::Byte:
            {
                QIntValidator* validator = new QIntValidator(0,255);
                ui->writeValueEdit->setValidator(validator);
            }
            break;
        case MDDataType::SByte:
            {
                QIntValidator* validator = new QIntValidator(-128,127);
                ui->writeValueEdit->setValidator(validator);
            }
            break;
        case MDDataType::Int16:
            {
                QIntValidator* validator = new QIntValidator(-32768,32767);
                ui->writeValueEdit->setValidator(validator);
            }
            break;
        case MDDataType::UInt16:
            {
                QIntValidator* validator = new QIntValidator(0,65535);
                ui->writeValueEdit->setValidator(validator);
            }
            break;
        case MDDataType::Int32:
            {
                QIntValidator* validator = new QIntValidator(-2147483648,2147483647);
                ui->writeValueEdit->setValidator(validator);
            }
            break;
        case MDDataType::UInt32:
        case MDDataType::UInt64:
            {
                QRegExp reg("^[\\d+]?\\d*$");
                ui->writeValueEdit->setValidator(new QRegExpValidator(reg,this));
            }
            break;
        case MDDataType::Int64:
            {
                QRegExp reg("^[-?\\d+]?\\d*$");
                ui->writeValueEdit->setValidator(new QRegExpValidator(reg,this));
            }
            break;
        default:
            break;
        }

        ui->nameEdit->setText(m_variable->name());
        ui->descriptioEdit->setText(m_variable->getDescription());
        ui->currentValueEdit->setText(m_variable->currentValue().toString());
    }
    m_objectMutex.unlock();
}
//!刷新当前值
void CVariableWriteValueDialog::onTimerOut()
{
        m_objectMutex.lock();
        if(m_variable != NULL)
        {
            QString strValue;
            if(m_variable->dataType()==MDDataType::Float)
            {
                strValue = QString::number(m_variable->originalValue().toFloat(),'f',m_variable->precision());
            }
            else if(m_variable->dataType()==MDDataType::Double)
            {
                strValue =QString::number(m_variable->originalValue().toDouble(),'f',m_variable->precision());
            }
            else
            {
                strValue =m_variable->currentValue().toString();
            }
            ui->currentValueEdit->setText(strValue);
        }
        m_objectMutex.unlock();
}
//!确定写值
void CVariableWriteValueDialog::onWriteVarButtonClicked()
{
    QString strValue = ui->writeValueEdit->text();

    if(strValue.isNull()||strValue.isEmpty())
    {

        //****************************** Y2hw *****************************
        /*
        QMessageBox::warning(this, "Error", "请设置待写值！", QMessageBox::Ok);
        */
//        QMessageBox box;
//        box.setText(tr("请设置待写值！"));
//        box.setWindowTitle(tr("Error"));
//        box.setStandardButtons(QMessageBox::Ok);

//        CMDMultiLanguage::instance()->translateWidget(&box);
//        box.exec();
        //----------------------------------------------------------------*/
        if(m_variable == NULL ||
                (m_variable->dataType() != MDDataType::String))
            return;
    }
    //!限制边界值
    if(m_variable != NULL &&
            (m_variable->dataType() > MDDataType::Bool || m_variable->dataType() < MDDataType::DateTime))
    {
        switch (m_variable->dataType())
        {
        case MDDataType::SByte:
            {
                qint16 val = strValue.toInt();
                if(val > 127)
                    strValue = "127";
            }
            break;
        case MDDataType::Int16:
            {
                qint16 val = strValue.toInt();
                if(val > 32767)
                    strValue = "32767";
            }
            break;
        case MDDataType::Int32:
            {
                qint64 val = strValue.toLongLong();
                if(val > 2147483647)
                    strValue = "2147483647";
            }
            break;
        case MDDataType::UInt32:
            {
                qint64 val = strValue.toLongLong();
                if(val > 4294967295)
                    strValue = "4294967295";
            }
            break;
        case MDDataType::Int64:
            {
                qint64 val = strValue.toLongLong();

                if(val < -9223372036854775808)
                {
                    strValue = "-9223372036854775808";
                }
                else if(val > 9223372036854775807)
                {
                    strValue = "9223372036854775807";
                }
            }
            break;
        case MDDataType::UInt64:
            {
                quint64 val = strValue.toULongLong();
                if(val  > 18446744073709551615)
                {
                    strValue = "18446744073709551615";
                }
            }
            break;
        default:
            break;
        }
        m_variable->writeValue(QVariant(strValue));
    }
    //this->setResult(QDialog::Accepted);
    //this->accept();
}

void CVariableWriteValueDialog::onCloseButtonClicked()
{
    this->hide();
    //this->setResult(QDialog::Rejected);
    //this->reject ();
}
//以前变量待写值为空时，点击确定按钮时，弹出的‘请设置待写值’对话框在写变量对话框的后面。
//现改为
void CVariableWriteValueDialog::on_writeValueEdit_textChanged(const QString &arg1)
{
    QString strValue = ui->writeValueEdit->text();

    if(strValue.isNull()||strValue.isEmpty())
    {
        if(m_variable != NULL &&
                (m_variable->dataType() == MDDataType::String))
            ui->writeVarButton->setEnabled(true);
        else
            ui->writeVarButton->setEnabled(false);
    }
    else
        ui->writeVarButton->setEnabled(true);
}

void CVariableWriteValueDialog::on_bt_copy_clicked()
{
    QString strVdotPath = ui->nameEdit->text();
    QString strVslashPath = strVdotPath.replace('.', '/');
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strVslashPath);
}

void CVariableWriteValueDialog::on_bt_copy_value_clicked()
{
    QString strValue = ui->currentValueEdit->text();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(strValue);
}
