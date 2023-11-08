/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，新建和修改数据源界面类，
    用来实现新建或修改数据源操作】。

*/

#ifndef NEWDATASOURCEDIALOG_H
#define NEWDATASOURCEDIALOG_H

#include <QDialog>
#include "MDDBQueryConfigure.h"

namespace Ui {
class NewDataSourceDialog;
}

class NewDataSourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDataSourceDialog(QString title,QWidget *parent = 0);
    ~NewDataSourceDialog();

public:
    void getDataSourceParameter();                          //! 获取修改后的数据源参数信息
    void setOriginalDSParameter(DBQ::DBSource *DBSource);   //! 设置数据源原始参数信息
    DBQ::DBSource *getDBSource() const;                     //! 获取新建数据源指针

private slots:
    void on_pbt_Confirm_clicked();                          //! 确认新建或修改数据源
    void on_pbt_Cancel_clicked();                           //! 确认新建或修改数据源

    void on_comboBox_dbType_currentIndexChanged(const QString &arg1);

private:
    Ui::NewDataSourceDialog *ui;
    DBQ::DBSource *m_DBSource;                     //! 数据源指针
    QString m_windowTitle;                                  //! 对话框标题
    QString m_TablePrefix;                                   //! 表前缀 SQL Server为dbo KingBase为publicf
};

#endif // NEWDATASOURCEDIALOG_H
