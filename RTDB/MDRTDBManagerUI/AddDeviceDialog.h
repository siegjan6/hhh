/*!
\author enli 2015.01.08
\section 配置模式下添加驱动设备对话框
*/
#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include "IMDRTDBManager.h"
namespace Ui {
class CAddDeviceDialog;
}

class CAddDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddDeviceDialog(QWidget *parent = 0);
    ~CAddDeviceDialog();

private slots:
    void on_cancelButton_clicked();
    void on_okButton_clicked();
    //!驱动变化
    void on_driverBox_currentIndexChanged(int index);

private:
    Ui::CAddDeviceDialog *ui;

public:
    void setDriverManager(IMDDriverManager *driverManager);
    QString getDeviceName();//Y2hw
private:
    //驱动管理器
    IMDDriverManager  *m_driverManager;
};

#endif // ADDDEVICEDIALOG_H
