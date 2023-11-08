#ifndef MDDEVICECONFIGDIALOG_H
#define MDDEVICECONFIGDIALOG_H

#include <QDialog>
#include "MDDevice.h"
namespace Ui {
class CMDDeviceConfigDialog;
}

class CMDDeviceConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDDeviceConfigDialog(QWidget *parent = 0);
    ~CMDDeviceConfigDialog();

private slots:
    void okButton_clicked();
    void cancelButton_clicked();
    void communicationTypeBox_currentIndexChanged(int index);
    /*!
     * \brief on_ModelType_currentIndexChanged
     * dev配置界面PLC类型选择变化时：更新对应显示及参数
     */
    void ModelType_currentIndexChanged(int index);

private:
    Ui::CMDDeviceConfigDialog *ui;

public:
    void setDevice(CMDDevice *device);
private:
    CMDDevice *m_device;
private:
    //QIntValidator *m_intValidator;
};

#endif // MDDEVICECONFIGDIALOG_H
