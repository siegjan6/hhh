/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图列表管理对话框，
    用来实现获取选中的视图名列表】。

*/

#ifndef VIEWLISTDIALOG_H
#define VIEWLISTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>

namespace Ui {
class ViewListDialog;
}

class ViewListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ViewListDialog(QWidget *parent = 0);
    ~ViewListDialog();

public:
    //! 加载视图
    bool LoadViews(const QString & dir);
    //! 获取选中的视图名列表
    QStringList GetViewNameSelected();
    //! 选中当前选中视图名称所在行
    void selectedCurrentViewName(QString viewName);


private:
    //! 从视图文件中获取视图名称
    bool GetViewNameFromFileName(const QString & strIn, QString & viewNameOut);

private slots:
    //! 确认/取消操作
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::ViewListDialog *ui;
    QStandardItemModel * m_pModel;      //! 标准模型
    QStringList m_ViewNamesSelected;    //! 选中的视图名
};

#endif // VIEWLISTDIALOG_H
