/*!

\author dGFuZ3p3

\section 功能

事件：通用属性（如名称、注释等）配置对话类。

*/

#ifndef MDEVENTGENERALDIALOG_H
#define MDEVENTGENERALDIALOG_H

#include "MDObjectPropertyDialog.h"
#include "MDEventConstant.h"

namespace Ui {
class CMDEventGeneralDialog;
}

class CMDEventProperty_Config;
class CMDEventGeneralDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDEventGeneralDialog(QWidget *parent = 0);
    ~CMDEventGeneralDialog();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
    MDEventType m_type;
private slots:
    void on_selectObjectButton_clicked();

private:
    Ui::CMDEventGeneralDialog *ui;
};

#endif // MDEVENTGENERALDIALOG_H
