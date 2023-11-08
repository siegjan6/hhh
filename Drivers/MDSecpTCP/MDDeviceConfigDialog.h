#ifndef MDDEVICECONFIGDIALOG_H
#define MDDEVICECONFIGDIALOG_H

#include <QDialog>
#include "MDDevice.h"
#include <QTimer>

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
    void onCommunicationTypeBoxCurrentIndexChanged(int index);
    /*!
     * \brief 监测配置获取时的状态.
     */
    void monitorGetConfigStatus();


    void configButton_clicked();

private:
    Ui::CMDDeviceConfigDialog *ui;

public:
    void setDevice(CMDDevice *device);
private:
    CMDDevice *m_device;
private:
    //QIntValidator *m_intValidator;
    QTimer *m_pTimer;
    int     m_nCounter;//!计数
    bool  m_uiStatuFlag;

};

#endif // MDDEVICECONFIGDIALOG_H
