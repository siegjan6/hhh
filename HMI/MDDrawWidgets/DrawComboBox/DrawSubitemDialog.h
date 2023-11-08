/*!
\author Y2hhaWhq

\section 功能

在属性框添加item按钮点击后,显示并添加控件的子项.
*/
#ifndef DRAWSUBITEMDIALOG_H
#define DRAWSUBITEMDIALOG_H

#include <QDialog>

namespace Ui {
class DrawSubitemDialog;
}

class CDrawSubitemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CDrawSubitemDialog(QWidget *parent = 0);
    ~CDrawSubitemDialog();

private:
    Ui::DrawSubitemDialog *ui;
/*!
\section  编辑子项
*/
public:
    void setItems(const QStringList &str);
    QString items();
/*!
\section  换行符
*/
private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    QString breakString() const
    {
#ifdef Q_OS_LINUX
        return {"\n"};
#elif defined(Q_OS_WIN)
        return {"\n"};
#endif
    }
};

#endif // DRAWSUBITEMDIALOG_H
