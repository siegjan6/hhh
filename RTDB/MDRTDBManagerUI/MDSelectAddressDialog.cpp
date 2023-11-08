#include <QMessageBox>

#include "MDSelectAddressDialog.h"
#include "ui_SelectAddressDialog.h"


#include "MDMultiLanguage.h"
CMDSelectAddressDialog::CMDSelectAddressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMDSelectAddressDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("选择变量地址"));

    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setColumnWidth(0,ui->tableWidget->width()*2);
    QStringList headers;
    headers << tr("设备可选地址");
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignLeft);
    connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int, int)),this,SLOT(onAddressTableWidgetCellDoubleClicked(int, int)));

    m_device = NULL;
    m_variable = NULL;
}

CMDSelectAddressDialog::~CMDSelectAddressDialog()
{
    delete ui;
}

void CMDSelectAddressDialog::setDataBlockAddressList(const QStringList& addrList)
{
   ui->tableWidget->setRowCount(0);
   ui->tableWidget->setRowCount(addrList.size());
   for(int i=0;i<addrList.size();i++)
   {
       ui->tableWidget->setItem(i, 0, new QTableWidgetItem(addrList[i]));
   }
}

void CMDSelectAddressDialog::onAddressTableWidgetCellDoubleClicked(int row, int column)
{
    QString address = ui->tableWidget->item(row,column)->text();
    if(!address.isEmpty())
    {
        int pos = address.indexOf("-");
        if(pos>0)
            address = address.left(pos);
        ui->lineEdit->setText(address);
    }
}

void CMDSelectAddressDialog::setDeviceAndVariable(IDevice* device, IMDRTDBVariable* variable)
{
    m_device = device;
    m_variable = variable;
}

void CMDSelectAddressDialog::on_OkButton_clicked()
{
    QString strOldAddr = m_variable->address();
    if(m_device != NULL && m_variable != NULL)
    {
        QString strAddr = ui->lineEdit->text();
        strAddr.trimmed();
        if(strAddr.isEmpty())
        {
            QMessageBox box;
            box.setText(tr("变量名为空!"));
            box.setWindowTitle(tr("警告"));
            box.setStandardButtons(QMessageBox::Ok);

            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            return;
        }

        m_variable->setAddress(ui->lineEdit->text());

        QString strErr;

        if(!checkAddress(strErr))
        {
            QMessageBox box;
            box.setText(strErr);
            box.setWindowTitle(tr("警告"));
            box.setStandardButtons(QMessageBox::Ok);

            CMDMultiLanguage::instance()->translateWidget(&box);
            box.exec();
            m_variable->setAddress(strOldAddr);
            return;
        }
    }
    this->accept();
}

void CMDSelectAddressDialog::on_CancelButton_clicked()
{
    this->reject();
}
bool CMDSelectAddressDialog::checkAddress(QString& strErr)
{
    bool bCheckOK = m_device->checkVarAddress(m_variable,strErr);
    return bCheckOK;
}
