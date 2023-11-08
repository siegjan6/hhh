/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，新建数据库界面类，
    用来实现新建数据库操作】。

*/

#ifndef NEWDATABASEDIALOG_H
#define NEWDATABASEDIALOG_H

#include <QDialog>
#include "MDDBQueryConfigure.h"

namespace Ui {
class NewDataBaseDialog;
}

class NewDataBaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDataBaseDialog(QString title,QWidget *parent = 0);
    ~NewDataBaseDialog();

public:
    void setDatabaseParameter(DBQ::DBSource *dbsource); //! 从当前数据源树节点读取信息设置数据库参数信息
    QString getDBName();                                //! 获取数据库名
    void setDBName(QString &dbName);                    //! 设置数据库名

private slots:
    void on_pbt_Confirm_clicked();                      //! 确认创建数据库
    void on_pbt_Cancel_clicked();                       //! 取消创建数据库
    void on_pbt_connectTest_clicked();                  //! 根据配置的数据库信息测试连接

private:
    Ui::NewDataBaseDialog *ui;
    QString m_DBName;                                   //! 数据库名
    QString m_windowTitle;                              //! 对话框标题
};

#endif // NEWDATABASEDIALOG_H
