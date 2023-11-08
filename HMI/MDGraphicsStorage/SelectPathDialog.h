/*!
\author aGRw 2015.07.28
\section 目录选择对话框
*/
#ifndef SELECTPATHDIALOG_H
#define SELECTPATHDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include "StorageFileNode.h"

namespace Ui {
class CSelectPathDialog;
}

class CSelectPathDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CSelectPathDialog(QWidget *parent = 0);
    ~CSelectPathDialog();
private slots:
    void on_pushButtonOk_clicked();
    void on_pushButtonCancel_clicked();
private:
    Ui::CSelectPathDialog *ui;
//////////////////////////////////////////////////////////////////////////
public:
    void init(const QString& path);
    void reset();
    QString selectPath(){return m_selectPath;}
private:
    void setSelectPath(const QString& value){m_selectPath = value;}
private:
    CStorageFileNode m_node;
    QString m_selectPath;
/*!
\section language
*/
public:
    void changeLanguage();
private:
    ITranslateWidgetTool* m_transTool;


};

#endif // SELECTPATHDIALOG_H
