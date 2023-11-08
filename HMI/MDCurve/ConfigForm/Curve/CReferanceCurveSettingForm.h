/*!

\author xuk

\section 功能

    参考曲线配置界面（待完善）
*/
#ifndef CREFERANCECURVESETTINGFORM_H
#define CREFERANCECURVESETTINGFORM_H

#include <QWidget>

namespace Ui {
class CReferanceCurveSettingForm;
}

class CReferanceCurveSettingForm : public QWidget
{
    Q_OBJECT

public:
    explicit CReferanceCurveSettingForm(QWidget *parent = 0);
    ~CReferanceCurveSettingForm();

private:
    Ui::CReferanceCurveSettingForm *ui;
};

#endif // CREFERANCECURVESETTINGFORM_H
