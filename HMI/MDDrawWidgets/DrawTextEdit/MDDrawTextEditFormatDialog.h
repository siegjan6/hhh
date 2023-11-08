/*!
\author mxl 2016.08.03
\section 文本框变量格式，配置对话框
*/
#ifndef MDDRAWTEXTEDITFORMATDIALOG_H
#define MDDRAWTEXTEDITFORMATDIALOG_H

#include <QDialog>
#include "MDDrawTextEdit.h"

namespace Ui {
class MDDrawTextEditFormatDialog;
}

class MDDrawTextEditFormatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MDDrawTextEditFormatDialog(QWidget *parent = 0);
    ~MDDrawTextEditFormatDialog();

private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();

private:
    Ui::MDDrawTextEditFormatDialog *ui;

////////////////////////////////////////////////////////////
public:
    QJsonObject* value(){return &m_value;}
private:
    QJsonObject m_value;
////////////////////////////////////////////////////////////

public:
    void init(CMDDrawTextEdit* obj);
private:
    void saveData();
////////////////////////////////////////////////////////////
};

#endif // MDDRAWTEXTEDITFORMATDIALOG_H
