#ifndef MDRECIPEDOWNLOADPARADIALOG_H
#define MDRECIPEDOWNLOADPARADIALOG_H

#include <QDialog>
#include "MDObjectPropertyDialog.h"
#include "MDRecipe.h"

namespace Ui {
class CMDRecipeDownloadParaDialog;
}

class CMDRecipeDownloadParaDialog : public CMDObjectPropertyDialog
{
    Q_OBJECT

public:
    explicit CMDRecipeDownloadParaDialog(QWidget *parent = 0);
    ~CMDRecipeDownloadParaDialog();
public:
    void initial(void* objectUI, void *sourceProperty);
    bool UI2Property(void *destProperty);
    void enableUI(DownloadMode mode);
private slots:
    void on_downloadModeComboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::CMDRecipeDownloadParaDialog *ui;
};

#endif // MDRECIPEDOWNLOADPARADIALOG_H
