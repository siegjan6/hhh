/*!

\author dGFuZ3p3

\section 功能

报警等级：颜色配置对话类。

*/

#ifndef MDALARMLEVELCOLORDIALOG_H
#define MDALARMLEVELCOLORDIALOG_H

#include "MDObjectPropertyDialog.h"
#include "MDAlarmLevel.h"
namespace Ui {
class CMDAlarmLevelColorDialog;
}

class CMDAlarmLevel;
class CMDAlarmConfiger;
class CMDAlarmLevelColorDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDAlarmLevelColorDialog(QWidget *parent = 0);
    virtual ~CMDAlarmLevelColorDialog();

    CMDAlarmConfiger* m_alarmConfiger ;// NULL;

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
    QColor selectColor(QColor &initialColor, QPushButton* button);
    QRgb selectColor(QRgb initialColor, QPushButton* button);
    void setButtonColor(QColor &Color, QPushButton* button);
    void setButtonColor(QRgb Color, QPushButton* button);
    CMDAlarmLevel* m_src ;// NULL;
    CMDAlarmLevel m_destFromUI;
private slots:
    void on_occurButtonF_clicked();
    void on_occurButtonB_clicked();

    void on_confirmNotEndButtonF_clicked();
    void on_confirmNotEndButtonB_clicked();

    void on_confirmEndButtonF_clicked();
    void on_confirmEndButtonB_clicked();

    void on_endNotConfirmButtonF_clicked();
    void on_endNotConfirmButtonB_clicked();

    void on_endConfirmButtonF_clicked();
    void on_endConfirmButtonB_clicked();

private:
    Ui::CMDAlarmLevelColorDialog *ui;
};

#endif // MDALARMLEVELCOLORDIALOG_H
