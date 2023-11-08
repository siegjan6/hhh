/*!

\author xuk

\section 功能

属性调用对话框
*/
#ifndef CDIALOG_1_H
#define CDIALOG_1_H

#include <QDialog>
#include <QString>
#include <QPixmap>

namespace Ui {
class CDialog_1;
}

class CDialog_1 : public QDialog
{

    Q_OBJECT

public:
    explicit CDialog_1(QWidget *parent = 0);
    ~CDialog_1();

private slots:
    void on_pushButton_clicked();

Q_SIGNALS:
    void changeValue(QString);
    void changeIcon(QPixmap);
private:
    Ui::CDialog_1 *ui;
};

#endif // CDIALOG_1_H
