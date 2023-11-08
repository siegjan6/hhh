#ifndef MDLOADPROGRESSDIALOG_H
#define MDLOADPROGRESSDIALOG_H

#include <QDialog>

namespace Ui {
class CMDLoadProgressDialog;
}

class CMDLoadProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMDLoadProgressDialog(QWidget *parent = 0);
    ~CMDLoadProgressDialog();

    QString m_msg;
    void onLoadProgress(const QString& msg, bool finished, int finishCode);

private:
    Ui::CMDLoadProgressDialog *ui;
private slots:
    void update();
};

#endif // MDLOADPROGRESSDIALOG_H
