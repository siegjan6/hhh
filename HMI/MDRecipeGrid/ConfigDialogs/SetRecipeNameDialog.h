/*!
author Y2hvdWps 2016.09.25
section 批量设置配方名称对话框
详细说明
*/
#ifndef SETRECIPENAMEDIALOG_H
#define SETRECIPENAMEDIALOG_H

#include <QDialog>

namespace Ui {
class CSetRecipeNameDialog;
}

class CSetRecipeNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSetRecipeNameDialog(QWidget *parent = 0);
    virtual ~CSetRecipeNameDialog();

private slots:
    void on_pushbuttonOk_clicked();

    void on_pushbuttonCancel_clicked();

    void on_pushbuttonSelectRecipe_clicked();

private:
    Ui::CSetRecipeNameDialog *ui;
};

#endif // SETRECIPENAMEDIALOG_H
