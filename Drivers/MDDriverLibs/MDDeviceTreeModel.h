#ifndef CMDDEVICETREEMODEL_H
#define CMDDEVICETREEMODEL_H

#include <QModelIndex>
#include <QVariant>
#include "IMDTreeModel.h"

class IMDTreeItem;
class IMDDriverManager;

class CMDDeviceTreeModel: public IMDTreeModel
{
    Q_OBJECT
public:
    CMDDeviceTreeModel();
    ~CMDDeviceTreeModel();
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
public:
    void initial(IMDDriverManager *drvierManager);
private:
    IMDTreeItem * getItem(const QModelIndex &index) const;
    IMDTreeItem *m_rootTreeItem;
    QStringList m_headerList;
public:
    void setTreeItemIcon(QVariant icon,MDTreeItemType itemType);
private:
    QVariant m_deviceIcon;
    QVariant m_dataBlockIcon;
    QVariant m_variableIcon;
};

#endif // CMDDEVICETREEMODEL_H
