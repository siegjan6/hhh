#ifndef TESTORDIALOG_H
#define TESTORDIALOG_H

#include <QDialog>

namespace Ui {
class TestorDialog;
}

class TestorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TestorDialog(QWidget *parent = 0);
    ~TestorDialog();
private slots:
    void on_dbSourceTreeButton_clicked();

    void on_runtimeFilterButton_clicked();

private:
    Ui::TestorDialog *ui;
};

#endif // TESTORDIALOG_H
