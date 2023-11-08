/*!
author Y2hvdWps 2016.09.08
section 表格控件的属性设置对话框
*/
#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include "RecipeGridView.h"

namespace Ui {
class CPropertyDialog;
}

class CPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CPropertyDialog(const PropertyPara& property, QWidget *parent = 0);
    virtual ~CPropertyDialog();
public:
    void initWidgets();

    PropertyPara m_property;
private slots:
    virtual void accept();
private:
    Ui::CPropertyDialog *ui;
};

#endif // PROPERTYDIALOG_H
