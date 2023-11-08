#ifndef CDATABLOCKMONITORDIALOG_H
#define CDATABLOCKMONITORDIALOG_H

#include <QDialog>
#include <QMutex>
#include <QMutexLocker>
#include "IDataBlock.h"
class ITranslateWidgetTool;
namespace Ui {
class CDataBlockMonitorDialog;
}

class CDataBlockMonitorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDataBlockMonitorDialog(QWidget *parent = 0);
    ~CDataBlockMonitorDialog();
    //!设置监控数据块
    void setMonitoredDataBlock(IDataBlock* dataBlock);
    //!响应数据包事件
    void customEvent(QEvent *e);
    //!多语言
    void changeLanguage();

private slots:
    //改变监控数据包格式
    void on_monitorAscii_stateChanged(int arg1);
    void on_clearListWidgetBtn_clicked();
    void on_exportPacketBtn_clicked();

    void on_pauseMonitorBtn_clicked();

    void on_closeDlgBtn_clicked();

private:
    Ui::CDataBlockMonitorDialog *ui;
    IDataBlock*  m_DataBlock;
    QMutex m_dataBlockMutex;
    bool m_Monitoring;
    ITranslateWidgetTool* m_translateTool;
};

#endif // CDATABLOCKMONITORDIALOG_H
