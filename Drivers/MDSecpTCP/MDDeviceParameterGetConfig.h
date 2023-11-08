#ifndef MDDEVICEPARAMETERGETCONFIG_H
#define MDDEVICEPARAMETERGETCONFIG_H

#include <QDialog>
#include <QTime>
class CMDDevice;
namespace Ui {
class CMDDeviceParameterGetConfig;
}

class CMDDeviceParameterGetConfig : public QDialog
{
    Q_OBJECT

public:
    explicit CMDDeviceParameterGetConfig(QWidget *parent = 0);
    ~CMDDeviceParameterGetConfig();

private:
    Ui::CMDDeviceParameterGetConfig *ui;
public:
    void setDevicePoint(CMDDevice* pdev);
private slots:
    void configButton_clicked();
    void monitorGetConfigStatus();
private:
    CMDDevice* m_pDevice;
    QTimer *m_pTimer;
    int     m_nCounter;//!计数
};

#endif // MDDEVICEPARAMETERGETCONFIG_H
