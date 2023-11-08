/*!
author Y2hvdWps 2016.09.25
section 批量设置数据源对话框
详细说明
*/
#ifndef SETDATASOURCEDIALOG_H
#define SETDATASOURCEDIALOG_H

#include <QDialog>
#include "GridItems/GridBaseItem.h"

namespace Ui {
class CSetDataSourceDialog;
}

class CSetDataSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CSetDataSourceDialog(dataSourceType dataSource, QWidget *parent = 0);
    virtual ~CSetDataSourceDialog();

    dataSourceType m_dataSource;

private slots:
    void on_pushbuttonOk_clicked();

    void on_pushbuttonCancel_clicked();

private:
    Ui::CSetDataSourceDialog *ui;

};

#endif // SETDATASOURCEDIALOG_H
