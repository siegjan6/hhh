/*!
\author enli 2015.01.08
\section 类似CMDVariableRunTreeModel，少了定时刷新功能
*/
#ifndef CMDVARIABLEGROUPMODEL_H
#define CMDVARIABLEGROUPMODEL_H


#include <QModelIndex>
#include <QVariant>
#include "IMDTreeModel.h"

class IMDTreeItem;
class IMDVariableGroup;
class IMDRTDBManager;
class CMDVariableTreeModel: public IMDTreeModel
{
    Q_OBJECT
public:
    CMDVariableTreeModel();
    ~CMDVariableTreeModel();
public:
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
    //!test insert rows
    //bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());//1234
public:
    void initial(IMDVariableGroup *rootVariableGroup);
private:
    IMDTreeItem * getItem(const QModelIndex &index) const;
    IMDTreeItem *m_rootTreeItem;
    QStringList m_headerList;
    //!多语言切换,最初调用by tang
public:
    void changeLanguage();

public:
    void setTreeItemIcon(QVariant icon,MDTreeItemType itemType);
    QList<QModelIndex> getCheckedItems();
    void setCheckedable(bool enableChecked);
    void deleteTreeItem(const QModelIndex &itemIndex);
    void setCurRootTreeItem(const QString& rootTreeItemName);
    void setExpandItem(const QModelIndex &itemIndex);
private:
   QVariant m_variableGroupIcon;
   QVariant m_variableIcon;
   bool m_enableChecked;
   QList<QModelIndex> m_checkedList;
   //!RTDBManager的Group管理节点，获取父与子Group节点
   IMDVariableGroup* m_variableGroup;
};

#endif // CMDVARIABLEGROUPMODEL_H
