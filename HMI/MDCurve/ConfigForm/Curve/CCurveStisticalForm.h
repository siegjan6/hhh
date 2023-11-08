/*!

\author eHVr

\section 功能
    曲线统计设置界面（待完善）
*/
#ifndef CCURVESTISTICALFORM_H
#define CCURVESTISTICALFORM_H

#include <QWidget>


namespace Ui {
class CCurveStisticalForm;
}

class CCurveStisticalForm : public QWidget
{
    Q_OBJECT

public:
    explicit CCurveStisticalForm(QWidget *parent = 0);
    ~CCurveStisticalForm();

private:
    Ui::CCurveStisticalForm *ui;
};

#endif // CCURVESTISTICALFORM_H
