/*!
author Y2hvdWps 2016.08.30
section 行/列的高/宽度设置对话框
*/
#ifndef SETHWDIALOG_H
#define SETHWDIALOG_H

#include <QDialog>

namespace Ui {
class CSetHWDialog;
}

class CSetHWDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSetHWDialog(quint32 defaultRowHeight, QWidget *parent = 0);
    virtual ~CSetHWDialog();

    bool m_row;//! 行or列
    quint32 m_number;//! 高or宽度

protected:
    virtual void accept();
private:
    Ui::CSetHWDialog *ui;
};

#endif // SETHWDIALOG_H
