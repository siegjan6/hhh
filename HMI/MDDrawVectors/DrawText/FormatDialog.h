/*!
\author aGRw 2015.11.04
\section 文本格式，配置对话框
*/
#ifndef FORMATDIALOG_H
#define FORMATDIALOG_H

#include <QDialog>
#include <QJsonObject>
#include "MDDrawText.h"

namespace Ui {
class CFormatDialog;
}

class CFormatDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CFormatDialog(QWidget *parent = 0);
    ~CFormatDialog();
private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CFormatDialog *ui;
////////////////////////////////////////////////////////////
public:
    QJsonObject* value(){return &m_value;}
private:
    QJsonObject m_value;
////////////////////////////////////////////////////////////
public:
    void init(CMDDrawText* obj);
private:
    void saveData();
};

#endif // FORMATDIALOG_H
