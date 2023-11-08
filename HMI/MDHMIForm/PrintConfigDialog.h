/*!
\author aGRw 2015.11.05
\section 打印设置对话框
*/
#ifndef PRINTCONFIGDIALOG_H
#define PRINTCONFIGDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include "Environment.h"

namespace Ui {
class CPrintConfigDialog;
}

class CPrintConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPrintConfigDialog(QWidget *parent = 0);
    ~CPrintConfigDialog();
private slots:
    //!添加自定义字符
    void on_pushButtonDate_clicked();
    void on_pushButtonTime_clicked();
    void on_pushButtonPage_clicked();
    void on_pushButtonPageCount_clicked();

    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CPrintConfigDialog *ui;
//////////////////////////////////////////////////////////////
protected:
    bool eventFilter(QObject *o, QEvent *e);
//////////////////////////////////////////////////////////////
private:
    QString sDate;
    QString sTime;
    QString sPage;
    QString sPageCount;
//////////////////////////////////////////////////////////////
public:
    void init(CEnvironment* environment);
private:
    void saveData();
    //!添加自定义字符
    void addCustomString(const QString& custom);
private:
    CEnvironment* m_environment;
    QTextEdit* m_lastFocusInEdit;
};

#endif // PRINTCONFIGDIALOG_H
