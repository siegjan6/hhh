/*!
\author aGRw 2016.06.21
\section 变量绑定对话框
*/
#ifndef BINDVARDIALOG_H
#define BINDVARDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "MDExpInfo.h"
#include "IDrawEnvironment.h"

namespace Ui {
class CBindVarDialog;
}

class CBindVarDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CBindVarDialog(QList<CMDExpInfo*>* data, IDrawEnvrionment* envir,
        const QString& selectName = "", QWidget *parent = 0);
    ~CBindVarDialog();
private:
    Ui::CBindVarDialog *ui;

private slots:
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();
    void on_pushButtonShowVar_clicked();

private:
    void init(QList<CMDExpInfo*>* data);
    QTreeWidgetItem* createItem(QTreeWidget *tree, CMDExpInfo *exp);
    //创建组
    QTreeWidgetItem *createGroup(QTreeWidget *tree, CMDExpGroup *group);
    //设置可编辑的列
    void setItemEditable(QTreeWidgetItem* item, int column);
    void save();
    void saveExpInfo(QTreeWidgetItem* item);
    void addVar(const QString& var);
    void setVarVisible(bool value);
private:
    QMap<QString, QTreeWidgetItem*> m_groups;//组字典
    IDrawEnvrionment* m_envir;
    QWidget* m_varWidget;
};

#endif // BINDVARDIALOG_H
