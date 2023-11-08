/*!
author Y2hvdWps 2016.09.12
section 替换粘贴设置对话框
*/
#ifndef REPLACETEXTDIALOG_H
#define REPLACETEXTDIALOG_H

#include <QDialog>

namespace Ui {
class CReplaceTextDialog;
}

class CReplaceTextDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CReplaceTextDialog(QWidget *parent = 0);
    virtual ~CReplaceTextDialog();

    QString m_src;
    QString m_replace;
private:
    Ui::CReplaceTextDialog *ui;

private slots:
    virtual void accept();
};

#endif // REPLACETEXTDIALOG_H
