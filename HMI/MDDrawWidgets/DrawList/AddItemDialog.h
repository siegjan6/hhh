/*!
\author Y2hhaWhq

\section 功能

在属性框添加item按钮点击后,显示并添加控件的子项.
*/
#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>

namespace Ui {
class AddItemDialog;
}

class CAddItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddItemDialog(QWidget *parent = 0);
    ~CAddItemDialog();
/*!
\section  编辑子项
*/
    QString items() const;
    void setItems(const QStringList &str);
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

private:
    Ui::AddItemDialog *ui;
};

#endif // ADDITEMDIALOG_H
