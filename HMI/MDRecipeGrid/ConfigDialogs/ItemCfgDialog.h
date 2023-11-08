/*!
author Y2hvdWps 2016.09.24
section 单元格属性设置对话框
详细说明
*/
#ifndef ITEMCFGDIALOG_H
#define ITEMCFGDIALOG_H

#include <QDialog>
#include <QLineEdit>

#include "RecipeGridModel.h"
#include "GridItems/GridBaseItem.h"


namespace Ui {
class CItemCfgDialog;
}

class CItemCfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CItemCfgDialog(CRecipeGridModel *model, const QModelIndex& index, IDrawEnvrionment* environment, QWidget *parent = 0 );
    virtual ~CItemCfgDialog();

private slots:
    void on_pushbuttonOk_clicked();

    void on_pushbuttonCancel_clicked();

    void on_pushbuttonSelectVar_clicked();

    void on_checkboxVarBind_toggled(bool checked);

    void on_pushbuttonSelectMin_clicked();

    void on_pushbuttonSelectMax_clicked();

    void on_checkboxTooltip_toggled(bool checked);

    void on_pushbuttonValueMapEdit_clicked();

    void on_comboboxDataSource_currentIndexChanged(int);

    void on_comboboxType_currentIndexChanged(int);

private:
    void replaceWidget(QWidget *srcWidget, QWidget *destWidget);
private:
    Ui::CItemCfgDialog *ui;

    IDrawEnvrionment* m_environment;
    CRecipeGridModel *m_model;
    QModelIndex m_index;
    CGridBaseItem* m_itemCopy;
    QString m_imageItemTitle;
    QRegExpValidator* m_realNumber;

    QLineEdit* m_lineeditCheckBoxLabel;
    qint32 m_replaceIndex;
    qint32 m_stretch;

    QString m_minValue;
    QString m_minVar;
    QString m_maxValue;
    QString m_maxVar;
    QString m_imageName;
    QString m_recipeName;
};

#endif // ITEMCFGDIALOG_H
