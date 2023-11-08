/*!
\author aGRw 2015.02.27
\section 配置层窗体
*/
#ifndef LAYERCONFIGDIALOG_H
#define LAYERCONFIGDIALOG_H

#include <QDialog>
#include "MDHMIForm.h"

namespace Ui {
class CLayerConfigDialog;
}


class CLayerConfigDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CLayerConfigDialog(CMDHMIForm* data,  QWidget *parent = 0);
    ~CLayerConfigDialog();

private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::CLayerConfigDialog *ui;
//////////////////////////////////////////////////////////////////////////
private:
    void init();
    void close();
//////////////////////////////////////////////////////////////////////////
private:
    CMDHMIForm* m_data;//!需要编辑的数据

};

#endif // LAYERCONFIGDIALOG_H


