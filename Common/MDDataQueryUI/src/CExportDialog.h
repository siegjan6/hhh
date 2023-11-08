#ifndef CEXPORTDIALOG_H
#define CEXPORTDIALOG_H

#include <QDialog>

namespace Ui {
class CExportDialog;
}

class CExportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CExportDialog(QWidget *parent = 0);
    ~CExportDialog();

    enum class exType{
        Csv = 0,
        Xml,
        Json
    };

    int exportType() const;
private slots:
    void on_confirmButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::CExportDialog *ui;
};

#endif // CEXPORTDIALOG_H
