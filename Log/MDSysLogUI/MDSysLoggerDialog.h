/*!

\author dGFuZ3p3

\section 功能

用于显示日志的对话类。

*/

#ifndef MDSYSLOGGERDIALOG_H
#define MDSYSLOGGERDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class CMDSysLoggerDialog;
}

class CMDSysLogTableModel;
class CMDSysLogger;
class CMDSysLoggerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDSysLoggerDialog(QWidget *parent = 0);
    ~CMDSysLoggerDialog();
/*!
\section 初始化
*/
public:
    void setlogger(CMDSysLogger* logger);

/*!
\section 关闭处理
*/
public:
    void setCloseDialog(bool closeDialog);
protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent * event);
    bool m_closeDialog ;// true;

/*!
\section 日志数据模型；刷新界面
*/
private:
    CMDSysLogTableModel *m_logModel ;// NULL;
    QTimer m_updateTimer;
 private slots:
    void updateLogTable();
private:
    Ui::CMDSysLoggerDialog *ui;
};

#endif // MDSYSLOGGERDIALOG_H
