/*!

\author dGFuZ3p3

\section 功能

事件：周期事件配置对话类。

*/

#ifndef MDEVENTCONDITIONDIALOG_CYCLETIMER_H
#define MDEVENTCONDITIONDIALOG_CYCLETIMER_H

#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDEventConditionDialog_CycleTimer;
}

class CMDEventConditionDialog_CycleTimer : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDEventConditionDialog_CycleTimer(QWidget *parent = 0);
    ~CMDEventConditionDialog_CycleTimer();

    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
private:
    Ui::CMDEventConditionDialog_CycleTimer *ui;
};

#endif // MDEVENTCONDITIONDIALOG_CYCLETIMER_H
