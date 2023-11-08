/*!

\author xuk

\section 功能

 测试类对话框，用于返回一个string变量和一个pixmap
*/
#ifndef CDIALOG_H
#define CDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QImage>
#include <QPixmap>
#include <QPainter>


namespace Ui {
class CDialog;
}

class CDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDialog(QWidget *parent = 0);
    ~CDialog();

protected:
    void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_2_clicked();//! 发出changeValue和changeIcon信号
Q_SIGNALS:
    void changeValue(QString);
    void changeIcon(QPixmap);
    void dialogClosed();
private:
    Ui::CDialog *ui;

};


#endif // CDIALOG_H
