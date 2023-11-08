/*!

\author dGFuZ3p3

\section 功能

动作：通用属性（如名称、注释等）配置对话类。

*/

#ifndef MDACTIONGENERALDIALOG_H
#define MDACTIONGENERALDIALOG_H

#include <QDialog>
#include "MDObjectPropertyDialog.h"
#include "APC.h"
#include "MDActionUIImp.h"

namespace Ui {
class CMDActionGeneralDialog;
}

class CMDActionProperty_Config;
class CMDActionUIImp;
class CMDActionGeneralDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDActionGeneralDialog(QWidget *parent = 0);
    ~CMDActionGeneralDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
    MDActionType m_type;
private slots:
    void onDelayChanged(int state);
    void onExecExpChanged(int state);
    void on_selectObjectButton_clicked();

private:
    Ui::CMDActionGeneralDialog *ui;
};

#endif // MDACTIONGENERALDIALOG_H
