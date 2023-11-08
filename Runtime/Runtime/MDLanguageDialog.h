#ifndef MDLANGUAGEDIALOG_H
#define MDLANGUAGEDIALOG_H

#include <QDialog>

namespace Ui {
class CMDLanguageDialog;
}

class CMDLanguageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDLanguageDialog(QWidget *parent = 0);
    ~CMDLanguageDialog();

private:
    Ui::CMDLanguageDialog *ui;
};

#endif // MDLANGUAGEDIALOG_H
