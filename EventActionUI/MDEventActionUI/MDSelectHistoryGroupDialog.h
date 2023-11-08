/*!

\author dGFuZ3p3

\section 功能

用于选择历史组的对话类。

*/

#ifndef MDSELECTHISTORYGROUPDIALOG_H
#define MDSELECTHISTORYGROUPDIALOG_H

#include <QDialog>

namespace Ui {
class CMDSelectHistoryGroupDialog;
}

class CMDProjectConfiger;
class CMDSelectHistoryGroupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDSelectHistoryGroupDialog(QWidget *parent = 0);
    ~CMDSelectHistoryGroupDialog();

    void initial(CMDProjectConfiger* projectConfiger);
    QString m_selectedGroup;

private slots:
    void on_OKButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::CMDSelectHistoryGroupDialog *ui;
};

#endif // MDSELECTHISTORYGROUPDIALOG_H
