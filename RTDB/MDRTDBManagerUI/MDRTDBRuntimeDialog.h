/*!
\author enli 2015.01.08
\section 运行时实时数据库管理,由CMDRTDBRuntimeUI创建
*/
#ifndef MDRTDBRUNTIMEDIALOG_H
#define MDRTDBRUNTIMEDIALOG_H

#include <QListWidget>
#include <QStackedWidget>
#include <QDialog>
#include <QHBoxLayout>
#include "MDRTDBDevicesWidget.h"
#include "MDRTDBVariablesWidget.h"
class CMDRTDBRuntimeDialog: public QDialog
{
Q_OBJECT
public:
    CMDRTDBRuntimeDialog(QWidget *parent = 0);
        ~CMDRTDBRuntimeDialog();
public:
    void setProjectConfiger(CMDProjectConfiger *projectConfiger);
private:
    //!配套使用的两个Widget
    QListWidget *m_listWidget;
    QStackedWidget *m_stackWidget;
    //!m_stackWidget的两个子框
    CMDRTDBDevicesWidget *m_devicesWidget;
    CMDRTDBVariablesWidget*m_variablesWidget;
    QHBoxLayout *m_mainLayout;

protected:
    void showEvent(QShowEvent *event) override;
};

#endif // MDRTDBRUNTIMEDIALOG_H
