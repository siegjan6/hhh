/*!
\author aGRw 2015.06.03
\section 自定义属性编辑对话框
*/
#ifndef CUSTOMPROPERTYDIALOG_H
#define CUSTOMPROPERTYDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>
#include "MDDrawGroup.h"

namespace Ui {
class CCustomPropertyDialog;
}

class CCustomPropertyDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CCustomPropertyDialog(QWidget *parent = 0);
    ~CCustomPropertyDialog();
private:
    Ui::CCustomPropertyDialog *ui;
//////////////////////////////////////////////////////////////////////////
private:
    CMDDrawGroup* m_object;
    int m_maxIndex;
private:
    QString sBaseName;
//////////////////////////////////////////////////////////////////////////
public:
    void init(CMDDrawGroup* obj);
private:
    void saveData();
    //!获取名称的最大索引
    int getMaxIndex();
    void getMaxIndex(CMDDrawGroup* group, int& index);
    //!名称是否是基本名称+数字格式;如是,返回数字
    bool isNameIndex(const QString &name, int& index);
    void itemDoubleClicked(QTreeWidgetItem* item, int column);
    void checkMaxIndex(const QString&name, int& maxIndex);
    QStringList treeNames();
//////////////////////////////////////////////////////////////////////////
private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonDelete_clicked();
    void on_pushButtonOK_clicked();
    void on_pushButtonCancel_clicked();

};

#endif // CUSTOMPROPERTYDIALOG_H
