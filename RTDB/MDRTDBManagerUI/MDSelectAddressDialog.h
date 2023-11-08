#ifndef MDSELECTADDRESSDIALOG_H
#define MDSELECTADDRESSDIALOG_H

#include <QDialog>

#include "IMDRTDBVariable.h"
#include "IDevice.h"
#include "IDataBlock.h"

namespace Ui {
class CMDSelectAddressDialog;
}

class CMDSelectAddressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDSelectAddressDialog(QWidget *parent = 0);
    ~CMDSelectAddressDialog();

public:
    //!设定数据块地址列表
    void setDataBlockAddressList(const QStringList& addrList);
    //!修改变量地址时，验证地址用
    void setDeviceAndVariable(IDevice* device, IMDRTDBVariable* variable);

private:
    bool checkAddress(QString& strErr);

private slots:
    void onAddressTableWidgetCellDoubleClicked(int row, int column);
    void on_OkButton_clicked();

    void on_CancelButton_clicked();
private:
    Ui::CMDSelectAddressDialog *ui;
    IDevice* m_device;
    IMDRTDBVariable* m_variable;
};

#endif // MDSELECTADDRESSDIALOG_H
