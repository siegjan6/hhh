/*!
\author aGRw 2015.08.10
\section 窗体属性设置对话框
*/
#ifndef FORMTYPEDIALOG_H
#define FORMTYPEDIALOG_H

#include <QDialog>
#include "MDFormDefine.h"

namespace Ui {
class CFormTypeDialog;
}

class CFormTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CFormTypeDialog(QWidget *parent = 0);
    ~CFormTypeDialog();
private slots:
    void on_comboBoxType_currentIndexChanged(int index);
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CFormTypeDialog *ui;
//////////////////////////////////////////////////////////////////////////
public:
    void init(FormType type, bool hasFrame, bool fullScreen);

    FormType type();
    bool hasFrame();
    bool fullScreen();
};

#endif // FORMTYPEDIALOG_H
