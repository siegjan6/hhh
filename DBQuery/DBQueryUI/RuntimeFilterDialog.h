#ifndef RUNTIMEFILTERDIALOG_H
#define RUNTIMEFILTERDIALOG_H

#include <QDialog>
#include <QItemDelegate>

namespace Ui {
class RuntimeFilterDialog;
}
class QStringListModel;
class QStandardItemModel;
class IMDDBQueryEngine;
class FiledEnumValueDelegate;
class QTableView;

class RuntimeFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RuntimeFilterDialog(IMDDBQueryEngine * pDBQueryEngine, QWidget *parent = 0);
    ~RuntimeFilterDialog();
    //!是否改变了查询参数或数据源配置
    bool isChanged();
protected:
    virtual void timerEvent(QTimerEvent *);//继承于QObject
    void updateParameterTable(const QModelIndex & index);
    void updateDataSourceTable(const QModelIndex & index);
    void updateDataSourceTable(const QString & viewName);
signals:
    void comboBoxItemListSignal(QStringList & itemList, int row, int col, int currentIndex);
protected slots:
    void listViewclicked(const QModelIndex &index);
private slots:
    void on_paraSaveButton_clicked();
    void on_dsAddButton_clicked();
    void on_dsDelButton_clicked();
    void on_paraCancelButton_clicked();

private:
    Ui::RuntimeFilterDialog *ui;
    QStringListModel  * m_pViewNameModel;
    QStandardItemModel * m_pParameterModel;
    QStandardItemModel * m_pDBSourcsModel;
    QString m_ViewPath;
    int m_nTimerId;
    IMDDBQueryEngine * m_pDBQueryEngine;
    FiledEnumValueDelegate * m_paramViewDelegate; //参数视图代理
    bool m_bIsChanged; //是否改变了查询参数或数据源配置
};

/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，视图变量表格枚举值子项combobox控件代理类，
    用来实现在视图变量tableview中变量类型为枚举时“值”Item项内嵌QCombobox控件】。

*/
//包含Q_OBJECT的类要放到.h文件中 .这样moc命令才能生成相应的代码.不然会出现
//undefined reference to `vtable for的错误
class FiledEnumValueDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    FiledEnumValueDelegate(QTableView * m_ParameterTableView, QObject *parent);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    //void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
public slots:
    void receiveItemListSlot(QStringList & itemList, int row, int col, int currentIndex);
private slots:
    void changeCurrentItemSlot(int index);
private:
    QTableView * m_ParameterTableView;
};
#endif // RUNTIMEFILTERDIALOG_H
