/*!

\author eHVr

\section 功能
    打印页面设置（待完善）
*/
#ifndef CPAGESETTINGFORM_H
#define CPAGESETTINGFORM_H

#include <QWidget>

namespace Ui {
class CPageSettingForm;
}

class CPageSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CPageSettingForm(QWidget *parent = 0);
    ~CPageSettingForm();

private:
    Ui::CPageSettingForm *ui;
};

#endif // CPAGESETTINGFORM_H
