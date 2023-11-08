/*!
author Y2hvdWps 2016.08.29
section 插入/添加行/列设置对话框
*/
#ifndef INSERTRCDIALOG_H
#define INSERTRCDIALOG_H

#include <QDialog>

namespace Ui {
class CInsertRCDialog;
}

class CInsertRCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CInsertRCDialog(QWidget *parent = 0);
    virtual ~CInsertRCDialog();

public:
    quint32 m_count;
    bool m_row;  //! row/column

private slots:
    void on_confirmButton_clicked();

    void on_cancelButton_clicked();

    void on_rowButton_toggled(bool checked);

private:
    Ui::CInsertRCDialog *ui;
};

#endif // INSERTRCDIALOG_H
