/*!
\author aGRw 2016.01.04
\section 字典编辑对话框
*/
#ifndef DICTEDITDIALOG_H
#define DICTEDITDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "DictionaryManager.h"
#include "ItemDelegate.h"

namespace Ui {
class CDictEditDialog;
}

class CDictEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CDictEditDialog(QWidget *parent = 0);
    virtual ~CDictEditDialog();
private:
    Ui::CDictEditDialog *ui;
private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonDel_clicked();
    void on_pushButtonFind_clicked();
    void on_pushButtonExport_clicked();
    void on_pushButtonImport_clicked();

private:
    bool m_isSystem;//!是否是系统字典
    CDictionaryManager* m_manager;
    CReadOnlyDelegate m_readOnlyDelegate;
    QStandardItemModel m_model;

public:
    void init(CDictionaryManager* manager, bool isSytem = false);
    void save();
    void refresh();

private:
    void itemChanged(QStandardItem * item);
    void clicked(const QModelIndex& index);
};

#endif // DICTEDITDIALOG_H
