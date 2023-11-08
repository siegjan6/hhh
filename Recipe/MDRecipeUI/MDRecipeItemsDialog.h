#ifndef MDRECIPEITEMSDIALOG_H
#define MDRECIPEITEMSDIALOG_H

#include <QDialog>
#include "MDObjectPropertyDialog.h"

namespace Ui {
class CMDRecipeItemsDialog;
}

class CMDProjectConfiger;
class CMDRecipeItemsDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDRecipeItemsDialog(CMDProjectConfiger* pConfiger, QWidget *parent = 0);
    ~CMDRecipeItemsDialog();
public:
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
    void addVar(const QString &var);
protected:
     CMDProjectConfiger* m_pConfiger;
private slots:
    void on_addButton_clicked();

    void on_removeButton_clicked();

private:
    Ui::CMDRecipeItemsDialog *ui;
    QWidget* m_varWidget;
};

#endif // MDRECIPEITEMSDIALOG_H
