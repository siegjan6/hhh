/*!
\author enli 2015.01.08
\section 目前未使用
*/
#ifndef MDDRIVERCONFIGDIALOG_H
#define MDDRIVERCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class CMDDriverConfigDialog;
}

class CMDDriverConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDDriverConfigDialog(QWidget *parent = 0);
    ~CMDDriverConfigDialog();

private:
    Ui::CMDDriverConfigDialog *ui;
};

#endif // MDDRIVERCONFIGDIALOG_H
