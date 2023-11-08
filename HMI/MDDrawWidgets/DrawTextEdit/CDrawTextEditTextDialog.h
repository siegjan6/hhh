/*!
\author bWVpeGw=

\section 功能

编辑框控件文本输入对话框
*/

#ifndef CDRAWTEXTEDITTEXTDIALOG_H
#define CDRAWTEXTEDITTEXTDIALOG_H

#include <QDialog>

namespace Ui {
class CDrawTextEditTextDialog;
}

class CDrawTextEditTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDrawTextEditTextDialog(QWidget *parent = 0);
    ~CDrawTextEditTextDialog();

/*!
\section  编辑框文本输入
*/
public:
    void setItems(const QString &str);
    QString items();

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

/*!
\section  换行符
*/
private:
    QString breakString() const
    {
#ifdef Q_OS_LINUX
        return {"\n"};
#elif defined(Q_OS_WIN)
        return {"\n"};
#endif
    }

private:
    Ui::CDrawTextEditTextDialog *ui;
};

#endif // CDRAWTEXTEDITTEXTDIALOG_H
