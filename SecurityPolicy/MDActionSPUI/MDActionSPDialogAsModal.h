/*!

\author dGFuZ3p3

\section 功能

CMDActionSP 链表配置对话类，以模态方式显示。

*/

#ifndef MDACTIONSPDIALOGASMODAL_H
#define MDACTIONSPDIALOGASMODAL_H

#include "MDActionSPListDialog.h"

namespace Ui {
class CMDActionSPDialogAsModal;
}

class CMDActionSPDialogAsModal : public CMDActionSPListDialog
{
    Q_OBJECT

public:
    explicit CMDActionSPDialogAsModal(QWidget *parent = 0);
    ~CMDActionSPDialogAsModal();

private slots:
    void on_OKButton_clicked();

    void on_CancelButton_clicked();

private:
    Ui::CMDActionSPDialogAsModal *ui;
};

#endif // MDACTIONSPDIALOGASMODAL_H
