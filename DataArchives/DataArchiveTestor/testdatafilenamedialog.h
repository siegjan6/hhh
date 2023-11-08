#ifndef TESTDATAFILENAMEDIALOG_H
#define TESTDATAFILENAMEDIALOG_H

#include <QWidget>

namespace Ui {
class TestDataFileNameDialog;
}

class TestDataFileNameDialog : public QWidget
{
    Q_OBJECT

public:
    explicit TestDataFileNameDialog(QWidget *parent = 0);
    ~TestDataFileNameDialog();

private:
    Ui::TestDataFileNameDialog *ui;
};

#endif // TESTDATAFILENAMEDIALOG_H
