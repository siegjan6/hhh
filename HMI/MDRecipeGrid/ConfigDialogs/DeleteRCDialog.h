/*!
author Y2hvdWps 2016.08.29
section 删除行/列设置对话框
*/
#ifndef DELETERCDIALOG_H
#define DELETERCDIALOG_H

#include <QDialog>

namespace Ui {
class CDeleteRCDialog;
}

class CDeleteRCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDeleteRCDialog(QWidget *parent = 0);
    virtual ~CDeleteRCDialog();

public:
    bool m_row;

private slots:
    void on_rowRadioButton_toggled(bool checked);

    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CDeleteRCDialog *ui;
};

#endif // DELETERCDIALOG_H
