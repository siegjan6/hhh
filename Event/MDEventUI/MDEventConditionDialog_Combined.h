/*!

\author dGFuZ3p3

\section 功能

事件：组合事件配置对话类。

*/

#ifndef MDEVENTCONDITIONDIALOG_COMBINED_H
#define MDEVENTCONDITIONDIALOG_COMBINED_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDEventConditionDialog_Combined;
}

class CMDEventConditionDialog_Combined : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDEventConditionDialog_Combined(QWidget *parent = 0);
    ~CMDEventConditionDialog_Combined();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDEventConditionDialog_Combined *ui;
};

#endif // MDEVENTCONDITIONDIALOG_COMBINED_H
