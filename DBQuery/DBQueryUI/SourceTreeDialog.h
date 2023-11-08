/*!

\author bWVpeGw=

\section 功能

管理【数据查询模块，数据归档数据源树结构对话框管理类，
    用来实现点击数据源树结构不同子节点返回相应节点信息接口】。

*/

#ifndef SOURCETREEDIALOG_H
#define SOURCETREEDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QListWidget>
#include "MDDBQueryConfigure.h"
#include "IMDDataArchives.h"
#include "IMDDataSource.h"
#include "IMDDataTable.h"
#include "IMDDataField.h"

Q_DECLARE_METATYPE(IMDDataSource*)
Q_DECLARE_METATYPE(IMDDataTable*)
Q_DECLARE_METATYPE(IMDDataField*)
Q_DECLARE_METATYPE(DBQ::DBSource*)

namespace RETRUNMODE {
    enum Value { TYPE_DBSOURCE = 0,
                 TYPE_VIEWFIELD
                };
}

namespace Ui {
class SourceTreeDialog;
}

class SourceTreeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SourceTreeDialog(QString title, QString path, QWidget *parent = 0);
    ~SourceTreeDialog();

public:
    //! 获取选中的数据源指针
    DBQ::DBSource *getDBSouce();
    //! 获取选中的字段指针
    DBQ::ViewField *getViewField();
    //! 设置返回模式
    void setReturnMode(int mode);
    //! 获取返回模式
    int getReturnMode();
    //! 获取选中的数据源链表
    QList<DBQ::DBSource *> getDBSourcesSelected() const;
    //! 获取选择的字段链表
    QList<DBQ::ViewField *> getFieldsSelected() const;
    //! 设置stackWidget显示模式
    void setStackWidgetShowMode(int mode);
    //! 设置视图数据源名称链表
    void setDbsourceNameList(const QStringList &dbsourceNameList);
    //! 初始化数据源树结构
    void initDataSourceTree();

private:
    //! 解析数据归档字段类型
    DBQ::VIEW_FIELD_TYPE getViewFieldValueTypeString(MDDataType fieldType);
    //! 添加数据源
    void addDBSource();
    //! 添加选中的数据源到链表中
    void addDBSourcesToList(QList<QListWidgetItem*> selectedItemList,int size);
    //! 添加数据查询字段
    void addDBViewField(QTreeWidgetItem* item);
    //! 从数据归档数据表中添加数据查询字段到视图字段链表
    void addTableFiledsToDBViewFieldsList(QTreeWidgetItem* item);
    //! 添加数据查询字段到链表中
    void addSelectedFiledsToDBViewFieldsList(QList<QTreeWidgetItem *> selectedItemList, int size);

private slots:
    //! 确认/取消操作
    void on_btnConfirm_clicked();
    void on_btnCancel_clicked();

private:
    Ui::SourceTreeDialog *ui;
    QString m_dasourceTreePath;                 //! 数据源树结构路径
    int m_ReturnType;                           //! 返回类型
    QTreeWidgetItem *m_pRoot;                   //! 树结构根节点
    IMDDataArchives *m_DataArchives;            //! 数据归档管理指针
    DBQ::DBSource *m_DBSourceSelected;          //! 选中的数据源指针
    QList<DBQ::DBSource*> m_DBSourcesSelected;  //! 选中的数据源链表
    DBQ::ViewField *m_FieldSelected;            //! 选中的字段指针
    QList<IMDDataField *> m_Fields;             //! 数据归档字段链表
    QList<DBQ::ViewField *> m_FieldsSelected;   //! 视图字段链表
    QStringList m_dbsourceNameList;             //! 视图数据源名称链表
    QString m_windowTitle;                      //! 对话框标题
};

#endif // SOURCETREEDIALOG_H
