/*!
\author aGRw 2015.10.26
\section 前景填充对话框
*/
#ifndef FILLDIALOG_H
#define FILLDIALOG_H

#include <QDialog>
#include "MDFill.h"

namespace Ui {
class CFillDialog;
}

class CFillDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CFillDialog(QWidget *parent = 0);
    ~CFillDialog();
private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonDynamicColor_clicked();
    void on_pushButtonBrush_clicked();

private:
    Ui::CFillDialog *ui;
/*!
\section property
*/
public:
    CMDFill* fill(){return &m_fill;}
private:
    CMDFill m_fill;
/*!
\section public function
*/
public:
    void init(CMDFill* fill);
/*!
\section private function
*/
private:
    void saveData();
    void updatePaintButton();
};

/*!
\section 绘图按钮
*/
class CPaintPushButton : public QPushButton
{
public:
    explicit CPaintPushButton(QWidget *parent=0);
public:
    QBrush m_brush;
protected:
    void paintEvent(QPaintEvent * e);
};

#endif // FILLDIALOG_H
