/*!
author Y2hvdWps 2016.11.18
section 配方模板配置对话框
*/
#ifndef SETRECIPETEMPLATEDIALOG_H
#define SETRECIPETEMPLATEDIALOG_H

#include <QDialog>

namespace Ui {
class CSetRecipeTemplateDialog;
}

class CSetRecipeTemplateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSetRecipeTemplateDialog(QWidget *parent = 0);
    virtual ~CSetRecipeTemplateDialog();

private slots:
    void on_pushbuttonOk_clicked();

    void on_pushbuttonCancel_clicked();

private:
    Ui::CSetRecipeTemplateDialog *ui;
};

#endif // SETRECIPETEMPLATEDIALOG_H
