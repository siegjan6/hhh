/*!

\author dGFuZ3p3

\section 功能

报警参数界面类，用于配置报警的参数。

*/

#ifndef MDALARMPARAFORM_H
#define MDALARMPARAFORM_H

#include "MDAlarmUI_Global.h"
#include <QWidget>

namespace Ui {
class CMDAlarmParaForm;
}

class CMDAlarmConfiger;
class MDALARMUISHARED_EXPORT CMDAlarmParaForm : public QWidget
{
    Q_OBJECT

public:
    explicit CMDAlarmParaForm(QWidget *parent = 0);
    virtual ~CMDAlarmParaForm();
public:
    bool initial(CMDAlarmConfiger* configer);
    bool accept();
private:
    Ui::CMDAlarmParaForm *ui;
    CMDAlarmConfiger* m_configer;
};

#endif // MDALARMPARAFORM_H
