/*!
author Y2hvdWps 2016.09.25
section 多单元格设置对话框
详细说明
*/
#ifndef MULITITEMCFGDIALOG_H
#define MULITITEMCFGDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include "RecipeGridModel.h"
#include "GridItems/GridBaseItem.h"


namespace Ui {
class CMulitItemCfgDialog;
}

class CMulitItemCfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CMulitItemCfgDialog(CRecipeGridModel *model, const QModelIndexList& selectList, IDrawEnvrionment* environment, QWidget *parent = 0);
    virtual ~CMulitItemCfgDialog();

private slots:
    void on_pushbuttonOk_clicked();

    void on_pushbuttonCancel_clicked();

    void on_checkboxTooltip_toggled(bool checked);

    void on_comboboxDataSource_currentIndexChanged(int);

    void on_comboboxType_currentIndexChanged(int);

private:
    void replaceWidget(QWidget *srcWidget, QWidget *destWidget);
    void saveData();
    bool setDataDifferent(CGridBaseItem* destitem, CGridBaseItem* srcitem);
    void setMulitItems();
private:
    Ui::CMulitItemCfgDialog *ui;

    IDrawEnvrionment* m_environment;
    CRecipeGridModel *m_model;
    const QModelIndexList* m_selectList;
    CGridBaseItem* m_itemCopy;

    QLineEdit* m_lineeditCheckBoxLabel;
    qint32 m_replaceIndex;
    qint32 m_stretch;

    QString m_recipeName;
};

#endif // MULITITEMCFGDIALOG_H
