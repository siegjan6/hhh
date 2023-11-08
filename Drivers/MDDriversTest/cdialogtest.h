#ifndef CDIALOGTEST_H
#define CDIALOGTEST_H

#include <QDialog>

namespace Ui {
class CDialogTest;
}
#include "IMDRTDBManager.h"
#include "MDRTDBManagerFactory.h"

class CDialogTest : public QDialog
{
    Q_OBJECT

public:
    explicit CDialogTest(QWidget *parent = 0);
    ~CDialogTest();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::CDialogTest *ui;

    IMDRTDBManager *m_nsRTDBManager;
};

#endif // CDIALOGTEST_H
