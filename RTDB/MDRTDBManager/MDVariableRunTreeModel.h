/*!
\author enli 2015.01.08
\section 类似CMDDeviceRunTreeModel，由CMDRTDBVariablesWidget创建。管理变量树节点
*/
#ifndef CMDVARIABLERUNTREEMODEL_H
#define CMDVARIABLERUNTREEMODEL_H
#include <QModelIndex>
#include <QVariant>
#include "IMDTreeModel.h"
#include <QTimer>

class IMDTreeItem;
class IMDVariableGroup;
class CMDVariableRunTreeModel: public IMDTreeModel
{
    Q_OBJECT
public:
    CMDVariableRunTreeModel();
    ~CMDVariableRunTreeModel();
public:
/*!
\section  变量树节点信息
*/
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) Q_DECL_OVERRIDE;
    /*
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    */
public:
    void initial(IMDVariableGroup *rootVariableGroup);
    //!多语言切换,最初调用by tang
public:
    void changeLanguage();
private:
    IMDTreeItem * getItem(const QModelIndex &index) const;
    IMDTreeItem *m_rootTreeItem;
    QStringList m_headerList;
/*!
\section 设置变量树节点状态
*/
public:
    void setTreeItemIcon(QVariant icon,MDTreeItemType itemType);
    QList<QModelIndex> getCheckedItems();
    //!是否可选中，编辑时可选
    void setCheckedable(bool enableChecked);
    void deleteTreeItem(const QModelIndex &itemIndex);
    void setCurRootTreeItem(const QString& rootTreeItemName);
    void setExpandItem(const QModelIndex &itemIndex);
private:
   QVariant m_variableGroupIcon;
   QVariant m_variableIcon;
   bool m_enableChecked;
   QList<QModelIndex> m_checkedList;
   IMDVariableGroup* m_variableGroup;
};

#endif // CMDVARIABLERUNTREEMODEL_H
